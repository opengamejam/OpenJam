//
//  CTextureCache.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 7/7/16.
//
//

#ifndef CTEXTURECACHE_H
#define CTEXTURECACHE_H

#include "CResourceCache.hpp"
#include "ITexture.h"

namespace jam
{
CLASS_PTR(IRenderer)
CLASS_PTR(ITexture)
  
class CTextureCache final : public CResourceCache<ITexture>
{
public:
    CTextureCache(IRendererPtr renderer);
    virtual ~CTextureCache() = default;
    
    ITexturePtr Load(const std::string& filename);
    
private:
    IRendererPtr m_Renderer;
};
    
}; // namespace jam

#endif /* CTEXTURECACHE_H */
