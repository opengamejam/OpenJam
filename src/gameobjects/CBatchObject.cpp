//
//  CBatchObject.cpp
//  OpenJam
//
//  Created by yev on 10/29/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CBatchObject.h"
#include "IMesh.h"

using namespace jam;

CBatchObject::CBatchObject(const CVector3Df& position,
                           const CVector3Df& rotation,
                           const CVector3Df& scale)
: IGameObject("", position, rotation, scale)
, m_Mask(nullptr)
{

}

CBatchObject::~CBatchObject()
{

}

IGameObjectPtr CBatchObject::Mask() const
{
    return m_Mask;
}

void CBatchObject::Mask(IGameObjectPtr mask)
{
    m_Mask = mask;
}

void CBatchObject::Update(unsigned long dt, bool isDeepUpdate)
{
    IGameObject::Update(dt, false);
    
    if (Mask())
    {
        Mask()->Update(dt);
    }
    
    m_BatchedObjects.clear();
    Batch(dt, Childs(), m_BatchedObjects);
}

void CBatchObject::Bind()
{
    IGameObjectPtr mask = Mask();
    if (mask)
    {
        mask->Bind();
    }
}

void CBatchObject::Unbind()
{
    IGameObjectPtr mask = Mask();
    if (mask)
    {
        mask->Unbind();
    }
}

void CBatchObject::Draw(ICameraPtr camera)
{
    IGameObjectPtr mask = Mask();
    if (mask)
    {
        mask->Draw(camera);
    }
    
    const CBatchObject::TBatchedMap& batchedObjects = BatchedChilds();
    std::for_each(batchedObjects.begin(), batchedObjects.end(), [camera](const TBatchedMap::value_type& element)
    {
        const TBatchedMap::value_type::second_type& objects = element.second;
        
        IGameObjectPtr object = objects.front();
        object->Bind();
        
        std::for_each(objects.begin(), objects.end(), [camera](IGameObjectPtr object)
        {
            object->Draw(camera);
        });
        
        /*std::string hash = (object->Mesh() ? object->Mesh()->Hash() : "");
        printf("Object %s has drawn %lu instances, hash: %s\n", object->Filename().c_str(), objects.size(), hash.c_str());*/
        
        object->Unbind();
    });
}

bool CBatchObject::Batchable() const
{
    return false;
}

void CBatchObject::Batchable(bool isBatchable)
{
    
}

const CBatchObject::TBatchedMap& CBatchObject::BatchedChilds() const
{
    return m_BatchedObjects;
}

void CBatchObject::Batch(unsigned long dt, const IGameObject::TGameObjectsList& childs, CBatchObject::TBatchedMap& batchedMap)
{
    std::for_each(childs.begin(), childs.end(), [&](IGameObjectPtr child)
    {
        if (child->Mesh())
        {
            //batchedMap[child->Mesh()->Hash()].push_back(child);
        }
        
        child->Update(dt, false);
        
        if (child->Batchable())
        {
            Batch(dt, child->Childs(), batchedMap);
        }
        else
        {
            batchedMap["non_batchable"].push_back(child);
        }
    });
}