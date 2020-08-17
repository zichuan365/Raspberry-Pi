#include "serverSocket.h"
#ifdef __linux
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
using namespace std;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

extern int errno;
int WSAGetLastError()
{
    return errno;
}

int closesocket(serverSocket::SOCKET sd)
{
    return close(sd);
}

void Sleep(int ms)
{
    usleep(ms * 1e3);
}

#endif // __linux or #ifdef  __linux__

serverSocket::serverSocket():
    layoffTime(1),
    errInf("no error"),
    sd(INVALID_SOCKET)
{
}

serverSocket::~serverSocket()
{
    closeSocket();
    if (monitorConnectionThread.joinable())
    {
        monitorConnectionThread.join();
    }
}

bool serverSocket::createSocket(UINT port)
{
#ifdef _WIN32
    WSADATA wsData;
    WORD wr = MAKEWORD(2, 2);
    if (WSAStartup(wr, &wsData) != 0)
    {
        return false;
    }
#endif // _WIN32

    //绑定套接字
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == INVALID_SOCKET)
    {
        std::cout << "create server listen socket failed! error code: " << WSAGetLastError() << std::endl;
        return false;
    }

    sockaddr_in sListen;
    sListen.sin_family = AF_INET;
    sListen.sin_addr.s_addr = htonl(INADDR_ANY);
    sListen.sin_port = htons(port);

    int opt = 1;
    setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));

    socklen_t nSize;
    nSize = sizeof(sockaddr_in);
    if (bind(sd, (sockaddr*)&sListen, nSize) == SOCKET_ERROR)
    {
        closesocket(sd);
        std::cout << "bind socket failed! error code: " << WSAGetLastError() << std::endl;
        return false;
    }
    if (listen(sd, backlog) == SOCKET_ERROR)
    {
        closesocket(sd);
        std::cout << "listen failed! error code: " << WSAGetLastError() << std::endl;
        return false;
    }

    auto monitor = [&]()
    {
        socklen_t nSize;
        nSize = sizeof(sockaddr_in);
        while (true)
        {

            sockaddr_in saRemote;
            SOCKET sdListen = accept(sd, (sockaddr*)&saRemote, &nSize);
            if (sdListen == INVALID_SOCKET)
            {
                std::cout << "accept client failed! error code: " << WSAGetLastError() << std::endl;
                return;
            }
            connectedSds.insert(sdListen);

            //printf("new client connected, current connected client number:%u\n",connectedSds.size());
             std::cout << "new client connected, current connected client number: " << connectedSds.size()<< "\n";
            //std::cout << "新的客户端上线, 当前客户端连接数为: " << connectedSds.size()<< "\n";


            int nRecvBuf = 1024000;  //receive buff size
            int nSendBuf = 1024000;  //send buff size
            //设置缓冲区大小
            setsockopt(sdListen, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));
            //发送缓冲区
            setsockopt(sdListen, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBuf, sizeof(int));

            //开始监听客户端消息
            auto msgMonitor = [&](SOCKET sdListen)
            {
                while (true)
                {
                    char buff[MAX_PACK_SIZE] = {};
                    if (!receiveBuff(buff,sdListen)) break;
                    //Msg *msg = (Msg *)buff;
                    ProcessMsg(buff, sdListen);
                }
                //std::lock_guard<std::mutex> loc(sdLock);
                closesocket(sdListen);
                connectedSds.erase(sdListen);
                std::cout << "client " << sdListen<<" is disconnected! current connected client number: " << connectedSds.size() << "\n";
                //std::cout <<sdListen<< "号客户端" << sdListen<<" 下线! 当前客户端连接数为: " << connectedSds.size() << "\n";
            };
            std::thread t = std::thread(msgMonitor, sdListen);
            t.detach();

        }
    };
    monitorConnectionThread = std::thread(monitor);
    return true;
}

void serverSocket::closeSocket()
{
    closesocket(sd);
}

bool serverSocket::sendFile(char *filePath, SOCKET sd)
{
    if (sd == INVALID_SOCKET)
    {
        return false;
    }
    string path, fileName, fileExt;
    parseFilepath(filePath, path, fileName, fileExt);
    //发送文件头
    MsgFileHeader fileHeader;
    strcpy(fileHeader.fileName, fileName.c_str());
    strcpy(fileHeader.fileExt, fileExt.c_str());

    FILE *pFile;
    //todo::fopen无法以独占方式打开文件，存在隐患
    pFile = fopen(filePath, "rb+");
    if (pFile == NULL){
        errInf = "open file failed!";
        return false;
    }
    fseek(pFile, 0, SEEK_END);//重定位流上的文件指针
    long long fileLength = ftell(pFile);
    std::cout << "发送的文件大小：" << fileLength << "\n";
    fileHeader.fileLength = fileLength;

    ////////Send the headerMessage:1.fileName 2.fileExt 3.fileLength
    if (!sendMsg(&fileHeader, sizeof(MsgFileHeader),sd))
    {
        errInf = "send file header msg failed!";
        return false;
    }
    Sleep(layoffTime);

    ////////发送文件内容
    //std::lock_guard<std::mutex> lok(sendLock);
    fseek(pFile, 0, SEEK_SET);   //定位到文件首位置
    unsigned int i = 0;
    char buff[MAX_PACK_SIZE]={};
    while (i<fileLength)
    {
        int nSize;
        if (i + MAX_PACK_SIZE>fileLength)
        {
            nSize = (int)(fileLength - i);
        }
        else
        {
            nSize = MAX_PACK_SIZE - 1;
        }
        fread(buff, sizeof(char), nSize, pFile);
        int nSend;
        nSend = send(sd, buff, nSize, 0);
        if (nSend == SOCKET_ERROR)
        {
            std::cout << "发送失败\n";
            return false;
        }
        //Sleep(layoffTime);
        i += nSend;
        fseek(pFile, -(nSize - nSend), SEEK_CUR);  //定位到实际已发送到的位置
        memset(buff, 0, sizeof(char)*MAX_PACK_SIZE); //将buff清空
    }
    fclose(pFile);
    Sleep(layoffTime);
    return true;
}

bool serverSocket::receiveFile(char *fileSavePath, long long fileLength, SOCKET sd)
{
    //std::lock_guard<std::mutex> lok(recLock);
    if (!fileSavePath)
    {
        return false;
    }
    if (sd == INVALID_SOCKET) return false;
    char buff[MAX_PACK_SIZE];
    FILE *pFile;
    //char RevFile[_MAX_FNAME + _MAX_EXT] = "./sev/";
    //pFile=fopen(fileName,"ab+");
    //strcat(RevFile, fileName);
    pFile = fopen(fileSavePath, "wb+");
    if (pFile == NULL) {
        errInf = "open file failed!";
        return false;
    }
    unsigned int i = 0;
    std::cout << "receive file length：" << fileLength << "\n";
    while (i + 1 < fileLength)
    {
        int nRecv = recv(sd, buff, MAX_PACK_SIZE, 0);
        if (nRecv == SOCKET_ERROR)
        {
            return false;
        }
        fwrite(buff, sizeof(char), nRecv, pFile);
        i += nRecv;
        memset(buff, 0, sizeof(char)*MAX_PACK_SIZE);
    }
    fclose(pFile);
    return true;
}

bool serverSocket::sendMsg(Msg *msg, UINT sizeOfMsg, SOCKET sd)
{
    //std::lock_guard<std::mutex> lok(sendLock);
    if (sd == INVALID_SOCKET) return false;
    if (send(sd, (char *)msg, sizeOfMsg, 0) < 1)
        return false;
    Sleep(layoffTime);
    return true;
}

bool serverSocket::receiveBuff(char *buff, SOCKET sd)
{
    //std::lock_guard<std::mutex> lok(recLock);
    if (sd == INVALID_SOCKET) return false;
    if (recv(sd, buff, MAX_PACK_SIZE, 0) < 1)
    {
        //std::cout << "receive data failed! error code: " << WSAGetLastError() << "\n";
        return false;
    }
    return true;
}

bool serverSocket::sendBuff(char *buff, int size, serverSocket::SOCKET sd)
{
    if (sd == -1) return false;
    if (send(sd, buff, size, 0) == -1)
        return false;
    return true;
}

using string = std::string ;
void parseFilepath(std::string filepath, std::string &path, std::string &filename, std::string &ext)
{
    if (filepath.empty()) return;
    if (filepath.find('/') != string::npos)//find the '/'
    {
        int locpoint = filepath.find_last_of('.');
        int locfilename = filepath.find_last_of('/');
        ext = filepath.substr(locpoint);//获取文件后缀
        filename = filepath.substr(locfilename + 1, locpoint - locfilename - 1);
        path = filepath.substr(0, locfilename);
    }
    else if (filepath.find('\\') != string::npos)
    {
        int locpoint = filepath.find_last_of('.');
        int locfilename = filepath.find_last_of('\\');
        ext = filepath.substr(locpoint);//获取文件后缀
        filename = filepath.substr(locfilename + 1, locpoint - locfilename - 1);
        path = filepath.substr(0, locfilename);
    }
    return;

}

