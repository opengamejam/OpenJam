 //
//  CTypeId.hpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 4/27/16.
//
//

#ifndef CTYPEID_H
#define CTYPEID_H

typedef uintptr_t typeid_t;

template <typename T>
class CTypeId {
public:
    CTypeId(){};
    ~CTypeId(){};

    static typeid_t Id()
    {
        static char const type_id = '\0';
        return reinterpret_cast<typeid_t>(&type_id);
    }

    static const std::string& Name()
    {
        static std::string name = ToString(Id());
        return name;
    }

private:
    static std::string ToString(typeid_t value)
    {
        std::stringstream ss;
        ss << value;

        return ss.str();
    }
};

/* 
 * JAM_OBJECT_BASE add meta info to retrieve unique id to basic class,
 * but return nothing. All derived classes should use JAM_OBJECT to be 
 * able retrieve its unique identifiers
 * Don't use JAM_OBJECT_BASE in derived classes, only interfaces
 */
#define JAM_OBJECT_BASE                                     \
public:                                                     \
    virtual typeid_t GetId() const                          \
    {                                                       \
        assert(false);                                      \
        return 0;                                           \
    }                                                       \
                                                            \
    uint64_t GetUid()                                       \
    {                                                       \
        static uint64_t idx = NextUid();                    \
        return idx;                                         \
    }                                                       \
                                                            \
private:                                                    \
    uint64_t NextUid()                                      \
    {                                                       \
        std::lock_guard<std::mutex> lock(m_NextUidMutex);   \
        static uint64_t nextId;                             \
        uint64_t idx = nextId;                              \
        nextId++;                                           \
        return idx;                                         \
    }                                                       \
    std::mutex m_NextUidMutex;

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

#endif /* CTYPEID_H */


