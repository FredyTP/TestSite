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

#ifndef _bg2e_wnd_window_controller_hpp_
#define _bg2e_wnd_window_controller_hpp_

#include <bg/export.hpp>
#include <bg/base/keyboard_event.hpp>
#include <bg/base/mouse_event.hpp>
#include <bg/base/event_handler.hpp>

#include <ctime>
#include <thread>
#include <mutex>
#include <atomic>

namespace bg {
namespace wnd {

class Window;
class BG2E_EXPORT WindowController : public bg::base::ReferencedPointer {
public:
	WindowController(bg::base::EventHandler * eventHandler);

	void windowRectChanged(int, int, int, int);
	void initGL();
	void reshape(int, int);
	void draw();
	void frame(float);
	void destroy();
	void keyUp(const bg::base::KeyboardEvent &);
	void keyDown(const bg::base::KeyboardEvent &);
	void charPress(const bg::base::KeyboardEvent &);
	void mouseDown(const bg::base::MouseEvent &);
	void mouseDrag(const bg::base::MouseEvent &);
	void mouseMove(const bg::base::MouseEvent &);
	void mouseUp(const bg::base::MouseEvent &);
	void mouseWheel(const bg::base::MouseEvent &);

	inline std::clock_t getLastClock() { return _lastClock; }
	inline void setLastClock(std::clock_t clock) { _lastClock = clock; }

	inline void setWindow(Window * win) {
		_window = win;
	}
	inline Window * window() { return _window; }
	
	inline bg::base::EventHandler * eventHandler() { return _eventHandler.getPtr(); }

    void willDestroy() {
        _running = false;
    }
    
protected:
	virtual ~WindowController();
	std::clock_t _lastClock;

	Window * _window;
	bg::ptr<bg::base::EventHandler> _eventHandler;
 
    std::mutex _presentMutex;
    std::atomic_bool _running;
};

}
}

#endif
