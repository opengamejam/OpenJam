//
//  CVector2D.h
//  OpenJam
//
//  Created by yev on 3/27/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#ifndef CVECTOR2D_H
#define CVECTOR2D_H

#include "Global.h"

namespace jam
{

template <class T>
class CVector2D
{
public:
    static const CVector2D<T> zero;
    
public:
    CVector2D(T _x = 0, T _y = 0);
    CVector2D(const std::string& str, char delimeter = ',', const CVector2D<T>& defaultValue = CVector2D<T>());
    ~CVector2D();
    
    void         X(T _x);
    float        X() const;
    void         Y(T _y);
    float        Y() const;
    void         Set(float _x, float _y);
    double       Angle(const CVector2D<T>& other);
    double       Length() const;
    const CVector2D<T>& operator+= (const CVector2D<T>& other);
    CVector2D<T> operator+ (const CVector2D<T>& other);
    const CVector2D<T>& operator-= (const CVector2D<T>& other);
    CVector2D<T> operator- (const CVector2D<T>& other);
    const CVector2D<T>& operator*= (T scalar);
    CVector2D<T> operator* (T scalar);
    
private:
    T x;
    T y;
};

typedef CVector2D<float> CVector2Df;
typedef CVector2D<double> CVector2Dd;
typedef CVector2D<int> CVector2Di;

}; // namespace jam

#endif /* defined(CVECTOR2D_H) */
