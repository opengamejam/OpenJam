//
//  CBatchObject.h
//  OpenJam
//
//  Created by yev on 10/29/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef __OpenJam__CBatchObject__
#define __OpenJam__CBatchObject__

#include "Global.h"
#include "IGameObject.h"

namespace jam
{

class CBatchObject : public IGameObject
{
public:
    typedef std::map<std::string, std::list<IGameObjectPtr> > TBatchedMap;
    
public:
    CBatchObject(const CVector3Df& position = CVector3Df(),
                 const CVector3Df& rotation = CVector3Df(),
                 const CVector3Df& scale = CVector3Df(1.0f, 1.0f, 1.0f));
    virtual ~CBatchObject();
    
    virtual IGameObjectPtr Mask() const;
    virtual void Mask(IGameObjectPtr mask);
    
    virtual void Update(unsigned long dt, bool isDeepUpdate = false);
    virtual void Bind();
    virtual void Unbind();
    virtual void Draw(ICameraPtr camera);
    virtual bool Batchable() const;
    virtual void Batchable(bool isBatchable);
    
    const TBatchedMap& BatchedChilds() const;
    
private:
    void Batch(unsigned long dt, const TGameObjectsList& childs, TBatchedMap& batchedMap);
    
private:
    TBatchedMap m_BatchedObjects;
    IGameObjectPtr m_Mask;
};

}; // namespace jam

#endif /* defined(__OpenJam__CBatchObject__) */
