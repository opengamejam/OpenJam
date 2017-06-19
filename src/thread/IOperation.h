//
//  IOperation.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 4/7/17.
//
//

#ifndef IOPERATION_H
#define IOPERATION_H

#include "Global.h"

namespace jam {
    
CLASS_PTR(IOperation)
    
class IOperation : public std::enable_shared_from_this<IOperation>
{
public:
    typedef std::list<IOperationPtr> TDependencies;
    typedef std::function<void()> TCompletionBlock;
    typedef std::list<TCompletionBlock> TCompletionBlocks;
    
public:
    /*
     * Constructor
     */
    IOperation() = default;
    
    /*
     * Destructor
     */
    virtual ~IOperation() = default;
    
    /*
     * Start operation execution
     */
    virtual void Start() = 0;

    /*
     * Cancel operation execution
     */
    virtual void Cancel() = 0;
    
    /*
     * Returns true if operation was cancelled
     */
    virtual bool IsCancelled() const = 0;
    
    /*
     * Returns true if operation started execution
     */
    virtual bool IsExecuting() const = 0;
    
    /*
     * Returns true if operation finished execution successfully
     */
    virtual bool IsFinished() const = 0;
    
    /*
     * Set 'isAsynchronous' to execute task asynchronously
     */
    virtual void Asynchronous(bool isAsynchronous) = 0;
    
    /*
     * Returns true if task performs asyncronously
     */
    virtual bool IsAsynchronous() const = 0;
    
    /*
     * Returns true if all dependencies finished
     */
    virtual bool IsReady() const = 0;
    
    /*
     * Add task dependencies. Main task won't perform execution until all of its dependent 
     * operations have finished executing. Adding operation twice won't have effect
     */
    virtual void AddDependency(IOperationPtr operation) = 0;
    
    /*
     * Remove operation from dependencies
     */
    virtual void RemoveDependency(IOperationPtr operation) = 0;
    
    /*
     * List off all dependencies
     */
    virtual TDependencies Dependecies() const = 0;
    
    /*
     * If this execution block assigned it will called after main task finished successfully
     */
    virtual void AddCompletionBlock(TCompletionBlock completionBlock) = 0;
    
    /*
     * Set operation name
     */
    virtual void Name(const std::string& name) = 0;
    
    /*
     * Returns operation name
     */
    virtual std::string Name() const = 0;
    
protected:
    /*
     * Override this method to perform main task
     */
    virtual void Main() = 0;
};
    
}; // namespace jam

#endif /* IOPERATION_H */
