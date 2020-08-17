#ifndef MESSAGE_H
#define MESSAGE_H

#pragma once
#include<iostream>

#define MAX_PACK_SIZE  10240		//数据包的长度
#define MAX_FILE_NAME_LENGTH 256	//文件名的长度
#define MAX_FILE_EXT_LENGTH 6		//文件名的长度
#define MAX_INFOR_LENGTH 256
#define MAX_PARTID_LENGTH 256

enum MsgID
{
    INVALID_MSG		    = -1,
    MSG_ERROR			= 3,
    MSG_DETECT			= 4,
    MSG_RESET			= 5,
    MSG_SCAN			= 6,
    MSG_PART_INFO_END	= 7,
    MSG_PART_INFOS		= 8,
    MSG_DEVICE_POS		= 9,
    MSG_FILE_HEADER		= 10,
    MSG_CLOSE			= 11
};

struct MsgHead     //头消息
{
    MsgID msgId;    //消息标识
    MsgHead(MsgID msg = INVALID_MSG) :msgId(msg){};
};

//错误信息
struct MsgError :public MsgHead
{
    int errorCode;
    MsgError() : MsgHead(MSG_ERROR){ errorCode = 0; }
};

//开始扫描消息
struct MsgScan :public MsgHead
{
    float x, y, z;
    MsgScan() : MsgHead(MSG_SCAN){ x = y = z = 0; }
};


//零件的信息消息
struct  MsgPartInfos:public MsgHead
{
    float minScore;
    int	  findNum;
    int   foundNum;
    char partID[MAX_PARTID_LENGTH];
    MsgPartInfos() :MsgHead(MSG_PART_INFOS){
        minScore = 0.6;
        findNum	 = 1;
        foundNum = 0;
    }
};

//设备的位姿消息
struct MsgDevicePos :public MsgHead
{
    float x;
    float y;
    float z;
    MsgDevicePos() : MsgHead(MSG_DEVICE_POS){}
};

//文件头消息
struct MsgFileHeader : public MsgHead
{
    char fileName[MAX_FILE_NAME_LENGTH];
    char fileExt[MAX_FILE_EXT_LENGTH];
    long long fileLength;
    MsgFileHeader(): MsgHead(MSG_FILE_HEADER){}
};




#endif // MESSAGE_H
