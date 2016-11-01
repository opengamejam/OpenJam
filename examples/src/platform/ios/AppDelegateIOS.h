//
//  AppDelegateIOS.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 1/9/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#if defined(OS_IPHONE)

#import <UIKit/UIKit.h>

@interface AppDelegateIOS : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow* window;

@end

#endif /* OS_IPHONE */

