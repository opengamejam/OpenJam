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
    if (IsFinished()) {
        return;
    }
    
    if (!IsReady()) {
        assert("Cannot start operation when it is not ready" && false);
        return;
    }
    
    if (!IsExecuting()) {
        Main();
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

bool CBlockOperation::IsAsynchronous() const
{
    return false;
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
    m_IsExecuting = 1;
    
    TExecutionBlocks copyExecutions;
    TCompletionBlocks copyCompletions;
    {
        std::lock_guard<decltype(m_Mutex)> lock(m_Mutex);
        m_ExecutionBlocks.swap(copyExecutions);
        m_CompletionBlock.swap(copyCompletions);
    }
    
    CBlockOperationWeak weakPtr = std::static_pointer_cast<CBlockOperation>(shared_from_this());
    if (!IsCancelled()) {
        std::all_of(copyExecutions.begin(), copyExecutions.end(), [weakPtr](const TExecutionBlock& executionBlock) {
            CBlockOperationPtr strongPtr = weakPtr.lock();
            if (strongPtr) {
                executionBlock();
                
                return strongPtr->IsCancelled();
            }
            
            return true;
        });
    }
    
    m_IsFinished = 1;
    
    // Notify that task completed
    std::for_each(copyCompletions.begin(), copyCompletions.end(),
                  [](const IOperation::TCompletionBlock& completionBlock) {
        completionBlock();
    });
    
    m_IsExecuting = 0;
}

// *****************************************************************************
// Private Methods
// *****************************************************************************
