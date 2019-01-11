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


#include <bg/platform.hpp>
#include <bg/system/system.hpp>

#include <bg/wnd/window.hpp>

#include <bg/wnd/window_controller.hpp>

#if BG2E_WINDOWS
#include <bg/wnd/win32_window.hpp>
#elif BG2E_MAC==1
#include <bg/wnd/cocoa_window.hpp>
#elif BG2E_LINUX==1
#include <bg/wnd/gdl_window.hpp>
#endif

namespace bg {
namespace wnd {

Window * Window::New() {
	Window * win = nullptr;
#if BG2E_WINDOWS==1
	win = new Win32Window();
#elif BG2E_MAC==1
	win = new CocoaWindow();
#elif BG2E_LINUX==1
	win = new GdlWindow();
#endif
	return win;
}

Window::Window() :_title("Window"), _rect(math::Rect(0, 0, 0, 0)), _fullscreen(false), _context(nullptr) {
	_controller = new WindowController(new bg::base::EventHandler());
}

Window::~Window() {
	std::cout << "Window destroyed" << std::endl;
}
	
void Window::setEventHandler(bg::base::EventHandler * handler) {
	bg::base::Context * ctx = nullptr;
	if (context()) {
		ctx = context();
	}
	setWindowController(new WindowController(handler));
	if (ctx) {
		handler->setContext(ctx);
	}
}

void Window::setWindowController(WindowController * controller) {
	_controller = controller;
	if (controller) {
		_controller->setWindow(this);
	}
}

}
}
