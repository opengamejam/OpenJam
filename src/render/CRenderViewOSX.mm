//
//  CRenderViewOSX.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_MAC)

#include "CRenderViewOSX.h"
#import "GLKit/GLKit.h"
#include "RenderGlobal.h"

#include "CRendererOGL1_5.h"
#include "CFrameBufferOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderViewOSX::CRenderViewOSX(unsigned int width, unsigned int height, void* glkView, RenderApi renderApi)
	: IRenderView(width, height, 1.0f)
	, m_GLView((__bridge NSOpenGLView*)glkView)
    , m_GLContext(nil)
    , m_RenderApi(renderApi)
    , m_Renderer(nullptr)
    , m_DefaultRenderTarget(nullptr)
{
}

CRenderViewOSX::~CRenderViewOSX()
{
}

void CRenderViewOSX::CreateView()
{
    NSOpenGLPixelFormat *pixelformat = nil;
    switch (m_RenderApi)
    {
        case OGLLegacy:
        {
            NSOpenGLPixelFormatAttribute attributes[] =
            {
                NSOpenGLPFADoubleBuffer,
                NSOpenGLPFADepthSize, 24,
                NSOpenGLPFAStencilSize, 8,
                NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
                0
            };
            pixelformat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
            assert(pixelformat);
            m_GLContext = [[NSOpenGLContext alloc] initWithFormat:pixelformat
                                                     shareContext:nil];
            [m_GLContext makeCurrentContext];
            
            m_Renderer.reset(new CRendererOGL1_5(shared_from_this()));
            
            std::shared_ptr<CFrameBufferOGL1_5> renderTarget(new CFrameBufferOGL1_5(RealWidth(), RealHeight()));
            renderTarget->Initialize(0, 0);
            m_DefaultRenderTarget = renderTarget;
        }
        break;
            
        case OGL3_2:
        {
            /*m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            m_Renderer.reset(new CRendererOGLES2(shared_from_this()));*/
            CGLEnable([m_GLContext CGLContextObj], kCGLCECrashOnRemovedFunctions);
        }
        break;
            
        case OGL4_1:
        {
            /*m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
            m_Renderer.reset(new CRendererOGLES1(shared_from_this()));*/
        }
        break;
            
        default:
            break;
    };
    
    [m_GLView setPixelFormat:pixelformat];
    [m_GLView setOpenGLContext:m_GLContext];
    
    GLint swap = 1;
    [m_GLContext setValues:&swap forParameter:NSOpenGLCPSwapInterval];
}

void CRenderViewOSX::Begin() const
{
    [m_GLContext makeCurrentContext];
    //glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
}

void CRenderViewOSX::End() const
{
    //glBindRenderbuffer(GL_RENDERBUFFER, 0);
    [m_GLContext flushBuffer];
}

void CRenderViewOSX::UpdateEvents() const
{

}

IRendererPtr CRenderViewOSX::Renderer() const
{
    return m_Renderer;
}

IFrameBufferPtr CRenderViewOSX::DefaultRenderTarget() const
{
    return m_DefaultRenderTarget;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* OS_MAC */
