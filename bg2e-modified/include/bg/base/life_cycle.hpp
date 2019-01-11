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

#ifndef _bg2e_base_life_cycle_hpp_
#define _bg2e_base_life_cycle_hpp_

#include <bg/base/pipeline.hpp>
#include <bg/base/matrix_state.hpp>
#include <bg/base/keyboard_event.hpp>
#include <bg/base/mouse_event.hpp>
#include <bg/base/touch_event.hpp>
#include <bg/base/custom_event.hpp>

namespace bg {
namespace base {
	
class LifeCycle {
public:
	virtual void init() {}
	virtual void frame(float delta) {}
	virtual void willDisplay(Pipeline * pipeline, MatrixState & matrixState) {}
	virtual void display(Pipeline * pipeline, MatrixState & matrixState) {}
	virtual void didDisplay(Pipeline * pipeline, MatrixState & matrixState) {}
	virtual void reshape(Pipeline * pipeline, MatrixState & matrixState, int width, int height) {}
	virtual void keyDown(const bg::base::KeyboardEvent &) {}
	virtual void keyUp(const bg::base::KeyboardEvent &) {}
	virtual void charPress(const bg::base::KeyboardEvent &) {}
	virtual void mouseDown(const bg::base::MouseEvent &) {}
	virtual void mouseDrag(const bg::base::MouseEvent &) {}
	virtual void mouseMove(const bg::base::MouseEvent &) {}
	virtual void mouseUp(const bg::base::MouseEvent &) {}
	virtual void mouseWheel(const bg::base::MouseEvent &) {}
	virtual void touchStart(const bg::base::TouchEvent &) {}
	virtual void touchMove(const bg::base::TouchEvent &) {}
	virtual void touchEnd(const bg::base::TouchEvent &) {}
	virtual void customEvent(const bg::base::CustomEvent &) {}
};

}
}

#endif
