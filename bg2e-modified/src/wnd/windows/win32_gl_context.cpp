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


#include <bg/wnd/win32_gl_context.hpp>

#include <bg/wnd/win_window_proc.hpp>
#include <bg/wnd/win32_window.hpp>
#include <bg/wnd/window_controller.hpp>

#include <bg/engine/ogl_includes.hpp>

#include <bg/log.hpp>

PFNWGLSWAPINTERVALEXTPROC       wglSwapInterval = nullptr;
PFNWGLGETSWAPINTERVALEXTPROC    wglGetSwapInterval = nullptr;

namespace bg {
namespace wnd {

bool Win32GLContext::createContext() {
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA | PFD_SWAP_EXCHANGE,
		32,		// Color buffer depth
		0, 0, 0, 0, 0, 0,	// Color bits
		0,		// Alpha buffer
		0,		// Shift bits
		0,		// Accumulation buffer
		0, 0, 0, 0,	// Accumulation bits
		32,		// Depth buffer
		0,		// Stencil buffer
		0,		// Auxiliar buffer
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};

	Win32Window * win = dynamic_cast<Win32Window*>(_window);
	HDC hdc = bg::native_cast<HDC>(win->win_DeviceContext());
	GLuint pf = ChoosePixelFormat(hdc, &pfd);
	if (!pf || !SetPixelFormat(hdc, pf, &pfd)) {
		return false;
	}

	HGLRC hRC;
	if (!(hRC = wglCreateContext(hdc))) {
		destroy();
		return false;
	}
	_nativeContext = hRC;

	makeCurrent();

	wglSwapInterval = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglGetSwapInterval = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");

	if (wglSwapInterval) {
		bg::log(bg::log::kDebug) << "Initializing vsync to " << (_vsync ? "enabled":"disabled") << bg::endl;
		wglSwapInterval(_vsync ? 1:0);
	}
	else {
		bg::log(bg::log::kWarning) << "This hardware does not support vsync" << bg::endl;
	}

	return true;
}

void Win32GLContext::makeCurrent() {
	Win32Window * win = dynamic_cast<Win32Window*>(_window);
	HDC hdc = bg::native_cast<HDC>(win->win_DeviceContext());
	wglMakeCurrent(hdc, bg::native_cast<HGLRC>(_nativeContext));
}

void Win32GLContext::swapBuffers() {
	Win32Window * win = dynamic_cast<Win32Window*>(_window);
	HDC hdc = bg::native_cast<HDC>(win->win_DeviceContext());
	SwapBuffers(hdc);
}

void Win32GLContext::destroy() {
	if (_nativeContext) {
		wglMakeCurrent(0, 0);
		wglDeleteContext(bg::native_cast<HGLRC>(_nativeContext));
		_nativeContext = nullptr;
	}
}

void Win32GLContext::setVsyncEnabled(bool e) {
	if (wglSwapInterval && e) {
		GLContext::setVsyncEnabled(e);
		wglSwapInterval(e ? 1:0);
		bg::log(bg::log::kDebug) << "vsync set to " << (_vsync ? "enabled" : "disabled") << bg::endl;
	}
	else {
		bg::log(bg::log::kWarning) << "This hardware does not support vsync" << bg::endl;
	}
}

}
}
