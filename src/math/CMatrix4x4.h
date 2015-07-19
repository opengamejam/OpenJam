//
//  CMatrix4x4.h
//  OpenJam
//
//  Created by yev on 3/28/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#ifndef CMATRIX4X4_H
#define CMATRIX4X4_H

#include "Global.h"
#include "CVector3D.h"

namespace jam
{

template<class T>
class CMatrix4x4
{
public:
    CMatrix4x4();
    CMatrix4x4(const std::vector<T>& matrix);
    virtual ~CMatrix4x4();
    
    const std::vector<T>& operator()() const;
    
    static const CMatrix4x4<T>  Identity();
    static const CMatrix4x4<T>  Zero();
    static const CMatrix4x4<T>  Perspective(T fov, T aspect, T zNear, T zFar);
    static const CMatrix4x4<T>  Orthographic(T left, T right, T top, T bottom, T near, T far);
    
    CMatrix4x4<T>               Clone() const;
    CMatrix4x4<T>&              Transpose();
    bool                        IsEqual(const CMatrix4x4<T>& matrix);
    
    CMatrix4x4<T>&              Translate(const CVector3D<T>& translation);
    CMatrix4x4<T>&              Rotate(const CVector3D<T>& rotation);
    CMatrix4x4<T>&              Scale(const CVector3D<T>& scale);
    
    CMatrix4x4<T>&              Translate();
    CMatrix4x4<T>&              Rotate();
    CMatrix4x4<T>&              Scale();
    
    CVector3D<T>                Up() const;
    CVector3D<T>                Forward() const;
    CVector3D<T>                Right() const;
    
    void                        Element(unsigned short x, unsigned short y, T element);
    const T&                    Element(unsigned short x, unsigned short y) const;
    void                        SwapElement(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
    
    CMatrix4x4<T>               operator*(const CMatrix4x4<T>& matrix);
    CMatrix4x4<T>&              operator*=(const CMatrix4x4<T>& matrix);
    CMatrix4x4<T>               operator+(const CMatrix4x4<T>& matrix);
    CMatrix4x4<T>&              operator+=(const CMatrix4x4<T>& matrix);
    CMatrix4x4<T>&              operator=(const std::vector<T>& other);
    
    void                        Dump() const;
    
private:
    static const unsigned short s_Dimension;
    
    std::vector<T> m_Matrix;
    static CMatrix4x4<T> s_Identity;
    static CMatrix4x4<T> s_Zero;
    
    static T s_nullValue;
};

typedef CMatrix4x4<float> CMatrix4x4f;
typedef CMatrix4x4<double> CMatrix4x4d;
typedef CMatrix4x4<int> CMatrix4x4i;

}; // namespace jam

#endif /* defined(CMATRIX4X4_H) */
