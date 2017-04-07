//
//  CTask.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 4/7/17.
//
//

#include "CTask.h"
#include "CSemaphore.h"

using namespace jam;

CTask::CTask()
: m_ExecuteBlock(nullptr)
, m_Semaphore(nullptr)
{

}

CTask::CTask(const TExecuteBlock& executeBlock)
: m_ExecuteBlock(executeBlock)
, m_Semaphore(nullptr)
{
}

CTask::CTask(CSemaphorePtr semaphore, const TExecuteBlock& executeBlock)
: m_ExecuteBlock(executeBlock)
, m_Semaphore(semaphore)
{
}

CTask::~CTask()
{

}

void CTask::Execute()
{
    if (m_ExecuteBlock) {
        m_ExecuteBlock();
        if (m_Semaphore) {
            m_Semaphore->Notify();
        }
    }
}
