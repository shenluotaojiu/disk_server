#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <windows.h>
#include <queue>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#define MAXLEN  10000


class itask{
public:
    itask(){

    }
    virtual ~itask(){

    }
public:
    virtual void run() = 0;
};
class threadpool
{
public:
    threadpool();
    ~threadpool();
public:
    //1.创建线程池
    bool createThreadPool(long lMinThreadNum,long lMaxThreadNum);
    //2.销毁线程池
    void destroyThreadPool();
    //3.线程函数
    static DWORD WINAPI threadproc(LPVOID lpvoid);
    //4.投递任务
    bool push(itask*);
private:
    std::queue<itask*> m_qitask; //任务队列
    std::list<HANDLE>  m_lstThread; //线程句柄
    HANDLE m_hSemaphore;
    bool   m_bFlagQuit;
    long   m_lCreateThreadNum; //创建的线程个数
    long   m_lRunThreadNum; //运行的线程个数
    long   m_lMaxThreadNum; //最大的线程个数
    HANDLE m_hMutex;
};

#endif // THREADPOOL_H
