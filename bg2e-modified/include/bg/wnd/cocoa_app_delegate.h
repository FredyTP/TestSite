
#ifndef _bg2e_wnd_cocoa_apo_delegate_hpp_
#define _bg2e_wnd_cocoa_apo_delegate_hpp_

#include <bg/platform.hpp>

#if BG2E_MAC==1 && BG2E_IOS==0

#include <Cocoa/Cocoa.h>

@interface CocoaAppDelegate : NSObject<NSApplicationDelegate>

@end

#endif

#endif
