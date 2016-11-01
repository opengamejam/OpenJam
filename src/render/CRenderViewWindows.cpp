//
//  CRenderViewWindows.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_WINDOWS)

#include "CRenderViewWindows.h"
#include "RenderGlobal.h"
#include "CRendererOGLES2_0.h"
#include "CFrameBufferOGLES2_0.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CRenderViewWindows::CRenderViewWindows(unsigned int width, unsigned int height, HINSTANCE hInstance)
    : IRenderView(width, height, 1.0f)
    , m_eglDisplay(0)
    , m_DeviceContext(0)
    , m_Window(0)
    , m_Instance(hInstance)
    , m_Renderer(nullptr)
    , m_DefaultRenderTarget(nullptr)
{
}

CRenderViewWindows::~CRenderViewWindows()
{
    eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglTerminate(m_eglDisplay);

    if (m_DeviceContext) {
        ReleaseDC(m_Window, m_DeviceContext);
        m_DeviceContext = 0;
    }
    if (m_Window) {
        DestroyWindow(m_Window);
        m_Window = 0;
    }
}

void CRenderViewWindows::CreateView()
{
    std::string windowName = "WND";

    WNDCLASS sWC;
    sWC.style = CS_HREDRAW | CS_VREDRAW;
    sWC.lpfnWndProc = WndProc2;
    sWC.cbClsExtra = 0;
    sWC.cbWndExtra = 0;
    sWC.hInstance = m_Instance;
    sWC.hIcon = 0;
    sWC.hCursor = 0;
    sWC.lpszMenuName = 0;
    sWC.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    sWC.lpszClassName = windowName.c_str();

    ATOM registerClass = RegisterClass(&sWC);
    assert(registerClass);

    int screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

    RECT sRect;
    SetRect(&sRect, 0, 0, RealWidth(), RealHeight());
    AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, false, 0);
    m_Window = CreateWindow(windowName.c_str(),
        windowName.c_str(),
        WS_VISIBLE | WS_SYSMENU,
        (screenWidth - RealWidth()) / 2,
        (screenHeight - RealHeight()) / 2,
        RealWidth(), RealHeight(),
        NULL, NULL, m_Instance, NULL);

    // Get the associated device context
    m_DeviceContext = GetDC(m_Window);
    m_eglWindow = m_Window;

    // Create OGL context
    m_eglDisplay = eglGetDisplay(m_DeviceContext);
    if (m_eglDisplay == EGL_NO_DISPLAY) {
        m_eglDisplay = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);
    }

    EGLBoolean result = false;
    result = eglInitialize(m_eglDisplay, 0, 0);
    assert(result);

    eglBindAPI(EGL_OPENGL_ES_API);

    const EGLint pi32ConfigAttribs[] = {
        EGL_LEVEL, 0,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NATIVE_RENDERABLE, EGL_FALSE,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_LUMINANCE_SIZE, 0,
        EGL_NONE
    };

    int iConfigs;
    EGLConfig eglConfig;
    result = (eglChooseConfig(m_eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1));
    assert(result);

    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, eglConfig, m_eglWindow, 0);

    if (m_eglSurface == EGL_NO_SURFACE) {
        eglGetError(); // Clear error
        m_eglSurface = eglCreateWindowSurface(m_eglDisplay, eglConfig, 0, 0);
    }

    EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    m_eglContext = eglCreateContext(m_eglDisplay, eglConfig, EGL_NO_CONTEXT, ai32ContextAttribs);
    assert(m_eglContext != EGL_NO_SURFACE);

    eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

    m_Renderer.reset(new CRendererOGLES2_0(shared_from_this()));

    std::shared_ptr<CFrameBufferOGLES2_0> renderTarget(new CFrameBufferOGLES2_0(RealWidth(), RealHeight()));
    renderTarget->Initialize(0);
    m_DefaultRenderTarget = renderTarget;
}

void CRenderViewWindows::Begin() const
{
}

void CRenderViewWindows::End() const
{
    eglSwapBuffers(m_eglDisplay, m_eglSurface);
}

void CRenderViewWindows::UpdateEvents() const
{
    MSG msg = { 0 };
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

IRendererPtr CRenderViewWindows::Renderer() const
{
    return f;
}

IFrameBufferPtr CRenderViewWindows::DefaultRenderTarget() const
{
    return m_DefaultRenderTarget;
}

LRESULT CALLBACK CRenderViewWindows::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return 0;
}

LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_SYSCOMMAND:
    case WM_KEYDOWN:
        break;

    case WM_KEYUP:
        break;

    /*case WM_LBUTTONDOWN:
        {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = m_WindowHeight - GET_Y_LPARAM(lParam);
        CInputManager::STouchesData touch = CInputManager::STouchesData(true, xPos, yPos);
        CInputManager::GetInstance()->AddTouches(touch);
        }
        break;

        case WM_LBUTTONUP:
        {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = m_WindowHeight - GET_Y_LPARAM(lParam);
        CInputManager::STouchesData touch = CInputManager::STouchesData(false, xPos, yPos);
        CInputManager::GetInstance()->AddTouches(touch);
        }
        break;*/
    case WM_MOUSEMOVE: {
        /* int xPos = GET_X_LPARAM(lParam);
        int yPos = 640 - GET_Y_LPARAM(lParam); // TODO:
        bool isPressed = (wParam && MK_MBUTTON);
        CInputManager::STouchesData touch = CInputManager::STouchesData(isPressed, xPos, yPos);
        CInputManager::GetInstance()->AddTouches(touch);*/
    } break;

    case WM_CLOSE:
        PostQuitMessage(0);
        return 1;

    default:
        break;
    }

    // Calls the default window procedure for messages we did not handle
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif /* OS_WINDOWS */
