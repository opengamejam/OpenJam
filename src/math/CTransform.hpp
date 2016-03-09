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
typedef CTransform<float, glm::vec2, glm::mat3x3> CTransform2Df;

    
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
    , m_IsDirty(true)
    {
        
    }
    
    virtual ~CTransform()
    {
    }
    
    INL Vec& Position()
    {
        m_IsDirty = true;
        return m_Position;
    }
    
    INL const Vec& Position() const
    {
        return m_Position;
    }
    
    INL void Position(const Vec& position)
    {
        m_IsDirty = true;
        m_Position = position;
    }
    
    INL Vec& Rotation()
    {
        m_IsDirty = true;
        return m_Rotation;
    }
    
    INL const Vec& Rotation() const
    {
        return m_Rotation;
    }
    
    INL void Rotation(const Vec& rotation)
    {
        m_IsDirty = true;
        m_Rotation = rotation;
    }
    
    INL Vec& Scale()
    {
        m_IsDirty = true;
        return m_Scale;
    }
    
    INL const Vec& Scale() const
    {
        return m_Scale;
    }
    
    INL void Scale(const Vec& scale)
    {
        m_IsDirty = true;
        m_Scale = scale;
    }
    
    INL CTransform<T, Vec, Mat> operator+(const CTransform<T, Vec, Mat>& other) const
    {
        CTransform<T, Vec, Mat> result(*this);
        result += other;
        
        return result;
    }
    
    INL CTransform<T, Vec, Mat>& operator+=(const CTransform<T, Vec, Mat>& other)
    {
        Position() += const_cast<CTransform&>(other).Position();
        Rotation() += const_cast<CTransform&>(other).Rotation();
        Scale() *= const_cast<CTransform&>(other).Scale();
        
        return *this;
    }
    
    INL CTransform<T, Vec, Mat> operator-(const CTransform<T, Vec, Mat>& other) const
    {
        CTransform<T, Vec, Mat> result(*this);
        result -= other;
        
        return result;
    }
    
    INL CTransform<T, Vec, Mat>& operator-=(const CTransform<T, Vec, Mat>& other)
    {
        Position() -= const_cast<CTransform&>(other).Position();
        Rotation() -= const_cast<CTransform&>(other).Rotation();
        Scale() /= const_cast<CTransform&>(other).Scale();
        
        return *this;
    }
    
    INL Mat operator()() const
    {
        if (m_IsDirty)
        {
            m_Transform = glm::translate(Mat(1.0f), Position());
            
            const glm::vec3& rot = Rotation();
            m_Transform = glm::rotate(m_Transform, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
            m_Transform = glm::rotate(m_Transform, rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
            m_Transform = glm::rotate(m_Transform, rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
            
            m_Transform = glm::scale(m_Transform, Scale());
            
            m_IsDirty = false;
        }
        
        return m_Transform;
    }
    
    INL bool IsZero() const
    {
        return (Position().x == 0.0f &&
                Position().y == 0.0f &&
                Position().z == 0.0f &&
                Rotation().x == 0.0f &&
                Rotation().y == 0.0f &&
                Rotation().z == 0.0f &&
                Scale().x == 0.0f &&
                Scale().y == 0.0f &&
                Scale().z == 0.0f);
    }
    
private:
    Vec m_Position;
    Vec m_Rotation;
    Vec m_Scale;
    mutable bool m_IsDirty;
    mutable Mat m_Transform;
};

}; // namespace jam

#endif /* defined(CTRANSFORM_HPP) */
