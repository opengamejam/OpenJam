//
//  CThreadPool.cpp
//  TestApp
//
//  Created by yev on 8/10/15.
//
//

#include "CThreadPool.h"
#include "CSemaphore.h"

using namespace jam;

CThreadPoolPtr CThreadPool::s_Instance = nullptr;
std::thread::id CThreadPool::s_MainThreadId;

CThreadPool::CThreadPool()
: m_MainTasksCount(0)
{
}

CThreadPool::~CThreadPool()
{
}

CThreadPoolPtr CThreadPool::Get()
{
    if (!s_Instance) {
        s_Instance.reset(new CThreadPool());
        s_MainThreadId = std::this_thread::get_id();
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
    for (uint32_t i = 0; i < threadsNum; ++i) {
        CThreadExecutorPtr executor(new CThreadExecutor());
        m_ThreadExecutors.push_back(executor);
    }
}

void CThreadPool::Shutdown()
{
    std::unique_lock<std::mutex> locker(m_Mutex);
    m_ThreadExecutors.clear();
}

void CThreadPool::RunAsync(ThreadType threadType, CTask::TExecuteBlock block)
{
    if (!block) {
        return;
    }

    if (threadType == CThreadPool::Main) {
        if (!IsMainThread()) {
            std::unique_lock<std::mutex> locker(m_Mutex);
            m_MainThreadTasks.push(CTask(block));
            m_MainTasksCount++;
        } else {
            block();
        }
    } else {
        CThreadExecutorPtr executor = FindLeisureExecutor();
        if (executor) {
            executor->AddTask(CTask(block));
        }
    }
}

void CThreadPool::RunSync(ThreadType threadType, CTask::TExecuteBlock block)
{
    if (!block) {
        return;
    }
    
    if (threadType == CThreadPool::Main) {
        if (!IsMainThread()) {
            CSemaphorePtr semaphore(new CSemaphore);
            {
                std::unique_lock<std::mutex> locker(m_Mutex);
                m_MainThreadTasks.push(CTask(semaphore, block));
                m_MainTasksCount++;
            }
            semaphore->Wait();
        } else {
            block();
        }
    } else {
        CThreadExecutorPtr executor = FindLeisureExecutor();
        if (executor) {
            CSemaphorePtr semaphore(new CSemaphore);
            executor->AddTask(CTask(semaphore, block));
            semaphore->Wait();
        }
    }
}

void CThreadPool::Update(unsigned long dt)
{
    while (m_MainTasksCount) {
        CTask task;
        {
            std::unique_lock<std::mutex> locker(m_Mutex);
            if (!m_MainThreadTasks.empty()) {
                task = m_MainThreadTasks.front();
                m_MainThreadTasks.pop();
                m_MainTasksCount--;
            }
        }
        task.Execute();
    }
}

CThreadExecutorPtr CThreadPool::FindLeisureExecutor()
{
    CThreadExecutorPtr executor = nullptr;
    uint32_t minTasks = std::numeric_limits<uint32_t>::max();

    std::all_of(m_ThreadExecutors.begin(), m_ThreadExecutors.end(), [&](CThreadExecutorPtr ex) {
        uint32_t taskCount = ex->TaskCount() + static_cast<uint32_t>(ex->IsExecuting());

        if (taskCount == 0) {
            executor = ex;
            return false;
        } else if (minTasks > taskCount) {
            minTasks = taskCount;
            executor = ex;
        }

        return true;
    });

    return executor;
}
