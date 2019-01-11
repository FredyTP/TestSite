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


#ifndef _bg2e_system_system_hpp_
#define _bg2e_system_system_hpp_

#include <bg/export.hpp>
#include <string>

namespace bg {
namespace system {

extern BG2E_EXPORT bool isBigEndian();
extern BG2E_EXPORT bool isLittleEndian();

enum Platform {
	kUnknown	= 0,
	kMac		= 1,
	kWindows	= 2,
	kLinux		= 3,
	kiOS		= 4,
	kAndroid	= 5
};

extern BG2E_EXPORT Platform currentPlatform();
extern BG2E_EXPORT bool isDesktop();
extern BG2E_EXPORT bool isMobile();
extern BG2E_EXPORT std::string uuid();

}
}

#include <bg/system/archive.hpp>
#include <bg/system/energy_saver.hpp>
#include <bg/system/mouse_cursor.hpp>
#include <bg/system/path.hpp>
#include <bg/system/screen.hpp>
#include <bg/system/utf.hpp>

#endif
