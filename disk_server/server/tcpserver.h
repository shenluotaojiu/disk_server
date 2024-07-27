#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <list>
#include <windows.h>
#include <map>
#include "inet.h"
#include "kernel/tcpkernel.h"


class TCPServer : public Inet
{
public:
    TCPServer();
    ~TCPServer();
public:
    //1.初始化网络--加载、创建socket\bind\listen
    bool initNetWork(const char* szip = "127.0.0.1",unsigned short nport = 1234);
    void unInitNetWork(const char* szerr = "null"); //卸载网络
    bool sendData(SOCKET sockWaiter,const char* szbuf,int nlen); //向指定客户端发送数据
    void recvData(); //接收数据
    static DWORD WINAPI threadAccept(LPVOID lpvoid);
    static DWORD WINAPI threadRecv(LPVOID lpvoid);
private:
    SOCKET m_socklisten;
    std::list<HANDLE> m_lstThread;
    bool  m_bFlagQuit;
    std::map<DWORD,SOCKET> m_mapThreadIdToSocket;
};

#endif // TCPSERVER_H
