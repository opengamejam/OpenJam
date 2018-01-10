//
//  ViewController.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 1/9/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#if defined(OS_IPHONE)

#import <UIKit/UIKit.h>
#import <MetalKit/MetalKit.h>

@interface VulkanViewController : UIViewController<MTKViewDelegate>

@end

#endif /* OS_IPHONE */
