//
//  main.m
//  test_cocoa
//
//  Created by Yevgeniy Logachev on 4/23/15.
//  Copyright (c) 2015 yev. All rights reserved.
//

#if defined(OS_MAC)

#import <Cocoa/Cocoa.h>
#import "AppDelegateOSX.h"

int main(int argc, const char * argv[])
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    [NSApplication sharedApplication];
    
    AppDelegateOSX *appDelegate = [[AppDelegateOSX alloc] init];
    [NSApp setDelegate:appDelegate];
    [NSApp run];
    
    [appDelegate release];
    [pool release];
    
    return 0;
}

#endif /* OS_MAC */