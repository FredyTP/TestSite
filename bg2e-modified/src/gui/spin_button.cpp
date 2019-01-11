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

#include <bg/gui/spin_button.hpp>

#include <sstream>
#include <iomanip>

namespace bg {
namespace gui {

SpinButtonField::SpinButtonField(bg::base::Context * ctx, Skin * s)
	:View(ctx, s)
	,_precision(3)
	,_fineIncrement(0.1f)
	,_increment(1.0f)
	,_coarseIncrement(10.0f)
	,_hugeIncrement(100.0f)
{
	_textInput = View::Create<TextInput>(ctx, s);
	_textInput->setValidator(new NumericTextValidator());
	_spinUp = View::Create<SpinButtonUp>(ctx, s);
	_spinDown = View::Create<SpinButtonDown>(ctx, s);
	_spinDown->setPosition(bg::math::Position2Di(0,14));
	addChild(_textInput);
	addChild(_spinUp);
	addChild(_spinDown);
	setValue(0.0f);

	_spinUp->setAction([&]() {
		setValue(_value + _increment);
	});

	_spinDown->setAction([&]() {
		setValue(_value - _increment);
	});

	using namespace bg::base;
	_textInput->setKeyAction(Keyboard::kKeyUp, [&](const KeyboardEvent & evt) {
		setValue(_value + getIncrement(evt));
	});

	_textInput->setKeyAction(Keyboard::kKeyDown, [&](const KeyboardEvent & evt) {
		setValue(_value - getIncrement(evt));
	});
	
	_textInput->setEditFinishedClosure([&](FormField * field) {
		_value = std::stof(_textInput->text());
		updateValueText();	// This will update the text numeric format
	});
}

SpinButtonField::~SpinButtonField() {

}

void SpinButtonField::updateContent(bg::base::Pipeline * pipeline) {
	bg::math::Size2Di textSize = size();
	bg::math::Size2Di spinSize(28, textSize.height() / 2);
	textSize.x(textSize.x() - spinSize.x());
	_textInput->setSize(textSize);
	bg::math::Position2Di spinPos(textSize.width(), 0);
	_spinUp->setPosition(spinPos);
	spinPos.y(14);
	_spinDown->setPosition(spinPos);
	View::updateContent(pipeline);
	
}

void SpinButtonField::updateValueText() {
	std::stringstream str;
	str << std::fixed << std::setprecision(_precision) << _value;
	_textInput->setText(str.str());
}

float SpinButtonField::getIncrement(const bg::base::KeyboardEvent & evt) {
	using namespace bg::base;
	if (evt.keyboard().getModifierStatus(Keyboard::kShiftKey) &&
		evt.keyboard().getModifierStatus(Keyboard::kAltKey)
	) {
		return _hugeIncrement;
	}
	else if (evt.keyboard().getModifierStatus(Keyboard::kAltKey)) {
		return _fineIncrement;
	}
	else if (evt.keyboard().getModifierStatus(Keyboard::kShiftKey)) {
		return _coarseIncrement;
	}
	else {
		return _increment;
	}
}

}
}
