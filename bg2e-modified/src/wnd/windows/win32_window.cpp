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


#include <bg/wnd/win32_window.hpp>
#include <bg/wnd/window_controller.hpp>
#include <bg/wnd/win_window_proc.hpp>

#include <bg/system/screen.hpp>

#ifndef WIN32
#define WIN32
#endif
#include <WinSock2.h>
#include <windows.h>

#include <bg/engine/ogl_includes.hpp>

#include <bg/engine.hpp>

#include <bg/engine/directx11.hpp>
#include <bg/engine/opengl_core.hpp>
#include <bg/engine/vulkan.hpp>

#include <bg/wnd/win32_gl_context.hpp>
#include <bg/wnd/directx_context.hpp>
#include <bg/wnd/win32_vk_context.hpp>


#include <ShellScalingApi.h>

static LPCSTR WindowClass = "OpenGL";

namespace bg {
namespace wnd {

bool Win32Window::create() {
	WNDCLASSA wc;
	DWORD dwExStyle;
	DWORD dwStyle;
	RECT rect;

	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	if (_fullscreen && (_rect.width() == 0 || _rect.height() == 0)) {
		initFullScreenFrame();
	}
	
	if (_rect.width() == 0 || _rect.height()==0) {
		_rect.width(300);
	}
	if (_rect.height() == 0) {
		_rect.height(250);
	}

	rect.left = _rect.x();
	rect.right = static_cast<long>(_rect.width() + _rect.x());
	rect.top = _rect.y();
	rect.bottom = static_cast<long>(_rect.height() + _rect.y());

	_hInstance = GetModuleHandle(0);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)vwgl_app_WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = bg::native_cast<HINSTANCE>(_hInstance);
	wc.hIcon = LoadIcon(0, IDI_WINLOGO);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = WindowClass;
	
	_controller->eventHandler()->buildMenu(_menu);

	if (!RegisterClassA(&wc)) {
		return false;
	}

	if (_fullscreen) {
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = _rect.width();
		dmScreenSettings.dmPelsHeight = _rect.height();
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			_fullscreen = false;
		}
	}
	
	if (_fullscreen) {
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		_rect.x(0);
		_rect.y(0);
		rect.left = 0;
		rect.right = static_cast<long>(_rect.width());
		rect.top = 0;
		rect.bottom = static_cast<long>(_rect.height());
	}
	else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&rect, dwStyle, _menu.size()>0, dwExStyle);
	int newPosX = _rect.x() + _rect.x() - rect.left;
	int newPosY = _rect.y() + _rect.y() - rect.top;

	LPCSTR title = _title.c_str();
	if (!(_hWnd = CreateWindowExA(dwExStyle, WindowClass, title, dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		_rect.x(), _rect.y(), rect.right - rect.left, rect.bottom - rect.top,
		0, 0, bg::native_cast<HINSTANCE>(_hInstance), 0)))
	{
		destroy();
		return false;
	}

	if (_controller->eventHandler()) {
		buildMenu();
		_controller->eventHandler()->willCreateContext();
	}

	if (//bg::Engine::Get()->identifier()==bg::Engine::Identifier<bg::engine::OpenGLCompatibility>() ||
		bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>()) {
		if (!(_hDC = GetDC(bg::native_cast<HWND>(_hWnd)))) {
			destroy();
			return false;
		}

		GLContext * glContext = new Win32GLContext(this);
		_context = glContext;
		if (!glContext->createContext()) {
			destroy();
			return false;
		}

		glContext->makeCurrent();
		
	}
	else if (bg::Engine::Get()->identifier() &&
		bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::Vulkan>())
	{
		Win32VKContext * vkContext = new Win32VKContext(this);
		_context = vkContext;
		if (!vkContext->createContext()) {
			destroy();
			return false;
		}
	}
	
	ShowWindow(bg::native_cast<HWND>(_hWnd), SW_SHOW);
	SetForegroundWindow(bg::native_cast<HWND>(_hWnd));
	SetFocus(bg::native_cast<HWND>(_hWnd));

	if (bg::Engine::Get()->identifier()==bg::Engine::Identifier<bg::engine::DirectX11>()) {
		DirectXContext * dxContext = new DirectXContext(this);
		_context = dxContext;
		if (!dxContext->createContext()) {
			destroy();
			return false;
		}
	}

	if (_controller->eventHandler()) {
		_controller->eventHandler()->setContext(_context.getPtr());
		_controller->setWindow(this);
	}

	_controller->initGL();
	_rect.x(newPosX);
	_rect.y(newPosY);

	_controller->reshape(_rect.width(), _rect.height());

	if (!_iconPath.empty()) {
		setIcon(_iconPath);
	}

	return true;
}

void Win32Window::destroy() {
	if (_controller->eventHandler()) {
		_controller->eventHandler()->willDestroyContext();
	}
	_controller->destroy();

	if (_fullscreen) {
		ChangeDisplaySettings(0, 0);
	}

	if (_context.valid()) {
		_context->destroy();
		_context = nullptr;
	}

	if (_hDC) {
		ReleaseDC(bg::native_cast<HWND>(_hWnd), bg::native_cast<HDC>(_hDC));
		_hDC = 0;
	}

	if (_hWnd) {
		DestroyWindow(bg::native_cast<HWND>(_hWnd));
		_hWnd = 0;
	}

	UnregisterClassA(WindowClass, bg::native_cast<HINSTANCE>(_hInstance));
}

void Win32Window::initFullScreenFrame() {
	_rect.width(GetSystemMetrics(SM_CXSCREEN));
	_rect.height(GetSystemMetrics(SM_CYSCREEN));
}

float Win32Window::scale() {
	HMONITOR monit = MonitorFromWindow(bg::native_cast<HWND>(_hWnd), MONITOR_DEFAULTTONEAREST);
	return bg::system::Screen::GetHMonitorScale(monit);
}


//MENU THINGS
void Win32Window::initSubMenus(const bg::wnd::PopUpMenu * menu, HMENU hmenu) const 
{
	menu->eachSubMenu([&](const bg::wnd::PopUpMenu * _subMenu, auto index)
	{
		HMENU hSubMenu = CreateMenu();
		_subMenu->setHMenu(bg::plain_ptr(hSubMenu));
		initSubMenus(_subMenu, hSubMenu);
		AppendMenuA(hmenu, MF_STRING | MF_POPUP,(UINT_PTR)hSubMenu, _subMenu->title().c_str());
	});
	menu->eachMenuItem([&](const bg::wnd::PopUpMenuItem & item, auto index) {
		std::string title = item.title.c_str();

		if (item.shortcut.valid()) {
			title += "";
			auto maxSeparation = 30;
			auto separation = maxSeparation - title.length();
			for (auto i = 0; i < separation; ++i) {
				title += " ";
			}
			title += " ";
			if (item.shortcut.modifierMask & bg::base::Keyboard::kCtrlKey) {
				title += "Ctrl+";
			}
			if (item.shortcut.modifierMask & bg::base::Keyboard::kShiftKey) {
				title += "Shift+";
			}
			if (item.shortcut.modifierMask & bg::base::Keyboard::kAltKey) {
				title += "Alt+";
			}
			title += static_cast<char>(item.shortcut.keyCode);
			_shortcutItems.push_back(item);
		}
		AppendMenuA(hmenu, MF_STRING , item.identifier, title.c_str());
	});
}
//MENU THINGS
void Win32Window::buildMenu() {
	using namespace bg::base;
	_hMenu = CreateMenu();
	_shortcutItems.clear();

	for (auto subMenu : _menu) {
		HMENU hSubMenu = CreatePopupMenu();
		subMenu->setHMenu(bg::plain_ptr(hSubMenu));
		this->initSubMenus(subMenu.getPtr(), hSubMenu);
		AppendMenuA(bg::native_cast<HMENU>(_hMenu), MF_STRING | MF_POPUP, reinterpret_cast<uint64_t>(hSubMenu), subMenu->title().c_str());
	}

	SetMenu(bg::native_cast<HWND>(_hWnd), bg::native_cast<HMENU>(_hMenu));
}

void Win32Window::setIcon(const std::string & iconPath) {
	if (!_hInstance) {
		_iconPath = iconPath;
	}
	else {
		HANDLE icon = LoadImage(bg::native_cast<HINSTANCE>(_hInstance), iconPath.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		SendMessage(bg::native_cast<HWND>(_hWnd), static_cast<UINT>(WM_SETICON), ICON_BIG, (LPARAM)icon);
	}
}

}
}
