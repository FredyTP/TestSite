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

#include <bg/log.hpp>

#if BG2E_ANDROID==1

#include <bg/android/app.hpp>

#endif

namespace bg {

log::Level log::_currentLevel = Level::kDebug;
const std::string endl = "\n";

log::~log() {
	switch(_level) {
		case kError:
#if BG2E_ANDROID==0
		std::cerr << "ERROR: " << _out.str();
#else
		bg::android::App::Get().logError("ERROR: " + _out.str());
#endif
		break;
	case kWarning:
#if BG2E_ANDROID==0
		std::cerr << "WARNING: " << _out.str();
#else
		bg::android::App::Get().logWarning("WARNING: " + _out.str());
#endif
		break;
	case kDebug:
#if BG2E_ANDROID==0
		std::cout << _out.str();
#else
		bg::android::App::Get().logDebug(_out.str());
#endif
		break;
	}
}

void log::printLog(const std::string & log) {
	if (_currentLevel<_level) {
		return;
	}
	_out << log;
}

}