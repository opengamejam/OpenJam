//
//  CStringUtils.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 11/19/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CStringUtils.h"

void jam::SplitString(std::vector<std::string>& tokens, const std::string& text, char delimeter)
{
    size_t start = 0;
    size_t end = 0;
    while ((end = text.find(delimeter, start)) != std::string::npos)
    {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
}

std::string jam::ReplaceString(std::string string, const std::string& from, const std::string& to)
{
    size_t pos = 0;
    while ((pos = string.find(from, pos)) != std::string::npos)
    {
        string.replace(pos, from.length(), to);
        pos += to.length();
    }
    return string;
}
