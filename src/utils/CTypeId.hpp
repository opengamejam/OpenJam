 //
//  CTypeId.hpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 4/27/16.
//
//

#ifndef CTYPEID_H
#define CTYPEID_H

#include "Global.h"

typedef uintptr_t typeid_t;

template <typename T>
class CTypeId final {
public:
    CTypeId() = default;
    ~CTypeId() = default;

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

#endif /* CTYPEID_H */


