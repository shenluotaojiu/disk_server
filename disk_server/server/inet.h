#ifndef INET_H
#define INET_H

#include <winsock2.h>

class Inet
{
public:
    Inet() = default;
    virtual ~Inet() = default;

public:
    virtual bool initNetWork(const char* szip = "127.0.0.1",unsigned short nport = 1234) = 0;
    virtual void unInitNetWork(const char* szerr = "null") = 0;                                     //卸载网络
    virtual bool sendData(SOCKET sockWaiter,const char* szbuf,int nlen) = 0;                        //向指定客户端发送数据
    virtual void recvData() = 0;                                                                    //接收数据
};

#endif // INET_H
