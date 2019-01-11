/*
 *	bg2 engine license
 *	Copyright (c) 2016 Fernando Serrano <ferserc1@gmail.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *	of the Software, and to permit persons to whom the Software is furnished to do
 *	so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 *	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */


#ifndef _PLATFORM_HPP_

#define ON 1
#define OFF 0

#ifdef _WIN32

#define PLATFORM_WINDOWS 1
#define PLATFORM_MAC 0
#define PLATFORM_LINUX 0
#define PLATFORM_ANDROID 0

#elif defined(__APPLE__)

#define PLATFORM_WINDOWS 0
#define PLATFORM_MAC 1
#define PLATFORM_LINUX 0
#define PLATFORM_ANDROID 0

#elif defined(__ANDROID__)

#define PLATFORM_WINDOWS 0
#define PLATFORM_MAC 0
#define PLATFORM_LINUX 0
#define PLATFORM_ANDROID 1

#else

#define PLATFORM_WINDOWS 0
#define PLATFORM_MAC 0
#define PLATFORM_LINUX 1
#define PLATFORM_ANDROID 0

#endif

#if PLATFORM_WINDOWS==1

#define BG2E_WINDOWS 1
#define BG2E_MAC 0
#define BG2E_IOS 0
#define BG2E_IOS_SIMULATOR 0
#define BG2E_LINUX 0
#define BG2E_MOBILE 0
#define BG2E_ANDROID 0

#define BG2E_DIRECTX_AVAILABLE 0
#define BG2E_OPENGL_AVAILABLE 1
#define BG2E_OPENGL_ES_AVAILABLE 0

#define BG2E_OPENSLES_AVAILABLE 0
#define BG2E_OPENAL_AVAILABLE 1

#elif PLATFORM_ANDROID==1

#define BG2E_WINDOWS 0
#define BG2E_MAC 0
#define BG2E_LINUX 0
#define BG2E_IOS 0
#define BG2E_IOS_SIMULATOR 0
#define BG2E_MOBILE 1
#define BG2E_ANDROID 1

#define BG2E_DIRECTX_AVAILABLE 0
#define BG2E_OPENGL_AVAILABLE 0
#define BG2E_OPENGL_ES_AVAILABLE 1

#define BG2E_OPENSLES_AVAILABLE 1
#define BG2E_OPENAL_AVAILABLE 1

#elif PLATFORM_MAC==1
#include "TargetConditionals.h"
#if TARGET_OS_SIMULATOR

#define BG2E_WINDOWS 0
#define BG2E_MAC 1
#define BG2E_LINUX 0
#define BG2E_IOS 1
#define BG2E_IOS_SIMULATOR 1
#define BG2E_MOBILE 1
#define BG2E_ANDROID 0

#define BG2E_DIRECTX_AVAILABLE 0
#define BG2E_OPENGL_AVAILABLE 0
#define BG2E_OPENGL_ES_AVAILABLE 1

#define BG2E_OPENSLES_AVAILABLE 0
#define BG2E_OPENAL_AVAILABLE 1

#elif TARGET_OS_IPHONE

#define BG2E_WINDOWS 0
#define BG2E_MAC 1
#define BG2E_IOS 1
#define BG2E_IOS_SIMULATOR 0
#define BG2E_LINUX 0
#define BG2E_MOBILE 1
#define BG2E_ANDROID 0

#define BG2E_DIRECTX_AVAILABLE 0
#define BG2E_OPENGL_AVAILABLE 0
#define BG2E_OPENGL_ES_AVAILABLE 1

#define BG2E_OPENSLES_AVAILABLE 0
#define BG2E_OPENAL_AVAILABLE 1

#elif TARGET_OS_MAC

#define BG2E_WINDOWS 0
#define BG2E_MAC 1
#define BG2E_IOS 0
#define BG2E_IOS_SIMULATOR 0
#define BG2E_LINUX 0
#define BG2E_MOBILE 0
#define BG2E_ANDROID 0

#define BG2E_DIRECTX_AVAILABLE 0
#define BG2E_OPENGL_AVAILABLE 1
#define BG2E_OPENGL_ES_AVAILABLE 0

#define BG2E_OPENSLES_AVAILABLE 0
#define BG2E_OPENAL_AVAILABLE 1

#else

#define BG2E_WINDOWS 0
#define BG2E_MAC 0
#define BG2E_IOS 0
#define BG2E_IOS_SIMULATOR 0
#define BG2E_LINUX 0
#define BG2E_MOBILE 0
#define BG2E_ANDROID 0

#define BG2E_DIRECTX_AVAILABLE 0
#define BG2E_OPENGL_AVAILABLE 0
#define BG2E_OPENGL_ES_AVAILABLE 0

#define BG2E_OPENSLES_AVAILABLE 0
#define BG2E_OPENAL_AVAILABLE 0

#endif

#elif PLATFORM_LINUX==1

#define BG2E_WINDOWS 0
#define BG2E_MAC 0
#define BG2E_IOS 0
#define BG2E_IOS_SIMULATOR 0
#define BG2E_LINUX 1
#define BG2E_MOBILE 0
#define BG2E_ANDROID 0

#define BG2E_DIRECTX_AVAILABLE 0
#define BG2E_OPENGL_AVAILABLE 1
#define BG2E_OPENGL_ES_AVAILABLE 0

#define BG2E_OPENSLES_AVAILABLE 0
#define BG2E_OPENAL_AVAILABLE 1

#endif

#if defined(_USRDLL)
#define BG2E_DYNAMIC_LIB 1
#endif

#ifdef __GNUC__
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif

#endif
