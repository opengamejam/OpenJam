//
//  CTouchSystem.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 8/31/16.
//
//

#include "CTouchSystem.h"
#include "IRenderer.h"
#include "IRenderView.h"
#include "IFrameBuffer.h"

using namespace jam;

CTouchSystem::CTouchSystem(IRenderViewPtr renderView)
{
    IRendererPtr renderer = renderView->Renderer();
    //m_RenderTarget =
}

CTouchSystem::~CTouchSystem()
{
}

void CTouchSystem::Update(unsigned long dt)
{
}
