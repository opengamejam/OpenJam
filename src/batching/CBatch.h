//
//  CBatch.h
//  TestApp
//
//  Created by yev on 8/25/15.
//
//

#ifndef __TestApp__CBatch__
#define __TestApp__CBatch__

#include "Global.h"
#include "CTransform.h"

namespace jam
{
CLASS_PTR(IMesh);
CLASS_WEAK(IMesh);
CLASS_PTR(IMaterial);
CLASS_PTR(IShaderProgram);
CLASS_PTR(ITexture);
CLASS_PTR(IVertexBuffer);
CLASS_PTR(IIndexBuffer);
    
class CBatch
{
public:
    CBatch();
    virtual ~CBatch();
    
    bool Initialize(IMaterialPtr material, IShaderProgramPtr shader, const std::list<ITexturePtr> textures);
    bool IsInitialized() const;
    void Shutdown();
    
    const IMeshPtr Mesh() const;
    const IMaterialPtr Material() const;
    const IShaderProgramPtr Shader() const;
    const std::list<ITexturePtr> Textures() const;
    
    bool AddGeometry(IMeshPtr mesh, const CTransform3Df& transform);
    void RemoveGeometry(IMeshPtr mesh);
    void Update();
    
private:
    bool ValidateStreams(IVertexBufferPtr vb1, IVertexBufferPtr vb2) const;
    
public:
    static const uint64_t k_MaxVertexBufferSize;
    
private:
    bool m_IsDirty;
    IMeshPtr m_BatchedMesh;
    IMaterialPtr m_Material;
    IShaderProgramPtr m_ShaderProgram;
    std::list<ITexturePtr> m_Textures;
    
    struct SGeometry
    {
        IMeshWeak mesh;
        CTransform3Df transform;
    };
    
    typedef std::map<uint64_t, SGeometry> TGeometries;
    TGeometries m_Geometries;
};
    
}; // namespace jam

#endif /* defined(__TestApp__CBatch__) */
