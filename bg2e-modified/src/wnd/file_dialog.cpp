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


#include <bg/wnd/file_dialog.hpp>

#include <bg/wnd/cocoa_file_dialog.hpp>
#include <bg/wnd/win32_file_dialog.hpp>

#include <bg/platform.hpp>
#include <bg/system/system.hpp>
#include <bg/system/path.hpp>

namespace bg {
namespace wnd {

FileDialog::FileDialog()
	:_type(kTypeOpenFile)
{
	if (bg::system::currentPlatform()!=bg::system::kMac) {
		_initPath = bg::system::Path::ExecDir().text();
	}
}
	
FileDialog::~FileDialog() {
	
}
	
	
FileDialog * FileDialog::Get() {
	bg::ptr<FileDialog> result;
	
	if (bg::system::currentPlatform()==bg::system::Platform::kMac) {
		result = new CocoaFileDialog();
	}
	else if (bg::system::currentPlatform()==bg::system::Platform::kWindows) {
		result = new Win32FileDialog();
	}

	return result.release();
}

}
}
