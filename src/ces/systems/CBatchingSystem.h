//
//  CBatchingSystem.hpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 4/6/16.
//
//

#ifndef CBATCHINGSYSTEM_H
#define CBATCHINGSYSTEM_H

#include "ISystem.h"

namespace jam
{
CLASS_PTR(CBatchingSystem);
    
CLASS_PTR(IMesh);
CLASS_PTR(IMaterial);
CLASS_PTR(IShaderProgram);
CLASS_PTR(ITexture);
CLASS_PTR(IVertexBuffer);
CLASS_PTR(IIndexBuffer);
CLASS_PTR(IRenderer);

class CBatchingSystem : public ISystem
{
    JAM_OBJECT
public:
    CBatchingSystem(IRendererPtr renderer);
    virtual ~CBatchingSystem();
    
    virtual void Update(unsigned long dt) override;
    
private:
    struct SGeometry;
    SGeometry CreateNewGeometry(IMaterialPtr material,
                                ITexturePtr texture,
                                IShaderProgramPtr shader,
                                uint64_t elementVertexSize,
                                uint64_t elementIndexSize) const;
    void CopyToBuffer(IMeshPtr srcMesh, SGeometry& geometry);
    void ApplyTransform(IMeshPtr mesh,
                        uint64_t offset,
                        uint64_t size,
                        glm::mat4 oldTransform,
                        glm::mat4 transform);
    
private:
    IRendererPtr m_Renderer;
    
    struct SGeometry
    {
        uint64_t offsetVB;
        uint64_t offsetIB;
        
        IMeshPtr mesh;
        IMaterialPtr material;
        ITexturePtr texture;
        IShaderProgramPtr shader;
        
        SGeometry()
        : offsetVB(0)
        , offsetIB(0)
        , mesh(nullptr)
        , material(nullptr)
        , texture(nullptr)
        , shader(nullptr)
        {}
    };
    
    typedef std::unordered_map<uint64_t, SGeometry> TBatchCache;
    TBatchCache m_Batches;
};
    
} // namespace jam

#endif /* CBATCHINGSYSTEM_H */
