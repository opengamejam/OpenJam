//
//  CVector2D.cpp
//  OpenJam
//
//  Created by yev on 3/27/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#include "CVector2D.h"
#include "CStringUtils.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

template <class T>
const CVector2D<T> CVector2D<T>::zero = CVector2D<T>(static_cast<T>(0.0), static_cast<T>(0.0));

// *****************************************************************************
// Public Methods
// *****************************************************************************

template <class T>
CVector2D<T>::CVector2D(T _x, T _y)
    : x(_x)
    , y(_y)
{
    
}

template <class T>
CVector2D<T>::CVector2D(const std::string& str, char delimeter, const CVector2D<T>& defaultValue)
: CVector2D(defaultValue)
{
    std::vector<std::string> components;
    SplitString(components, ReplaceString(str, " ", ""), delimeter);
    if (components.size() > 0)
    {
        X(FromString<T>(components[0]));
    }
    if (components.size() > 1)
    {
        Y(FromString<T>(components[1]));
    }
}

template <class T>
CVector2D<T>::~CVector2D()
{

}

template <class T>
void CVector2D<T>::X(T _x)
{
    x = _x;
}

template <class T>
float CVector2D<T>::X() const
{
    return x;
}

template <class T>
void CVector2D<T>::Y(T _y)
{
    y = _y;
}

template <class T>
float CVector2D<T>::Y() const
{
    return y;
}

template <class T>
void CVector2D<T>::Set(float _x, float _y)
{
    x = _x;
    y = _y;
}

template <class T>
double CVector2D<T>::Angle(const CVector2D<T>& other)
{
    return atan2(x * other.Y() - other.X() * y, x * other.X() + y * other.Y());
}

template <class T>
double CVector2D<T>::Length() const
{
    return sqrt(x * x + y * y);
}

template <class T>
const CVector2D<T>& CVector2D<T>::operator+= (const CVector2D<T>& other)
{
    this->x += other.X();
    this->y += other.Y();
    
    return *this;
}

template <class T>
CVector2D<T> CVector2D<T>::operator+ (const CVector2D<T>& other)
{
    CVector2D newVec(this->x + other.X(), this->y + other.Y());
    
    return newVec;
}

template <class T>
const CVector2D<T>& CVector2D<T>::operator-= (const CVector2D<T>& other)
{
    this->x -= other.X();
    this->y -= other.Y();
    
    return *this;
}

template <class T>
CVector2D<T> CVector2D<T>::operator- (const CVector2D<T>& other)
{
    CVector2D newVec(this->x - other.X(), this->y - other.Y());
    
    return newVec;
}

template <class T>
const CVector2D<T>& CVector2D<T>::operator*= (T scalar)
{
    this->x *= scalar;
    this->y += scalar;
    
    return *this;
}

template <class T>
CVector2D<T> CVector2D<T>::operator* (T scalar)
{
    CVector2D newVec(this->x * scalar, this->y * scalar);
    
    return newVec;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

template class jam::CVector2D<float>;
template class jam::CVector2D<double>;
template class jam::CVector2D<int>;