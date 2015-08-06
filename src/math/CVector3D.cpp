//
//  CVector3D.cpp
//  OpenJam
//
//  Created by yev on 3/27/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#include "CVector3D.h"
#include "CStringUtils.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

template <class T>
const CVector3D<T> CVector3D<T>::zero = CVector3D<T>(static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0));

// *****************************************************************************
// Public Methods
// *****************************************************************************

template <class T>
CVector3D<T>::CVector3D(T _x, T _y, T _z)
    : x(_x)
    , y(_y)
    , z(_z)
{
}

template <class T>
CVector3D<T>::CVector3D(const std::string& str, char delimeter, const CVector3D<T>& defaultValue)
: CVector3D(defaultValue)
{
    std::vector<std::string> components;
    SplitString(components, str, delimeter);
    if (components.size() > 0)
    {
        X(FromString<T>(ReplaceString(components[0], " ", "")));
    }
    if (components.size() > 1)
    {
        Y(FromString<T>(ReplaceString(components[1], " ", "")));
    }
    if (components.size() > 2)
    {
        Z(FromString<T>(ReplaceString(components[2], " ", "")));
    }
}

template <class T>
CVector3D<T>::CVector3D(const CVector2D<T>& vector2D)
: x(vector2D.X())
, y(vector2D.Y())
, z(0)
{
}

template <class T>
CVector3D<T>::~CVector3D() {}

template <class T>
void  CVector3D<T>::X(T _x) { x = _x; }

template <class T>
T CVector3D<T>::X() const   { return x; }

template <class T>
void  CVector3D<T>::Y(T _y) { y = _y; }

template <class T>
T CVector3D<T>::Y() const   { return y; }

template <class T>
void  CVector3D<T>::Z(T _z) { z = _z; }

template <class T>
T CVector3D<T>::Z() const   { return z; }

template <class T>
void CVector3D<T>::Set(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }

template <class T>
T CVector3D<T>::Angle(const CVector3D<T>& other)
{
    return atan2(x * other.Y() - other.X() * y, x * other.X() + y * other.Y());
}

template <class T>
T CVector3D<T>::Length() const
{
    return sqrt(x * x + y * y + z * z);
}

template <class T>
CVector3D<T> CVector3D<T>::operator+ (const CVector3D<T>& other) const
{
    CVector3D<T> result = *this;
    
    result.x += other.X();
    result.y += other.Y();
    result.z += other.Z();
    
    return result;
}

template <class T>
CVector3D<T>& CVector3D<T>::operator+= (const CVector3D<T>& other)
{
    this->x += other.X();
    this->y += other.Y();
    this->z += other.Z();
    
    return *this;
}

template <class T>
CVector3D<T> CVector3D<T>::operator- (const CVector3D<T>& other) const
{
    CVector3D<T> result = *this;
    
    result.x -= other.X();
    result.y -= other.Y();
    result.z -= other.Z();
    
    return result;
}

template <class T>
CVector3D<T>& CVector3D<T>::operator-= (const CVector3D<T>& other)
{
    this->x -= other.X();
    this->y -= other.Y();
    this->z -= other.Z();
    
    return *this;
}

template <class T>
CVector3D<T> CVector3D<T>::operator* (const CVector3D<T>& other) const
{
    CVector3D<T> result = *this;
    
    result.x *= other.X();
    result.y *= other.Y();
    result.z *= other.Z();
    
    return result;
}

template <class T>
CVector3D<T>& CVector3D<T>::operator*= (const CVector3D<T>& other)
{
    this->x *= other.X();
    this->y *= other.Y();
    this->z *= other.Z();
    
    return *this;
}

template <class T>
CVector3D<T> CVector3D<T>::operator/ (const CVector3D<T>& other) const
{
    CVector3D<T> result = *this;
    
    result.x /= other.X();
    result.y /= other.Y();
    result.z /= other.Z();
    
    return result;
}

template <class T>
CVector3D<T>& CVector3D<T>::operator/= (const CVector3D<T>& other)
{
    this->x /= other.X();
    this->y /= other.Y();
    this->z /= other.Z();
    
    return *this;
}

template <class T>
const CVector3D<T>& CVector3D<T>::Normalize()
{
    T l = 1.0f / Length();
    
    x *= l;
    y *= l;
    z *= l;
    
    return *this;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

template class jam::CVector3D<float>;
template class jam::CVector3D<double>;
template class jam::CVector3D<int>;