
#include <bg/wnd/cocoa_app_delegate.h>
#include <bg/wnd/main_loop.hpp>
#include <bg/wnd/window.hpp>
#include <bg/wnd/cocoa_window.hpp>
#include <bg/wnd/window_controller.hpp>

@interface CocoaAppDelegate ()

@end

@implementation CocoaAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
	bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
	bg::wnd::Window * window = mainLoop->window();
	bg::wnd::CocoaWindow * cocoaWindow = dynamic_cast<bg::wnd::CocoaWindow*>(window);
	if (cocoaWindow && cocoaWindow->createCocoaWindow() &&
		cocoaWindow->windowController() && cocoaWindow->windowController()->eventHandler()) {
		cocoaWindow->windowController()->eventHandler()->setContext(window->context());
	}
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
	if (bg::wnd::MainLoop::Get()->notifyQuit()) {
		return NSTerminateNow;
	}
	return NSTerminateCancel;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
	bg::wnd::MainLoop::Destroy();
}

@end
