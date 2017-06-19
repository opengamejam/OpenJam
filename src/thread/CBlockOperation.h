//
//  CBlockOperation.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 4/7/17.
//
//

#ifndef CBLOCKOPERATION_H
#define CBLOCKOPERATION_H

#include "IOperation.h"

namespace jam {
    
CLASS_PTR(CBlockOperation)
    
class CBlockOperation : public IOperation
{
public:
    typedef std::function<void()> TExecutionBlock;
    typedef std::vector<TExecutionBlock> TExecutionBlocks;
    
public:
    /*
     * Constructor
     */
    CBlockOperation();
    
    /*
     * Destructor
     */
    ~CBlockOperation();
    
    /*
     * Add block to execution. For asynchronous execution order is not garantied
     */
    void AddExecutionBlock(const TExecutionBlock& executionBlock);
    
    /*
     * Array of all added execution blocks
     */
    TExecutionBlocks ExecutionBlocks() const;
    
    /*
     * Start operation execution
     */
    virtual void Start() override;
    
    /*
     * Cancel operation execution
     */
    virtual void Cancel() override;
    
    /*
     * Returns true if operation was cancelled
     */
    virtual bool IsCancelled() const override;
    
    /*
     * Returns true if operation started execution
     */
    virtual bool IsExecuting() const override;
    
    /*
     * Returns true if operation finished execution successfully
     */
    virtual bool IsFinished() const override;
    
    /*
     * Set 'isAsynchronous' to execute task asynchronously
     */
    virtual void Asynchronous(bool isAsynchronous) override;
    
    /*
     * Returns true if task performs asyncronously
     */
    virtual bool IsAsynchronous() const override;
    
    /*
     * Returns true if all dependencies finished
     */
    virtual bool IsReady() const override;
    
    /*
     * Add task dependencies. Main task won't perform execution until all of its dependent
     * operations have finished executing. Adding operation twice won't have effect
     */
    virtual void AddDependency(IOperationPtr operation) override;
    
    /*
     * Remove operation from dependencies
     */
    virtual void RemoveDependency(IOperationPtr operation) override;
    
    /*
     * List off all dependencies
     */
    virtual TDependencies Dependecies() const override;
    
    /*
     * If this execution block assigned it will called after main task finished successfully
     */
    virtual void AddCompletionBlock(TCompletionBlock completionBlock) override;
    
    /*
     * Set operation name
     */
    virtual void Name(const std::string& name) override;
    
    /*
     * Returns operation name
     */
    virtual std::string Name() const override;
    
protected:
    /*
     * Override this method to perform main task
     */
    virtual void Main() override;
    
private:
    TDependencies m_Dependencies;
    TCompletionBlocks m_CompletionBlock;
    TExecutionBlocks m_ExecutionBlocks;
    
    std::atomic_int m_IsCancelled;
    std::atomic_int m_IsExecuting;
    std::atomic_int m_IsFinished;
    std::atomic_int m_IsAsynchronous;
    
    std::string m_Name;
    mutable std::mutex m_Mutex;
    bool m_IsStartedFromMainThread;
};
    
}; // namespace jam

#endif /* CBLOCKOPERATION_H */
