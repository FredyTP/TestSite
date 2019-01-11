#include <bg/wnd/cocoa_gl_context.hpp>

#include <bg/wnd/window_controller.hpp>

#import <OpenGL/OpenGL.h>
#import <Cocoa/Cocoa.h>

namespace bg {
namespace wnd {

bool CocoaGLContext::createContext() {
	return true;
}

void CocoaGLContext::makeCurrent() {
	NSOpenGLContext * context =  (__bridge NSOpenGLContext*)_nativeContext;
	if (context) {
		[context makeCurrentContext];
	}
}

void CocoaGLContext::swapBuffers() {
	NSOpenGLContext * context =  (__bridge NSOpenGLContext*)_nativeContext;
	if (context) {
		[context flushBuffer];
	}
}

void CocoaGLContext::destroy() {
}

}
}
