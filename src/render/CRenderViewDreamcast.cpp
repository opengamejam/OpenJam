/*
* File:   CRenderViewDreamcast.cpp
* Author: yevgeniy.logachev
*/

#if defined(OS_KOS)

#include "CRenderViewDreamcast.h"
#include "CRenderAPIOGL1_0.h"
#include "IRenderable.h"
#include "IEventable.h"
#include "CTouchEvent.h"
#include "IEventDispatcher.hpp"
#include "RenderGlobal.h"
#include "IRenderTarget.h"

#include "CRendererOGL1_3.h"
#include "CFrameBufferTargetOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderViewDreamcast::CRenderViewDreamcast()
	: IRenderView(vid_mode->width, vid_mode->height)
    , m_DefaultRenderTarget(nullptr)
	, m_Joy(maple_enum_type(0, MAPLE_FUNC_CONTROLLER))
{
    IEventable::RegisterDispatcher(std::make_shared<IEventDispatcher>(IEventDispatcher()));
}

CRenderViewDreamcast::~CRenderViewDreamcast()
{
}

void CRenderViewDreamcast::CreateView()
{
	pvr_init_defaults();
	glKosInit();
	glEnable(GL_TEXTURE_2D);
	//glFrontFace(GL_CW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	GRenderer.reset(new CRendererOGL1_3(shared_from_this()));
	m_DefaultRenderTarget.reset(new CFrameBufferTargetOGL1_3(Width(), Height()));
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
	if (!m_Joy)
	{
		return;
	}

	cont_state_t *joy_state = (cont_state_t *)maple_dev_status(m_Joy);
	if (joy_state->joyx != 0 ||
		joy_state->joyy != 0)
	{
		CTouchEventPtr event = std::make_shared<CTouchEvent>(CTouchEvent(CVector2Df(joy_state->joyx * 0.3f, joy_state->joyy * 0.3f), CTouchEvent::Move));
		Dispatcher()->DispatchEvent(event);
	}
}

IRenderTargetPtr CRenderViewDreamcast::DefaultRenderTarget() const
{
    return m_DefaultRenderTarget;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif


