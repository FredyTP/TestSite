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

#ifndef _bg2e_gui_skin_hpp_
#define _bg2e_gui_skin_hpp_

#include <bg/base/referenced_pointer.hpp>

#include <bg/gui/common.hpp>

#include <bg/text/font.hpp>
#include <bg/math/vector.hpp>

namespace bg {
namespace gui {

class Skin : public bg::base::ReferencedPointer {
public:

	enum WidgetType {
		kView = 0,
		kButton,
		kImageButton,
		kTextInput,
		kLabel,
		kCheckBox,
		kRadioButton,
		kSpinButtonUp,
		kSpinButtonDown,
		kCheckBoxField,
		kRadioButtonField,
		kSpinButtonField,
		kScrollControl,
		kScrollBar,
		kToolbarButton,
		kToolbarItem
	};

	struct Properties {
		bg::math::Color backgroundColor;
		bg::math::Color borderColor;
		int borderWidth;
		int padding;
		bg::ptr<bg::base::Texture> image;
		bg::math::Color textColor;
		bg::math::Size2Di defaultSize;
	};

	Skin() {}

	inline const bg::text::Font * font() const { return _font.getPtr(); }
	inline bg::text::Font * font() { return _font.getPtr(); }
	inline void setFont(bg::text::Font * f) { _font = f; }

	inline Properties & widgetProperties(WidgetType type, Status ms = Status::kStatusNormal) { return _widgetProperties[type][ms]; }
	inline void setWidgetProperties(WidgetType type, const Properties & p, Status ms = Status::kStatusAll) {
        if (_widgetProperties[type].size()<kMaxMouseStates) {
            _widgetProperties[type].resize(kMaxMouseStates);
        }
		if (ms < kMaxMouseStates) {
			_widgetProperties[type][ms] = p;
		}
		else {
			for (auto status = 0; status < kMaxMouseStates; ++status) {
				_widgetProperties[type][status] = p;
			}
		}
	}

protected:
	virtual ~Skin() {}

    std::map<WidgetType, std::vector<Properties>> _widgetProperties;
	bg::ptr<bg::text::Font> _font;
};

}
}

#endif
