//
//  CTransform.cpp
//  OpenJam
//
//  Created by yev on 7/10/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CTransform.h"

using namespace jam;

template <class T, template <class X> class Vec, template <class Y> class Mat>
CTransform<T, Vec, Mat>::CTransform(const Vec<T>& position,
                                    const Vec<T>& rotation,
                                    const Vec<T>& scale)
: m_Position(position)
, m_Rotation(rotation)
, m_Scale(scale)
, m_Size(Vec<T>(static_cast<T>(1.0), static_cast<T>(1.0), static_cast<T>(1.0)))
{
    
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
CTransform<T, Vec, Mat>::~CTransform()
{
    
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
Vec<T>& CTransform<T, Vec, Mat>::Position()
{
    return m_Position;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
const Vec<T>& CTransform<T, Vec, Mat>::Position() const
{
    return m_Position;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
void CTransform<T, Vec, Mat>::Position(const Vec<T>& position)
{
    m_Position = position;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
Vec<T>& CTransform<T, Vec, Mat>::Rotation()
{
    return m_Rotation;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
const Vec<T>& CTransform<T, Vec, Mat>::Rotation() const
{
    return m_Rotation;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
void CTransform<T, Vec, Mat>::Rotation(const Vec<T>& rotation)
{
    m_Rotation = rotation;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
Vec<T>& CTransform<T, Vec, Mat>::Scale()
{
    return m_Scale;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
const Vec<T>& CTransform<T, Vec, Mat>::Scale() const
{
    return m_Scale;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
void CTransform<T, Vec, Mat>::Scale(const Vec<T>& scale)
{
    m_Scale = scale;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
Vec<T>& CTransform<T, Vec, Mat>::Offset()
{
    return m_Offset;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
const Vec<T>& CTransform<T, Vec, Mat>::Offset() const
{
    return m_Offset;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
void CTransform<T, Vec, Mat>::Offset(const Vec<T>& offset)
{
    m_Offset = offset;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
Vec<T>& CTransform<T, Vec, Mat>::Size()
{
    return m_Size;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
const Vec<T>& CTransform<T, Vec, Mat>::Size() const
{
    return m_Size;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
void CTransform<T, Vec, Mat>::Size(const Vec<T>& size)
{
    m_Size = size;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
CTransform<T, Vec, Mat>& CTransform<T, Vec, Mat>::operator+=(const CTransform<T, Vec, Mat>& other)
{
    Position() += const_cast<CTransform&>(other).Position();
    Rotation() += const_cast<CTransform&>(other).Rotation();
    Scale() *= const_cast<CTransform&>(other).Scale();
    
    return *this;
}

template <class T, template <class X> class Vec, template <class Y> class Mat>
Mat<T> CTransform<T, Vec, Mat>::operator()()
{
    Mat<T> transform = Mat<T>::Identity();
    
    Mat<T> p = Mat<T>::Identity();
    Mat<T> r = Mat<T>::Identity();
    Mat<T> s = Mat<T>::Identity();
    Mat<T> o = Mat<T>::Identity();
    
    p.Translate(Position());
    r.Rotate(Rotation());
    o.Translate(Offset());
    s.Scale(Scale() * Size());
    
    transform *= p;
    transform *= r;
    transform *= o;
    transform *= s;
    
    return transform;
}

template class jam::CTransform<float, CVector3D, CMatrix4x4>;
template class jam::CTransform<double, CVector3D, CMatrix4x4>;
template class jam::CTransform<int, CVector3D, CMatrix4x4>;