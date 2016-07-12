//
//  AppDelegateOSX.m
//  test_cocoa
//
//  Created by Yevgeniy Logachev on 4/23/15.
//  Copyright (c) 2015 yev. All rights reserved.
//

#if defined(OS_MAC)

#import "AppDelegateOSX.h"

#include "CRenderViewOSX.h"
#include "CGame.h"
#include "CSystem.h"
#include "CGameScene.h"
#include "CTouchEvent.h"
#include "IEventDispatcher.hpp"

using namespace jam;

CLASS_PTR(CTouchEvent)
CLASS_PTR(CGame)
CLASS_PTR(IScene)

@interface AppDelegateOSX ()
{
    IRenderViewPtr m_RenderView;
    CGamePtr m_Game;
    IScenePtr m_MainScene;
    CGRect m_ScreenSize;
}

@property (assign) NSWindow *window;

@end

@implementation AppDelegateOSX

- (id)init
{
    self = [super init];
    if (self)
    {
        m_RenderView = nullptr;
        m_Game = nullptr;
        m_MainScene = nullptr;
    }
    return self;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Create window
    NSRect windowFrame = NSMakeRect(0, 0, 800, 600);
    NSUInteger style = (NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask);
    self.window  = [[NSWindow alloc] initWithContentRect:windowFrame
                                               styleMask:style
                                                 backing:NSBackingStoreBuffered
                                                   defer:NO];
    [self.window setBackgroundColor:[NSColor whiteColor]];
    [self.window makeKeyAndOrderFront:NSApp];
    
    // Move window to center
    NSRect screenFrame = [[self.window screen] frame];
    NSRect centerFrame = NSMakeRect((screenFrame.size.width - windowFrame.size.width) * 0.5f,
                                    (screenFrame.size.height - windowFrame.size.height) * 0.5f,
                                    windowFrame.size.width,
                                    windowFrame.size.height);
    [self.window setFrame:centerFrame display:YES];
    
    // OpenGL view
    NSOpenGLView *openGLView = [[NSOpenGLView alloc] initWithFrame:CGRectMake(0.0,
                                                                              0.0,
                                                                              windowFrame.size.width,
                                                                              windowFrame.size.height)];
    [self.window setContentView:openGLView];
    
    m_ScreenSize = windowFrame;
    
    m_RenderView.reset(new CRenderViewOSX(m_ScreenSize.size.width,
                                          m_ScreenSize.size.height,
                                          (__bridge void*)(openGLView),
                                          CRenderViewOSX::OGLLegacy));
    m_Game.reset(new CGame(m_RenderView));
    m_Game->Initialize();
    
    m_MainScene.reset(new CGameScene(m_Game));
    m_Game->PushScene(m_MainScene);
    
    [NSTimer scheduledTimerWithTimeInterval:.001
                                     target:self
                                   selector:@selector(update)
                                   userInfo:nil
                                    repeats:YES];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{

}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)_app
{
    return YES;
}

- (void)update
{
    static unsigned long prevTime = CSystem::GetTickCount();
    unsigned long currTime = CSystem::GetTickCount();
    unsigned long dt = currTime - prevTime;
    prevTime = currTime;
    
    m_Game->Update(dt);
    m_Game->Draw();
}

@end

#endif /* OS_MAC */
