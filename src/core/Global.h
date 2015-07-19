//
//  Global.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef GLOBAL_H
#define GLOBAL_H

#include <functional>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <future>
#include <thread>
#include <mutex>
#include <random>
#include <typeindex>
#include <math.h>
#include <assert.h>

// LINUX
#if defined(OS_LINUX)
#	include <EGL/egl.h>
#	include <GLES2/gl2.h>
#   include <sys/time.h>
#define LOGI(...)
// WINDOWS
#elif defined(OS_WINDOWS)
#	include <windows.h>
#	include <EGL/egl.h>
#	include <GLES2/gl2.h>
#define LOGI(...)
// MACOS
#elif defined(OS_MAC)
#	include <OpenGL/gl.h>
#	include <OpenGL/glext.h>
#   include <sys/time.h>
#if defined(__OBJC__)
#   import <Cocoa/Cocoa.h>
#endif
#define LOGI(...)
// IPHONE
#elif defined(OS_IPHONE)
#   include <OpenGLES/ES1/gl.h>
#   include <OpenGLES/ES1/glext.h>
#   include <OpenGLES/ES2/gl.h>
#   include <OpenGLES/ES2/glext.h>
//#   include <OpenGLES/ES3/gl.h>
//#   include <OpenGLES/ES3/glext.h>
#	include <sys/time.h>
#if defined(__OBJC__)
#   import <Foundation/Foundation.h>
#   import <UIKit/UIKit.h>
#endif
#define LOGI(...)
// ANDROID
#elif defined(OS_ANDROID)
#   include <stdlib.h>
#   include <jni.h>
#   include <android/log.h>
#	include <GLES2/gl2.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "INFO: ", __VA_ARGS__))
#endif

#define CLASS_PTR(_class) typedef std::shared_ptr<class _class> _class##Ptr
#define CLASS_TEMPL_PTR(_class, _out) typedef std::shared_ptr<class _class> _out##Ptr
#define CLASS_WEAK(_class) typedef std::weak_ptr<class _class> _class##Weak

#define INL inline __attribute__ ((always_inline))

#define MULTI_LINE_STRING(a) #a

#endif // GLOBAL_H