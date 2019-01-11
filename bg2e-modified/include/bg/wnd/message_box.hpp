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


#ifndef _bg2e_wnd_message_box_hpp_
#define _bg2e_wnd_message_box_hpp_

#include <bg/export.hpp>
#include <bg/wnd/window.hpp>

namespace bg {
namespace wnd {

#if BG2E_WINDOWS==1
#ifdef MessageBox 
#undef MessageBox
#endif
#endif

class BG2E_EXPORT MessageBox {
public:
	enum StandardButton {
		kButtonOk			= 1,
		kButtonOkCancel		= 2,
		kButtonYesNoCancel	= 3,
		kButtonYesNo		= 4
	};

	enum Response {
		kResponseOk		= 1,
		kResponseCancel	= 2,
		kResponseYes	= 3,
		kResponseNo		= 4
	};

	static Response Show(Window * parentModal, const std::string & title, const std::string & message, StandardButton standardButtons = kButtonOk);
};

}
}

#endif