//
//  CColor.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 5/28/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CColor.h"

using namespace jam;

static const std::regex s_HexMatcher("^#?(([0-9a-fA-F]{2}){4}|([0-9a-fA-F]){4})$");

CColor::CColor()
    : m_RGBA({1.0f, 1.0f, 1.0f, 1.0f})
{

}

CColor::CColor(const SRGBA& rgba)
    : m_RGBA(rgba)
{
}

CColor::CColor(float r, float g, float b, float a)
    : m_RGBA({r, g, b, a})
{

}

CColor::CColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : m_RGBA({r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f})
{

}

CColor::CColor(const std::string& stringHex)
    : m_RGBA({1.0f, 1.0f, 1.0f, 1.0f})
{
    if (std::regex_match(stringHex, s_HexMatcher))
    {
        std::string pureHex = stringHex;
        if (pureHex[0] == '#')
        {
            pureHex = pureHex.substr(1, pureHex.length() - 1);
        }
        
        int byte;
        std::stringstream converter;
        size_t len = pureHex.length() / 2;
        for (unsigned int i = 0; i < len; ++i)
        {
            std::string component = pureHex.substr(i * 2, 2);
            sscanf(component.c_str(), "%x", &byte);
            
            switch (i) {
                case 0:
                    m_RGBA.r = byte / 255.0f;
                    break;
                case 1:
                    m_RGBA.g = byte / 255.0f;
                    break;
                case 2:
                    m_RGBA.b = byte / 255.0f;
                    break;
                case 3:
                    m_RGBA.a = byte / 255.0f;
                    break;
            }
            
        }
    }
}

float CColor::A() const
{
    return m_RGBA.a;
}

float CColor::R() const
{
    return m_RGBA.r;
}

float CColor::G() const
{
    return m_RGBA.g;
}

float CColor::B() const
{
    return m_RGBA.b;
}

void CColor::A(float a)
{
    m_RGBA.a = a;
}

void CColor::R(float r)
{
    m_RGBA.r = r;
}

void CColor::G(float g)
{
    m_RGBA.g = g;
}

void CColor::B(float b)
{
    m_RGBA.b = b;
}

void CColor::A(unsigned char a)
{
    m_RGBA.a = a / 255.0f;
}

void CColor::R(unsigned char r)
{
    m_RGBA.r = r / 255.0f;
}

void CColor::G(unsigned char g)
{
    m_RGBA.g = g / 255.0f;
}

void CColor::B(unsigned char b)
{
    m_RGBA.b = b / 255.0f;
}

const CColor::SRGBA& CColor::RGBA() const
{
    return m_RGBA;
}

/*const CColor::SRGBA& CColor::RGB() const
{
    static CColor::SRGBA rgb;
    rgb = CColor::SRGBA({R(), G(), B(), 1.0f});
    
    return rgb;
}*/

void CColor::RGBA(const SRGBA& rgba)
{
    m_RGBA = rgba;
}

/*void CColor::RGB(const SRGBA& rgb)
{
    float a = m_RGBA.a;
    m_RGBA = rgb;
    m_RGBA.a = a;
}*/

CColor CColor::operator+ (const CColor& other) const
{
    float a = std::min(1.0f, A() + other.A());
    float r = std::min(1.0f, R() + other.R());
    float g = std::min(1.0f, G() + other.G());
    float b = std::min(1.0f, B() + other.B());
    
    return CColor(r, g, b, a);
}

CColor& CColor::operator+= (const CColor& other)
{
    A(std::min(1.0f, A() + other.A()));
    R(std::min(1.0f, R() + other.R()));
    G(std::min(1.0f, G() + other.G()));
    B(std::min(1.0f, B() + other.B()));
    
    return *this;
}

CColor CColor::operator* (const CColor& other) const
{
    float a = std::min(1.0f, A() * other.A());
    float r = std::min(1.0f, R() * other.R());
    float g = std::min(1.0f, G() * other.G());
    float b = std::min(1.0f, B() * other.B());
    
    return CColor(r, g, b, a);
}

CColor& CColor::operator*= (const CColor& other)
{
    A(std::min(1.0f, A() * other.A()));
    R(std::min(1.0f, R() * other.R()));
    G(std::min(1.0f, G() * other.G()));
    B(std::min(1.0f, B() * other.B()));
    
    return *this;
}