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
#include "CTransform.hpp"

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
    
    bool Initialize(IMaterialPtr material,
                    IShaderProgramPtr shader,
                    const std::list<ITexturePtr>& textures,
                    uint64_t elementVertexSize,
                    uint64_t elementIndexSize,
                    uint64_t maxVertexBufferSize = k_MaxBufferSize,
                    uint64_t maxIndexBufferSize = k_MaxBufferSize);
    bool IsInitialized() const;
    void Shutdown();
    
    const IMeshPtr Mesh() const;
    const IMaterialPtr Material() const;
    const IShaderProgramPtr Shader() const;
    const std::list<ITexturePtr>& Textures() const;
    
    bool AddGeometry(IMeshPtr mesh, const CTransform3Df& transform);
    void RemoveGeometry(IMeshPtr mesh);
    void Update();
    
public:
    static const uint64_t k_MaxBufferSize;
    
private:
    bool m_IsDirty;
    IMeshPtr m_BatchedMesh;
    IMaterialPtr m_Material;
    IShaderProgramPtr m_ShaderProgram;
    std::list<ITexturePtr> m_Textures;
    
    struct SGeometry
    {
        uint64_t offsetVB;
        uint64_t sizeVB;
        uint64_t offsetIB;
        uint64_t sizeIB;
        CTransform3Df transform;
        IMeshWeak mesh;
        
        SGeometry(uint64_t _offsetVB = 0,
                  uint64_t _sizeVB = 0,
                  uint64_t _offsetIB = 0,
                  uint64_t _sizeIB = 0,
                  const CTransform3Df& _transform = CTransform3Df(),
                  IMeshPtr _mesh = nullptr)
        : offsetVB(_offsetVB)
        , sizeVB(_sizeVB)
        , offsetIB(_offsetIB)
        , sizeIB(_sizeIB)
        , transform(_transform)
        , mesh(_mesh)
        {
        }
    };
    
    typedef std::map<uint64_t, SGeometry> TGeometries;
    TGeometries m_Geometries;
    
    uint64_t m_VertexOffset;
    uint64_t m_IndexOffset;
};
    
}; // namespace jam

#endif /* defined(__TestApp__CBatch__) */
