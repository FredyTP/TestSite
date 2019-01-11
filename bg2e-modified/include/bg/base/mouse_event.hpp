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

#ifndef _bg2e_wnd_mouse_event_hpp_
#define _bg2e_wnd_mouse_event_hpp_

#include <bg/base/mouse.hpp>
#include <bg/base/keyboard.hpp>
#include <bg/math/vector.hpp>

namespace bg {
namespace base {

class MouseEvent {
public:
	inline bg::base::Mouse & mouse() { return _mouse; }
	inline const bg::base::Mouse & mouse() const { return _mouse; }
    inline bg::base::Keyboard & keyboard() { return _keyboard; }
    inline const bg::base::Keyboard & keyboard() const { return _keyboard; }
    inline const bg::math::Vector2i & pos() const { return _pos; }
    inline void setPos(const bg::math::Vector2i & pos) { _pos = pos; }
    inline const bg::math::Vector2 & delta() const { return _delta; }
	inline void setDelta(const bg::math::Vector2 & delta) { _delta = delta; }

protected:
    bg::base::Mouse _mouse;
    bg::base::Keyboard _keyboard;
    bg::math::Vector2i _pos;
    bg::math::Vector2 _delta;
};

}
}

#endif
