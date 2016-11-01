/*
 * main.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: yev
 */
#if defined(OS_KOS)

#include "CRenderViewDreamcast.h"
#include "CGameScene.h"
#include "CGame.h"
#include "CSystem.h"
#include "CTouchEvent.h"

using namespace jam;

CLASS_PTR(CGame)
CLASS_PTR(IScene)
CLASS_PTR(CTouchEvent)

IRenderViewPtr m_RenderView;
CGamePtr m_Game;
IScenePtr m_MainScene;

extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

int main()
{
    m_RenderView.reset(new CRenderViewDreamcast());
    m_Game.reset(new CGame(m_RenderView));
    m_Game->Initialize();

    m_MainScene.reset(new CGameScene(m_Game));
    m_Game->PushScene(m_MainScene);

    while (1) {
        static unsigned long prev_time = CSystem::GetTickCount();
        unsigned long curr_time = CSystem::GetTickCount();
        unsigned long dt = curr_time - prev_time;
        prev_time = curr_time;

        m_Game->Update(dt);
        m_Game->Draw();
    }

    return 0;
}

#endif // OS_KOS
