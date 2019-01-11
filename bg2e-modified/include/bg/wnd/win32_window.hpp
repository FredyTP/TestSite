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


#ifndef _bg2e_wnd_win32_window_hpp_
#define _bg2e_wnd_win32_window_hpp_

#include <bg/wnd/window.hpp>

namespace bg {
namespace wnd {

class BG2E_EXPORT Win32Window : public Window {
public:
	Win32Window() :Window(), _hDC(0), _hWnd(0), _hMenu(0) {}

	virtual bool create();
	virtual void destroy();

	inline void * win_DeviceContext() { return _hDC; }
	inline void * win_Wnd() { return _hWnd; }
	inline bool mouseLeaveSent() const { return _mouseLeaveSent; }
	inline void setMouseLeaveSent(bool mouseLeaveSent) { _mouseLeaveSent = mouseLeaveSent; }

	virtual float scale();

	inline const std::vector<bg::wnd::PopUpMenuItem> & shortcutItems() const { return _shortcutItems; }

	void setIcon(const std::string &);

	inline bg::plain_ptr hDc() { return _hDC; }
	inline bg::plain_ptr hInstance() { return _hInstance; }
	inline bg::plain_ptr hWnd() { return _hWnd; }

protected:
	virtual ~Win32Window() {}
	
	

	bg::plain_ptr _hDC = nullptr;
	bg::plain_ptr _hWnd= nullptr;
	bg::plain_ptr _hInstance = nullptr;
	bg::plain_ptr _hMenu = nullptr;
	bool _mouseLeaveSent;
	std::string _iconPath;

	void initSubMenus(const bg::wnd::PopUpMenu * menu,HMENU hmenu) const ;
	void buildMenu();

	void initFullScreenFrame();

	mutable std::vector<bg::wnd::PopUpMenuItem> _shortcutItems;
};

}
}

#endif
