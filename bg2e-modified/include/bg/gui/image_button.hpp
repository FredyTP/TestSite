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

#ifndef _bg2e_gui_image_button_hpp_
#define _bg2e_gui_image_button_hpp_

#include <bg/gui/button.hpp>

namespace bg {
namespace gui {

class BG2E_EXPORT ImageButton : public Button {
public:
	ImageButton(bg::base::Context *, Skin * s);

	inline void setImage(Status s, bg::base::Texture * t) {
		switch (s) {
		case kStatusNormal:
			_normal = t;
			break;
		case kStatusHover:
			_hover = t;
			break;
		case kStatusPress:
			_press = t;
			break;
		default:
			throw bg::base::InvalidParameterException("Invalid state specified (ImageButton::setImage())");
		}
	}

protected:
	virtual ~ImageButton();

	virtual void updateContent(bg::base::Pipeline *);
	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kImageButton; }

	bg::ptr<bg::base::Texture> _normal;
	bg::ptr<bg::base::Texture> _hover;
	bg::ptr<bg::base::Texture> _press;
};

}
}

#endif
