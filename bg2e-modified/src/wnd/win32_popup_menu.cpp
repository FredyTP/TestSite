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


#include <bg/wnd/win32_popup_menu.hpp>
#include <bg/system/system.hpp>
#include <iostream>

#if BG2E_WINDOWS

#ifndef WIN32
#define WIN32
#endif
#include <WinSock2.h>
#include <Windows.h>

#endif

namespace bg {
namespace wnd {

#if !BG2E_WINDOWS

Win32PopUpMenu::Win32PopUpMenu() {}
Win32PopUpMenu::~Win32PopUpMenu() {}
void Win32PopUpMenu::show(ItemSelectedClosure) {}
void Win32PopUpMenu::ProcessCommand(int index) {}
	
#else

static PopUpMenu::ItemSelectedClosure s_menuClosure = nullptr;
	
Win32PopUpMenu::Win32PopUpMenu()
{
}

Win32PopUpMenu::~Win32PopUpMenu() {
	s_menuClosure = nullptr;
}

void Win32PopUpMenu::show(ItemSelectedClosure closure) {
	//MODIFY THIS FUNCCCCCCC// NOT WORKIIIING _____ NEW SYNTAX
	HWND hwnd = GetActiveWindow();
	if (hwnd) {
		s_menuClosure = closure;
		RECT windowRect;
		GetWindowRect(hwnd, &windowRect);
		HMENU hPopupMenu = CreatePopupMenu();

		int titleHeight = GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYEDGE)) * 2;

		/*eachMenuItem([&](const PopUpMenuItem & item) {
			InsertMenuA(hPopupMenu, -1, MF_STRING, item.identifier, item.title.c_str());
		});*/

		
		TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN, _position.x() + windowRect.left, _position.y() + windowRect.top + titleHeight, 0, hwnd, nullptr);
	}
}
	
void Win32PopUpMenu::ProcessCommand(MenuItemIdentifier item) {
	if (s_menuClosure) {
		s_menuClosure(item);
	}
}

void Win32PopUpMenu::setCheck(const PopUpMenu * baseMenu, bool check) const
{
	if (_identifier != -1)
	{
		if (check)
		{
			CheckMenuItem(bg::native_cast<HMENU>(baseMenu->hMenu()), (UINT)_identifier, MF_BYCOMMAND | MF_CHECKED);

		}
		else
		{
			CheckMenuItem(bg::native_cast<HMENU>(baseMenu->hMenu()), (UINT)_identifier, MF_BYCOMMAND | MF_UNCHECKED);
		}
		
	}
}

bool Win32PopUpMenu::isChecked(const PopUpMenu * baseMenu) const
{
	DWORD fdwMenu;
	fdwMenu = GetMenuState(bg::native_cast<HMENU>(baseMenu->hMenu()), (UINT)_identifier, MF_BYCOMMAND);
	if (!(fdwMenu & MF_CHECKED))
	{
		return false;
	}
	else
	{
		return true;
	}
}

int Win32PopUpMenu::modifyCheckState(const PopUpMenu * baseMenu) const
{
	if (_identifier != -1)
	{
		if (!this->isChecked(baseMenu))
		{
			CheckMenuItem(bg::native_cast<HMENU>(baseMenu->hMenu()), (UINT)_identifier, MF_BYCOMMAND | MF_CHECKED);
			return 1;
		}
		else
		{
			CheckMenuItem(bg::native_cast<HMENU>(baseMenu->hMenu()), (UINT)_identifier, MF_BYCOMMAND | MF_UNCHECKED);
			return 0;
		}
	}
	return -1;
}

#endif
	
}
}