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

#include <bg/gui/layout.hpp>

#include <bg/gui/view.hpp>

namespace bg {
namespace gui {


void Layout::GetPosition(Alignment align, const bg::math::Size2Di & parentSize, const bg::math::Size2Di & containerSize, const bg::math::Size2Di & margin, bg::math::Position2Di & pos) {
	switch (align) {
	case kAlignTopLeft:
		pos.set(
			margin.width(),
			margin.height());
		break;
	case kAlignTopCenter:
		pos.set(
			parentSize.width() / 2 - containerSize.width() / 2,
			margin.height());
		break;
	case kAlignTopRight:
		pos.set(
			parentSize.width() - containerSize.width() - margin.width(),
			margin.height());
		break;
	case kAlignMiddleLeft:
		pos.set(
			margin.width(),
			parentSize.height() / 2 - containerSize.height() / 2);
		break;
	case kAlignMiddleCenter:
		pos.set(
			parentSize.width() / 2 - containerSize.width() / 2,
			parentSize.height() / 2 - containerSize.height() / 2);
		break;
	case kAlignMiddleRight:
		pos.set(
			parentSize.width() - containerSize.width() - margin.width(),
			parentSize.height() / 2 - containerSize.height() / 2);
		break;
	case kAlignBottomLeft:
		pos.set(
			margin.width(),
			parentSize.height() - containerSize.height() - margin.height());
		break;
	case kAlignBottomCenter:
		pos.set(
			parentSize.width() / 2 - containerSize.width() / 2,
			parentSize.height() - containerSize.height() - margin.height());
		break;
	case kAlignBottomRight:
		pos.set(
			parentSize.width() - containerSize.width() - margin.width(),
			parentSize.height() - containerSize.height() - margin.height());
		break;
	}
}

bool Layout::UpdateSize(Alignment align, Anchor anchor, const bg::math::Size2Di & parentSize, const bg::math::Size2Di & containerSize, const bg::math::Size2Di & margin, const bg::math::Size2Di & anchorMargin, bg::math::Size2Di & size) {
// TODO: Complete implementation
	bool sizeChanged = false;
	size = containerSize;
	// Extend height
	if (((align==kAlignTopLeft || align==kAlignTopRight || align==kAlignTopCenter) &&
		(anchor & kAnchorBottom)) ||
		((align == kAlignBottomLeft || align == kAlignBottomRight || align == kAlignBottomCenter) &&
		(anchor & kAnchorTop))
	) {
		size.height(parentSize.height() - (margin.height() + anchorMargin.height()));
		sizeChanged = true;
	}

	// Extend width
	if (((align==kAlignTopLeft || align==kAlignBottomLeft || align==kAlignMiddleLeft) &&
		(anchor & kAnchorRight)) ||
		((align == kAlignTopRight || align == kAlignBottomRight || align == kAlignMiddleRight) &&
		(anchor & kAnchorLeft))
	) {
		size.width(parentSize.width() - (margin.width() + anchorMargin.width()));
		sizeChanged = true;
	}

	return sizeChanged;
}

Layout::Layout(View * view)
	:_view(view)
	,_borderPadding(10)
	,_innerPadding(5)
	,_viewPaddingLeft(0)
	,_viewPaddingTop(0)
	,_viewPaddingRight(0)
	,_viewPaddingBottom(0)
{

}

Layout::Layout()
	:_borderPadding(10)
	,_innerPadding(5)
	,_viewPaddingLeft(0)
	,_viewPaddingTop(0)
	,_viewPaddingRight(0)
	,_viewPaddingBottom(0)
{

}

Layout::~Layout() {

}

Skin * Layout::skin() {
	return _view.valid() ? _view->skin() : nullptr;
}

void VerticalLayout::beginLayout() {
	_viewPaddingLeft = 0;
	_viewPaddingTop = 0;
	_viewPaddingRight = 0;
	_viewPaddingBottom = 0;
	for (auto d : _view->decorators()) {
		d->addPadding(_viewPaddingLeft, _viewPaddingTop, _viewPaddingRight, _viewPaddingBottom);
	}
	_topPosition = _borderPadding + _viewPaddingTop;
}

void VerticalLayout::setNext(View * view) {
	bg::math::Position2Di pos(_borderPadding + _viewPaddingLeft, _topPosition);
	view->setPosition(pos);
	_topPosition += view->size().height() + _innerPadding;
	int w = _view->size().width() - _borderPadding * 2 - _viewPaddingLeft - _viewPaddingRight;
	view->setWidth(w);
}

void HorizontalLayout::beginLayout() {
	_viewPaddingLeft = 0;
	_viewPaddingTop = 0;
	_viewPaddingRight = 0;
	_viewPaddingBottom = 0;
	for (auto d : _view->decorators()) {
		d->addPadding(_viewPaddingLeft, _viewPaddingTop, _viewPaddingRight, _viewPaddingBottom);
	}
	_leftPosition = _borderPadding + _viewPaddingLeft;
}

void HorizontalLayout::setNext(View * view) {
	bg::math::Position2Di pos(_leftPosition, _borderPadding + _viewPaddingTop);
	view->setPosition(pos);
	_leftPosition += view->size().width() + _innerPadding;
	int h = _view->size().height() - _borderPadding * 2 - _viewPaddingTop - _viewPaddingBottom;
	view->setHeight(h);
}

}
}
