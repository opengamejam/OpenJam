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
    
class CStringUtils
{
public:
    CStringUtils() = default;
    ~CStringUtils() = default;
    
    static void SplitString(std::vector<std::string>& tokens, const std::string& text, char delimeter);
    static std::string ReplaceString(std::string string, const std::string& search, const std::string& replace);
    
    template <class T>
    static std::string ToString(T value)
    {
        std::stringstream ss;
        ss << value;
        
        return ss.str();
    }
    
    template <class T>
    static T FromString(const std::string& string)
    {
        std::stringstream ss(string);
        
        T result;
        ss >> result;
        
        return result;
    }
    
    static bool StringEndsWith(std::string const& fullString, std::string const& ending);
    static bool StringStartsWith(std::string const& fullString, std::string const& starting);
    
    static glm::vec2 ToVec2(const std::string& value, char delimeter = ',');
    static glm::vec3 ToVec3(const std::string& value, char delimeter = ',');
    
}; // CStringUtils
    
}; // namespace jam

#endif /* defined(CSTRINGUTILS_H) */
