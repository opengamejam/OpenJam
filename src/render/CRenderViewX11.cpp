//
//  CRenderViewX11.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_LINUX)

#include "CRenderViewX11.h"
#include "CInputManager.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderViewX11::CRenderViewX11(unsigned int width, unsigned int height,
                               unsigned int targetWidth, unsigned int targetHeight)
    : IRenderView(width, height, 1.0f)
    , m_EglWindow(0)
    , m_EglDisplay(0)
    , m_EglSurface(0)
    , m_EglContext(0)
    , m_Window(0)
    , m_Display(0)
{}

CRenderViewX11::~CRenderViewX11()
{
    eglMakeCurrent(m_EglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglTerminate(m_EglDisplay);
}

void CRenderViewX11::CreateView()
{
    std::string windowName = "X11";

    long                x11Screen   = 0;
    XVisualInfo*        x11Visual   = 0;
    Colormap            x11Colormap = 0;

    Window		sRootWindow;
    XSetWindowAttributes sWA;
    unsigned int	ui32Mask;
    int			i32Depth;
    int 		i32Width;
    int                 i32Height;

    m_Display = XOpenDisplay( 0 );
    assert(m_Display);

    x11Screen = XDefaultScreen( m_Display );

    // Gets the window parameters
    sRootWindow = RootWindow(m_Display, x11Screen);
    i32Depth = DefaultDepth(m_Display, x11Screen);
    x11Visual = new XVisualInfo;
    XMatchVisualInfo( m_Display, x11Screen, i32Depth, TrueColor, x11Visual);
    assert(x11Visual);

    x11Colormap = XCreateColormap( m_Display, sRootWindow, x11Visual->visual, AllocNone );
    sWA.colormap = x11Colormap;

    // Add to these for handling other events
    sWA.event_mask = StructureNotifyMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask;
    ui32Mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;

    i32Width  = (RealWidth()  < XDisplayWidth(m_Display, x11Screen)) ? Width() : XDisplayWidth(m_Display, x11Screen);
    i32Height = (RealHeight() < XDisplayHeight(m_Display,x11Screen)) ? Height() : XDisplayHeight(m_Display,x11Screen);

    // Creates the X11 window
    m_Window = XCreateWindow(m_Display, RootWindow(m_Display, x11Screen), 0, 0, i32Width, i32Height,
                             0, CopyFromParent, InputOutput, CopyFromParent, ui32Mask, &sWA);
    XMapWindow(m_Display, m_Window);
    XFlush(m_Display);
    
    /*Atom wmDelete = XInternAtom(m_Display, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(m_Display, m_Window, &wmDelete, 1);*/

    m_EglWindow = (EGLNativeWindowType)m_Window;
    m_EglDisplay = eglGetDisplay((EGLNativeDisplayType)m_Display);

    // Create OGL context
    EGLBoolean result = false;
    EGLint iMajorVersion, iMinorVersion;
    result = eglInitialize(m_EglDisplay, &iMajorVersion, &iMinorVersion);
    assert(result);

    eglBindAPI(EGL_OPENGL_ES_API);

    const EGLint pi32ConfigAttribs[] =
    {
        EGL_LEVEL, 0,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NATIVE_RENDERABLE, EGL_FALSE,
        EGL_DEPTH_SIZE, EGL_DONT_CARE,
        EGL_NONE
    };

    EGLint iConfigs;
    EGLConfig eglConfig;
    result = (eglChooseConfig(m_EglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1));
    assert(result);

    m_EglSurface = eglCreateWindowSurface(m_EglDisplay, eglConfig, m_EglWindow, 0);

    if (m_EglSurface == EGL_NO_SURFACE)
    {
        eglGetError(); // Clear error
        m_EglSurface = eglCreateWindowSurface(m_EglDisplay, eglConfig, 0, 0);
    }

    EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    m_EglContext = eglCreateContext(m_EglDisplay, eglConfig, 0, ai32ContextAttribs);

    eglMakeCurrent(m_EglDisplay, m_EglSurface, m_EglSurface, m_EglContext);
}

void CRenderViewX11::SwapBuffer() const
{
    eglSwapBuffers(m_EglDisplay, m_EglSurface);
}

void CRenderViewX11::UpdateEvents() const
{
    int i32NumMessages = XPending( m_Display );
    for( int i = 0; i < i32NumMessages; i++ )
    {
        XEvent	event;
        XNextEvent( m_Display, &event );

        switch( event.type )
        {
        case ButtonPress:
        {
            int xPos = event.xmotion.x;
            int yPos = GetHeight() - event.xmotion.y;
            CInputManager::STouchesData touch = CInputManager::STouchesData(true, xPos, yPos);  // TODO
            CInputManager::GetInstance()->AddTouches(touch);
        }
        break;
        //case MotionNotify:
        //{
        //    int xPos = event.xmotion.x;
        //    int yPos = event.xmotion.y;
        //    bool isPressed = (event.xbutton.button == 1);
        //   CInputManager::STouchesData touch = CInputManager::STouchesData(isPressed, xPos, yPos);
		//	CInputManager::GetInstance()->AddTouches(touch);
        //}
        //break;

        default:
            //LogDebug(event.type);
            break;
        }
    }
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif
