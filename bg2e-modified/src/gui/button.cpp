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

#include <bg/gui/button.hpp>
#include <bg/gui/window.hpp>
#include <bg/gui/surface.hpp>
#include <bg/system/system.hpp>

namespace bg {
namespace gui {

Button::Button(bg::base::Context * ctx, Skin * skin)
	:View(ctx, skin)
{
	_label = View::Create<Label>(ctx, skin);
	_label->paragraph().setAlignment(bg::text::Paragraph::kHAlignCenter);
	_label->paragraph().setAlignment(bg::text::Paragraph::kVAlignCenter);
	addChild(_label.getPtr());
}

Button::~Button() {
}

void Button::mouseUp(const bg::base::MouseEvent & evt) {
	View::mouseUp(evt);
	if (parentSurface()) {
		parentSurface()->clearFocus();
	}
	if (_action) {
		_action();
	}
}

void Button::updateContent(bg::base::Pipeline * pipeline) {
	if (_label->size()!=size()) {
		_label->setSize(size());
	}
	View::updateContent(pipeline);
}

}
}
