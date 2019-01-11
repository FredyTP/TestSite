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

#ifndef _bg2e_base_event_handler_hpp_
#define _bg2e_base_event_handler_hpp_

#include <bg/base/context.hpp>
#include <bg/base/keyboard_event.hpp>
#include <bg/base/mouse_event.hpp>
#include <bg/base/touch_event.hpp>
#include <bg/base/sensor_event.hpp>
#include <bg/base/custom_event.hpp>

#include <bg/engine.hpp>

#include <bg/wnd/popup_menu.hpp>

namespace bg {
namespace base {

class BG2E_EXPORT EventHandler : public bg::base::ReferencedPointer {
public:
	EventHandler() :_context(nullptr) {}
	
	virtual void willCreateContext() {}
	virtual void initGL() {}
	virtual void willDestroyContext() {
		bg::Engine::Destroy();
	}
	
	virtual void windowRectChanged(int, int, int, int) {}
	virtual void reshape(int, int) {}
	virtual void draw() {}
	virtual void frame(float) {}
	virtual void destroy() {}
	virtual void keyUp(const bg::base::KeyboardEvent &) {}
	virtual void keyDown(const bg::base::KeyboardEvent &) {}
	virtual void charPress(const bg::base::KeyboardEvent &) {}
	virtual void mouseDown(const bg::base::MouseEvent &) {}
	virtual void mouseDrag(const bg::base::MouseEvent &) {}
	virtual void mouseMove(const bg::base::MouseEvent &) {}
	virtual void mouseUp(const bg::base::MouseEvent &) {}
	virtual void mouseWheel(const bg::base::MouseEvent &) {}

	// Window menu events (or macOS menu events)
	virtual void buildMenu(bg::wnd::MenuDescriptor & menu) {}
	virtual void menuSelected(const std::string & title, int32_t identifier) {}
	
	// Mobile devices events
	virtual void onMemoryWarning() {}
	virtual void touchStart(const bg::base::TouchEvent &) {}
	virtual void touchMove(const bg::base::TouchEvent &) {}
	virtual void touchEnd(const bg::base::TouchEvent &) {}
	virtual void sensorEvent(const bg::base::SensorEvent &) {}

	// Custom events
	virtual void customEvent(const bg::base::CustomEvent &) {}
	
	inline void setContext(bg::base::Context * ctx) { _context = ctx; }
	inline bg::base::Context * context() { return _context; }
    
	virtual float scale() const;
	
protected:
	bg::base::Context * _context;
};

}
}

#endif
