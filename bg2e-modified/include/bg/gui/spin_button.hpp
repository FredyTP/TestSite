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

#ifndef _bg2e_gui_spin_button_hpp_
#define _bg2e_gui_spin_button_hpp_

#include <bg/gui/text_input.hpp>
#include <bg/gui/button.hpp>

namespace bg {
namespace gui {

class SpinButtonUp : public Button {
public:
	SpinButtonUp(bg::base::Context * ctx, Skin * s) :Button(ctx, s) {}

protected:
	virtual ~SpinButtonUp() {}

	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kSpinButtonUp; }
};

class SpinButtonDown : public Button {
public:
	SpinButtonDown(bg::base::Context * ctx, Skin * s) :Button(ctx, s) {}

protected:
	virtual ~SpinButtonDown() {}

	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kSpinButtonDown; }
};

class BG2E_EXPORT SpinButtonField : public View {
public:
	SpinButtonField(bg::base::Context *, Skin * s);

	inline void setPrecision(int p) { _precision = p; updateValueText(); }
	inline void setValue(float v) { _value = v; updateValueText(); }
	inline float value() const { return _value; }

	void setBaseIncrement(float inc) {
		setIncrements(inc / 10.0f, inc, inc * 10.0f, inc * 100.0f);
	}
	void setIncrements(float fineInc, float inc, float coarseInc, float hugeInc) {
		_fineIncrement = fineInc;
		_increment = inc;
		_coarseIncrement = coarseInc;
		_hugeIncrement = hugeInc;
	}
	inline float fineIncrement() const { return _fineIncrement; }
	inline float increment() const { return _increment; }
	inline float coarseIncrement() const { return _coarseIncrement; }
	inline float hugeIncrement() const { return _hugeIncrement; }

protected:
	virtual ~SpinButtonField();

	virtual void updateContent(bg::base::Pipeline *);
	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kSpinButtonField; }

	void updateValueText();

	SpinButtonUp * _spinUp;
	SpinButtonDown * _spinDown;
	TextInput * _textInput;

	int _precision;
	float _value;
	float _fineIncrement;
	float _increment;
	float _coarseIncrement;
	float _hugeIncrement;

	float getIncrement(const bg::base::KeyboardEvent &);
};
}
}

#endif
