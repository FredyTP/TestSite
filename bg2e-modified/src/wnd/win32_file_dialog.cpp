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


#include <bg/wnd/win32_file_dialog.hpp>
#include <bg/system/system.hpp>

#include <sstream>

#if BG2E_WINDOWS

#ifndef WIN32
#define WIN32
#endif
#include <WinSock2.h>
#include <Windows.h>
#include <ShlObj.h>
#endif

namespace bg {
namespace wnd {

#if !BG2E_WINDOWS

Win32FileDialog::Win32FileDialog() { }
Win32FileDialog::~Win32FileDialog() { }
bool Win32FileDialog::show() { return false; }
	
#else
	
Win32FileDialog::Win32FileDialog() {

}

Win32FileDialog::~Win32FileDialog() {

}

bool Win32FileDialog::show() {
	switch (_type) {
	case kTypeOpenFile:
		return showOpenFileDialog();
	case kTypeSaveFile:
		return showSaveFileDialog();
	case kTypeSelectFolder:
		return showSelectFolderDialog();
	default:
		return false;
	}
}
	
bool Win32FileDialog::showOpenFileDialog() {
	HWND hwnd = GetActiveWindow();
	std::stringstream filters;
	OPENFILENAMEA ofn;
	char szFileName[MAX_PATH];
	std::string filterString;

	szFileName[0] = '\0';
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	ofn.lpstrInitialDir = _initPath.c_str();

	if (_filters.size() > 0) {
		

		for (std::string f : _filters) {
			filterString += f + " files (*." + f + ")" + '\0';
			filterString += "*." + f + '\0';
		}
		
		ofn.lpstrFilter = filterString.c_str();
	}

	if (GetOpenFileNameA(&ofn)) {
		_resultPath = szFileName;
		return true;
	}
	return false;
}

bool Win32FileDialog::showSaveFileDialog() {
	HWND hwnd = GetActiveWindow();
	std::stringstream filters;
	OPENFILENAMEA ofn;
	char szFileName[MAX_PATH];
	std::string filterString;

	szFileName[0] = '\0';
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	ofn.lpstrInitialDir = _initPath.c_str();

	if (_filters.size() > 0) {


		for (std::string f : _filters) {
			filterString += f + " files (*." + f + ")" + '\0';
			filterString += "*." + f + '\0';
		}

		ofn.lpstrFilter = filterString.c_str();
	}

	if (GetSaveFileNameA(&ofn)) {
		_resultPath = szFileName;
		return true;
	}
	return false;
}

bool Win32FileDialog::showSelectFolderDialog() {
	HWND hwnd = GetActiveWindow();
	bool result = false;

	BROWSEINFOA bi;
	memset(&bi, 0, sizeof(bi));

	bi.ulFlags = BIF_USENEWUI;
	bi.hwndOwner = hwnd;
	bi.lpszTitle = "";
	::OleInitialize(nullptr);

	LPITEMIDLIST pIDL = ::SHBrowseForFolderA(&bi);

	if (pIDL != nullptr) {
		char buffer[MAX_PATH] = { '\0' };
		if (::SHGetPathFromIDListA(pIDL, buffer) != 0) {
			_resultPath = buffer;
			result = true;
		}

		CoTaskMemFree(pIDL);
	}

	::OleUninitialize();

	return result;
}


#endif

}
}
