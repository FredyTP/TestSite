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

#include <bg/gui/toolbar_button.hpp>
#include <bg/gui/window.hpp>
#include <bg/gui/surface.hpp>
#include <bg/system/system.hpp>

#include <bg/gui/layout.hpp>

namespace bg {
namespace gui {

ToolbarButton::ToolbarButton(bg::base::Context * ctx, Skin * skin)
	:Button(ctx, skin)
{
}

ToolbarButton::~ToolbarButton() {
}

void ToolbarButton::updateContent(bg::base::Pipeline * pipeline) {
	if (_label->size()!=size()) {
		_label->setSize(size());
	}
	Button::updateContent(pipeline);
	if (_icon.valid()) {
		pipeline->drawTexture(_icon.getPtr());
	}
}

ToolbarItem::ToolbarItem(bg::base::Context * ctx, Skin * skin)
	:View(ctx,skin)
{
	
}

ToolbarItem::~ToolbarItem() {
}

void ToolbarItem::updateContent(bg::base::Pipeline * pipeline) {
	bg::math::Vector2 total;
	float horizontalPadding = 10;
	float iconRatio = 0.7f;
	float textRatio = 0.4f;
	float textPosRatio = 0.6f;
	float iconSize = static_cast<float>(size().height()) * iconRatio;
	float textSize = static_cast<float>(size().height()) * textRatio;
	_label->stringRenderer().getStringMetrics(_label->text(), total, true);
	_label->setTextPadding(0);
	if (_label->stringRenderer().font()) {
		float textScale = _label->stringRenderer().font()->textScale();
		float textSize = _label->stringRenderer().font()->size();
		total.scale(textScale * textSize);
	}
	setWidth(static_cast<int>(total.width() + horizontalPadding * 2.0f));
	_btn->setSize(bg::math::Size2Di(static_cast<int>(iconSize)));
	_btn->setPosition(bg::math::Position2Di(
		static_cast<int>(static_cast<float>(width())/2.0f - static_cast<float>(iconSize)/2.0f),
		0
	));
	_label->setSize(bg::math::Size2Di(size().width(),static_cast<int>(textSize)));
	_label->setPosition(bg::math::Position2Di(
		0,
		static_cast<int>(static_cast<float>(height()) * textPosRatio))
	);
	View::updateContent(pipeline);
}

void ToolbarItem::build() {
	View::build();

	_btn = View::Create<ToolbarButton>(context(),skin());
	addChild(_btn);
	_label = View::Create<Label>(context(),skin());
	_label->setTextScale(0.8f);
	_label->paragraph().setAlignment(bg::text::Paragraph::kHAlignCenter);
	addChild(_label);
}


}
}
