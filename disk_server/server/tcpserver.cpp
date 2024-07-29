#include "tcpserver.h"
#include "kernel/tcpkernel.h"

TCPServer::TCPServer()
{
    m_socklisten = 0;
    m_bFlagQuit = true;
}

TCPServer::~TCPServer()
{

}

bool TCPServer::initNetWork(const char *szip, unsigned short nport)
{
    //1.选择种类  韩餐 火锅  串串香   川菜 -- 加载库
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
        return false;
    }

    /* Confirm that the WinSock DLL supports 2.2.*/
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        printf("Could not find a usable version of Winsock.dll\n");
        unInitNetWork();
        return false;
    }
    else
        printf("The Winsock 2.2 dll was found okay\n");
    //2.雇人-店长--
    m_socklisten = socket(AF_INET,SOCK_STREAM,0);
    if(m_socklisten == INVALID_SOCKET){
        unInitNetWork("socket err");
        return false;
    }
    //3.选择地址--
    sockaddr_in addrserver;
    addrserver.sin_family = AF_INET;
    addrserver.sin_addr.S_un.S_addr = inet_addr(szip);
    addrserver.sin_port = htons(nport);
    if( SOCKET_ERROR == bind(m_socklisten,(sockaddr*)&addrserver,sizeof(addrserver))){
        unInitNetWork("bind err");
        return false;
    }
    //4.宣传--
    if( SOCKET_ERROR == listen(m_socklisten,1000)){ //  1_1_1_1______
        unInitNetWork("listen err");
        return false;
    }
    //创建接收连接的线程
    HANDLE hThread = CreateThread(0,0,&threadAccept,this,0,0);
    if(hThread)
        m_lstThread.push_back(hThread);
    return true;
}

DWORD TCPServer::threadAccept(LPVOID lpvoid)
{
    TCPServer *pthis = (TCPServer*)lpvoid;
    sockaddr_in addrclient;
    int nsize = sizeof(addrclient);
    DWORD dwThreadid;
    while(pthis->m_bFlagQuit){
        printf("wait client connect......\n");
        SOCKET sockWaiter = accept(pthis->m_socklisten,(sockaddr*)&addrclient,&nsize);
        printf("client ip:%s port:%d\n",inet_ntoa(addrclient.sin_addr),addrclient.sin_port );
        //recv---创建线程
        HANDLE hThread = CreateThread(0,0,&threadRecv,(void*)pthis,0,&dwThreadid);
        if(hThread){
            pthis->m_lstThread.push_back(hThread);
            pthis->m_mapThreadIdToSocket[dwThreadid] = sockWaiter;
        }

    }

    return true;
}

DWORD TCPServer::threadRecv(LPVOID lpvoid)
{
     TCPServer *pthis = (TCPServer*)lpvoid;
     pthis->recvData();

    return 0;
}
void TCPServer::recvData()
{
    SOCKET sockWaiter =  m_mapThreadIdToSocket[GetCurrentThreadId()];
    int nPackSize;
    int nReadNum;
    char *pszbuf;
    while(m_bFlagQuit){
        //recv
        //接收包大小
        nReadNum = recv(sockWaiter,(char*)&nPackSize,sizeof(int),0);
        //接收包内容
        if(nPackSize <=0 || nReadNum <=0){
            if(GetLastError() == 10054)
            {
                closesocket(sockWaiter);
                break;
            }
            continue;
        }

        pszbuf =new char[nPackSize]; //100M
        int offset = 0;
        while(nPackSize){
           nReadNum = recv(sockWaiter,pszbuf+offset,nPackSize,0);
            if(nReadNum >0){
               offset += nReadNum;
                nPackSize -=nReadNum;
           }
        }
        //todo
        TCPKernel::getKernel()->dealData(sockWaiter,pszbuf);
        delete []pszbuf;
        pszbuf = 0;

    }
}


void TCPServer::unInitNetWork(const char* szerr)
{
    printf(szerr);
    if(m_socklisten){
        closesocket(m_socklisten);
        m_socklisten = 0;
    }
    WSACleanup();
    //结束所有的线程
    m_bFlagQuit = false;
    auto ite = m_lstThread.begin();
    while(ite !=m_lstThread.end()){
        //判断线程状态
        if(WAIT_TIMEOUT == WaitForSingleObject(*ite,100))
            TerminateThread(*ite,-1);

        CloseHandle(*ite);
        *ite = NULL;
        ++ite;
    }

    m_lstThread.clear();
}

bool TCPServer::sendData(SOCKET sockWaiter, const char *szbuf, int nlen)
{
    //发送的包大小
    if(send(sockWaiter,(const char*)&nlen,sizeof(int),0) <=0)
        return false;
    //发送包内容
    if(send(sockWaiter,szbuf,nlen,0) <=0)
        return false;
    return true;
}





