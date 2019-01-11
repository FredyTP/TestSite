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

#ifndef _bg2e_gui_radio_button_hpp_
#define _bg2e_gui_radio_button_hpp_


#include <bg/gui/label.hpp>

#include <bg/gui/common.hpp>

#include <functional>

namespace bg {
namespace gui {

class BG2E_EXPORT RadioButton : public bg::gui::View {
public:
	RadioButton(bg::base::Context *, Skin * s);

	void setOnValueChangedAction(std::function<void(bool, int)> a) { _action = a; }

	inline void addToGroup(RadioButton * btn) { _groupButtons.push_back(btn); btn->_master = this; initStatus(); }

	virtual void mouseUp(const bg::base::MouseEvent &);

	void setActiveOption(int index);
	void setActiveOption(RadioButton * btn);

protected:
	virtual ~RadioButton();

	virtual void updateContent(bg::base::Pipeline *);
	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kRadioButton; }

	std::function<void(bool, int)> _action;
	bool _checked;
	std::vector<bg::ptr<RadioButton>> _groupButtons;
	RadioButton * _master;

	void callAction();
	void initStatus();
};

class BG2E_EXPORT RadioButtonField : public bg::gui::View {
public:
	RadioButtonField(bg::base::Context *, Skin * s);

	inline void setOnValueChangedAction(std::function<void(bool, int)> a) { _radioButton->setOnValueChangedAction(a); }

	inline void addToGroup(RadioButton * btn) { _radioButton->addToGroup(btn); }
	inline void addToGroup(RadioButtonField * btn) { _radioButton->addToGroup(btn->_radioButton); }

	inline void setActiveOption(int index) { _radioButton->setActiveOption(index); }
	inline void setActiveOption(RadioButton * btn) { _radioButton->setActiveOption(btn); }

	inline void setText(const std::string & t) { _label->setText(t); }
	inline const std::string & text() const { return _label->text(); }

protected:
	virtual ~RadioButtonField();

	virtual void updateContent(bg::base::Pipeline *);
	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kRadioButtonField; }

	RadioButton * _radioButton;
	ClickableLabel * _label;
};

}
}

#endif
