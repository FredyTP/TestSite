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

#ifndef _bg2e_scene_input_visitor_hpp_
#define _bg2e_scene_input_visitor_hpp_

#include <bg/export.hpp>
#include <bg/scene/node_visitor.hpp>
#include <bg/scene/node.hpp>

namespace bg {
namespace scene {

class BG2E_EXPORT InputVisitor : public NodeVisitor {
public:
	enum Operation {
		kOperationNone = 0,
		kOperationKeyUp = 1,
		kOperationKeyDown,
		kOperationCharPress,
		kOperationMouseDown,
		kOperationMouseDrag,
		kOperationMouseMove,
		kOperationMouseUp,
		kOperationMouseWheel,
		kOperationTouchStart,
		kOperationTouchMove,
		kOperationTouchEnd,
		kOperationSensorEvent,
		kOperationCustomEvent
	};

	InputVisitor();

	virtual void visit(Node *);

	void keyUp(Node *, const bg::base::KeyboardEvent & e);
	void keyDown(Node *, const bg::base::KeyboardEvent & e);
	void charPress(Node *, const bg::base::KeyboardEvent & e);
	void mouseDown(Node *, const bg::base::MouseEvent & e);
	void mouseDrag(Node *, const bg::base::MouseEvent & e);
	void mouseMove(Node *, const bg::base::MouseEvent & e);
	void mouseUp(Node *, const bg::base::MouseEvent & e);
	void mouseWheel(Node *, const bg::base::MouseEvent & e);
	void touchStart(Node *, const bg::base::TouchEvent & e);
	void touchMove(Node *, const bg::base::TouchEvent & e);
	void touchEnd(Node *, const bg::base::TouchEvent & e);
	void sensorEvent(Node *, const bg::base::SensorEvent & e);
	void customEvent(Node *, const bg::base::CustomEvent & e);

protected:
	virtual ~InputVisitor();

	Operation _operation;
	bg::base::KeyboardEvent _kbEvent;
	bg::base::MouseEvent _mouseEvent;
	bg::base::TouchEvent _touchEvent;
	bg::base::SensorEvent _sensorEvent;
	bg::base::CustomEvent _customEvent;
};

}
}

#endif
