//
//  CTextureRenderTarget.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if 0

#include "CTextureRenderTarget.h"
#include "CTexture.h"
#include "CImageRaw.h"
#include "CResourceCache.hpp"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CTextureRenderTarget::CTextureRenderTarget(unsigned int width, unsigned int height)
: CFrameBufferRenderTarget(width, height)
, m_Texture(nullptr)
{
    CResourceCache resourceCache;
    
    IImagePtr image = resourceCache.AcquireResource<IImage>("renderTextureImage", true,
                                                            [=](const std::string& filename) -> IResourcePtr
    {
        IResource::TResourceData textureData(width * height * 4);  // RGBA8888
        memset(&textureData[0], 0, textureData.size());
        
        IImagePtr image(new CImageRaw(filename, textureData, width, height, TexelProps(TF_RGBA8888, TT_UByte, 1)));
      
        return image;
    });
    
    if (image)
    {
        m_Texture.reset(new CTexture("renderTextureImage", image));
        
        image->Unload();
    }
    
    if (m_Texture)
    {
        AttachRenderBuffer(m_Texture->Id(), true);
    }
}

CTextureRenderTarget::~CTextureRenderTarget()
{
}

ITexturePtr CTextureRenderTarget::Texture() const
{
    return m_Texture;
}

void CTextureRenderTarget::SaveImage(const std::string& filepath)
{
#if defined(__OBJC__) && defined(OS_IPHONE)
    IRenderTarget::TRawData rawData = RawData();
    
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, &rawData[0], rawData.size(), NULL);
    
    unsigned int bitsPerComponent = 8;
    unsigned int bitsPerPixel = 32;
    unsigned int bytesPerRow = 4 * RealWidth();
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    CGImageRef imageRef = CGImageCreate(RealWidth(),
                                        RealHeight(),
                                        bitsPerComponent,
                                        bitsPerPixel,
                                        bytesPerRow,
                                        colorSpaceRef,
                                        bitmapInfo,
                                        provider, NULL, NO, renderingIntent);
    UIImage *image = [UIImage imageWithCGImage:imageRef];
    
    UIImageView *imageView = [[UIImageView alloc] initWithImage:image];
    UIGraphicsBeginImageContext(CGSizeMake(RealWidth(), RealHeight()));
    CGContextRef context = UIGraphicsGetCurrentContext();
    [imageView.layer renderInContext:context];
    image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *imageFilePath = [[paths objectAtIndex:0] stringByAppendingPathComponent:
                               [NSString stringWithCString:filepath.c_str()
                                                  encoding:[NSString defaultCStringEncoding]]];
    [UIImagePNGRepresentation(image) writeToFile:imageFilePath atomically:YES];
#endif
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif