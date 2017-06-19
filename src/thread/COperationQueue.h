//
//  COperationQueue.hpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 4/7/17.
//
//

#ifndef COPERATIONQUEUE_H
#define COPERATIONQUEUE_H

#include "Global.h"

namespace jam {
    
CLASS_PTR(IOperation)
CLASS_PTR(COperationQueue)
    
class COperationQueue final
{
    friend class COperationQueueManager;
public:
    typedef std::queue<IOperationPtr> TOperations;
    
public:
    /*
     * Desctructor
     */
    ~COperationQueue();
    
    /*
     * Add new operation
     */
    void AddOperation(IOperationPtr operation);
    
    /*
     * Add new operations from array
     */
    void AddOperations(const std::vector<IOperationPtr>& operations);
    
    /*
     * Get copy array of operations
     */
    TOperations Operations() const;
    
    /*
     * Returns operations count
     */
    uint64_t OperationsCount();
    
    /*
     * Set number of operations that can be executes simultanously
     */
    void MaxConcurrentOperationCount(uint64_t count);
    
    /*
     * Returns number of operations that can be executes simultanously
     */
    uint64_t MaxConcurrentOperationCount() const;
    
    /*
     * Set queue name
     */
    void Name(const std::string& name);
    
    /*
     * Returns queue name
     */
    std::string Name() const;
    
    /*
     * Cancel all operations
     */
    void CancelAllOperations();
    
    /*
     * Create new operation queue
     */
    static COperationQueuePtr CreateOperationQueue();
    
    /*
     * Get main operation queue
     */
    static COperationQueuePtr MainQueue();
    
private:
    /*
     * Constructor
     */
    COperationQueue();
    
    void ExecuteTop();
    
private:
    TOperations m_Operations;
    std::atomic_uint_fast64_t m_OperationsSize;
    mutable std::mutex m_Mutex;
    std::atomic_uint_fast64_t m_MaxConcurrentOperationCount;
    std::string m_Name;
    std::atomic_bool m_IsExecuting;
    
    std::atomic_uint_fast64_t m_MaxConcurrent;
    std::atomic_uint_fast64_t m_ExecutedCount;
    
    
    static COperationQueuePtr s_MainOperationQueue;
};
    
}; // namespace jam

#endif /* COPERATIONQUEUE_H */
