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

#include <bg/gui/radio_button.hpp>
#include <bg/gui/window.hpp>
#include <bg/gui/surface.hpp>
#include <bg/system/system.hpp>

namespace bg {
namespace gui {

RadioButton::RadioButton(bg::base::Context * ctx, Skin * s)
	:View(ctx, s)
	,_checked(false)
	,_master(nullptr)
{
}

RadioButton::~RadioButton() {
}

void RadioButton::mouseUp(const bg::base::MouseEvent & evt) {
	View::mouseUp(evt);
	setActiveOption(this);
	callAction();
}

void RadioButton::setActiveOption(int index) {
	if (_master == nullptr) {
		if (index == 0) {
			_checked = true;
			setContentChanged(true);
			for (auto btn : _groupButtons) {
				btn->_checked = false;
				btn->setContentChanged(true);
				btn->callAction();
			}
			callAction();
		}
		else {
			_checked = false;
			setContentChanged(true);
			for (auto i = 0; i < _groupButtons.size(); ++i) {
				if (i == static_cast<size_t>(index - 1)) {
					_groupButtons[i]->_checked = true;
				}
				else {
					_groupButtons[i]->_checked = false;
				}
				_groupButtons[i]->setContentChanged(true);
			}
			callAction();
		}
	}
	else {
		_master->setActiveOption(index);
	}
}

void RadioButton::setActiveOption(RadioButton * btn) {
	if (_master == nullptr) {
		if (btn == this) {
			setActiveOption(0);
		}
		else {
			for (auto i = 0; i < _groupButtons.size(); ++i) {
				if (_groupButtons[i].getPtr() == btn) {
					setActiveOption(static_cast<int>(i + 1));
					break;
				}
			}
		}
	}
	else {
		_master->setActiveOption(btn);
	}
}

void RadioButton::updateContent(bg::base::Pipeline * pipeline) {
	pipeline->setClearColor(_skinProperties[_status].backgroundColor);
	pipeline->clearBuffers();
	Status s = _checked ? kStatusPress : kStatusNormal;
	bg::base::Texture * tex = _skinProperties[s].image.getPtr();
	if (tex) pipeline->drawTexture(tex);
}

void RadioButton::callAction() {
	if (_master == nullptr) {
		auto selectedIndex = 0;
		for (auto i = 0; i < _groupButtons.size(); ++i) {
			if (_groupButtons[i]->_checked) {
				selectedIndex = i + 1;
				break;
			}
		}
		if (_action) {
			_action(_checked, selectedIndex);
		}
		for (auto btn : _groupButtons) {
			if (btn->_action) btn->_action(btn->_checked, selectedIndex);
		}
	}
	else {
		_master->callAction();
	}
}

void RadioButton::initStatus() {
	_checked = true;
	setContentChanged(true);
	for (auto btn : _groupButtons) {
		btn->_checked = false;
		btn->setContentChanged(true);
	}
}

RadioButtonField::RadioButtonField(bg::base::Context * ctx, Skin * s)
	:View(ctx, s)
{
	_label = View::Create<ClickableLabel>(ctx, s);
	_radioButton = View::Create<RadioButton>(ctx, s);
	addChild(_radioButton);
	addChild(_label);
	_label->setPosition(bg::math::Position2Di(22, 0));
	_label->setAction([&]() {
		_radioButton->setActiveOption(_radioButton);
	});
}

RadioButtonField::~RadioButtonField() {

}

void RadioButtonField::updateContent(bg::base::Pipeline * pipeline) {
	bg::math::Position2Di labelSize(size().width() - 22, size().height());
	if (_label->size() != labelSize) {
		_label->setSize(labelSize);
	}
	View::updateContent(pipeline);
}

}
}
