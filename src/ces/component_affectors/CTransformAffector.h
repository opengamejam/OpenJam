//
//  CTransformAffector.h
//  TestApp
//
//  Created by yev on 8/7/15.
//
//

#ifndef CTRANSFORMAFFECTOR_H
#define CTRANSFORMAFFECTOR_H

#include "Global.h"

namespace jam
{
CLASS_PTR(IEntity)
CLASS_PTR(CTransformationComponent)

class CTransformAffector
{
public:
    CTransformAffector() = default;
    ~CTransformAffector() = default;
    
    static void Translating(IEntityPtr entity, const glm::vec3& translation);
    static void Rotating(IEntityPtr entity, const glm::vec3& rotation);
    static void Scaling(IEntityPtr entity, const glm::vec3& scaling);
    
    static void Position(IEntityPtr entity, const glm::vec3& position);
    static void Rotation(IEntityPtr entity, const glm::vec3& rotation);
    static void Scale(IEntityPtr entity, const glm::vec3& scaling);
    
    static glm::vec3 Position(IEntityPtr entity);
    static glm::vec3 Rotation(IEntityPtr entity);
    static glm::vec3 Scale(IEntityPtr entity);
    
    static void Translating(CTransformationComponentPtr component, const glm::vec3& translation);
    static void Rotating(CTransformationComponentPtr component, const glm::vec3& rotation);
    static void Scaling(CTransformationComponentPtr component, const glm::vec3& scaling);
    
    static void Position(CTransformationComponentPtr component, const glm::vec3& position);
    static void Rotation(CTransformationComponentPtr component, const glm::vec3& rotation);
    static void Scale(CTransformationComponentPtr component, const glm::vec3& scaling);
};

}; // namespace jam

#endif /* defined(CTRANSFORMAFFECTOR_H) */
