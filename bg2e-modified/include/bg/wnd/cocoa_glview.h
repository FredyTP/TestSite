
#ifndef _bg2e_wnd_cocoa_gl_view_hpp_
#define _bg2e_wnd_cocoa_gl_view_hpp_

#include <bg/platform.hpp>
#include <bg/wnd/cocoa_window.hpp>

#if BG2E_MAC==1

#import <OpenGL/OpenGL.h>
#import <Cocoa/Cocoa.h>

@interface CocoaView : NSOpenGLView

@property (readwrite) bg::wnd::CocoaWindow * cocoaWindow;

- (void)notifyQuit;

@end

#endif

#endif
