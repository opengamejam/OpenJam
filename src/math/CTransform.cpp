//
//  CTransform.cpp
//  OpenJam
//
//  Created by yev on 7/10/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CTransform.h"

using namespace jam;

template <class T, class Vec, template <class Y> class Mat>
CTransform<T, Vec, Mat>::CTransform(const Vec& position,
                                    const Vec& rotation,
                                    const Vec& scale)
: m_Position(position)
, m_Rotation(rotation)
, m_Scale(scale)
, m_Size(Vec(static_cast<T>(1.0), static_cast<T>(1.0), static_cast<T>(1.0)))
{
    
}

template <class T, class Vec, template <class Y> class Mat>
CTransform<T, Vec, Mat>::~CTransform()
{
    
}

template <class T, class Vec, template <class Y> class Mat>
Vec& CTransform<T, Vec, Mat>::Position()
{
    return m_Position;
}

template <class T, class Vec, template <class Y> class Mat>
const Vec& CTransform<T, Vec, Mat>::Position() const
{
    return m_Position;
}

template <class T, class Vec, template <class Y> class Mat>
void CTransform<T, Vec, Mat>::Position(const Vec& position)
{
    m_Position = position;
}

template <class T, class Vec, template <class Y> class Mat>
Vec& CTransform<T, Vec, Mat>::Rotation()
{
    return m_Rotation;
}

template <class T, class Vec, template <class Y> class Mat>
const Vec& CTransform<T, Vec, Mat>::Rotation() const
{
    return m_Rotation;
}

template <class T, class Vec, template <class Y> class Mat>
void CTransform<T, Vec, Mat>::Rotation(const Vec& rotation)
{
    m_Rotation = rotation;
}

template <class T, class Vec, template <class Y> class Mat>
Vec& CTransform<T, Vec, Mat>::Scale()
{
    return m_Scale;
}

template <class T, class Vec, template <class Y> class Mat>
const Vec& CTransform<T, Vec, Mat>::Scale() const
{
    return m_Scale;
}

template <class T, class Vec, template <class Y> class Mat>
void CTransform<T, Vec, Mat>::Scale(const Vec& scale)
{
    m_Scale = scale;
}

template <class T, class Vec, template <class Y> class Mat>
Vec& CTransform<T, Vec, Mat>::Offset()
{
    return m_Offset;
}

template <class T, class Vec, template <class Y> class Mat>
const Vec& CTransform<T, Vec, Mat>::Offset() const
{
    return m_Offset;
}

template <class T, class Vec, template <class Y> class Mat>
void CTransform<T, Vec, Mat>::Offset(const Vec& offset)
{
    m_Offset = offset;
}

template <class T, class Vec, template <class Y> class Mat>
Vec& CTransform<T, Vec, Mat>::Size()
{
    return m_Size;
}

template <class T, class Vec, template <class Y> class Mat>
const Vec& CTransform<T, Vec, Mat>::Size() const
{
    return m_Size;
}

template <class T, class Vec, template <class Y> class Mat>
void CTransform<T, Vec, Mat>::Size(const Vec& size)
{
    m_Size = size;
}

template <class T, class Vec, template <class Y> class Mat>
CTransform<T, Vec, Mat>& CTransform<T, Vec, Mat>::operator+=(const CTransform<T, Vec, Mat>& other)
{
    Position() += const_cast<CTransform&>(other).Position();
    Rotation() += const_cast<CTransform&>(other).Rotation();
    Scale() *= const_cast<CTransform&>(other).Scale();
    
    return *this;
}

template <class T, class Vec, template <class Y> class Mat>
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

template class jam::CTransform<float, CVector3Df, CMatrix4x4>;
template class jam::CTransform<double, CVector3Dd, CMatrix4x4>;
template class jam::CTransform<int32_t, CVector3Di, CMatrix4x4>;