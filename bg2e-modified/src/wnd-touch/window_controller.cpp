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


#include <bg/wnd/window_controller.hpp>

namespace bg {
namespace wnd {


WindowController::WindowController(bg::base::EventHandler * eventHandler)
	:_window(nullptr)
	,_eventHandler(eventHandler)
{
	
}

WindowController::~WindowController() {
	
}

void WindowController::initGL() {
	_eventHandler->initGL();
}

void WindowController::reshape(int w, int h) {
	_eventHandler->reshape(w, h);
}

void WindowController::draw() {
	_eventHandler->draw();
}

void WindowController::frame(float delta) {
	_eventHandler->frame(delta);
}

void WindowController::destroy() {
	_eventHandler->destroy();
}

void WindowController::keyUp(const bg::base::KeyboardEvent & evt) {
	_eventHandler->keyUp(evt);
}

void WindowController::keyDown(const bg::base::KeyboardEvent & evt) {
	_eventHandler->keyDown(evt);
}

void WindowController::charPress(const bg::base::KeyboardEvent & evt) {
	_eventHandler->charPress(evt);
}

void WindowController::mouseDown(const bg::base::MouseEvent & evt) {
	_eventHandler->mouseDown(evt);
}

void WindowController::mouseDrag(const bg::base::MouseEvent & evt) {
	_eventHandler->mouseDrag(evt);
}

void WindowController::mouseMove(const bg::base::MouseEvent & evt) {
	_eventHandler->mouseMove(evt);
}

void WindowController::mouseUp(const bg::base::MouseEvent & evt) {
	_eventHandler->mouseUp(evt);
}

void WindowController::mouseWheel(const bg::base::MouseEvent & evt) {
	_eventHandler->mouseWheel(evt);
}


}
}

