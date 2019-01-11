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

#include <bg/gui/check_box.hpp>
#include <bg/gui/window.hpp>
#include <bg/gui/surface.hpp>
#include <bg/system/system.hpp>

namespace bg {
namespace gui {

CheckBox::CheckBox(bg::base::Context * ctx, Skin * s)
	:View(ctx, s)
	,_value(kValueOff)
{
}

CheckBox::~CheckBox() {
}

void CheckBox::mouseUp(const bg::base::MouseEvent & evt) {
	View::mouseUp(evt);
	switch (_value) {
	case kValueOff:
	case kValueMixed:
		_value = kValueOn;
		break;
	case kValueOn:
		_value = kValueOff;
		break;
	}
	callAction();
}

void CheckBox::updateContent(bg::base::Pipeline * pipeline) {
	pipeline->setClearColor(_skinProperties[_status].backgroundColor);
	pipeline->clearBuffers();
	Status s = kStatusNormal;
	switch (_value) {
	case kValueOff:
		s = kStatusNormal;
		break;
	case kValueMixed:
		s = kStatusHover;
		break;
	case kValueOn:
		s = kStatusPress;
		break;
	}
	bg::base::Texture * tex = _skinProperties[s].image.getPtr();
	if (tex) pipeline->drawTexture(tex);
}

CheckBoxField::CheckBoxField(bg::base::Context * context, Skin * s)
	:View(context, s)
{
	_label = View::Create<ClickableLabel>(context, s);
	_checkBox = View::Create<CheckBox>(context, s);
	addChild(_checkBox);
	addChild(_label);
	_label->setPosition(bg::math::Position2Di(22, 0));
	_label->setAction([&]() {
		_checkBox->setValue(_checkBox->value()==CheckBox::kValueOff ? CheckBox::kValueOn : CheckBox::kValueOff);
	});
}

CheckBoxField::~CheckBoxField() {
	
}

void CheckBoxField::updateContent(bg::base::Pipeline * pipeline) {
	bg::math::Position2Di labelSize(size().width() - 22,size().height());
	if (_label->size() != labelSize) {
		_label->setSize(labelSize);
	}
	View::updateContent(pipeline);
}

}
}
