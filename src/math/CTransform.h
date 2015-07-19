//
//  CTransform.h
//  OpenJam
//
//  Created by yev on 7/10/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef __OpenJam__CTransform3D__
#define __OpenJam__CTransform3D__

#include "Global.h"
#include "CMatrix4x4.h"

namespace jam
{

template <class T, template <class X> class Vec, template <class Y> class Mat>
class CTransform
{
public:
    CTransform(const Vec<T>& position = Vec<T>(),
               const Vec<T>& rotation = Vec<T>(),
               const Vec<T>& scale = Vec<T>(static_cast<T>(1.0), static_cast<T>(1.0), static_cast<T>(1.0)));
    virtual ~CTransform();
    
    Vec<T>& Position();
    const Vec<T>& Position() const;
    void Position(const Vec<T>& position);
    
    Vec<T>& Rotation();
    const Vec<T>& Rotation() const;
    void Rotation(const Vec<T>& rotation);
    
    Vec<T>& Scale();
    const Vec<T>& Scale() const;
    void Scale(const Vec<T>& scale);
    
    // TODO: deprecated
    Vec<T>& Offset();
    const Vec<T>& Offset() const;
    void Offset(const Vec<T>& offset);
    
    Vec<T>& Size();
    const Vec<T>& Size() const;
    void Size(const Vec<T>& size);
    
    CTransform<T, Vec, Mat>& operator+=(const CTransform<T, Vec, Mat>& other);
    Mat<T> operator()();
    
private:
    Vec<T> m_Position;
    Vec<T> m_Rotation;
    Vec<T> m_Scale;
    Vec<T> m_Offset;
    Vec<T> m_Size;
};

typedef CTransform<float, CVector3D, CMatrix4x4> CTransform3Df;
typedef CTransform<double, CVector3D, CMatrix4x4> CTransform3Dd;
typedef CTransform<int, CVector3D, CMatrix4x4> CTransform3Di;

}; // namespace jam

#endif /* defined(__OpenJam__CTransform3D__) */
