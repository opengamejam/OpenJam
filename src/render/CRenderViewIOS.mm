//
//  CRenderViewIOS.mm
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_IPHONE)

#include "CRenderViewIOS.h"
#import <OpenGLES/EAGL.h>
#import "GLKit/GLKit.h"
#include "RenderGlobal.h"
#include "IRenderTarget.h"

#include "CRendererOGLES1_1.h"
#include "CRendererOGLES2_0.h"
#include "CFrameBufferOGLES1_1.h"
#include "CFrameBufferOGLES2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderViewIOS::CRenderViewIOS(void* glkView, RenderApi renderApi)
	: IRenderView(((__bridge UIView*)glkView).frame.size.width,
                  ((__bridge UIView*)glkView).frame.size.height,
                  ((__bridge UIView*)glkView).contentScaleFactor)
	, m_GLKView((__bridge UIView*)glkView)
    , m_GLContext(nil)
    , m_RenderApi(renderApi)
    , m_Renderer(nullptr)
    , m_DefaultRenderTarget(nullptr)
{
}

CRenderViewIOS::~CRenderViewIOS()
{

}

void CRenderViewIOS::CreateView()
{
    switch (m_RenderApi)
    {
        case OGLES1_1:
        {
            m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
            [EAGLContext setCurrentContext:m_GLContext];
            
            m_Renderer.reset(new CRendererOGLES1_1(shared_from_this()));
            
            glGenRenderbuffers(1, &m_ColorBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_ColorBuffer);
            [m_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(m_GLKView.layer)];
            
            std::shared_ptr<CFrameBufferOGLES1_1> renderTarget(new CFrameBufferOGLES1_1(RealWidth(), RealHeight()));
            renderTarget->Initialize(-1, m_ColorBuffer);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ColorBuffer);
            glViewport(0, 0, RealWidth(), RealHeight());
            m_DefaultRenderTarget = renderTarget;
        }
        break;
            
        case OGLES2_0:
        {
            m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            [EAGLContext setCurrentContext:m_GLContext];
            
            m_Renderer.reset(new CRendererOGLES2_0(shared_from_this()));
            
            glGenRenderbuffers(1, &m_ColorBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_ColorBuffer);
            [m_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(m_GLKView.layer)];
            
            std::shared_ptr<CFrameBufferOGLES2_0> renderTarget(new CFrameBufferOGLES2_0(RealWidth(), RealHeight()));
            renderTarget->Initialize(-1, m_ColorBuffer);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ColorBuffer);
            glViewport(0, 0, RealWidth(), RealHeight());
            m_DefaultRenderTarget = renderTarget;
        }
        break;
            
        case OGLES3_0:
        {   // TODO: OGLES3
            m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
            [EAGLContext setCurrentContext:m_GLContext];
            
            m_Renderer.reset(new CRendererOGLES1_1(shared_from_this()));
            
            glGenRenderbuffers(1, &m_ColorBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_ColorBuffer);
            [m_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(m_GLKView.layer)];
            
            std::shared_ptr<CFrameBufferOGLES1_1> renderTarget(new CFrameBufferOGLES1_1(RealWidth(), RealHeight()));
            renderTarget->Initialize(-1, m_ColorBuffer);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ColorBuffer);
            glViewport(0, 0, RealWidth(), RealHeight());
            m_DefaultRenderTarget = renderTarget;
        }
        break;
            
        default:
            break;
    };
    
    m_DefaultRenderTarget->CreateDepthAttachment();
    m_DefaultRenderTarget->CreateStencilAttachment();
    
    assert(m_Renderer);
}

void CRenderViewIOS::Begin() const
{
    [EAGLContext setCurrentContext:m_GLContext];
    glBindRenderbuffer(GL_RENDERBUFFER, m_ColorBuffer);
}

void CRenderViewIOS::End() const
{
    [m_GLContext presentRenderbuffer:GL_RENDERBUFFER];
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CRenderViewIOS::UpdateEvents() const
{

}

IRendererPtr CRenderViewIOS::Renderer() const
{
    return m_Renderer;
}

IRenderTargetPtr CRenderViewIOS::DefaultRenderTarget() const
{
    return m_DefaultRenderTarget;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* OS_IPHONE */
