//
//  CThreadPool.cpp
//  TestApp
//
//  Created by yev on 8/10/15.
//
//

#include "CThreadPool.h"

using namespace jam;

CThreadPoolPtr CThreadPool::s_Instance = nullptr;
std::thread::id CThreadPool::s_MainThreadId;

CThreadPool::CThreadPool()
{
    s_MainThreadId = std::this_thread::get_id();
}

CThreadPool::~CThreadPool()
{

}

CThreadPoolPtr CThreadPool::Get()
{
    if (!s_Instance)
    {
        s_Instance.reset(new CThreadPool());
    }
    
    return s_Instance;
}

bool CThreadPool::IsMainThread()
{
    return (s_MainThreadId == std::this_thread::get_id());
}

void CThreadPool::Initialize(uint32_t threadsNum)
{
    threadsNum = std::max<uint32_t>(1, threadsNum);
    
    std::unique_lock<std::mutex> locker(m_Mutex);
    
    m_ThreadExecutors.clear();
    for (uint32_t i = 0; i < threadsNum; ++i)
    {
        CThreadExecutorPtr executor = CThreadExecutor::Create();
        m_ThreadExecutors.push_back(executor);
    }
}

void CThreadPool::Shutdown()
{
    std::unique_lock<std::mutex> locker(m_Mutex);
    m_ThreadExecutors.clear();
}

void CThreadPool::RunAsync(ThreadType threadType, const CThreadExecutor::TExecuteBlock& block)
{
    if (!block)
    {
        return;
    }
    
    if (threadType == CThreadPool::Main)
    {
        if (!IsMainThread())
        {
            std::unique_lock<std::mutex> locker(m_Mutex);
            m_MainThreadTasks.push(block);
        }
        else
        {
            block();
        }
    }
    else
    {
        CThreadExecutorPtr executor = FindLeisureExecutor();
        if (executor)
        {
            executor->AddTask(block);
        }
    }
}

void CThreadPool::Update(unsigned long dt)
{
    CThreadExecutor::TExecuteBlock block = nullptr;
    do
    {
        block = nullptr;
        {
            std::unique_lock<std::mutex> locker(m_Mutex);
            if (!m_MainThreadTasks.empty())
            {
                block = m_MainThreadTasks.front();
                m_MainThreadTasks.pop();
            }
        }
        
        if (block)
        {
            block();
        }
    }
    while (block);
}

CThreadExecutorPtr CThreadPool::FindLeisureExecutor()
{
    CThreadExecutorPtr executor = nullptr;
    uint32_t minTasks = UINT32_MAX;
    
    std::all_of(m_ThreadExecutors.begin(), m_ThreadExecutors.end(), [&](CThreadExecutorPtr ex)
    {
        uint32_t taskCount = ex->TaskCount();
        
        if (taskCount == 0)
        {
            executor = ex;
            return false;
        }
        else if (minTasks > taskCount)
        {
            minTasks = taskCount;
            executor = ex;
        }
        
        return true;
    });
    
    return executor;
}
