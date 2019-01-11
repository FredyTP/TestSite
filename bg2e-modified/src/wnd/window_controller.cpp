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
    ,_running(true)
{
	
}

WindowController::~WindowController() {
	
}

void WindowController::windowRectChanged(int x, int y, int w, int h) {
	_eventHandler->windowRectChanged(x, y, w, h);
}

void WindowController::initGL() {
    std::lock_guard<std::mutex> lock(_presentMutex);
	_eventHandler->initGL();
}

void WindowController::reshape(int w, int h) {
    if (_running) {
        std::lock_guard<std::mutex> lock(_presentMutex);
        _eventHandler->reshape(w, h);
    }
}

void WindowController::draw() {
    if (_running) {
        std::lock_guard<std::mutex> lock(_presentMutex);
        _eventHandler->draw();
    }
}

void WindowController::frame(float delta) {
    if (_running) {
        std::lock_guard<std::mutex> lock(_presentMutex);
        _eventHandler->frame(delta);
    }
}

void WindowController::destroy() {
    _running = false;
    {
        std::lock_guard<std::mutex> lock(_presentMutex);
        _eventHandler->destroy();
        _eventHandler = nullptr;
    }
}

void WindowController::keyUp(const bg::base::KeyboardEvent & evt) {
    std::lock_guard<std::mutex> lock(_presentMutex);
	_eventHandler->keyUp(evt);
}

void WindowController::keyDown(const bg::base::KeyboardEvent & evt) {
    std::lock_guard<std::mutex> lock(_presentMutex);
	_eventHandler->keyDown(evt);
}

void WindowController::charPress(const bg::base::KeyboardEvent & evt) {
    std::lock_guard<std::mutex> lock(_presentMutex);
	_eventHandler->charPress(evt);
}

void WindowController::mouseDown(const bg::base::MouseEvent & evt) {
    std::lock_guard<std::mutex> lock(_presentMutex);
	_eventHandler->mouseDown(evt);
}

void WindowController::mouseDrag(const bg::base::MouseEvent & evt) {
    std::lock_guard<std::mutex> lock(_presentMutex);
	_eventHandler->mouseDrag(evt);
}

void WindowController::mouseMove(const bg::base::MouseEvent & evt) {
    std::lock_guard<std::mutex> lock(_presentMutex);
	_eventHandler->mouseMove(evt);
}

void WindowController::mouseUp(const bg::base::MouseEvent & evt) {
    std::lock_guard<std::mutex> lock(_presentMutex);
	_eventHandler->mouseUp(evt);
}

void WindowController::mouseWheel(const bg::base::MouseEvent & evt) {
    std::lock_guard<std::mutex> lock(_presentMutex);
	_eventHandler->mouseWheel(evt);
}


}
}

