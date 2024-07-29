#include "tcpkernel.h"

iKernel* TCPKernel::m_pKernel = new TCPKernel();

// 可以减少代码更改
_BEGIN_PROTOCOL_MAP
    PM(_default_login_rq,&TCPKernel::loginrq)
    PM(_default_register_rq,&TCPKernel::registerrq)
    PM(_default_getfilelist_rq,&TCPKernel::getfilelistrq)
_END_PROTOCOL_MAP

TCPKernel::TCPKernel()
{
    m_pSql = new CMySql();
    m_pNet = new TCPServer();
}

TCPKernel::~TCPKernel()
{
    delete m_pNet;
    delete m_pSql;
}

bool TCPKernel::open()
{
    if(!m_pSql->ConnectMySql(host,user,pass,db))
    {
        printf("mysql connect error\n");
        return false;
    }
    if(!m_pNet->initNetWork(ip,port))
    {
        printf("net init error\n");
        return false;
    }

    return true;
}

void TCPKernel::close()
{
    m_pSql->DisConnect();
    m_pNet->unInitNetWork();
}

void TCPKernel::dealData(SOCKET sock, char *szbuf)
{
    // // 判断包中第一个字节
    // switch(*szbuf)
    // {
    // case _default_register_rq:
    //     registerrq(sock,szbuf);
    //     break;
    // case _default_login_rq:
    //     loginrq(sock,szbuf);
    //     break;
    // case _default_getfilelist_rq:
    //     getfilelistrq(sock,szbuf);
    //     break;
    // }

    // 包映射表
    int count=0;
    while(1)
    {
        if(g_PMEntries[count].m_nType == *szbuf)
        {
            (this->*g_PMEntries[count].m_pfun)(sock,szbuf);
            break;
        }
        else if(g_PMEntries[count].m_nType == 0 && g_PMEntries[count].m_pfun == 0)
            break;

        count++;
    }

}

iKernel *TCPKernel::getKernel()
{
    // if(!m_pKernel)
    //     m_pKernel = new TCPKernel();

    return m_pKernel;
}

void TCPKernel::registerrq(SOCKET sock, char *szbuf)
{
    // 将数据写入数据库
    STRU_REGISTER_RQ* psrr = (STRU_REGISTER_RQ*)szbuf;
    char szsql[SQLLEN] = {0};
    sprintf(szsql,"insert into user(u_tel,u_name,u_password) values(%lld,'%s','%s')",
            psrr->m_tel,psrr->m_szName,psrr->m_szPassword);
    bool flag = m_pSql->UpdateMySql(szsql);
    if(flag)
    {
        list<string> lststr;
        sprintf(szsql,"select u_id from user where u_tel = %lld",psrr->m_tel);
        m_pSql->SelectMySql(szsql,1,lststr);
        if(lststr.size() > 0)
        {
            string strUserId = lststr.front();
            lststr.pop_front();
            char szPath[MAX_PATH] = {0};
            sprintf(szPath,"%s%s",m_szSystemPath,strUserId.c_str());
            CreateDirectoryA(szPath,0); //注册成功后为用户创建一个目录
        }
    }
    STRU_REGISTER_RS srr;
    srr.m_szResult = flag ? _register_success : _register_err;
    // 向客户端发送回复
    m_pNet->sendData(sock,(const char*)&srr,sizeof(srr));
}

void TCPKernel::loginrq(SOCKET sock, char *szbuf)
{
    list<string> lststr;
    STRU_LOGIN_RQ* pslr = (STRU_LOGIN_RQ*) szbuf;
    char szsql[SQLLEN] = {0};
    sprintf(szsql,"select u_password,u_id from user where u_name = '%s'",pslr->m_szName);
    m_pSql->SelectMySql(szsql,2,lststr);
    // 正确，错误
    STRU_LOGIN_RS sls;
    sls.m_szResult = _login_usernoexists;   // 没查到用户名
    if(lststr.size() > 0)                   // 查到了
    {
        string u_pw = lststr.front();lststr.pop_front();
        string u_id = lststr.front();lststr.pop_front();
        if(u_pw == pslr->m_szPassword)      // 密码匹配成功
        {
            sls.m_szResult = _login_success;
            sls.m_userID = atoll(u_id.c_str());
        }
        else                                // 密码错误
            sls.m_szResult = _login_password_err;
    }
    m_pNet->sendData(sock,(const char*)&sls,sizeof(sls));
}

void TCPKernel::getfilelistrq(SOCKET sock, char *szbuf)
{
    STRU_GETFILELIST_RQ *psgr = (STRU_GETFILELIST_RQ*)szbuf;
    list<string> lststr;
    char szsql[SQLLEN];
    sprintf(szsql,"select f_name,f_size,f_updatetime from myview where u_id = %lld",psgr->m_userID);
    m_pSql->SelectMySql(szsql,3,lststr);
    STRU_GETFILELIST_RS sgr;
    int count = 0;
    while(lststr.size())        // 处理
    {
        // string strFilename = lststr.front();lststr.pop_front();
        // string strFilesize = lststr.front();lststr.pop_front();
        // string strFileupdatime = lststr.front();lststr.pop_front();

        // test1
        // strcpy(sgr.m_aryFile[count].m_szFileName , lststr.front().c_str());lststr.pop_front();
        // sgr.m_aryFile[count].m_llFileSize = atoll(lststr.front().c_str());lststr.pop_front();
        // strcpy(sgr.m_aryFile[count].m_szDateTime , lststr.front().c_str());lststr.pop_front();
        // count++;

        FILEINFO fi;
        strcpy(fi.m_szFileName , lststr.front().c_str());lststr.pop_front();
        fi.m_llFileSize = atoll(lststr.front().c_str());lststr.pop_front();
        strcpy(fi.m_szDateTime , lststr.front().c_str());lststr.pop_front();
        /*
         * sgr.m_aryFile[count++] = fi; 是否会有问题
        */
        sgr.m_aryFile[count++] = std::move(fi);
        if(count == MAXFILENUM || lststr.size() == 0)   //如果数目已满，或者查到最后，发送数据
        {
            sgr.m_nFileNum = count;
            count = 0;
            m_pNet->sendData(sock,(char*)&sgr,sizeof(sgr));
        }
    }

}


























