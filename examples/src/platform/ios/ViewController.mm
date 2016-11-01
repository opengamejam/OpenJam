
//
//  ViewController.m
//  OpenJam
//
//  Created by Yevgeniy Logachev on 1/9/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#if defined(OS_IPHONE)

#import "ViewController.h"

#include "CRenderViewIOS.h"
#include "CGame.h"
#include "CSystem.h"
#include "CGameScene.h"
#include "CTouchEvent.h"
#include "CEventSystem.h"

using namespace jam;

CLASS_PTR(IRender)
CLASS_PTR(CGame)
CLASS_PTR(IScene)

@interface ViewController () {
    IRenderViewPtr m_RenderView;
    CGamePtr m_Game;
    IScenePtr m_MainScene;
}

@end

@implementation ViewController

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

- (void)loadView
{
    [super loadView];

    self.view = [[GLKView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.preferredFramesPerSecond = 60;

    m_RenderView.reset(new CRenderViewIOS((__bridge void*)(self.view), CRenderViewIOS::OGLES2_0));
    m_Game.reset(new CGame(m_RenderView));
    m_Game->Initialize();

    m_MainScene.reset(new CGameScene(m_Game));
    m_Game->PushScene(m_MainScene);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

#pragma mark - GLKViewController delegate methods

- (void)update
{
    unsigned long dt = [self timeSinceLastUpdate] * 1000.0f;

    m_Game->Update(dt);
    m_Game->Draw();
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* uitouch = [[[event allTouches] allObjects] objectAtIndex:0];
    CGPoint point = [uitouch locationInView:self.view];

    CTouchEventPtr touchevent(new CTouchEvent(glm::vec2(point.x, point.y), CTouchEvent::Down));
    emit m_RenderView->OnTouchSignal(touchevent);
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* uitouch = [[[event allTouches] allObjects] objectAtIndex:0];
    CGPoint point = [uitouch locationInView:self.view];

    CTouchEventPtr touchevent(new CTouchEvent(glm::vec2(point.x, point.y), CTouchEvent::Move));
    emit m_RenderView->OnTouchSignal(touchevent);
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* uitouch = [[[event allTouches] allObjects] objectAtIndex:0];
    CGPoint point = [uitouch locationInView:self.view];

    CTouchEventPtr touchevent(new CTouchEvent(glm::vec2(point.x, point.y), CTouchEvent::Up));
    emit m_RenderView->OnTouchSignal(touchevent);
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* uitouch = [[[event allTouches] allObjects] objectAtIndex:0];
    CGPoint point = [uitouch locationInView:self.view];

    CTouchEventPtr touchevent(new CTouchEvent(glm::vec2(point.x, point.y), CTouchEvent::Reset));
    emit m_RenderView->OnTouchSignal(touchevent);
}

@end

#endif /* OS_IPHONE */

