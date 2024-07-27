#ifndef IKERNEL_H
#define IKERNEL_H

#include <winsock2.h>

class iKernel
{
public:
    iKernel() = default;
    virtual ~iKernel() = default;

public:
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual void dealData(SOCKET sock,char* szbuf) = 0;
    void set_sql_parameter(const char* host,const char* user,const char* pass,const char* db)
    {
        this->host = host;
        this->db = db;
        this->pass = pass;
        this->user = user;
    }
    void set_inet_parameter(const char* ip,unsigned short port)
    {
        this->ip = ip;
        this->port = port;
    }
protected:


protected:
    const char *host = nullptr;
    const char *user = nullptr;
    const char *pass = nullptr;
    const char *db = nullptr;
    const char *ip = nullptr;
    unsigned short port = 0;
};


#endif // IKERNEL_H
