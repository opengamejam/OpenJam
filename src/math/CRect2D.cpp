//
//  CRect.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 5/23/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CRect2D.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

template <class T>
template <class R>
CRect2D<T>::CIntersection<R>::CIntersection(R left, R top, R right, R bottom)
    : m_Left(left)
    , m_Top(top)
    , m_Right(right)
    , m_Bottom(bottom)
{
    m_Delta = CVector2D<R>(std::max(m_Left, m_Right), std::max(m_Top, m_Bottom));
};

template <class T>
template <class R>
bool CRect2D<T>::CIntersection<R>::HasIntersection() const
{
    return (m_Left > 0 || m_Right > 0) && (m_Top > 0 || m_Bottom > 0);
}

template <class T>
template <class R>
bool CRect2D<T>::CIntersection<R>::IsContain() const
{
    return (m_Left > 0 && m_Right > 0) && (m_Top > 0 && m_Bottom > 0);
}

template <class T>
template <class R>
bool CRect2D<T>::CIntersection<R>::IsTop() const
{
    return (m_Left > 0 || m_Right > 0) && (m_Top > 0);
}

template <class T>
template <class R>
bool CRect2D<T>::CIntersection<R>::IsLeft() const
{
    return (m_Left > 0) && (m_Top > 0 || m_Bottom > 0);
}

template <class T>
template <class R>
bool CRect2D<T>::CIntersection<R>::IsRight() const
{
    return (m_Right > 0) && (m_Top > 0 || m_Bottom > 0);
}

template <class T>
template <class R>
bool CRect2D<T>::CIntersection<R>::IsBottom() const
{
    return (m_Left > 0 || m_Right > 0) && (m_Bottom > 0);
}

template <class T>
CRect2D<T>::CRect2D()
    : m_Position(CVector2D<T>(static_cast<T>(0.0), static_cast<T>(0.0)))
    , m_Size(CVector2D<T>(static_cast<T>(0.0), static_cast<T>(0.0)))
{
}

template <class T>
CRect2D<T>::CRect2D(T left, T top, T right, T bottom)
    : m_Position(CVector2D<T>(left, top))
    , m_Size(CVector2D<T>(right - left, bottom - top))
{
    Validate();
}

template <class T>
CRect2D<T>::CRect2D(const CVector2D<T>& position, const CVector2D<T>& size)
    : m_Position(position)
    , m_Size(size)
{
    Validate();
}

template <class T>
CRect2D<T>::~CRect2D()
{
}

// Getters
template <class T>
T CRect2D<T>::Left() const
{
    return m_Position.X();
}

template <class T>
T CRect2D<T>::Top() const
{
    return m_Position.Y();
}

template <class T>
T CRect2D<T>::Right() const
{
    return m_Position.X() + m_Size.X();
}

template <class T>
T CRect2D<T>::Bottom() const
{
    return m_Position.Y() + m_Size.Y();
}

template <class T>
T CRect2D<T>::X() const
{
    return m_Position.X();
}

template <class T>
T CRect2D<T>::Y() const
{
    return m_Position.Y();
}

template <class T>
T CRect2D<T>::Width() const
{
    return m_Size.X();
}

template <class T>
T CRect2D<T>::Height() const
{
    return m_Size.Y();
}

template <class T>
const CVector2D<T>& CRect2D<T>::Position() const
{
    return m_Position;
}

template <class T>
const CVector2D<T>& CRect2D<T>::Size() const
{
    return m_Size;
}

// Setters
template <class T>
void CRect2D<T>::Left(T left)
{
    m_Position.X(left);
    Validate();
}

template <class T>
void CRect2D<T>::Top(T top)
{
    m_Position.Y(top);
    Validate();
}

template <class T>
void CRect2D<T>::Right(T right)
{
    m_Size.X(right - m_Position.X());
    Validate();
}

template <class T>
void CRect2D<T>::Bottom(T bottom)
{
    m_Size.Y(bottom - m_Position.Y());
    Validate();
}

template <class T>
void CRect2D<T>::X(T x)
{
    m_Position.X();
    Validate();
}

template <class T>
void CRect2D<T>::Y(T y)
{
    m_Position.Y();
    Validate();
}

template <class T>
void CRect2D<T>::Width(T width)
{
    m_Size.X(width);
    Validate();
}

template <class T>
void CRect2D<T>::Height(T height)
{
    m_Size.Y(height);
    Validate();
}

template <class T>
void CRect2D<T>::Position(const CVector2D<T>& position)
{
    m_Position = position;
    Validate();
}

template <class T>
void CRect2D<T>::Size(const CVector2D<T>& size)
{
    m_Size = size;
    Validate();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

template <class T>
void CRect2D<T>::Validate()
{
    T left = m_Position.X();
    T top = m_Position.Y();
    T right = m_Position.X() + m_Size.X();
    T bottom = m_Position.Y() + m_Size.Y();
    
    if (left > right)
    {
        m_Position.X(right);
        m_Size.X(left - right);
    }
    
    if (top > bottom)
    {
        m_Position.Y(bottom);
        m_Size.Y(top - bottom);
    }
}

template class jam::CRect2D<float>;
template class jam::CRect2D<double>;
template class jam::CRect2D<int>;