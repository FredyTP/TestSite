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

#include <bg/gui/window.hpp>
#include <bg/gui/surface.hpp>

namespace bg {
namespace gui {

Window::Window(Surface * surface)
	:ContextObject(surface->context())
	,_position(0,0)
	,_size(200,200)
	,_sizeChanged(true)
	,_parentSurface(nullptr)
	,_visible(true)
{
	surface->addWindow(this);
}

Window::~Window() {
}

void Window::create() {
	setContentView(View::Create<View>(context(),_parentSurface->skin()));
	_drawVisitor = new DrawVisitor();
	_frameVisitor = new FrameVisitor();
	init();
}

void Window::create(View * contentView) {
	if (!contentView) {
		throw bg::base::NullParameterException("Error creating gui::Window: null content view.");
	}
	setContentView(contentView);
	create();
}

Skin * Window::skin() {
	return parentSurface() ? parentSurface()->skin() : nullptr;
}

const Skin * Window::skin() const {
	return parentSurface() ? parentSurface()->skin() : nullptr;
}

void Window::init() {
}

void Window::draw(bg::base::Pipeline * pipeline) {
	if (_contentView.isNull()) return;
	if (_sizeChanged) {
		if (_parentSurface) {
			_parentSurface->updateLayout(this);
		}
		_contentView->setSize(_size);
		_sizeChanged = false;
	}
    _contentView->setParentWindow(this);
	_contentView->draw(pipeline);
}

}
}
