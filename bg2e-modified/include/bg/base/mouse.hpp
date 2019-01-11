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


#ifndef _bg2e_base_mouse_hpp_
#define _bg2e_base_mouse_hpp_

#include <iostream>

namespace bg {
namespace base {

class Mouse {
public:
	enum Button {
		kLeftButton		= 1 << 0,
		kMiddleButton	= 1 << 1,
		kRightButton	= 1 << 2,

		kButtonNone		= 1 << 16
	};

	Mouse() :_buttonMask(0), _releasedButton(kButtonNone) {}

	inline void setMouseDown(Button btn) { _buttonMask = _buttonMask | btn; }
	inline void setMouseUp(Button btn) { _buttonMask = _buttonMask & ~btn; }
	inline bool getButtonStatus(Button btn) const { return (_buttonMask & btn)!=0; }
	inline unsigned int buttonMask() const { return _buttonMask; }
	inline void setButtonMask(unsigned int mask) { _buttonMask = mask; }
	inline bool anyButtonPressed() const { return _buttonMask!=0; }
	inline void setReleasedButton(Button btn) { _releasedButton = btn; }
	inline Button getReleasedButton() const { return _releasedButton; }

	inline void operator=(const Mouse & m) { _buttonMask = m._buttonMask; _releasedButton = m._releasedButton; }

protected:
	unsigned int _buttonMask;
	Button _releasedButton;
};

}
}

#endif
