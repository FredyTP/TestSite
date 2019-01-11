
#ifndef _bg2e_wnd_cocoa_mtl_view_hpp_
#define _bg2e_wnd_cocoa_mtl_view_hpp_

#include <bg/platform.hpp>
#include <bg/wnd/cocoa_window.hpp>

#if BG2E_MAC==1

#import <AppKit/AppKit.h>

@interface MetalView : NSView
@property (readwrite) bg::wnd::CocoaWindow * cocoaWindow;

- (id)initWithFrame:(NSRect)frameRect window:(bg::wnd::CocoaWindow*)window;
- (void)initGL;
- (void)notifyQuit;

@end

#endif

#endif
