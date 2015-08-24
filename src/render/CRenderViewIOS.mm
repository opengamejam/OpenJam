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
#include "IEventable.h"
#include "IEventDispatcher.hpp"
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

CRenderViewIOS::CRenderViewIOS(unsigned int width, unsigned int height, void* glkView, RenderApi renderApi)
	: IRenderView(width, height)
	, m_GLKView((__bridge UIView*)glkView)
    , m_GLContext(nil)
    , m_RenderApi(renderApi)
    , m_DefaultRenderTarget(nullptr)
{
    IEventable::RegisterDispatcher(std::make_shared<IEventDispatcher>(IEventDispatcher()));
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
            
            GRenderer.reset(new CRendererOGLES1_1(shared_from_this()));
            
            glGenRenderbuffers(1, &m_ColorBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_ColorBuffer);
            [m_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(m_GLKView.layer)];
            
            std::shared_ptr<CFrameBufferOGLES1_1> renderTarget(new CFrameBufferOGLES1_1(Width(), Height()));
            renderTarget->Initialize(-1, m_ColorBuffer);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ColorBuffer);
            glViewport(0, 0, Width(), Height());
            m_DefaultRenderTarget = renderTarget;
        }
        break;
            
        case OGLES2_0:
        {
            m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            [EAGLContext setCurrentContext:m_GLContext];
            
            GRenderer.reset(new CRendererOGLES2_0(shared_from_this()));
            
            glGenRenderbuffers(1, &m_ColorBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_ColorBuffer);
            [m_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(m_GLKView.layer)];
            
            std::shared_ptr<CFrameBufferOGLES2_0> renderTarget(new CFrameBufferOGLES2_0(Width(), Height()));
            renderTarget->Initialize(-1, m_ColorBuffer);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ColorBuffer);
            glViewport(0, 0, Width(), Height());
            m_DefaultRenderTarget = renderTarget;
        }
        break;
            
        case OGLES3_0:
        {   // TODO: OGLES3
            m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
            [EAGLContext setCurrentContext:m_GLContext];
            
            GRenderer.reset(new CRendererOGLES1_1(shared_from_this()));
            
            glGenRenderbuffers(1, &m_ColorBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_ColorBuffer);
            [m_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(m_GLKView.layer)];
            
            std::shared_ptr<CFrameBufferOGLES1_1> renderTarget(new CFrameBufferOGLES1_1(Width(), Height()));
            renderTarget->Initialize(-1, m_ColorBuffer);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ColorBuffer);
            glViewport(0, 0, Width(), Height());
            m_DefaultRenderTarget = renderTarget;
        }
        break;
            
        default:
            break;
    };
    
    m_DefaultRenderTarget->CreateDepthAttachment();
    m_DefaultRenderTarget->CreateStencilAttachment();
    
    assert(GRenderer);
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
