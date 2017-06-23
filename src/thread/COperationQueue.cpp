//
//  COperationQueue.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 4/7/17.
//
//

#include "COperationQueue.h"
#include "IOperation.h"
#include "CThreadPool.h"

using namespace jam;

COperationQueuePtr COperationQueue::s_MainOperationQueue = nullptr;

COperationQueue::COperationQueue()
: m_MaxConcurrentOperationCount(1)
, m_IsExecuting(false)
, m_ExecutedCount(0)
, m_IsMain(false)
{

}

COperationQueue::~COperationQueue()
{

}


void COperationQueue::AddOperation(IOperationPtr operation)
{
    {
        std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
        m_Operations.push(operation);
        m_OperationsSize = m_Operations.size();
    }
    
    ExecuteTop();
}

void COperationQueue::AddOperations(const std::vector<IOperationPtr>& operations)
{
    {
        std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
        std::for_each(operations.begin(), operations.end(), [&](IOperationPtr operation){
            m_Operations.push(operation);
        });
        m_OperationsSize = m_Operations.size();
    }
    
    ExecuteTop();
}

COperationQueue::TOperations COperationQueue::Operations() const
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    return m_Operations;
}

uint64_t COperationQueue::OperationsCount()
{
    return m_OperationsSize;
}

void COperationQueue::MaxConcurrentOperationCount(uint64_t count)
{
    if (count > 0) {
        m_MaxConcurrentOperationCount = count;
    }
}

uint64_t COperationQueue::MaxConcurrentOperationCount() const
{
    return m_MaxConcurrentOperationCount;
}

void COperationQueue::Name(const std::string& name)
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    m_Name = name;
}

std::string COperationQueue::Name() const
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    return m_Name;
}

void COperationQueue::CancelAllOperations()
{
    std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
    while (m_Operations.size()) {
        IOperationPtr op = m_Operations.front();
        m_Operations.pop();
        
        op->Cancel();
    }
    m_OperationsSize = m_Operations.size();
}

COperationQueuePtr COperationQueue::CreateOperationQueue()
{
    COperationQueuePtr queue(new COperationQueue());
    return queue;
}

COperationQueuePtr COperationQueue::MainQueue()
{
    if (!s_MainOperationQueue) {
        s_MainOperationQueue = COperationQueue::CreateOperationQueue();
        s_MainOperationQueue->Name("Main Operation Queue");
        s_MainOperationQueue->SetIsMain(true);
    }
    return s_MainOperationQueue;
}

void COperationQueue::ExecuteTop()
{
    if (m_OperationsSize == 0 || m_ExecutedCount > 0) {
        return;
    }
    
    if (m_OperationsSize > 0) {
        uint64_t concurrentCount = std::min<uint64_t>(MaxConcurrentOperationCount(), m_OperationsSize);
        std::vector<IOperationPtr> concurrentOps(static_cast<size_t>(concurrentCount));
        {
            std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
            for (uint64_t i = 0; i < concurrentCount; ++i) {
                IOperationPtr op = m_Operations.front();
                m_Operations.pop();
                if (op->IsReady()) {
                    concurrentOps[static_cast<size_t>(i)] = op;
                } else {
                    // Add first not finished dependencies
                    IOperation::TDependencies deps = op->Dependecies();
                    std::all_of(deps.begin(), deps.end(),
                                [&](IOperationPtr operation) {
                        if (!operation->IsFinished()) {
                            concurrentOps[static_cast<size_t>(i)] = operation;
                            return false;
                        }
                        return true;
                    });
                    // Move back main operation while it not finished yet
                    m_Operations.push(op);
                }
            }
            m_OperationsSize = m_Operations.size();
        }
        m_ExecutedCount = concurrentOps.size();
        
        CThreadPool::ThreadType threadType = m_IsMain ? CThreadPool::Main : CThreadPool::Background;
        COperationQueueWeak weak = shared_from_this();
        CThreadPoolPtr threadPool = CThreadPool::Get();
        std::for_each(concurrentOps.begin(), concurrentOps.end(), [threadType, threadPool, weak](IOperationPtr operation){
            threadPool->RunAsync(threadType, [weak, operation](){
                operation->AddCompletionBlock([weak](){
                    COperationQueuePtr strong = weak.lock();
                    if (strong) {
                        strong->m_ExecutedCount--;
                        strong->ExecuteTop();
                    }
                });
                
                operation->Start();
            });
        });
    }
}

void COperationQueue::SetIsMain(bool isMain)
{
    m_IsMain = isMain;
}
