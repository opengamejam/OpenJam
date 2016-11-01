/* 
 * File:   main.c
 * Author: yevgeniy.logachev
 */

#if defined(OS_LINUX)

#include <stdio.h>
#include <stdlib.h>

#include "CGame.h"
#include "CSystem.h"
#include "CRenderViewX11.h"
#include "CGameScene.h"

int main(int argc, char** argv)
{
    IRenderView* renderView = new CRenderViewX11(960, 640);
    IRender* render = new CRenderOGLES2(renderView);
    CGame* game = new CGame(render);
    game->Start();

    IScene* scene = new CGameScene();
    game->PushScene(scene);

    unsigned long lastTimestamp = CSystem::GetTickCount();
    while (game->IsRunning()) {
        unsigned long currentTimestamp = CSystem::GetTickCount();
        unsigned long dt = currentTimestamp - lastTimestamp;
        lastTimestamp = currentTimestamp;

        game->Update(dt);
    }

    game->Stop();

    game->PopScene();
    delete scene;

    delete game;
    delete render;
    delete renderView;

    return (EXIT_SUCCESS);
}

#endif //defined(OS_LINUX)
