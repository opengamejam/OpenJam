//
//  CThreadExecutor.cpp
//  TestApp
//
//  Created by yev on 8/10/15.
//
//

#include "CThreadExecutor.h"

using namespace jam;

CThreadExecutor::CThreadExecutor()
    : m_Thread(&CThreadExecutor::ThreadCall, this)
    , m_IsEnabled(true)
{
}

CThreadExecutor::~CThreadExecutor()
{
    m_IsEnabled = false;
    m_ConditionVariable.notify_one();
    m_Thread.join();
}

// Must be called on main thread
void CThreadExecutor::AddTask(const CTask& task)
{
    std::unique_lock<std::mutex> locker(m_Mutex);
    m_Tasks.push(task);
    m_ConditionVariable.notify_one();
}

uint32_t CThreadExecutor::TaskCount()
{
    std::unique_lock<std::mutex> locker(m_Mutex);
    return static_cast<uint32_t>(m_Tasks.size());
}

bool CThreadExecutor::IsEmpty()
{
    std::unique_lock<std::mutex> locker(m_Mutex);
    return m_Tasks.empty();
}

void CThreadExecutor::ThreadCall()
{
    while (m_IsEnabled) {
        std::unique_lock<std::mutex> locker(m_Mutex);
        m_ConditionVariable.wait(locker, [&]() {
            return !m_Tasks.empty() || !m_IsEnabled;
        });

        while (!m_Tasks.empty()) {
            CTask task = m_Tasks.front();
            m_Tasks.pop();

            locker.unlock();
            task.Execute();
            locker.lock();
        }
    }
}

