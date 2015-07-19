//
//  CColor.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 5/28/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CCOLOR_H
#define CCOLOR_H

#include "Global.h"
#include <regex>

namespace jam
{

class CColor
{
public:
    struct SRGBA
    {
        float r;
        float g;
        float b;
        float a;
    };
    
public:
    CColor();
    CColor(const SRGBA& rgba);
    CColor(float r, float g, float b, float a);
    CColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    CColor(const std::string& stringHex);
    
    float A() const;
    float R() const;
    float G() const;
    float B() const;
    
    void A(float a);
    void R(float r);
    void G(float g);
    void B(float b);
    
    void A(unsigned char a);
    void R(unsigned char r);
    void G(unsigned char g);
    void B(unsigned char b);
    
    const SRGBA& RGBA() const;
    const SRGBA& RGB() const;
    
    void RGBA(const SRGBA& rgba);
    void RGB(const SRGBA& rgb);
    
    CColor operator+ (const CColor& other) const;
    CColor& operator+= (const CColor& other);
    CColor operator* (const CColor& other) const;
    CColor& operator*= (const CColor& other);
    
private:
    SRGBA m_RGBA;
};

}; // namespace jam

#endif /* defined(CCOLOR_H) */
