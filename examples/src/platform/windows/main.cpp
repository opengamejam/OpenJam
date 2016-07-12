/* 
 * File:   main.cpp
 * Author: yevgeniy.logachev
 *
 * Created on September 10, 2012, 3:43 PM
 */

#if defined(OS_WINDOWS)

#include <windows.h>
#include <Windowsx.h>

#include "CGame.h"
#include "CSystem.h"
#include "CRenderViewWindows.h"
#include "CGameScene.h"

using namespace jam;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR *lpCmdLine, int nCmdShow)
{
    IRenderViewPtr renderView(new CRenderViewWindows(960, 640, hInstance));
	CGamePtr game(new CGame(renderView));
	game->Initialize();

    IScenePtr scene(new CGameScene(game));
    game->PushScene(scene);
    
    unsigned long lastTimestamp = CSystem::GetTickCount();
    while(1)
    {
        unsigned long currentTimestamp = CSystem::GetTickCount();
        unsigned long dt = currentTimestamp - lastTimestamp;
        lastTimestamp = currentTimestamp;

		renderView->UpdateEvents();

        game->Update(dt);
		game->Draw();
    }

    return 0;
}

#endif // defined(OS_WINDOWS)