/*
*socket server frame, user can use this frame by inherit serverSocket class and overwrite the ProcessMsg funtion to
*their actual work to deal with the receive msg.
*author:write by zzl
*time:2018-9-24
*/
#pragma once
#include "message.h"
#include <mutex>
#include <thread>
#include <set>

#ifdef _WIN32
#include<WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")
#endif // _WIN32

class serverSocket
{
public:
    typedef std::string string;
    typedef MsgHead Msg;
    typedef unsigned int UINT;
//#ifdef __linux or #ifdef  __linux__
    typedef int SOCKET;
//#endif // __linux or #ifdef  __linux__

    serverSocket();
    virtual ~serverSocket();
    //create server socket
    bool createSocket(UINT port);
    //close server
    void closeSocket();
    string getError(){ return errInf; }

    static const int backlog = 10;		  //connect required queue size
    //static const int nRecvBuf = 1024000;  //receive buff size
    //static const int nSendBuf = 1024000;  //send buff size
protected:
    //send file to client with connected sd
    bool sendFile(char *filePath, SOCKET sd);
    //send msg to client with connected sd
    bool sendMsg(Msg *msg, UINT sizeOfMsg, SOCKET sd);
    //receive msg from client with connected sd
    bool receiveMsg(Msg *msg, UINT sizeOfMsg, SOCKET sd);


    //receive contain file from client with connected sd and save in local file path
    bool receiveFile(char *fileSavePath, long long fileLength, SOCKET sd);
    //receive char to buff
    bool receiveBuff(char *buff, SOCKET sd);
    //send char to client
    bool sendBuff(char *buff, int size, SOCKET sd);
    //process msg from client: need to overwrite in sub concrete class
    virtual bool ProcessMsg(char *buff, SOCKET sd) = 0;


    bool	isMonitor;
    UINT	layoffTime;
    string	errInf;
    SOCKET	sd;
private:
    std::set<SOCKET> connectedSds;			//set of all connected client's socket
    std::mutex sdLock;						//lock for connected socket set operate
    std::thread monitorConnectionThread;
};

void parseFilepath(std::string filepath, std::string &path, std::string &filename, std::string &ext);

