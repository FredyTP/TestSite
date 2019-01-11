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


#ifndef _bg2e_wnd_window_hpp_
#define _bg2e_wnd_window_hpp_

#include <string>

#include <bg/base/referenced_pointer.hpp>
#include <bg/base/event_handler.hpp>

#include <bg/wnd/gl_context.hpp>

#include <bg/wnd/popup_menu.hpp>

#include <bg/math/vector.hpp>

namespace bg {
namespace wnd {

class WindowController;

class BG2E_EXPORT Window : public bg::base::ReferencedPointer {
public:
	static Window * New();

	Window();
	virtual ~Window();

	void setEventHandler(bg::base::EventHandler * handler);
	void setWindowController(WindowController * controller);

	inline void setTitle(const std::string & title) { _title = title; }
	inline void setRect(const bg::math::Rect & rect) { _rect = rect; }
	inline void setPosition(int x, int y) { _rect.x(x); _rect.y(y); }
	inline void setPosition(const bg::math::Position2Di & pos) { _rect.x(pos.x()); _rect.y(pos.y()); }
	inline void setSize(int w, int h) { _rect.width(w); _rect.height(h); }
	inline void setSize(const bg::math::Size2Di & size) { _rect.width(size.width()); _rect.height(size.height()); }
	inline void setFullScreen(bool fs) { _fullscreen = fs; }
	inline const std::string & title() const { return _title; }
	inline const bg::math::Rect & rect() const { return _rect; }
	inline bool isFullScreen() const { return _fullscreen; }


	inline WindowController * windowController() { return _controller.getPtr(); }

	virtual bool create() = 0;
	virtual void destroy() = 0;

	inline Context * context() { return _context.getPtr(); }
	inline GLContext * glContext() { return dynamic_cast<GLContext*>(_context.getPtr()); }
    
    virtual float scale() { return 1.0f; }

	
	inline const MenuDescriptor & menu() const { return _menu;  }

	virtual void setIcon(const std::string &) {}

protected:
	std::string _title;
	bg::math::Rect _rect;
	bool _fullscreen;

	bg::ptr<Context> _context;
	bg::ptr<WindowController> _controller;
	
	MenuDescriptor _menu;
	
};

}
}
#endif // _VWGL_APP_WINDOW_HPP_
