#ifndef TCPKERNEL_H
#define TCPKERNEL_H

#include "ikernel.h"
#include "mysql/CMySql.h"
#include "server/tcpserver.h"
#include"Packdef.h"
class TCPKernel;

typedef void (TCPKernel::*PFUN)(SOCKET,char*);
struct MyProtocolMap
{
    char m_nType;
    PFUN m_pfun;
};

class TCPKernel : public iKernel
{
private:
    TCPKernel();
    ~TCPKernel();
    TCPKernel(const TCPKernel&) = delete;
    TCPKernel& operator=(const TCPKernel&) = delete;
public:
    virtual bool open();
    virtual void close();
    virtual void dealData(SOCKET sock,char* szbuf);
    static iKernel* getKernel();
    void registerrq(SOCKET sock,char* szbuf);
    void loginrq(SOCKET sock,char* szbuf);
    void getfilelistrq(SOCKET sock,char* szbuf);
private:
    CMySql *m_pSql = nullptr;
    Inet *m_pNet = nullptr;
    static iKernel *m_pKernel;
    const char* m_szSystemPath = "../../disk/";

};

#endif // TCPKERNEL_H
