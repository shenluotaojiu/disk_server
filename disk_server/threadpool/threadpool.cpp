#include "threadpool.h"

threadpool::threadpool()
{
    m_bFlagQuit = true;
    m_lRunThreadNum = 0;
    m_lCreateThreadNum = 0;
    m_lMaxThreadNum = 0;
}

threadpool::~threadpool()
{

}

bool threadpool::createThreadPool(long lMinThreadNum, long lMaxThreadNum)
{
    //校验参数
    if(lMinThreadNum <=0 || lMaxThreadNum <lMinThreadNum){
        printf("thread num is in(1-100)\n");
        return false;
    }
    //初始化线程资源
    m_hSemaphore = CreateSemaphore(0,0,MAXLEN,0);
    m_hMutex = CreateMutex(0,0,0);
    //1.创建最小个数的线程
    for(long i =0; i < lMinThreadNum;i++){
        HANDLE hThread = CreateThread(0,0,&threadproc,this,0,0);
        if(hThread){
            m_lstThread.push_back(hThread);
        }
    }
    m_lMaxThreadNum = lMaxThreadNum;
    m_lCreateThreadNum=lMinThreadNum;
    return true;
}

void threadpool::destroyThreadPool()
{
    //销毁所有的线程
    m_bFlagQuit = false;
    auto ite = m_lstThread.begin();
    while(ite != m_lstThread.end()){
        if(WAIT_TIMEOUT == WaitForSingleObject(*ite,100))
            TerminateThread(*ite,-1);

        CloseHandle(*ite);
        *ite = NULL;
        ite++;
    }
    m_lstThread.clear();

    if(m_hSemaphore){
        CloseHandle(m_hSemaphore);
        m_hSemaphore = NULL;
    }
    //将任务全部释放
    itask *p = NULL;
    while(!m_qitask.empty()){
        p= m_qitask.front();
        m_qitask.pop();
        delete p;
        p = NULL;
    }
}

DWORD threadpool::threadproc(LPVOID lpvoid)
{
    threadpool *pthis = (threadpool*)lpvoid;
    itask* ptask = NULL;
    while(pthis->m_bFlagQuit){
        //等信号量
        WaitForSingleObject(pthis->m_hSemaphore,INFINITE);
        //pthis->m_lRunThreadNum++;
        InterlockedIncrement(&pthis->m_lRunThreadNum);
        //从任务队列中取出任务
        if(!pthis->m_qitask.empty()){
            WaitForSingleObject(pthis->m_hMutex,INFINITE);
            if(pthis->m_qitask.empty()){
                ReleaseMutex(pthis->m_hMutex);
                break;
            }
            ptask = pthis->m_qitask.front();
            pthis->m_qitask.pop();
            ReleaseMutex(pthis->m_hMutex);
            //执行任务
            ptask->run();
            //释放任务
            delete ptask;
        }


        InterlockedDecrement(&pthis->m_lRunThreadNum);
    }
    return 0;
}

bool threadpool::push(itask *ptask)
{
    if(!ptask)
        return false;
    //1.将任务加入队列
    WaitForSingleObject(m_hMutex,INFINITE);
    m_qitask.push(ptask);
    ReleaseMutex(m_hMutex);

  //  1.如果有空闲服务员

    if(m_lRunThreadNum == m_lCreateThreadNum &&
               m_lCreateThreadNum < m_lMaxThreadNum  ){
   // 2.服务员都属于忙碌状态， 但是没有上限
         //创建新的线程
        HANDLE hThread = CreateThread(0,0,&threadproc,this,0,0);
        if(hThread){
            m_lstThread.push_back(hThread);
        }
        ++m_lCreateThreadNum;
    }
   //3.爆满


   //释放信号量
    ReleaseSemaphore(m_hSemaphore,1,0);
    return true;
}
