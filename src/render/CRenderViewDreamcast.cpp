/*
* File:   CRenderViewDreamcast.cpp
* Author: yevgeniy.logachev
*/

#if defined(OS_KOS)

#include "CRenderViewDreamcast.h"
#include "CTouchEvent.h"
#include "RenderGlobal.h"
#include "IFrameBuffer.h"

#include "CRendererOGL1_3.h"
#include "CFrameBufferOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderViewDreamcast::CRenderViewDreamcast()
    : IRenderView(640, 480, 1.0f)
    , m_Renderer(nullptr)
    , m_DefaultFrameBuffer(nullptr)
    //, m_Joy(maple_enum_type(0, MAPLE_FUNC_CONTROLLER))
{
}

CRenderViewDreamcast::~CRenderViewDreamcast()
{
}

void CRenderViewDreamcast::CreateView()
{
    glKosInit();

    m_Renderer.reset(new CRendererOGL1_3(shared_from_this()));
    m_DefaultFrameBuffer.reset(new CFrameBufferOGL1_3(RealWidth(), RealHeight()));
}

void CRenderViewDreamcast::Begin() const
{
}

void CRenderViewDreamcast::End() const
{
    glutSwapBuffers();
}

void CRenderViewDreamcast::UpdateEvents() const
{
    if (!m_Joy) {
        return;
    }

    cont_state_t* joy_state = (cont_state_t*)maple_dev_status(m_Joy);
    if (joy_state->joyx != 0 || joy_state->joyy != 0) {
        //CTouchEventPtr event = std::make_shared<CTouchEvent>(CTouchEvent(glm::vec2(joy_state->joyx, joy_state->joyy),
        //    CTouchEvent::Move));
        //emit OnTouchSignal(event);
    }
}

IRendererPtr CRenderViewDreamcast::Renderer() const
{
    return m_Renderer;
}

IFrameBufferPtr CRenderViewDreamcast::DefaultFrameBuffer() const
{
    return m_DefaultFrameBuffer;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif

