/* 
 * File:   AppWrapper.h
 * Author: yevgeniy
 */

#if defined(OS_ANDROID)

#include "Global.h"
#include "CGame.h"
#include "CSystem.h"
#include "CRenderViewAndroid.h"
#include "CRenderOGLES2.h"
#include "CGameScene.h"

bool g_isInitialized = false;
IRenderView* g_renderView = 0;
IRender* g_render = 0;
CGame* g_game = 0;
unsigned long g_lastTimestamp = CSystem::GetTickCount();

extern "C"
{
    JNIEXPORT void JNICALL Java_com_protectjaminc_protectjam_MainActivity_OnCreateNative( JNIEnv* env, jobject obj )
    {

    }

    JNIEXPORT void JNICALL Java_com_protectjaminc_protectjam_MainActivity_SetSurface( JNIEnv* env, jclass clazz, jobject javaSurface )
    {
    }

    JNIEXPORT void JNICALL Java_com_protectjaminc_protectjam_MainActivity_SetSurfaceSize( JNIEnv* env, jclass clazz, int Width, int Height )
    {
        if (!g_isInitialized)
        {
            g_renderView = new CRenderViewAndroid(Width, Height);
            g_render = new CRenderOGLES2(g_renderView);
            g_game = new CGame(g_render);
            g_game->Start();

            IScene* scene = new CGameScene();
            game->PushScene(scene);

            g_isInitialized = true;
        }
    }

    JNIEXPORT void JNICALL Java_com_protectjaminc_protectjam_MainActivity_DrawFrame( JNIEnv* env, jobject obj )
    {
        unsigned long currentTimestamp = CSystem::GetTickCount();
        unsigned long dt = currentTimestamp - g_lastTimestamp;
        g_lastTimestamp  = currentTimestamp;

        g_game->Update(dt);       
    }
    
    JNIEXPORT void JNICALL Java_com_protectjaminc_protectjam_MainActivity_SetupArchiveDir(JNIEnv * env, jobject obj, jstring dir)
    {
        const char* temp = env->GetStringUTFChars(dir, NULL);
        std::string stringDir(temp);

        CSystem::SetBundlePath(stringDir + "/");
    }

} // extern "C"

#endif // defined(OS_ANDROID)


