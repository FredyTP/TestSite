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


#include <bg/wnd/message_box.hpp>
#include <bg/wnd/window_controller.hpp>

#if BG2E_WINDOWS==1

#ifndef WIN32
#define WIN32
#endif
#include <WinSock2.h>
#include <Windows.h>
#include <bg/wnd/win32_window.hpp>

#undef MessageBox
#define MessageBoxNative MessageBoxA

namespace bg {
namespace wnd {

MessageBox::Response MessageBox::Show(Window * parentModal, const std::string & title, const std::string & message, StandardButton standardButtons) {
	HWND hwnd = nullptr;

	Win32Window * window = dynamic_cast<Win32Window*>(parentModal);
	if (window) {
		hwnd = static_cast<HWND>(window->win_Wnd());
	}

	UINT type;
	switch (standardButtons) {
	case kButtonOk:
		type = MB_OK;
		break;
	case kButtonOkCancel:
		type = MB_OKCANCEL;
		break;
	case kButtonYesNoCancel:
		type = MB_YESNOCANCEL;
		break;
	case kButtonYesNo:
		type = MB_YESNO;
		break;
	}

	switch (MessageBoxNative(hwnd,
		static_cast<LPCSTR>(message.c_str()),
		static_cast<LPCSTR>(title.c_str()),
		type)) {
	case IDNO:
		return kResponseNo;
	case IDOK:
		return kResponseOk;
	case IDYES:
		return kResponseYes;
	case IDCANCEL:
	default:
		return kResponseCancel;
	}
}

}
}


#endif