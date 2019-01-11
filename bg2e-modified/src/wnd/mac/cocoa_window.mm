
#include <bg/wnd/cocoa_window.hpp>
#include <bg/wnd/cocoa_glview.h>
#include <bg/wnd/cocoa_mtlview.h>
#include <bg/wnd/cocoa_gl_context.hpp>
#include <bg/wnd/cocoa_mvk_context.hpp>
#include <bg/wnd/window_controller.hpp>

#include <Cocoa/Cocoa.h>
#include <OpenGL/OpenGL.h>

namespace bg {
namespace wnd {

bool CocoaWindow::create() {
	_createWindow = true;
	return true;
}

bool CocoaWindow::createCocoaWindow() {
	if (_createWindow) {
		NSRect screenFrame = [NSScreen mainScreen].frame;
		NSRect frame = NSMakeRect(_rect.x(), screenFrame.size.height - _rect.height() - _rect.y(), _rect.width(), _rect.height());
		NSUInteger styleMask =  NSTitledWindowMask |
		NSClosableWindowMask |
		NSMiniaturizableWindowMask |
		NSResizableWindowMask;
		NSRect rect = [NSWindow contentRectForFrameRect:frame styleMask:styleMask];
		NSWindow * window = [[NSWindow alloc] initWithContentRect:rect styleMask:styleMask backing:NSBackingStoreBuffered defer:false];
		
		if (_controller.getPtr() && _controller->eventHandler()) {
			_controller->eventHandler()->willCreateContext();
		}
        
        MetalView * mtlView = nullptr;
        CocoaView * glView = nullptr;
        if (bg::Engine::Get() && bg::Engine::IsOpenGLCore()) {
            glView = [[CocoaView alloc] initWithFrame:frame pixelFormat:nil];
            glView.cocoaWindow = this;
            [window setContentView:glView];
            NSOpenGLContext * nativeContext = glView.openGLContext;
            CocoaGLContext * context = new CocoaGLContext(this);
            context->setCocoaGLContext((__bridge void*)nativeContext);
            _context = context;
            [window setAcceptsMouseMovedEvents:YES];
            [window makeFirstResponder:glView];
            _controller->eventHandler()->setContext(_context.getPtr());
        }
        else {
            // Metal or Vulkan (MoltenVK)
            mtlView = [[MetalView alloc] initWithFrame:frame window:this];
            [window setContentView:mtlView];
            
            CocoaMVKContext * context = new CocoaMVKContext(this);
            _context = context;
            context->setCocoaView((__bridge void*)mtlView);
            context->createContext();
            _controller->eventHandler()->setContext(_context.getPtr());

            [window setAcceptsMouseMovedEvents:YES];
            [window makeFirstResponder:mtlView];
            
            [mtlView initGL];
        }

		
		_windowPtr = (__bridge_retained void*)window;
		[window makeKeyAndOrderFront:window];
		
		
		if (_fullscreen && glView) {
			[glView enterFullScreenMode:[NSScreen mainScreen] withOptions:nil];
		}
		return true;
	}
	return false;
}

void CocoaWindow::destroy() {
	if (_windowPtr) {
		if (_controller.valid() && _controller->eventHandler()) {
            _controller->willDestroy();
			_controller->eventHandler()->willDestroyContext();
		}
		NSWindow * window = (__bridge_transfer NSWindow*)_windowPtr;
        [window.contentView notifyQuit];
		[window orderOut:window];
		window = nil;
		_windowPtr = nullptr;
	}
}
    
float CocoaWindow::scale() {
    NSWindow * window = (__bridge NSWindow*)_windowPtr;
    return window.backingScaleFactor;
}

}
}
