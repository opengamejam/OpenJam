//
//  CTransform.hpp
//  OpenJam
//
//  Created by yev on 7/10/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CTRANSFORM_HPP
#define CTRANSFORM_HPP

#include "Global.h"
#include "glm/ext.hpp"

namespace jam
{

// *****************************************************************************
// Predeclarations
// *****************************************************************************

template <class T, class Vec, class Mat>
class CTransform;
    
typedef CTransform<float, glm::vec3, glm::mat4x4> CTransform3Df;

    
// *****************************************************************************
// CTransform contain object's position, rotation and scale and returns
// according matrix transformation
// *****************************************************************************

template <class T, class Vec, class Mat>
class CTransform
{
public:
    CTransform(const Vec& position = Vec(),
               const Vec& rotation = Vec(),
               const Vec& scale = Vec(1.0, 1.0, 1.0))
    : m_Position(position)
    , m_Rotation(rotation)
    , m_Scale(scale)
    {
        
    }
    
    virtual ~CTransform()
    {
    }
    
    INL Vec& Position()
    {
        return m_Position;
    }
    
    INL const Vec& Position() const
    {
        return m_Position;
    }
    
    INL void Position(const Vec& position)
    {
        m_Position = position;
    }
    
    INL Vec& Rotation()
    {
        return m_Rotation;
    }
    
    INL const Vec& Rotation() const
    {
        return m_Rotation;
    }
    
    INL void Rotation(const Vec& rotation)
    {
        m_Rotation = rotation;
    }
    
    INL Vec& Scale()
    {
        return m_Scale;
    }
    
    INL const Vec& Scale() const
    {
        return m_Scale;
    }
    
    INL void Scale(const Vec& scale)
    {
        m_Scale = scale;
    }
    
    INL CTransform<T, Vec, Mat>& operator+=(const CTransform<T, Vec, Mat>& other)
    {
        Position() += const_cast<CTransform&>(other).Position();
        Rotation() += const_cast<CTransform&>(other).Rotation();
        Scale() *= const_cast<CTransform&>(other).Scale();
        
        return *this;
    }
    
    INL Mat operator()()
    {
        Mat transform(1.0);
        
        Mat p = glm::translate(Mat(1.0), Position());
        Mat r(1.0); // TODO: glm
        Mat s = glm::scale(Mat(1.0), Scale());
        
        transform *= p;
        transform *= r;
        transform *= s;
        
        return transform;
    }
    
private:
    Vec m_Position;
    Vec m_Rotation;
    Vec m_Scale;
};

}; // namespace jam

#endif /* defined(CTRANSFORM_HPP) */
