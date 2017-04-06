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
    , m_DefaultFrameBuffer(nullptr)
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

    CRenderTargetDepthPtr depthTarget = m_Renderer->CreateDepthRenderTarget();
    depthTarget->Initialize(IRenderTarget::Depth24_Stencil8);

    m_DefaultFrameBuffer = m_Renderer->CreateFrameBuffer(RealWidth(), RealHeight());
    m_DefaultFrameBuffer->Initialize();
    m_DefaultFrameBuffer->Bind();
    m_DefaultFrameBuffer->AttachColor(colorTarget, 0);
    m_DefaultFrameBuffer->AttachDepth(depthTarget);

    m_DefaultFrameBuffer->Bind();
    assert(m_Renderer && m_DefaultFrameBuffer->IsValid());
}

void CRenderViewIOS::Begin() const
{
    [EAGLContext setCurrentContext:m_GLContext];
}

void CRenderViewIOS::End() const
{
    m_DefaultFrameBuffer->ColorAttachement(0)->Bind();
    [m_GLContext presentRenderbuffer:GL_RENDERBUFFER];
}

void CRenderViewIOS::UpdateEvents() const
{
}

IRendererPtr CRenderViewIOS::Renderer() const
{
    return m_Renderer;
}

IFrameBufferPtr CRenderViewIOS::DefaultFrameBuffer() const
{
    return m_DefaultFrameBuffer;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* OS_IPHONE */
