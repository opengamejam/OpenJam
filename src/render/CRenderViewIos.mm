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

#include "CRendererOGLES2.h"
#include "CRendererOGLES1.h"

#include "IRenderTarget.h"

#include "CFrameBufferTargetOGLES1.h"
#include "CFrameBufferTargetOGLES2.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderViewIos::CRenderViewIos(unsigned int width, unsigned int height, void* glkView, RenderApi renderApi)
	: IRenderView(width, height)
	, m_GLKView((__bridge UIView*)glkView)
    , m_GLContext(nil)
    , m_RenderApi(renderApi)
    , m_DefaultRenderTarget(nullptr)
{
    IEventable::RegisterDispatcher(std::make_shared<IEventDispatcher>(IEventDispatcher()));
}

CRenderViewIos::~CRenderViewIos()
{

}

void CRenderViewIos::CreateView()
{
    switch (m_RenderApi)
    {
        case OGLES1:
        {
            m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
            [EAGLContext setCurrentContext:m_GLContext];
            
            GRenderer.reset(new CRendererOGLES1(shared_from_this()));
            
            glGenRenderbuffers(1, &m_ColorBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_ColorBuffer);
            [m_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(m_GLKView.layer)];
            
            std::shared_ptr<CFrameBufferTargetOGLES1> renderTarget(new CFrameBufferTargetOGLES1(Width(), Height()));
            renderTarget->Initialize(-1, m_ColorBuffer);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ColorBuffer);
            glViewport(0, 0, Width(), Height());
            m_DefaultRenderTarget = renderTarget;
        }
        break;
            
        case OGLES2:
        {
            m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            [EAGLContext setCurrentContext:m_GLContext];
            
            GRenderer.reset(new CRendererOGLES2(shared_from_this()));
            
            glGenRenderbuffers(1, &m_ColorBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_ColorBuffer);
            [m_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(m_GLKView.layer)];
            
            std::shared_ptr<CFrameBufferTargetOGLES2> renderTarget(new CFrameBufferTargetOGLES2(Width(), Height()));
            renderTarget->Initialize(-1, m_ColorBuffer);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ColorBuffer);
            glViewport(0, 0, Width(), Height());
            m_DefaultRenderTarget = renderTarget;
        }
        break;
            
        case OGLES3:
        {
            m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
            [EAGLContext setCurrentContext:m_GLContext];
            
            GRenderer.reset(new CRendererOGLES1(shared_from_this()));
            
            glGenRenderbuffers(1, &m_ColorBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_ColorBuffer);
            [m_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(m_GLKView.layer)];
            
            std::shared_ptr<CFrameBufferTargetOGLES1> renderTarget(new CFrameBufferTargetOGLES1(Width(), Height()));
            renderTarget->Initialize(-1, m_ColorBuffer);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ColorBuffer);
            glViewport(0, 0, Width(), Height());
            m_DefaultRenderTarget = renderTarget;
        }
        break;
            
        default:
            break;
    };
    
    m_DefaultRenderTarget->ClearColor(CColor(0.0f, 0.0f, 1.0f, 1.0f));
    m_DefaultRenderTarget->CreateDepthAttachment();
    m_DefaultRenderTarget->CreateStencilAttachment();
    
    assert(GRenderer);
}

void CRenderViewIos::Begin() const
{
    [EAGLContext setCurrentContext:m_GLContext];
    glBindRenderbuffer(GL_RENDERBUFFER, m_ColorBuffer);
}

void CRenderViewIos::End() const
{
    [m_GLContext presentRenderbuffer:GL_RENDERBUFFER];
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void CRenderViewIos::UpdateEvents() const
{

}

IRenderTargetPtr CRenderViewIos::DefaultRenderTarget() const
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
