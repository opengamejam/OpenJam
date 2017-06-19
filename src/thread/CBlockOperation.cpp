//
//  CBlockOperation.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 4/7/17.
//
//

#include "CBlockOperation.h"
#include "CThreadPool.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CBlockOperation::CBlockOperation()
: m_IsCancelled(0)
, m_IsExecuting(0)
, m_IsFinished(0)
, m_IsAsynchronous(0)
, m_IsStartedFromMainThread(true)
{

}

CBlockOperation::~CBlockOperation()
{

}

void CBlockOperation::AddExecutionBlock(const CBlockOperation::TExecutionBlock& executionBlock)
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    m_ExecutionBlocks.push_back(executionBlock);
}

CBlockOperation::TExecutionBlocks CBlockOperation::ExecutionBlocks() const
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    return m_ExecutionBlocks;
}

void CBlockOperation::Start()
{
    if (IsExecuting()) {
        return;
    }
    
    m_IsCancelled = 0;
    m_IsExecuting = 1;
    m_IsFinished = 0;
    
    CThreadPoolPtr threadPool = CThreadPool::Get();
    m_IsStartedFromMainThread = threadPool->IsMainThread();
    if (IsAsynchronous() != m_IsStartedFromMainThread) {
        Main();
    } else {
        IOperationPtr thisPtr = shared_from_this();
        bool isBackground = IsAsynchronous() && m_IsStartedFromMainThread;
        CThreadPool::ThreadType threadType = (isBackground ? CThreadPool::Background : CThreadPool::Main);
        threadPool->RunAsync(threadType, [&](){
            Main();
        });
    }
}

void CBlockOperation::Cancel()
{
    m_IsCancelled = 1;
}

bool CBlockOperation::IsCancelled() const
{
    return m_IsCancelled;
}

bool CBlockOperation::IsExecuting() const
{
    return m_IsExecuting;
}

bool CBlockOperation::IsFinished() const
{
    return m_IsFinished;
}

void CBlockOperation::Asynchronous(bool isAsynchronous)
{
    if (!IsExecuting()) {
        m_IsAsynchronous = isAsynchronous;
    }
}

bool CBlockOperation::IsAsynchronous() const
{
    return m_IsAsynchronous;
}

bool CBlockOperation::IsReady() const
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    return m_Dependencies.size() == 0;
}

void CBlockOperation::AddDependency(IOperationPtr operation)
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    
    TDependencies::const_iterator it = std::find(m_Dependencies.begin(), m_Dependencies.end(), operation);
    if (it == m_Dependencies.end()) {
        m_Dependencies.push_back(operation);
    }
}

void CBlockOperation::RemoveDependency(IOperationPtr operation)
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    m_Dependencies.remove(operation);
}

IOperation::TDependencies CBlockOperation::Dependecies() const
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    return m_Dependencies;
}

void CBlockOperation::AddCompletionBlock(IOperation::TCompletionBlock completionBlock)
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    m_CompletionBlock.push_back(completionBlock);
}

void CBlockOperation::Name(const std::string& name)
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    m_Name = name;
}

std::string CBlockOperation::Name() const
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    return m_Name;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CBlockOperation::Main()
{
    TDependencies dependencies;
    {
        std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
        m_Dependencies.swap(dependencies);
    }
    std::for_each(dependencies.begin(), dependencies.end(), [&](IOperationPtr dependencyOp) {
        if (m_IsCancelled != 0) {
            dependencyOp->Cancel();
        } else {
            dependencyOp->Asynchronous(IsAsynchronous());
            dependencyOp->Start();
        }
    });
    
    if (m_IsCancelled == 0) {
        std::all_of(m_ExecutionBlocks.begin(), m_ExecutionBlocks.end(), [&](const TExecutionBlock& executionBlock) {
            executionBlock();
            
            return (m_IsCancelled != 0);
        });
    }
    
    m_IsFinished = !m_IsCancelled;
    m_IsExecuting = 0;
    
    // Notify that task completed
    if (!m_IsCancelled) {
        CThreadPoolPtr threadPool = CThreadPool::Get();
        CThreadPool::ThreadType threadType = m_IsStartedFromMainThread ? CThreadPool::Main : CThreadPool::Background;
        std::for_each(m_CompletionBlock.begin(), m_CompletionBlock.end(),
                      [threadPool, threadType](const IOperation::TCompletionBlock& block) {
            threadPool->RunAsync(threadType, block);
        });
    }
}

// *****************************************************************************
// Private Methods
// *****************************************************************************
