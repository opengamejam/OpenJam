//
//  IGameObject.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#ifndef IGAMEOBJECT_H
#define	IGAMEOBJECT_H

#include "Global.h"
#include "IEventable.h"
#include "CVector2D.h"
#include "CVector3D.h"
#include "CMatrix4x4.h"
#include "CRect2D.h"
#include "CTransform.h"

namespace jam
{
CLASS_PTR(IMaterial);
CLASS_PTR(IShader);
CLASS_PTR(IMesh);
CLASS_PTR(IAnimator);
CLASS_PTR(ICamera);
CLASS_PTR(IGameObject);
CLASS_WEAK(IGameObject);

class IGameObject : public IEventable, public std::enable_shared_from_this<IGameObject>
{
public:
    enum AbsoluteTransform
    {
        RelativeToParent = 0,
        AbsolutePosition = 0x1,
        AbsoluteRotation = 0x2,
        AbsoluteScale = 0x4,
        AbsolutePositionRotation        = (AbsolutePosition | AbsoluteRotation),
        AbsolutePositionScale           = (AbsolutePosition | AbsoluteScale),
        AbsoluteRotationScale           = (AbsoluteRotation | AbsoluteScale),
        AbsolutePositionRotationScale   = (AbsolutePosition | AbsoluteRotation | AbsoluteScale)
    };
    
    typedef std::list<IGameObjectPtr> TGameObjectsList;
    
public:
    IGameObject(const std::string& filename,
                const CVector3Df& position = CVector3Df(),
                const CVector3Df& rotation = CVector3Df(),
                const CVector3Df& scale = CVector3Df(1.0f, 1.0f, 1.0f),
                const CVector3Df& size = CVector3Df(1.0f, 1.0f, 1.0f));
    
    virtual ~IGameObject();
    
    virtual unsigned int         Id() const;
    virtual const std::string&   Filename() const;
    
    virtual void                 Name(const std::string& name);
    virtual const std::string&   Name() const;
    
    virtual void                 Position(const CVector3Df& position);
    virtual const CVector3Df&    Position();
	
    virtual void                 Offset(const CVector3Df& offset);
    virtual const CVector3Df&    Offset();
    
    virtual void                 Rotation(const CVector3Df& rotation);
    virtual const CVector3Df&    Rotation();
    
    virtual void                 Scale(const CVector3Df& scale);
    virtual const CVector3Df&    Scale();

    virtual void                 Size(const CVector3Df& size);
    virtual const CVector3Df&    Size();
    
    virtual void                 Visible(bool isVisible);
    virtual bool                 Visible() const;
    
    virtual void                 Transform(const CTransform3Df& transform);
    virtual CTransform3Df        Transform();
    virtual CTransform3Df        WorldTransform();
    virtual void                 AbsoluteTransform(enum AbsoluteTransform isAbsolute);  // TODO: Lock transform
    virtual enum AbsoluteTransform AbsoluteTransform() const;
    
    // Childs
    virtual IGameObjectWeak      Parent() const;
    virtual void                 Parent(IGameObjectWeak parent);
    virtual void                 RemoveFromParent();
    
    virtual void                 AddChild(IGameObjectPtr child);
    virtual void                 RemoveChild(IGameObjectPtr child);
    virtual void                 RemoveChildById(unsigned int id);
    virtual void                 RemoveAllChilds();
    virtual IGameObjectPtr       FindChild(unsigned int id) const;
    virtual IGameObjectPtr       FindFirstChild(const std::string& name) const;
    virtual const TGameObjectsList& Childs() const;
    
    virtual void                 Load(const std::string& filename);
    virtual void                 Update(unsigned long dt, bool isDeepUpdate = true);
    
    virtual void                 Bind();
    virtual void                 Unbind();
    virtual void                 Draw(ICameraPtr camera);
    
    virtual IAnimatorPtr         Animator() const;
    virtual void                 Animator(IAnimatorPtr animator);
    virtual IMeshPtr             Mesh() const;
    virtual void                 Mesh(IMeshPtr mesh);
    
    virtual bool                 Batchable() const;
    virtual void                 Batchable(bool isBatchable);
    
private:
    IGameObjectWeak              m_Parent;    
    TGameObjectsList            m_Childs;
    std::list<IGameObjectPtr>    m_ToRemove;
    
    unsigned int                 m_Id;
    std::string                  m_Filename;
    std::string                  m_Name;
    bool                         m_IsVisible;
    
    CTransform3Df                m_Transform;
    enum AbsoluteTransform       m_AbsoluteTransform;
    
    IAnimatorPtr                 m_Animator;
    IMeshPtr                     m_Mesh;
    
    bool                         m_IsBatchable;
    
    static unsigned int          s_UniqueId;
};
    
}; // namespace jam

#endif	/* CGAMEOBJECT_H */

