//
//  main.m
//  OpenJam
//
//  Created by Yevgeniy Logachev on 1/9/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#if defined(OS_IPHONE)

#import <UIKit/UIKit.h>

#import "AppDelegateIOS.h"

int main(int argc, char * argv[])
{
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegateIOS class]));
    }
}

#endif /* OS_IPHONE */
