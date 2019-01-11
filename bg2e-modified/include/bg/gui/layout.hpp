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

#ifndef _bg2e_gui_layout_hpp_
#define _bg2e_gui_layout_hpp_

#include <bg/math/vector.hpp>

#include <bg/export.hpp>

#include <bg/gui/skin.hpp>

namespace bg {
namespace gui {

class View;
class BG2E_EXPORT Layout : public bg::base::ReferencedPointer {
	friend class View;
public:

	static void GetPosition(Alignment align, const bg::math::Size2Di & parentSize, const bg::math::Size2Di & containerSize, const bg::math::Size2Di & margin, bg::math::Position2Di & pos);
	
	static bool UpdateSize(Alignment align, Anchor anchor, const bg::math::Size2Di & parentSize, const bg::math::Size2Di & containerSize, const bg::math::Size2Di & margin, const bg::math::Size2Di & anchorMargin, bg::math::Size2Di & size);

	Layout(View * view);
	Layout();

	Skin * skin();

	inline void setBorderPadding(int p) { _borderPadding = p; }
	inline int borderPadding() const { return _borderPadding; }

	inline void setInnerPadding(int p) { _innerPadding = p; }
	inline int innerPadding() const { return _innerPadding; }
protected:
	virtual ~Layout();

	virtual void beginLayout() = 0;
	virtual void setNext(View *) = 0;

	int _borderPadding;
	int _innerPadding;

	int _viewPaddingLeft;
	int _viewPaddingTop;
	int _viewPaddingRight;
	int _viewPaddingBottom;

	bg::ptr<View> _view;
};

class BG2E_EXPORT VerticalLayout : public Layout {
public:
	VerticalLayout(View * view) :Layout(view) {}
	VerticalLayout() :Layout() {}

protected:
	virtual ~VerticalLayout() {}

	virtual void beginLayout();
	virtual void setNext(View *);

	int _topPosition;

};

class BG2E_EXPORT HorizontalLayout : public Layout {
public:
	HorizontalLayout(View * view) :Layout(view) {}
	HorizontalLayout() :Layout() {}

protected:
	virtual ~HorizontalLayout() {}

	virtual void beginLayout();
	virtual void setNext(View *);

	int _leftPosition;
};

}
}

#endif
