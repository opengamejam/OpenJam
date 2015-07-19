//
//  CRect.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 5/23/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef RECT2D_H
#define RECT2D_H

#include "Global.h"
#include "CVector2D.h"

namespace jam
{

template <class T>
class CRect2D
{
public:
    template <class R>
    class CIntersection
    {
    public:
        CIntersection(R left, R top, R right, R bottom);
        
        const CVector2D<R>& Delta() const;
        bool HasIntersection() const;
        bool IsContain() const;
        bool IsTop() const;
        bool IsLeft() const;
        bool IsRight() const;
        bool IsBottom() const;
        
    private:
        R m_Left;
        R m_Top;
        R m_Right;
        R m_Bottom;
        
        CVector2D<R> m_Delta;
    };
    
public:
    CRect2D();
    CRect2D(T left, T top, T right, T bottom);
    CRect2D(const CVector2D<T>& position, const CVector2D<T>& size);
    ~CRect2D();
    
    // Getters
    T Left() const;
    T Top() const;
    T Right() const;
    T Bottom() const;
    
    T X() const;
    T Y() const;
    T Width() const;
    T Height() const;
    
    const CVector2D<T>& Position() const;
    const CVector2D<T>& Size() const;
    
    // Setters
    void Left(T left);
    void Top(T top);
    void Right(T right);
    void Bottom(T bottom);
    
    void X(T x);
    void Y(T y);
    void Width(T width);
    void Height(T height);
    
    void Position(const CVector2D<T>& position);
    void Size(const CVector2D<T>& size);
    
private:
    void Validate();
    
private:
    CVector2D<T> m_Position;
    CVector2D<T> m_Size;
};

typedef CRect2D<float> CRect2Df;
typedef CRect2D<double> CRect2Dd;
typedef CRect2D<int> CRect2Di;

}; // namespace jam

#endif /* defined(RECT2D_H) */
