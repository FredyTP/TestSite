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

#ifndef _bg2e_gui_check_box_hpp_
#define _bg2e_gui_check_box_hpp_


#include <bg/gui/label.hpp>

#include <bg/gui/common.hpp>

#include <functional>

namespace bg {
namespace gui {

class BG2E_EXPORT CheckBox : public bg::gui::View {
public:
	enum Value {
		kValueOff = 0,
		kValueOn = 1,
		kValueMixed = -1
	};
	CheckBox(bg::base::Context *, Skin * s);
	
	void setOnValueChangedAction(std::function<void(Value)> a) { _action = a; }

	virtual void mouseUp(const bg::base::MouseEvent &);

	inline void setValue(Value v) { setContentChanged(true); _value = v; callAction(); }
	inline Value value() const { return _value; }

protected:
	virtual ~CheckBox();

	virtual void updateContent(bg::base::Pipeline *);
	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kCheckBox; }

	std::function<void(Value)> _action;
	Value _value;

	void callAction() { if (_action) _action(_value); }
};

class BG2E_EXPORT CheckBoxField : public bg::gui::View {
public:
	CheckBoxField(bg::base::Context *, Skin *);

	inline void setOnValueChangedAction(std::function<void(CheckBox::Value)> a) { _checkBox->setOnValueChangedAction(a); }
	inline void setValue(CheckBox::Value v) { _checkBox->setValue(v); }
	inline CheckBox::Value value() const { return _checkBox->value(); }

	inline void setText(const std::string & t) { _label->setText(t); }
	inline const std::string & text() const { return _label->text(); }

protected:
	virtual ~CheckBoxField();

	virtual void updateContent(bg::base::Pipeline *);
	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kCheckBoxField; }

	CheckBox * _checkBox;
	ClickableLabel * _label;
};

}
}

#endif
