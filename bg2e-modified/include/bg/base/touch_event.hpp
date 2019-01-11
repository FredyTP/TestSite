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

#ifndef _bg2e_wnd_touch_event_hpp_
#define _bg2e_wnd_touch_event_hpp_

#include <bg/math/vector.hpp>

namespace bg {
namespace base {

class TouchEvent {
public:
	typedef std::vector<bg::math::Position2Di*> TouchSet;
	
	virtual ~TouchEvent() {
		for (auto t : _touches) {
			delete t;
		}
		_touches.clear();
	}
	
	void addTouch(const bg::math::Position2Di & t) { _touches.push_back(new bg::math::Position2Di(t.x(), t.y())); }
	void clearTouches() {
		for (auto t : _touches) {
			delete t;
		}
		_touches.clear();
	}
	
	TouchSet & touches() { return _touches; }
	const TouchSet & touches() const { return _touches; }
	
	void operator = (const TouchEvent & t) {
		clearTouches();
		for (auto touch : t._touches) {
			_touches.push_back(new bg::math::Position2Di(touch->x(), touch->y()));
		}
	}
	
protected:
	
	TouchSet _touches;
};

}
}

#endif
