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
#include "IFrameBuffer.h"
#include "IRenderTarget.h"

#include "CRendererOGL1_5.h"
#include "CFrameBufferOGL1_5.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderViewOSX::CRenderViewOSX(uint32_t width, uint32_t height, void* glkView, RenderApi renderApi)
    : IRenderView(width, height, 1.0f)
    , m_GLView((__bridge NSOpenGLView*)glkView)
    , m_GLContext(nil)
    , m_RenderApi(renderApi)
    , m_Renderer(nullptr)
    , m_DefaultFrameBuffer(nullptr)
{
}

CRenderViewOSX::~CRenderViewOSX()
{
}

void CRenderViewOSX::CreateView()
{
    NSOpenGLPixelFormat* pixelformat = nil;
    switch (m_RenderApi) {
    case OGLLegacy: {
        NSOpenGLPixelFormatAttribute attributes[] = {
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFAColorSize, 32,
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
    } break;

    case OGL3_2: {
        /*m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
            m_Renderer.reset(new CRendererOGLES2(shared_from_this()));*/
        CGLEnable([m_GLContext CGLContextObj], kCGLCECrashOnRemovedFunctions);
    } break;

    case OGL4_1: {
        /*m_GLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
            m_Renderer.reset(new CRendererOGLES1(shared_from_this()));*/
    } break;

    default:
        break;
    };

    [m_GLView setPixelFormat:pixelformat];
    [m_GLView setOpenGLContext:m_GLContext];

    CRenderTargetColorPtr colorTarget = m_Renderer->CreateColorRenderTarget();
    colorTarget->Initialize(IRenderTarget::ColorRGBA8888);

    CRenderTargetDepthPtr depthTarget = m_Renderer->CreateDepthRenderTarget();
    depthTarget->Initialize(IRenderTarget::Depth24);

    CRenderTargetStencilPtr stencilTarget = m_Renderer->CreateStencilRenderTarget();
    stencilTarget->Initialize(IRenderTarget::Stencil8);

    m_DefaultFrameBuffer = m_Renderer->CreateFrameBuffer(RealWidth(), RealHeight());
    // Use 0 for the defaultFBO which is appropriate for
    // OSX (but not iOS since iOS apps must create their own FBO)
    std::static_pointer_cast<CFrameBufferOGLBase>(m_DefaultFrameBuffer)->InitializeWithFBO(0);
    m_DefaultFrameBuffer->Bind();
    m_DefaultFrameBuffer->AttachColor(colorTarget, 0);
    m_DefaultFrameBuffer->AttachDepth(depthTarget);
    m_DefaultFrameBuffer->AttachStencil(stencilTarget);

    GLint swap = 1;
    [m_GLContext setValues:&swap forParameter:NSOpenGLCPSwapInterval];
}

void CRenderViewOSX::Begin() const
{
    [m_GLContext makeCurrentContext];
}

void CRenderViewOSX::End() const
{
    [m_GLContext flushBuffer];
}

void CRenderViewOSX::UpdateEvents() const
{
}

IRendererPtr CRenderViewOSX::Renderer() const
{
    return m_Renderer;
}

IFrameBufferPtr CRenderViewOSX::DefaultFrameBuffer() const
{
    return m_DefaultFrameBuffer;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* OS_MAC */
