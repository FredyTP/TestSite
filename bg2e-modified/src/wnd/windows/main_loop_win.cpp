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


#include <bg/wnd/main_loop.hpp>
#include <bg/wnd/window_controller.hpp>

#include <bg/wnd/win_window_proc.hpp>
#include <bg/wnd/win32_window.hpp>

namespace bg {
namespace wnd {

bool mouseOutDetection(Win32Window * wnd) {
	if (wnd && !wnd->mouseLeaveSent()) {
		POINT point;
		GetCursorPos(&point);
		bg::math::Rect rect = wnd->rect();
		bool inWindow = point.x >= rect.x() && point.x <= rect.x() + rect.width() &&
						point.y >= rect.y() && point.y <= rect.y() + rect.height();
		return !inWindow;
	}
	else {
		return false;
	}
}

int MainLoop::run() {
	MSG msg;
	BOOL done = FALSE;

	if (!_window.valid()) return 0;

	WindowController * controller = _window->windowController();
	Win32Window * win32Window = dynamic_cast<Win32Window*>(_window.getPtr());

	controller->setLastClock(std::clock());

	float delta = 0.0f;
	while (!done) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				if ((_onQuit && _onQuit(0)) || !_onQuit) {
					done = TRUE;
				}
			}
			if ((msg.message == WM_MOUSEMOVE ||
				msg.message == WM_LBUTTONDOWN ||
				msg.message == WM_RBUTTONDOWN ||
				msg.message == WM_MBUTTONDOWN ||
				msg.message == WM_MOUSEHOVER ||
				msg.message == WM_MOUSEACTIVATE ||
				msg.message == WM_MOUSEWHEEL) && win32Window) {
				win32Window->setMouseLeaveSent(false);
			}

			if (mouseOutDetection(win32Window)) {
				win32Window->setMouseLeaveSent(true);
				MSG newMsg;
				msg.message = WM_MOUSELEAVE;
				msg.hwnd = bg::native_cast<HWND>(win32Window->win_Wnd());
				msg.lParam = 0xFFFFFFFF;
				DispatchMessage(&newMsg);
			}
			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			for (auto cl : _customLoopClosure) {
				cl();
			}
			std::clock_t lastClock = controller->getLastClock();
			std::clock_t clock = std::clock();
			delta = static_cast<float>(clock - lastClock) / CLOCKS_PER_SEC;
			controller->setLastClock(std::clock());
			controller->frame(delta);
			controller->draw();
		}
	}

	MainLoop::Destroy();
	return 0;
}

void MainLoop::quit(int code) {
	PostQuitMessage(code);
}

}
}
