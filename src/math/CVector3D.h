//
//  CVector3D.h
//  OpenJam
//
//  Created by yev on 3/27/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#ifndef CVECTOR3D_H
#define CVECTOR3D_H

#include "Global.h"
#include "CVector2D.h"

namespace jam
{

template <class T>
class CVector3D
{
public:
    static const CVector3D<T> zero;
    
public:
    CVector3D(T _x = 0, T _y = 0, T _z = 0);
    CVector3D(const std::string& str, char delimeter = ',', const CVector3D<T>& defaultValue = CVector3D<T>());
    CVector3D(const CVector2D<T>& vector2D);
    ~CVector3D();
    
    void     X(T _x);
    T        X() const;
    
    void     Y(T _y);
    T        Y() const;
    
    void     Z(T _z);
    T        Z() const;
    
    void     Set(T _x, T _y, T _z);
    T        Angle(const CVector3D& other);
    T        Length() const;
    CVector3D<T> operator+ (const CVector3D& other) const;
    CVector3D<T>& operator+= (const CVector3D& other);
    CVector3D<T> operator- (const CVector3D& other) const;
    CVector3D<T>& operator-= (const CVector3D& other);
    CVector3D<T> operator* (const CVector3D& other) const;
    CVector3D<T>& operator*= (const CVector3D& other);
    CVector3D<T> operator/ (const CVector3D& other) const;
    CVector3D<T>& operator/= (const CVector3D& other);
    const CVector3D& Normalize();
    
private:    
    T x;
    T y;
    T z;
};


typedef CVector3D<float> CVector3Df;
typedef CVector3D<double> CVector3Dd;
typedef CVector3D<int> CVector3Di;

}; // namespace jam

#endif /* defined(CVECTOR3D_H) */
