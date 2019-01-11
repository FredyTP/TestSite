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

#include <bg/gui/scroll_control.hpp>

namespace bg {
namespace gui {

ScrollBar::ScrollBar(bg::base::Context * ctx, Skin * skin)
	:View(ctx,skin)
{

}

ScrollBar::~ScrollBar() {

}

void ScrollBar::updateContent(bg::base::Pipeline * pipeline) {
	pipeline->setClearColor(_skinProperties[_status].backgroundColor);
	pipeline->clearBuffers();
}

ScrollControl::ScrollControl(bg::base::Context * ctx, Skin * skin)
	:View(ctx, skin)
{
	using namespace bg::base;

	_scrollBar = View::Create<ScrollBar>(ctx, skin);
	addChild(_scrollBar.getPtr());
}

ScrollControl::~ScrollControl() {

}

void ScrollControl::updateContent(bg::base::Pipeline * pipeline) {
	using namespace bg::math;
	pipeline->setClearColor(_skinProperties[_status].backgroundColor);
	pipeline->clearBuffers();
	
	if (_targetView) {
		Size2Di contentSize = _targetView->contentSize();
		Size2Di visibleSize = _targetView->size();
		Position2Di scroll = _targetView->scroll();

		int scrollBarHeight = visibleSize.height() * size().height() / contentSize.height();
		int scrollBarPosition = scroll.y() * size().height() / contentSize.height();
		_scrollBar->setPosition(bg::math::Position2Di(0, scrollBarPosition));
		_scrollBar->setSize(bg::math::Size2Di(size().width(), scrollBarHeight));
	}
	else {
		_scrollBar->setPosition(bg::math::Position2Di(0, 0));
		_scrollBar->setSize(bg::math::Size2Di(size().width(), 30));
	}

}

ScrollDecorator::ScrollDecorator()
	:ViewDecorator(5, 5, 5, 5)
	, _scroll(nullptr)
{
}

ScrollDecorator::~ScrollDecorator() {
}

void ScrollDecorator::update(bg::gui::View * decoratedView, bg::base::Pipeline * pipeline, float scale) {
	if (!_scroll) {
		_scroll = bg::gui::View::Create<bg::gui::ScrollControl>(decoratedView->context(), decoratedView->skin());
		_scroll->setTargetView(decoratedView);
		_paddingRight += _scroll->size().width();
		_views.push_back(_scroll);
		decoratedView->setScrollEnabled(true);
	}

	bg::math::Size2Di scrollSize = _scroll->size();
	scrollSize.height(decoratedView->size().height() - _paddingTop - _paddingBottom);

	bg::math::Position2Di scrollPos(
		decoratedView->size().width() - _paddingLeft - scrollSize.width(),
		_paddingBottom
	);

	_scroll->setPosition(scrollPos);
	_scroll->setSize(scrollSize);
}

}
}
