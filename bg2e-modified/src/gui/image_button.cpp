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

#include <bg/gui/image_button.hpp>

#include <bg/gui/window.hpp>
#include <bg/gui/surface.hpp>

#include <bg/log.hpp>

namespace bg {
namespace gui {

ImageButton::ImageButton(bg::base::Context * ctx, Skin * s)
	:Button(ctx, s)
{
}

ImageButton::~ImageButton() {
}

void ImageButton::updateContent(bg::base::Pipeline * pipeline) {
	if(_label->size() != size()) {
		_label->setSize(size());
	}
	pipeline->setClearColor(bg::math::Color::Transparent());
	pipeline->clearBuffers();
	switch (_status) {
	case kStatusNormal:
		pipeline->drawTexture(_normal.getPtr());
		break;
	case kStatusHover:
		pipeline->drawTexture(_hover.getPtr());
		break;
	case kStatusPress:
		pipeline->drawTexture(_press.getPtr());
		break;
	case kStatusDisabled:
		pipeline->drawTexture(_normal.getPtr());
		break;
    default:
        bg::log(bg::log::kWarning) << "Unexpected mouse button status" << bg::endl;
	}
}

}
}
