/* 
 * File:   CGameObject2D.h
 * Author: yevgeniy.logachev
 *
 * Created on September 10, 2012, 5:36 PM
 */

#ifndef CGAMEOBJECT2D_H
#define	CGAMEOBJECT2D_H

#include "IGameObject.h"
#include "CGameObjectDesc.h"

#include "CMeshOGLES20.h"
#include "CSpriteXML.h"
#include "CTextureOGLES20.h"
#include "CMaterialOGLES20.h"
#include "CAnimator2D.h"
#include "IShader.h"
#include "ICamera.h"
#include "CMeshOGLES1.h"
#include "CMaterialOGLES1.h"

#include "CResourceCache.hpp"

namespace jam
{

// *****************************************************************************
// Declaration
// *****************************************************************************

template <class MeshClass = CMeshOGLES1, class MaterialClass = CMaterialOGLES1, class SpriteClass = CSpriteXML>
class CGameObject2D : public IGameObject
{
public:
	CGameObject2D(const std::string& filename,
                  const CVector3Df& position = CVector3Df(),
                  const CVector3Df& rotation = CVector3Df(),
                  const CVector3Df& scale = CVector3Df(1.0f, 1.0f, 1.0f));
    CGameObject2D(const CGameObjectDesc& desc);
    virtual ~CGameObject2D();
    
    virtual void Load(const std::string& filename);
    
private:
    void OnFrameUpdated(const CFrameDef& frame);
};

CLASS_TEMPL_PTR(CGameObject2D<>, CGameObject2D);             // CGameObject2DPtr
CLASS_TEMPL_PTR(CGameObject2D<CMeshOGLES1>, CRectObject); // CRectObjectPtr

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

template <class MeshClass, class MaterialClass, class SpriteClass>
CGameObject2D<MeshClass, MaterialClass, SpriteClass>::CGameObject2D(const std::string& filename,
                                                                    const CVector3Df& position,
                                                                    const CVector3Df& rotation,
                                                                    const CVector3Df& scale)
: IGameObject(filename, position, rotation, scale)
{
    Load(filename);
}

template <class MeshClass, class MaterialClass, class SpriteClass>
CGameObject2D<MeshClass, MaterialClass, SpriteClass>::CGameObject2D(const CGameObjectDesc& desc)
: IGameObject(desc.filename, desc.position, desc.rotation, desc.scale, desc.size)
{
    Load(desc.filename);
    Name(desc.name);
}

template <class MeshClass, class MaterialClass, class SpriteClass>
void CGameObject2D<MeshClass, MaterialClass, SpriteClass>::Load(const std::string& filename)
{
    if (filename.empty())
    {
        return;
    }
    
    /*CResourceCache resourceCache;
    // Mesh & Materials is unique resources per each objects
    std::string materialName = filename + ".mat";
    IMaterialPtr material = resourceCache.AcquireResource<IMaterial>(materialName, true,
                                                                     [](const std::string& filename) -> IResourcePtr
    {
        IMaterialPtr material(new MaterialClass(filename));
     
        return material;
    });*/
    //unsigned int spriteMatrixLoc = material->Shader()->Uniform("spriteMatrix");
    //material->BindUniformMatrix4x4f(spriteMatrixLoc, CMatrix4x4f::Identity());
    
    /*IMeshPtr mesh = resourceCache.AcquireResource<IMesh>(filename, true,
                                                         [material](const std::string& filename) -> IResourcePtr
    {
        IMeshPtr mesh(new MeshClass(filename));
     
        return mesh;
    });
    assert(mesh);
    mesh->Material(material);
    Mesh(mesh);*/
    
    /*std::string spriteName = filename + ".mpf";
    ISpritePtr sprite = resourceCache.AcquireResource<ISprite>(spriteName, false,
                                                               [](const std::string& filename) -> IResourcePtr
    {
        ISpritePtr sprite(new SpriteClass(filename));
     
        return sprite;
    });*/
    
    /*if (sprite)
    {
        CAnimator2DPtr animator = std::make_shared<CAnimator2D>(CAnimator2D(sprite));
        Animator(animator);
        animator->OnFrameUpdated(std::bind(&CGameObject2D::OnFrameUpdated, this, std::placeholders::_1));
        
        const ISprite::TTexturesList texturesList = sprite->Textures();
        unsigned int textureSetIndex = 0;
        std::for_each(texturesList.begin(), texturesList.end(), [&](const std::string& textureName)
        {
            std::vector<std::string> textures = {textureName};
            material->LoadTextures(textures, textureSetIndex);
          
            textureSetIndex++;
        });
    }*/
}

template <class MeshClass, class MaterialClass, class SpriteClass>
CGameObject2D<MeshClass, MaterialClass, SpriteClass>::~CGameObject2D()
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
template <class MeshClass, class MaterialClass, class SpriteClass>
void CGameObject2D<MeshClass, MaterialClass, SpriteClass>::OnFrameUpdated(const CFrameDef& frame)
{
    /*IMaterialPtr material = nullptr;
    if (Mesh())
    {
        material = Mesh()->Material();
    }
    
    if (!material)
    {
        return;
    }
    
    CVector3Df size = CVector3Df(frame.originalWidth, frame.originalHeight, 1.0f);
    CVector3Df offset = CVector3Df(frame.offsetX, frame.offsetY, 0.0f);
    
    Size(size);
    Offset(offset);
    
    CMatrix4x4f spriteMatrix = CMatrix4x4f::Identity();
    spriteMatrix.Translate(frame.textureFrame->u);
    CVector3Df scale = (frame.textureFrame->v - frame.textureFrame->u);
    scale.Z(1.0f);
    spriteMatrix.Scale(scale);
    
    //unsigned int spriteMatrixLoc = material->Shader()->Uniform("spriteMatrix");
    //material->BindUniformMatrix4x4f(spriteMatrixLoc, spriteMatrix);
    material->ActiveTextureSet(frame.textureFrame->textureIdx);*/

}

}; // namespace jam

#endif	/* CGAMEOBJECT2D_H */

