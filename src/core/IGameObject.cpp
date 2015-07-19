//
//  IGameObject.cpp
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 yev. All rights reserved.
//
#include "IGameObject.h"
#include "IMaterial.h"
#include "IShader.h"
#include "IMesh.h"
#include "ICamera.h"

using namespace jam;

unsigned int IGameObject::s_UniqueId = 0;

IGameObject::IGameObject(const std::string& filename,
                         const CVector3Df& position,
                         const CVector3Df& rotation,
                         const CVector3Df& scale,
                         const CVector3Df& size)
: m_Id(s_UniqueId++)
, m_Filename(filename)
, m_IsVisible(true)
, m_AbsoluteTransform(RelativeToParent)
, m_IsBatchable(true)
{
    Position(position);
    Rotation(rotation);
    Scale(scale);
    Size(size);
    
    printf("created: %s\tid: %d\n", Filename().c_str(), Id());
}

IGameObject::~IGameObject()
{
    printf("destroyed: %s\tid: %d\n", Filename().c_str(), Id());
}

unsigned int IGameObject::Id() const
{
    return m_Id; 
}

const std::string& IGameObject::Filename() const
{
    return m_Filename;
}

void IGameObject::Name(const std::string& name)
{
    m_Name = name;
}

const std::string& IGameObject::Name() const
{
    return m_Name;
}

void IGameObject::Position(const CVector3Df& position)
{
    m_Transform.Position(position);
}

const CVector3Df& IGameObject::Position()
{
    return m_Transform.Position();
}

void IGameObject::Offset(const CVector3Df& offset)
{
    m_Transform.Offset(offset);
}

const CVector3Df& IGameObject::Offset()
{
    return m_Transform.Offset();
}

void IGameObject::Rotation(const CVector3Df& rotation)
{
    m_Transform.Rotation(rotation);
}

const CVector3Df& IGameObject::Rotation()
{
    return m_Transform.Rotation();
}

void IGameObject::Scale(const CVector3Df& scale)
{
    m_Transform.Scale(scale);
}

const CVector3Df& IGameObject::Scale()
{
    return m_Transform.Scale();
}

void IGameObject::Size(const CVector3Df& size)
{
    m_Transform.Size(size);
}

const CVector3Df& IGameObject::Size()
{
    return m_Transform.Size();
}

void IGameObject::Visible(bool isVisible)
{
    m_IsVisible = isVisible;
}

bool IGameObject::Visible() const
{
    return m_IsVisible;
}

void IGameObject::Transform(const CTransform3Df& transform)
{
    m_Transform = transform;
}

CTransform3Df IGameObject::Transform()
{
    return m_Transform;
}

CTransform3Df IGameObject::WorldTransform()
{
    CTransform3Df transform = Transform();
    IGameObjectPtr parent = Parent().lock();
    if (parent)
    {
        CTransform3Df externalTransform = parent->Transform();
        if ((AbsoluteTransform() & IGameObject::AbsolutePosition) == IGameObject::AbsolutePosition)
        {
            externalTransform.Position(CVector3Df());
        }
        if ((AbsoluteTransform() & IGameObject::AbsoluteRotation) == IGameObject::AbsoluteRotation)
        {
            externalTransform.Rotation(CVector3Df());
        }
        if ((AbsoluteTransform() & IGameObject::AbsoluteScale) == IGameObject::AbsoluteScale)
        {
            externalTransform.Scale(CVector3Df(1.0f, 1.0f, 1.0f));
        }
        
        transform += externalTransform;
    }
    
    return transform;
}

void IGameObject::AbsoluteTransform(enum IGameObject::AbsoluteTransform isAbsolute)
{
    m_AbsoluteTransform = isAbsolute;
}

enum IGameObject::AbsoluteTransform IGameObject::AbsoluteTransform() const
{
    return m_AbsoluteTransform;
}

IGameObjectWeak IGameObject::Parent() const
{
    return m_Parent;
}

void IGameObject::Parent(IGameObjectWeak parent)
{
    IGameObjectPtr oldPtr = m_Parent.lock();
    IGameObjectPtr newPtr = parent.lock();
    
    if (oldPtr && newPtr && oldPtr->Id() == newPtr->Id())
    {
        return;
    }
    
    if (oldPtr)
    {
        oldPtr->RemoveChild(shared_from_this());
    }
    
    m_Parent = parent;
}

void IGameObject::RemoveFromParent()
{
    IGameObjectPtr parent = m_Parent.lock();
    if (parent)
    {
        parent->RemoveChild(shared_from_this());
    }
    m_Parent = IGameObjectWeak();   // clear
}

void IGameObject::AddChild(IGameObjectPtr child)
{
    assert(child && child->Id() != Id());
    if (child || child->Id() != Id())
    {
        IGameObjectPtr object = FindChild(child->Id());
        if (!object)
        {
            m_Childs.push_back(child);
        }
    }
}

void IGameObject::RemoveChild(IGameObjectPtr child)
{
    assert(child && child->Id() != Id());
    if (child && child->Id() != Id())
    {
        m_ToRemove.push_back(child);
    }
}

void IGameObject::RemoveChildById(unsigned int id)
{
    IGameObjectPtr object = FindChild(id);
    RemoveChild(object);
}

void IGameObject::RemoveAllChilds()
{
    std::for_each(m_Childs.begin(), m_Childs.end(), [&](IGameObjectPtr obj)
    {
        RemoveChild(obj);
    });
}

IGameObjectPtr IGameObject::FindChild(unsigned int id) const
{
    IGameObjectPtr object = nullptr;
    std::all_of( m_Childs.begin(), m_Childs.end(), [&](IGameObjectPtr obj)
    {
        if (id == obj->Id())
        {
            object = obj;
            return false;
        }
        
        return true;
    });
    
    return object;
}

IGameObjectPtr IGameObject::FindFirstChild(const std::string& name) const
{
    IGameObjectPtr object = nullptr;
    std::all_of( m_Childs.begin(), m_Childs.end(), [&](IGameObjectPtr obj)
    {
        if (name == obj->Name())
        {
            object = obj;
            return false;
        }
        
        return true;
    });
    
    return object;
}

const IGameObject::TGameObjectsList& IGameObject::Childs() const
{
    return m_Childs;
}

void IGameObject::Load(const std::string& filename)
{
    
}

void IGameObject::Update(unsigned long dt, bool isDeepUpdate)
{
    if (Visible() && Animator())
    {
        //Animator()->Update(dt);
    }
    
    IMaterialPtr material = nullptr;
    if (Mesh())
    {
        //material = Mesh()->Material();
    }
    
    /*if (material)
    {
        unsigned int modelLoc = material->Shader()->ModelMatrix();
        material->BindUniformMatrix4x4f(modelLoc, WorldTransform()());
    }*/
    
    TGameObjectsList::const_iterator beg = Childs().begin();
    TGameObjectsList::const_iterator end = Childs().end();
    while (beg != end)
    {
        (*beg)->Parent(shared_from_this());
        if (isDeepUpdate)
        {
            (*beg)->Update(dt, isDeepUpdate);
        }
        ++beg;
    }
    
    while (!m_ToRemove.empty())
    {
        m_Childs.remove(m_ToRemove.front());
        m_ToRemove.erase(m_ToRemove.begin());
    }
}

void IGameObject::Bind()
{
    if (Mesh())
    {
        Mesh()->Bind();
    }
}

void IGameObject::Unbind()
{
    if (Mesh())
    {
        Mesh()->Unbind();
    }
}

void IGameObject::Draw(ICameraPtr camera)
{
    if (!Visible())
    {
        return;
    }
    
    IMeshPtr mesh = Mesh();
    IMaterialPtr material = nullptr;
    if (mesh)
    {
        //material = mesh->Material();
    }
    if (material)
    {
        //material->BindUniformMatrix4x4f(material->Shader()->ProjectionMatrix(), camera->ProjectionMatrix());
        material->UpdateUniforms();
        //API()->DrawMesh(mesh);
    }
}

IAnimatorPtr IGameObject::Animator() const
{
    return m_Animator;
}

void IGameObject::Animator(IAnimatorPtr animator)
{
    m_Animator = animator;
}

IMeshPtr IGameObject::Mesh() const
{
    return m_Mesh;
}

void IGameObject::Mesh(IMeshPtr mesh)
{
    m_Mesh = mesh;
}

bool IGameObject::Batchable() const
{
    return m_IsBatchable;
}

void IGameObject::Batchable(bool isBatchable)
{
    m_IsBatchable = isBatchable;
}