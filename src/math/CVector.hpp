//
//  CVector.hpp
//  OpenJam
//
//  Created by yev on 3/27/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#ifndef CVECTOR_HPP
#define CVECTOR_HPP

#include "Global.h"
#include "CStringUtils.h"

namespace jam
{

// *****************************************************************************
// Predeclarations
// *****************************************************************************

template<class T, uint32_t N>
class CVector;

typedef CVector<float, 1> CVector1Df;
typedef CVector<double, 1> CVector1Dd;
typedef CVector<uint8_t, 1> CVector1Db;
typedef CVector<uint32_t, 1> CVector1Du;
typedef CVector<uint64_t, 1> CVector1Dul;
typedef CVector<int32_t, 1> CVector1Di;
typedef CVector<int64_t, 1> CVector1Dl;
typedef CVector<float, 2> CVector2Df;
typedef CVector<double, 2> CVector2Dd;
typedef CVector<uint8_t, 2> CVector2Db;
typedef CVector<uint32_t, 2> CVector2Du;
typedef CVector<uint64_t, 2> CVector2Dul;
typedef CVector<int32_t, 2> CVector2Di;
typedef CVector<int64_t, 2> CVector2Dl;
typedef CVector<float, 3> CVector3Df;
typedef CVector<double, 3> CVector3Dd;
typedef CVector<uint8_t, 3> CVector3Db;
typedef CVector<uint32_t, 3> CVector3Du;
typedef CVector<uint64_t, 3> CVector3Dul;
typedef CVector<int32_t, 3> CVector3Di;
typedef CVector<int64_t, 3> CVector3Dl;
typedef CVector<float, 4> CVector4Df;
typedef CVector<double, 4> CVector4Dd;
typedef CVector<uint8_t, 4> CVector4Db;
typedef CVector<uint32_t, 4> CVector4Du;
typedef CVector<uint64_t, 4> CVector4Dul;
typedef CVector<int32_t, 4> CVector4Di;
typedef CVector<int64_t, 4> CVector4Dl;

// *****************************************************************************
// CVector is a class-wrapper around primitive T[N] array,
// where T - type, N - components num. There are methods for manipulate with
// XYZ representation. If components num less than 4 then appropriate methods
// will do nothing
// *****************************************************************************
    
template <class T, uint32_t N>
class CVector
{
public:
    typedef T TXYZ[N];
    
public:
    CVector(T x = static_cast<T>(0.0), T y = static_cast<T>(0.0), T z = static_cast<T>(0.0))
    {
        X(x);
        Y(y);
        Z(z);
    }
    
    CVector(const TXYZ xyz)
    {
        memcpy(m_XYZ, xyz, sizeof(m_XYZ));
    }
    
    CVector(const std::string& str, char delimeter = ',', const CVector<T, N>& defaultValue = CVector<T, N>())
    {
        std::vector<std::string> components;
        SplitString(components, str, delimeter);
        for (size_t i = 0; i < components.size(); ++i)
        {
            Set(FromString<T>(ReplaceString(components[i], " ", "")), static_cast<int>(i));
        }
    }
    
    ~CVector()
    {}
    
    INL void X(T x)
    {
        Set(x, 0);
    }
    
    INL void Y(T y)
    {
        Set(y, 1);
    }
    
    INL void Z(T z)
    {
        Set(z, 2);
    }
    
    INL void Set(T value, uint32_t index)
    {
        if (index < N)
        {
            m_XYZ[index] = value;
        }
    }
    
    INL T X() const
    {
        return Get(0);
    }
    
    INL T Y() const
    {
        return Get(1);
    }
    
    INL T Z() const
    {
        return Get(2);
    }
    
    INL T Get(int index) const
    {
        if (index < N)
        {
            return m_XYZ[index];
        }
        return static_cast<T>(0.0);
    }
    
    INL T Length() const
    {
        T p2sum = 0;
        for (size_t i = 0; i < N; ++i)
        {
            p2sum += m_XYZ[i] * m_XYZ[i];
        }
        
        return static_cast<T>(sqrt(p2sum));
    }
    
    INL T DotProduct(const CVector<T, N>& other)
    {
        return std::inner_product(m_XYZ, m_XYZ + N, other.m_XYZ, 0);
    }
    
    INL CVector<T, N> CrossProduct(const CVector<T, N>& other)
    {
        static_assert(N == 3, "Cannot compute cross product for vector dimension not equal 3");
        if (N == 3)
        {
            T a = Y() * other.Z() - Z() * other.Y();
            T b = Z() * other.Z() - X() * other.Z();
            T c = X() * other.Y() - Y() * other.X();
            CVector<T, N> product(a , b , c);
            
            return product;
        }
        return *this;
    }
    
    INL CVector<T, N>& Normalize()
    {
        T l = static_cast<T>(1.0) / Length();
        *this *= l;
        
        return *this;
    }
    
    INL CVector<T, N> operator+ (const CVector<T, N>& other) const
    {
        CVector<T, N> result(*this);
        result += other;
        
        return result;
    }
        
    INL void operator+= (const CVector<T, N>& other)
    {
        X(X() + other.X());
        Y(Y() + other.Y());
        Z(Z() + other.Z());
    }
    
    INL CVector<T, N> operator- (const CVector<T, N>& other) const
    {
        CVector<T, N> result(*this);
        result -= other;
        
        return result;
    }
    
    INL void operator-= (const CVector<T, N>& other)
    {
        X(X() - other.X());
        Y(Y() - other.Y());
        Z(Z() - other.Z());
    }
    
    INL CVector<T, N> operator* (const CVector<T, N>& other) const
    {
        CVector<T, N> result(*this);
        result *= other;
        
        return result;
    }
    
    INL void operator*= (const CVector<T, N>& other)
    {
        X(X() * other.X());
        Y(Y() * other.Y());
        Z(Z() * other.Z());
    }
    
    INL CVector<T, N> operator/ (const CVector<T, N>& other) const
    {
        CVector<T, N> result(*this);
        result *= other;
        
        return result;
    }
    
    INL void operator/= (const CVector<T, N>& other)
    {
        X(X() / other.X());
        Y(Y() / other.Y());
        Z(Z() / other.Z());
    }
    
    INL CVector<T, N> operator+ (T scalar) const
    {
        CVector<T, N> result(*this);
        result += scalar;
        
        return result;
    }
    
    INL void operator+= (T scalar)
    {
        X(X() + scalar);
        Y(Y() + scalar);
        Z(Z() + scalar);
    }
    
    INL CVector<T, N> operator- (T scalar) const
    {
        CVector<T, N> result(*this);
        result -= scalar;
        
        return result;
    }
    
    INL void operator-= (T scalar)
    {
        X(X() - scalar);
        Y(Y() - scalar);
        Z(Z() - scalar);
    }
    
    INL CVector<T, N> operator* (T scalar) const
    {
        CVector<T, N> result(*this);
        result *= scalar;
        
        return result;
    }
    
    INL void operator*= (T scalar)
    {
        X(X() * scalar);
        Y(Y() * scalar);
        Z(Z() * scalar);
    }
    
    INL CVector<T, N> operator/ (T scalar) const
    {
        CVector<T, N> result(*this);
        result /= scalar;
        
        return result;
    }
    
    INL void operator/= (T scalar)
    {
        X(X() / scalar);
        Y(Y() / scalar);
        Z(Z() / scalar);
        
        return *this;
    }
    
    
private:
    static_assert(N > 0, "CVector cannot be zero-dimensioned");
    TXYZ m_XYZ;
};

}; // namespace jam

#endif /* defined(CVECTOR_HPP) */
