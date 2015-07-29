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
#include "IEventable.h"
#include "IEventDispatcher.hpp"
#include "RenderGlobal.h"

#include "CRendererOGLES1.h"

#include "CFrameBufferTargetOGLES1.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderViewOSX::CRenderViewOSX(unsigned int width, unsigned int height, void* glkView, RenderApi renderApi)
	: IRenderView(width, height)
	, m_GLView((__bridge NSOpenGLView*)glkView)
    , m_GLContext(nil)
    , m_RenderApi(renderApi)
    , m_DefaultRenderTarget(nullptr)
{
    IEventable::RegisterDispatcher(std::make_shared<IEventDispatcher>(IEventDispatcher()));
}

CRenderViewOSX::~CRenderViewOSX()
{
}

void CRenderViewOSX::CreateView()
{
    NSOpenGLPixelFormat *pixelformat = nil;
    switch (m_RenderApi)
    {
        case OGL1:
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
            
            GRenderer.reset(new CRendererOGLES1(shared_from_this()));
            
            std::shared_ptr<CFrameBufferTargetOGLES1> renderTarget(new CFrameBufferTargetOGLES1(Width(), Height()));
            renderTarget->Initialize(0, 0);
            m_DefaultRenderTarget = renderTarget;
        }
        break;
            
        case OGL3:
        {
            /*m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            GRenderer.reset(new CRendererOGLES2(shared_from_this()));*/
            CGLEnable([m_GLContext CGLContextObj], kCGLCECrashOnRemovedFunctions);
        }
        break;
            
        case OGL4:
        {
            /*m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
            GRenderer.reset(new CRendererOGLES1(shared_from_this()));*/
        }
        break;
            
        default:
            break;
    };
    
    [m_GLView setPixelFormat:pixelformat];
    [m_GLView setOpenGLContext:m_GLContext];
    
    GLint swap = 1;
    [m_GLContext setValues:&swap forParameter:NSOpenGLCPSwapInterval];
    
    m_DefaultRenderTarget->ClearColor(CColor(0.0f, 0.0f, 1.0f, 1.0f));
    
    //glGenRenderbuffers(1, &m_RenderBuffer);
    //glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, Width(), Height());
    
    /*IRenderTargetPtr defaultRenderTarget = GRenderer->CreateRenderTarget(Width(), Height());
    defaultRenderTarget->AttachRenderBuffer(RenderBuffer(), false);
    defaultRenderTarget->AttachDepthBuffer();
    defaultRenderTarget->ClearColor(CColor(1.0f, 0.0f, 1.0f, 1.0f));
    
    defaultRenderTarget->Bind();
    defaultRenderTarget->Clear();*/
    
    //defaultRenderTarget->Unbind();
    
    glClearColor(1, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    assert(GRenderer);
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

IRenderTargetPtr CRenderViewOSX::DefaultRenderTarget() const
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
