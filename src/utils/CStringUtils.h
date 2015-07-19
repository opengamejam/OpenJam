//
//  CStringUtils.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 11/19/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CSTRINGUTILS_H
#define CSTRINGUTILS_H

#include "Global.h"

namespace jam
{
    void SplitString(std::vector<std::string>& tokens, const std::string& text, char delimeter);
    std::string ReplaceString(std::string string, const std::string& search, const std::string& replace);
    
    template <class T>
    std::string ToString(T value)
    {
        std::stringstream ss;
        ss << value;
        
        return ss.str();
    }
    
    template <class T>
    T FromString(const std::string& string)
    {
        std::stringstream ss(string);
        
        T result;
        ss >> result;
        
        return result;
    }
};

#endif /* defined(CSTRINGUTILS_H) */
