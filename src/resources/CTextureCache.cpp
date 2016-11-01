
//
//  CTextureCache.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 7/7/16.
//
//

#include "CTextureCache.h"
#include "IRenderer.h"
#include "CImagePVR.h"
#include "CImageSegaPVR.h"

using namespace jam;

CTextureCache::CTextureCache(IRendererPtr renderer)
    : m_Renderer(renderer)
{
    assert(m_Renderer);
}

ITexturePtr CTextureCache::Load(const std::string& filename)
{
    IRendererPtr renderer = m_Renderer;

    ITexturePtr texture = AcquireResource(filename, false,
        [renderer](const std::string& filename) -> ITexturePtr {
            CResourceCache<IImage> imageCache;
            IImagePtr image = imageCache.AcquireResource(filename, false,
                [renderer](const std::string& filename) -> IImagePtr {
#ifdef OS_KOS
                    IImagePtr resultImage(new CImageDreamPVR(filename));
#else
                    IImagePtr resultImage(new CImagePVR(filename));
#endif
                    if (!resultImage->Load()) {
                        resultImage = nullptr;
                    }

                    return resultImage;
                });

            ITexturePtr resultTexture = nullptr;
            if (image) {
                resultTexture = renderer->CreateTexture();
                resultTexture->AssignImage(image);
            }

            return resultTexture;
        });

    return texture;
}

