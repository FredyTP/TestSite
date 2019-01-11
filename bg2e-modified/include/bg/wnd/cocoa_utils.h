
#ifndef _bg2e_wnd_cocoa_utils_hpp_
#define _bg2e_wnd_cocoa_utils_hpp_

#include <bg/platform.hpp>
#include <bg/wnd/cocoa_window.hpp>
#include <bg/wnd/main_loop.hpp>

#if BG2E_MAC==1

#include <Cocoa/Cocoa.h>

extern bg::base::Keyboard::KeyCode translateCode(char character, unsigned short code);
extern unsigned int getModifiers(NSUInteger flags);
extern void fillKeyboard(bg::base::Keyboard & kb, NSString * characters, unsigned short code, NSUInteger flags);
extern void fillMouseEvent(bg::base::MouseEvent & mouseEvent, bg::wnd::MainLoop * mainLoop, NSPoint point);

#endif

#endif
