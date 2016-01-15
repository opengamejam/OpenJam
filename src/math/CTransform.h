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
#include "CMatrix.hpp"

namespace jam
{

template <class T, class Vec, class Mat>
class CTransform
{
public:
    CTransform(const Vec& position = Vec(),
               const Vec& rotation = Vec(),
               const Vec& scale = Vec(1.0, 1.0, 1.0));
    virtual ~CTransform();
    
    Vec& Position();
    const Vec& Position() const;
    void Position(const Vec& position);
    
    Vec& Rotation();
    const Vec& Rotation() const;
    void Rotation(const Vec& rotation);
    
    Vec& Scale();
    const Vec& Scale() const;
    void Scale(const Vec& scale);
    
    // TODO: deprecated
    Vec& Offset();
    const Vec& Offset() const;
    void Offset(const Vec& offset);
    
    Vec& Size();
    const Vec& Size() const;
    void Size(const Vec& size);
    
    CTransform<T, Vec, Mat>& operator+=(const CTransform<T, Vec, Mat>& other);
    Mat operator()();
    
private:
    Vec m_Position;
    Vec m_Rotation;
    Vec m_Scale;
    Vec m_Offset;
    Vec m_Size;
};

typedef CTransform<float, CVector3Df, CMatrix4x4f> CTransform3Df;
typedef CTransform<double, CVector3Dd, CMatrix4x4d> CTransform3Dd;
typedef CTransform<int32_t, CVector3Di, CMatrix4x4i> CTransform3Di;

}; // namespace jam

#endif /* defined(__OpenJam__CTransform3D__) */
