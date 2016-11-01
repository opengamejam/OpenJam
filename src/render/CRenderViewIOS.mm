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
#include "IFrameBuffer.h"
#include "IRenderTarget.h"

#include "CRendererOGLES1_1.h"
#include "CRendererOGLES2_0.h"

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
    switch (m_RenderApi) {
    case OGLES1_1: {
        m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        [EAGLContext setCurrentContext:m_GLContext];

        m_Renderer.reset(new CRendererOGLES1_1(shared_from_this()));
    } break;

    case OGLES2_0: {
        m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        [EAGLContext setCurrentContext:m_GLContext];

        m_Renderer.reset(new CRendererOGLES2_0(shared_from_this()));
    } break;

    case OGLES3_0: { // TODO: OGLES3
        m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        [EAGLContext setCurrentContext:m_GLContext];

        //m_Renderer.reset(new CRendererOGLES1_1(shared_from_this()));
    } break;

    default:
        break;
    };

    CRenderTargetColorPtr colorTarget = m_Renderer->CreateColorRenderTarget();
    colorTarget->Initialize(IRenderTarget::ColorRGBA8888);
    colorTarget->Bind();
    [m_GLContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)(m_GLKView.layer)];
    colorTarget->Allocate(RealWidth(), RealHeight());

    CRenderTargetDepthPtr depthTarget = m_Renderer->CreateDepthRenderTarget();
    depthTarget->Initialize(IRenderTarget::Depth24_Stencil8);
    depthTarget->Bind();
    depthTarget->Allocate(RealWidth(), RealHeight());

    m_DefaultRenderTarget = m_Renderer->CreateFrameBuffer(RealWidth(), RealHeight());
    m_DefaultRenderTarget->Initialize();
    m_DefaultRenderTarget->Bind();
    m_DefaultRenderTarget->AttachColor(colorTarget, 0);
    m_DefaultRenderTarget->AttachDepth(depthTarget);
    m_DefaultRenderTarget->AttachStencil(depthTarget->StencilTarget());

    assert(m_Renderer && m_DefaultRenderTarget->IsValid());
}

void CRenderViewIOS::Begin() const
{
    [EAGLContext setCurrentContext:m_GLContext];
    m_DefaultRenderTarget->ColorAttachement(0)->Bind();
}

void CRenderViewIOS::End() const
{
    [m_GLContext presentRenderbuffer:GL_RENDERBUFFER];
    m_DefaultRenderTarget->ColorAttachement(0)->Unbind();
}

void CRenderViewIOS::UpdateEvents() const
{
}

IRendererPtr CRenderViewIOS::Renderer() const
{
    return m_Renderer;
}

IFrameBufferPtr CRenderViewIOS::DefaultRenderTarget() const
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
