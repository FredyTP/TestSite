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

#ifndef _bg2e_gui_scroll_contol_hpp_
#define _bg2e_gui_scroll_contol_hpp_

#include <bg/gui/spin_button.hpp>

namespace bg {
namespace gui {

class BG2E_EXPORT ScrollBar : public View {
public:
	ScrollBar(bg::base::Context *, Skin *);

	inline void setTargetViewSize(int s) { _targetViewSize = s; }
	inline void setTargetContentSize(int s) { _contentSize = s; }

protected:
	virtual ~ScrollBar();

	virtual void updateContent(bg::base::Pipeline *);

	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kScrollBar; }

	int _contentSize;
	int _targetViewSize;
};

class BG2E_EXPORT ScrollControl : public View {
public:
	ScrollControl(bg::base::Context *, Skin *);

	inline void setTargetView(View * v) { _targetView = v; }
	inline View * targetView() { return _targetView; }
	inline const View * targetView() const { return _targetView; }

protected:
	virtual ~ScrollControl();

	virtual void updateContent(bg::base::Pipeline *);

	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kScrollControl; }

	bg::ptr<SpinButtonUp> _spinUp;
	bg::ptr<SpinButtonDown> _spinDown;
	bg::ptr<ScrollBar> _scrollBar;
	View * _targetView;
};


class BG2E_EXPORT ScrollDecorator : public ViewDecorator {
public:
	ScrollDecorator();

	virtual void update(bg::gui::View * decoratedView, bg::base::Pipeline * pipeline, float scale);

protected:
	virtual ~ScrollDecorator();

	bg::gui::ScrollControl * _scroll;
};

}
}

#endif
