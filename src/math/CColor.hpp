//
//  CColor.hpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 5/28/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CCOLOR_HPP
#define CCOLOR_HPP

#include "Global.h"
#include <regex>

namespace jam {

// *****************************************************************************
// Predeclarations
// *****************************************************************************

template <class T, uint32_t N>
class CColor;

template <class T1, class T2, uint32_t N>
CColor<T1, N> ConvertColor(const CColor<T2, N>& color);

typedef CColor<float, 1> CColor1f;
typedef CColor<double, 1> CColor1d;
typedef CColor<uint8_t, 1> CColor1b;
typedef CColor<float, 2> CColor2f;
typedef CColor<double, 2> CColor2d;
typedef CColor<uint8_t, 2> CColor2b;
typedef CColor<float, 3> CColor3f;
typedef CColor<double, 3> CColor3d;
typedef CColor<uint8_t, 3> CColor3b;
typedef CColor<float, 4> CColor4f;
typedef CColor<double, 4> CColor4d;
typedef CColor<uint8_t, 4> CColor4b;

// *****************************************************************************
// CColor is a class-wrapper around primitive T[N] array,
// where T - type, N - components num. There are methods for manipulate with
// RGBA representation. If components num less than 4 then appropriate methods
// will do nothing
// *****************************************************************************

template <class T, uint32_t N>
class CColor {
public:
    typedef T TRGBA[N];

public:
    CColor(T r = static_cast<T>(1.0), T g = static_cast<T>(1.0), T b = static_cast<T>(1.0), T a = static_cast<T>(1.0))
    {
        R(r);
        G(g);
        B(b);
        A(a);
    }

    CColor(const TRGBA rgba)
    {
        memcpy(m_RGBA, rgba, sizeof(m_RGBA));
    }

    CColor(const std::string& stringHex)
        : CColor()
    {
        static const std::regex hexMatcher("^#?(([0-9a-fA-F]{2}){4}|([0-9a-fA-F]){4})$");

        if (std::regex_match(stringHex, hexMatcher)) {
            std::string pureHex = stringHex;
            if (pureHex[0] == '#') {
                pureHex = pureHex.substr(1, pureHex.length() - 1);
            }

            CColor<uint8_t, N> c;
            uint8_t byte;
            std::stringstream converter;
            size_t len = pureHex.length() / 2;
            len = std::min<size_t>(len, N);
            for (size_t i = 0; i < len; ++i) {
                std::string component = pureHex.substr(i * 2, 2);
                converter << std::hex << component;
                converter >> byte;

                uint8_t* data = c.RGBA();
                data[i] = byte;
            }

            *this = ConvertColor<T, uint8_t, N>(c);
        }
    }

    INL T R() const
    {
        if (N > 0) {
            return m_RGBA[0];
        }
        return static_cast<T>(0.0);
    }

    INL T G() const
    {
        if (N > 1) {
            return m_RGBA[1];
        }
        return static_cast<T>(0.0);
    }

    INL T B() const
    {
        if (N > 2) {
            return m_RGBA[2];
        }
        return static_cast<T>(0.0);
    }

    INL T A() const
    {
        if (N > 3) {
            return m_RGBA[3];
        }
        return static_cast<T>(0.0);
    }

    INL void R(T r)
    {
        if (N > 0) {
            m_RGBA[0] = r;
        }
    }

    INL void G(T g)
    {
        if (N > 1) {
            m_RGBA[1] = g;
        }
    }

    INL void B(T b)
    {
        if (N > 2) {
            m_RGBA[2] = b;
        }
    }

    INL void A(T a)
    {
        if (N > 3) {
            m_RGBA[3] = a;
        }
    }

    INL CColor operator+(const CColor<T, N>& other) const
    {
        T a = std::min<T>(static_cast<T>(1.0), A() + other.A());
        T r = std::min<T>(static_cast<T>(1.0), R() + other.R());
        T g = std::min<T>(static_cast<T>(1.0), G() + other.G());
        T b = std::min<T>(static_cast<T>(1.0), B() + other.B());

        return CColor(r, g, b, a);
    }

    INL CColor& operator+=(const CColor<T, N>& other)
    {
        A(std::min<T>(static_cast<T>(1.0), A() + other.A()));
        R(std::min<T>(static_cast<T>(1.0), R() + other.R()));
        G(std::min<T>(static_cast<T>(1.0), G() + other.G()));
        B(std::min<T>(static_cast<T>(1.0), B() + other.B()));

        return *this;
    }

    INL CColor operator*(const CColor<T, N>& other) const
    {
        float a = std::min<T>(static_cast<T>(1.0), A() * other.A());
        float r = std::min<T>(static_cast<T>(1.0), R() * other.R());
        float g = std::min<T>(static_cast<T>(1.0), G() * other.G());
        float b = std::min<T>(static_cast<T>(1.0), B() * other.B());

        return CColor(r, g, b, a);
    }

    INL CColor& operator*=(const CColor<T, N>& other)
    {
        A(std::min<T>(static_cast<T>(1.0), A() * other.A()));
        R(std::min<T>(static_cast<T>(1.0), R() * other.R()));
        G(std::min<T>(static_cast<T>(1.0), G() * other.G()));
        B(std::min<T>(static_cast<T>(1.0), B() * other.B()));

        return *this;
    }

private:
    static_assert(N > 0, "CColor cannot be zero-dimensioned");
    TRGBA m_RGBA;
};

// *****************************************************************************
// Color converter functions
// *****************************************************************************

template <class T1, class T2, uint32_t N>
INL CColor<T1, N> ConvertColor(const CColor<T2, N>& color)
{
    CColor<T1, N> out;

    out.A(static_cast<T1>(color.A()));
    out.R(static_cast<T1>(color.R()));
    out.G(static_cast<T1>(color.G()));
    out.B(static_cast<T1>(color.B()));

    return out;
}

template <uint32_t N>
INL CColor<float, N> ConvertColor(const CColor<uint8_t, N>& color)
{
    CColor<float, N> out;

    out.A(static_cast<float>(color.A() / 255.0f));
    out.R(static_cast<float>(color.R() / 255.0f));
    out.G(static_cast<float>(color.G() / 255.0f));
    out.B(static_cast<float>(color.B() / 255.0f));

    return out;
}

template <uint32_t N>
INL CColor<double, N> ConvertColor(const CColor<uint8_t, N>& color)
{
    CColor<double, N> out;

    out.A(static_cast<double>(color.A() / 255.0));
    out.R(static_cast<double>(color.R() / 255.0));
    out.G(static_cast<double>(color.G() / 255.0));
    out.B(static_cast<double>(color.B() / 255.0));

    return out;
}

template <uint32_t N>
INL CColor<uint8_t, N> ConvertColor(const CColor<float, N>& color)
{
    CColor<uint8_t, N> out;

    out.A(static_cast<uint8_t>(color.A() * 255.0f));
    out.R(static_cast<uint8_t>(color.R() * 255.0f));
    out.G(static_cast<uint8_t>(color.G() * 255.0f));
    out.B(static_cast<uint8_t>(color.B() * 255.0f));

    return out;
}

template <uint32_t N>
INL CColor<uint8_t, N> ConvertColor(const CColor<double, N>& color)
{
    CColor<uint8_t, N> out;

    out.A(static_cast<uint8_t>(color.A() * 255.0));
    out.R(static_cast<uint8_t>(color.R() * 255.0));
    out.G(static_cast<uint8_t>(color.G() * 255.0));
    out.B(static_cast<uint8_t>(color.B() * 255.0));

    return out;
}

}; // namespace jam

#endif /* defined(CCOLOR_HPP) */
