//
//  VulkanViewController.m
//  macOSTestApp
//
//  Created by Yevgeniy Logachev on 1/11/18.
//  Copyright © 2018 Yevgeniy Logachev. All rights reserved.
//

#import "VulkanViewController.h"
#import <Metal/Metal.h>
#import <MetalKit/MTKView.h>
#include "CRenderViewOSX.h"
#include "CGame.h"
#include "CSystem.h"
#include "CGameScene.h"
#include "CTouchEvent.h"

using namespace jam;

CLASS_PTR(CTouchEvent)
CLASS_PTR(CGame)
CLASS_PTR(IScene)

@implementation VulkanViewController
{
    MTKView *_view;
    
    IRenderViewPtr m_RenderView;
    CGamePtr m_Game;
    IScenePtr m_MainScene;
    CGRect m_ScreenSize;
}

- (id)init
{
    self = [super init];
    if (self) {
        m_RenderView = nullptr;
        m_Game = nullptr;
        m_MainScene = nullptr;
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    _view = (MTKView *)self.view;
    
    float displayScale = 1;
    if ([[NSScreen mainScreen] respondsToSelector:@selector(backingScaleFactor)]) {
        NSArray *screens = [NSScreen screens];
        for (int i = 0; i < [screens count]; i++) {
            float s = [[screens objectAtIndex:i] backingScaleFactor];
            if (s > displayScale)
                displayScale = s;
        }
    }
    
    m_RenderView.reset(new CRenderViewOSX(self.view,
                                          displayScale,
                                          CRenderViewOSX::Vulkan));
    m_Game.reset(new CGame(m_RenderView));
    m_Game->Initialize();
    
    m_MainScene.reset(new CGameScene(m_Game));
    m_Game->PushScene(m_MainScene);
    
    [NSTimer scheduledTimerWithTimeInterval:1.0 / 60.0
                                     target:self
                                   selector:@selector(update)
                                   userInfo:nil
                                    repeats:YES];
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
