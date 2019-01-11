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


#include <bg/scene/input_visitor.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace scene {

InputVisitor::InputVisitor()
	:_operation(kOperationNone)
{
}

InputVisitor::~InputVisitor() {
}

void InputVisitor::visit(Node * node) {
	switch (_operation) {
		case kOperationNone:
			break;
		case kOperationKeyUp:
			node->keyUp(_kbEvent);
			break;
		case kOperationKeyDown:
			node->keyDown(_kbEvent);
			break;
		case kOperationCharPress:
			node->charPress(_kbEvent);
			break;
		case kOperationMouseDown:
			node->mouseDown(_mouseEvent);
			break;
		case kOperationMouseDrag:
			node->mouseDrag(_mouseEvent);
			break;
		case kOperationMouseMove:
			node->mouseMove(_mouseEvent);
			break;
		case kOperationMouseUp:
			node->mouseUp(_mouseEvent);
			break;
		case kOperationMouseWheel:
			node->mouseWheel(_mouseEvent);
			break;
		case kOperationTouchStart:
			node->touchStart(_touchEvent);
			break;
		case kOperationTouchMove:
			node->touchMove(_touchEvent);
			break;
		case kOperationTouchEnd:
			node->touchEnd(_touchEvent);
			break;
		case kOperationSensorEvent:
			node->sensorEvent(_sensorEvent);
			break;
		case kOperationCustomEvent:
			node->customEvent(_customEvent);
			break;
	}
}

void InputVisitor::keyUp(Node * root, const bg::base::KeyboardEvent & e) {
	_operation = kOperationKeyUp;
	_kbEvent = e;
	root->accept(this);
}

void InputVisitor::keyDown(Node * root, const bg::base::KeyboardEvent & e) {
	_operation = kOperationKeyDown;
	_kbEvent = e;
	root->accept(this);
}

void InputVisitor::charPress(Node * root, const bg::base::KeyboardEvent & e) {
	_operation = kOperationCharPress;
	_kbEvent = e;
	root->accept(this);
}

void InputVisitor::mouseDown(Node * root, const bg::base::MouseEvent & e) {
	_operation = kOperationMouseDown;
	_mouseEvent = e;
	root->accept(this);
}

void InputVisitor::mouseDrag(Node * root, const bg::base::MouseEvent & e) {
	_operation = kOperationMouseDrag;
	_mouseEvent = e;
	root->accept(this);
}

void InputVisitor::mouseMove(Node * root, const bg::base::MouseEvent & e) {
	_operation = kOperationMouseMove;
	_mouseEvent = e;
	root->accept(this);
}

void InputVisitor::mouseUp(Node * root, const bg::base::MouseEvent & e) {
	_operation = kOperationMouseUp;
	_mouseEvent = e;
	root->accept(this);
}

void InputVisitor::mouseWheel(Node * root, const bg::base::MouseEvent & e) {
	_operation = kOperationMouseWheel;
	_mouseEvent = e;
	root->accept(this);
}

void InputVisitor::touchStart(Node * root, const bg::base::TouchEvent & e) {
	_operation = kOperationTouchStart;
	_touchEvent = e;
	root->accept(this);
}

void InputVisitor::touchMove(Node * root, const bg::base::TouchEvent & e) {
	_operation = kOperationTouchMove;
	_touchEvent = e;
	root->accept(this);
}

void InputVisitor::touchEnd(Node * root, const bg::base::TouchEvent & e) {
	_operation = kOperationTouchEnd;
	_touchEvent = e;
	root->accept(this);
}

void InputVisitor::sensorEvent(Node * root, const bg::base::SensorEvent & e) {
	_operation = kOperationSensorEvent;
	_sensorEvent = e;
	root->accept(this);
}

void InputVisitor::customEvent(Node * root, const bg::base::CustomEvent & e) {
	_operation = kOperationCustomEvent;
	_customEvent = e;
	root->accept(this);
}

}
}
