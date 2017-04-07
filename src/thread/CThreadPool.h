//
//  CThreadPool.h
//  TestApp
//
//  Created by yev on 8/10/15.
//
//

#ifndef CTHREADPOOL_H
#define CTHREADPOOL_H

#include "Global.h"
#include "CThreadExecutor.h"

namespace jam {
CLASS_PTR(CThreadPool)
CLASS_PTR(CThreadExecutor)
CLASS_PTR(CSemaphore)

class CThreadPool final {
public:
    enum ThreadType {
        Main,
        Background
    };

public:
    /*
     * Constructor
     */
    CThreadPool();
    
    /*
     * Destructor
     */
    ~CThreadPool();

    /*
     * Create and get instance of main thread pool. Call it first time on main thread
     */
    static CThreadPoolPtr Get();
    
    /*
     * Returns true if calling thing method thread is main thread
     */
    static bool IsMainThread();

    /*
     * Initialize number of background threads
     */
    void Initialize(uint32_t threadsNum = 1);
    
    /*
     * Shutdown background threads
     */
    void Shutdown();

    /*
     * Run execution block asynchronously.
     */
    void RunAsync(ThreadType threadType, const CTask::TExecuteBlock& block);
    
    /*
     * Run execution block synchronously. Don't call it from main thread because dedalock will happened
     */
    void RunSync(ThreadType threadType, const CTask::TExecuteBlock& block);
    
    /*
     * Call it from main thread.
     */
    void Update(unsigned long dt);

private:
    CThreadExecutorPtr FindLeisureExecutor();

private:
    static CThreadPoolPtr s_Instance;
    static std::thread::id s_MainThreadId;

    std::mutex m_Mutex;
    std::vector<CThreadExecutorPtr> m_ThreadExecutors;
    std::queue<CTask> m_MainThreadTasks;
    std::atomic_uint_fast64_t m_MainTasksCount;
    std::queue<CSemaphorePtr> m_Semaphores;
};

}; // namespace jam

#endif /* defined(CTHREADPOOL_H) */
