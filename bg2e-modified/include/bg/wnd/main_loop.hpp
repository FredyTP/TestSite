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


#ifndef _bg2e_wnd_main_loop_hpp_
#define _bg2e_wnd_main_loop_hpp_

#include <bg/wnd/window.hpp>
#include <bg/base/mouse.hpp>

#include <functional>

namespace bg {
namespace wnd {

class BG2E_EXPORT MainLoop {
public:
	static MainLoop * s_singleton;
	static MainLoop * Get() {
		if (s_singleton == nullptr) {
			s_singleton = new MainLoop();
		}
		return s_singleton;
	}
	static void Destroy() {
		if (s_singleton) {
			delete s_singleton;
			s_singleton = nullptr;
		}
	}
	
	void finalize();

	inline void setWindow(Window * wnd) { _window = wnd; }
	inline Window * window() { return _window.getPtr(); }
    
    Window * windowContainingHandler(const bg::base::EventHandler *);

	int run();

	void quit(int code);

	inline const bg::base::Mouse & getMouse() const { return _mouseStatus; }
	inline bg::base::Mouse & mouse() { return _mouseStatus; }
	
	inline void onQuit(std::function<bool (int)> closure) { _onQuit = closure; }
	inline bool notifyQuit() {
		bool performQuit = true;
		if (_onQuit) {
			performQuit = _onQuit(0);
		}
		return performQuit;
	}

	inline void addCustomLoopClosure(std::function<void()> closure) {
		_customLoopClosure.push_back(closure);
	}

protected:
	MainLoop() :_window(nullptr) {}
	virtual ~MainLoop() {
		if (_window.valid()) {
			_window->destroy();
		}
		std::cout << "Main loop destroyed" << std::endl;
	}

	bg::ptr<Window> _window;
	bg::base::Mouse _mouseStatus;
	
	std::function<bool (int)> _onQuit;
	std::vector<std::function<void()>> _customLoopClosure;
};

}
}

#endif
