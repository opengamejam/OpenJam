//
//  IObject.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 1/10/18.
//  Copyright © 2018 Yevgeniy Logachev. All rights reserved.
//

#ifndef IOBJECT_H
#define IOBJECT_H

#include "CTypeId.hpp"

/*
 * IObject adds meta info to retrieve unique id for class of this object.
 */
template<class T>
class IObject : public std::enable_shared_from_this<T>
{
public:
    /*
     * Get class id. Insert macros JAM_OBJECT to sepcific class to override this method
     */
    virtual typeid_t GetId() const = 0;
    
    /*
     * Get unique id for current object
     */
    uint64_t GetUid()
    {
        static uint64_t idx = NextUid();
        return idx;
    }
    
    /*
     * Cast shared pointer to specific type 'T'
     */
    template<class U, class = typename std::enable_if<std::is_base_of<T, U>::value, U>::type>
    std::shared_ptr<U> Ptr()
    {
        std::shared_ptr<T> thisPtr = this->shared_from_this();
        return std::static_pointer_cast<U>(thisPtr);
    }
    
private:
    uint64_t NextUid()
    {
        std::lock_guard<std::mutex> lock(m_NextUidMutex);
        static uint64_t nextId;
        uint64_t idx = nextId;
        nextId++;
        return idx;
    }
    std::mutex m_NextUidMutex;
};

/*
 * JAM_OBJECT should be declared inside class definition. It will add
 * to class some meta information to determine unique id for this class.
 * This macros should be used with derived classes, don't use it in
 * interfaces
 */
#define JAM_OBJECT                                                                                                  \
public:                                                                                                             \
    virtual typeid_t GetId() const override                                                                         \
    {                                                                                                               \
        return CTypeId<typename std::remove_const<typename std::remove_pointer<decltype(this)>::type>::type>::Id(); \
    }

#endif /* IOBJECT_H */
