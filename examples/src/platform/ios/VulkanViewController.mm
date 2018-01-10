
//
//  ViewController.m
//  OpenJam
//
//  Created by Yevgeniy Logachev on 1/9/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#if defined(OS_IPHONE)

#import "VulkanViewController.h"

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

@interface VulkanViewController () <MTKViewDelegate> {
    IRenderViewPtr m_RenderView;
    CGamePtr m_Game;
    IScenePtr m_MainScene;
    
    CADisplayLink* _displayLink;
}

@end

@implementation VulkanViewController

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
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    m_RenderView.reset(new CRenderViewIOS((__bridge void*)(self.view), CRenderViewIOS::Vulkan));
    m_Game.reset(new CGame(m_RenderView));
    m_Game->Initialize();

    m_MainScene.reset(new CGameScene(m_Game));
    m_Game->PushScene(m_MainScene);
    
    ((MTKView*)self.view).delegate = self;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

#pragma mark - GLKViewController delegate methods

// Called whenever view changes orientation or layout is changed
- (void)mtkView:(nonnull MTKView*)view drawableSizeWillChange:(CGSize)size
{
}

// Called whenever the view needs to render
- (void)drawInMTKView:(nonnull MTKView*)view
{
    static NSTimeInterval lastTime = 0;
    NSTimeInterval nowTime = [[NSDate date] timeIntervalSince1970];
    unsigned long dt = (nowTime - lastTime) * 1000.0f;
    lastTime = nowTime;
    
    m_Game->Update(dt);
    m_Game->Draw();
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* uitouch = [[[event allTouches] allObjects] objectAtIndex:0];
    CGPoint point = [uitouch locationInView:self.view];

    float scale = m_RenderView->ScaleFactor();
    CTouchEventPtr touchevent(new CTouchEvent(glm::vec2(point.x * scale, point.y * scale), CTouchEvent::Down));
    emit m_RenderView->OnTouchSignal(touchevent);
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* uitouch = [[[event allTouches] allObjects] objectAtIndex:0];
    CGPoint point = [uitouch locationInView:self.view];

    float scale = m_RenderView->ScaleFactor();
    CTouchEventPtr touchevent(new CTouchEvent(glm::vec2(point.x * scale, point.y * scale), CTouchEvent::Move));
    emit m_RenderView->OnTouchSignal(touchevent);
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* uitouch = [[[event allTouches] allObjects] objectAtIndex:0];
    CGPoint point = [uitouch locationInView:self.view];

    float scale = m_RenderView->ScaleFactor();
    CTouchEventPtr touchevent(new CTouchEvent(glm::vec2(point.x * scale, point.y * scale), CTouchEvent::Up));
    emit m_RenderView->OnTouchSignal(touchevent);
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* uitouch = [[[event allTouches] allObjects] objectAtIndex:0];
    CGPoint point = [uitouch locationInView:self.view];

    float scale = m_RenderView->ScaleFactor();
    CTouchEventPtr touchevent(new CTouchEvent(glm::vec2(point.x * scale, point.y * scale), CTouchEvent::Reset));
    emit m_RenderView->OnTouchSignal(touchevent);
}

@end

#endif /* OS_IPHONE */

