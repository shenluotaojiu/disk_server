#ifndef TCPKERNEL_H
#define TCPKERNEL_H

#include "ikernel.h"
#include "mysql/CMySql.h"
#include "server/tcpserver.h"
#include "Packdef.h"
#include <list>
class TCPKernel;

typedef void (TCPKernel::*PFUN)(SOCKET,char*);
struct MyProtocolMap
{
    char m_nType;
    PFUN m_pfun;
};

struct stru_fileinfo
{
    stru_fileinfo() {}
    FILE* m_pfile;
    long long m_filesize;
    long long m_userid;
    long long m_fileid;
    long long m_filepos;
};

class TCPKernel : public iKernel
{
private:
    TCPKernel();
    ~TCPKernel();
public:
    virtual bool open();
    virtual void close();
    virtual void dealData(SOCKET sock,char* szbuf);
    static iKernel* getKernel();
    void registerrq(SOCKET sock,char* szbuf);
    void loginrq(SOCKET sock,char* szbuf);
    void getfilelistrq(SOCKET sock,char* szbuf);
    void uploadfileinforq(SOCKET sock,char* szbuf);
    void uploadfileblockrq(SOCKET sock,char* szbuf);
private:
    CMySql *m_pSql = nullptr;
    Inet *m_pNet = nullptr;
    static iKernel *m_pKernel;
    const char* m_szSystemPath = "D:/code_practice/Web_disk/disk/";
    std::map<long long,stru_fileinfo*> m_mapFileIdtoFileInfo;
};

#endif // TCPKERNEL_H
