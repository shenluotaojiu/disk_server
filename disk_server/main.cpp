#include <QCoreApplication>
#include "./kernel/tcpkernel.h"
#include "Packdef.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    iKernel* i = TCPKernel::getKernel();
    i->set_sql_parameter("127.0.0.1","root","123456","0724_405");
    i->set_inet_parameter("127.0.0.1",1234);
    i->open();

    // 测试登录
    // STRU_REGISTER_RQ srr;
    // srr.m_tel = 114514;
    // strcpy(srr.m_szName,"114514");
    // strcpy(srr.m_szPassword,"114514");
    // i->dealData(0,(char*)&srr);

    // 测试注册
    // STRU_LOGIN_RQ slr;
    // strcpy(slr.m_szName,"114514");
    // strcpy(slr.m_szPassword,"114514-");
    // i->dealData(0,(char*)&slr);

    // 这是用来测试版本穿梭的一条注释

    // 测试显示文件
    STRU_GETFILELIST_RQ sgr;
    sgr.m_userID = 1;\
    i->dealData(0,(char*)&sgr);

    return a.exec();
}
