//
//  CSemaphore.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 4/7/17.
//
//

#include "CSemaphore.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CSemaphore::CSemaphore()
: m_Count(0)
{

}

CSemaphore::~CSemaphore()
{

}

void CSemaphore::Notify() {
    std::unique_lock<decltype(m_Mutex)> lock(m_Mutex);
    ++m_Count;
    m_Condition.notify_one();
}

void CSemaphore::Wait() {
    std::unique_lock<decltype(m_Mutex)> lock(m_Mutex);
    m_Condition.wait(lock, [&]{ return m_Count > 0; });
    --m_Count;
}

bool CSemaphore::TryWait() {
    std::unique_lock<decltype(m_Mutex)> lock(m_Mutex);
    if(m_Count) {
        --m_Count;
        return true;
    }
    return false;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
