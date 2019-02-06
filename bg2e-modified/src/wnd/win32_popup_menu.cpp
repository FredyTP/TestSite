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
	HWND hwnd = GetActiveWindow();
	if (hwnd) {
		s_menuClosure = closure;
		RECT windowRect;
		GetWindowRect(hwnd, &windowRect);
		_hMenu = CreatePopupMenu();

		int titleHeight = GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYEDGE)) * 2;

		this->initSubMenus();
		
		//THERE IS A BUG WHEN U USE TPM_LEFTBUTTON//
		TrackPopupMenuEx(bg::native_cast<HMENU>(_hMenu), TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON, _position.x() + windowRect.left, _position.y() + windowRect.top + titleHeight, hwnd, nullptr);
	}
}
	
void Win32PopUpMenu::ProcessCommand(MenuItemIdentifier item) {
	if (s_menuClosure) {
		s_menuClosure(item);
	}
}

void Win32PopUpMenu::setCheck(bool check) const
{
	if (check)
	{
		CheckMenuItem(bg::native_cast<HMENU>(_parent->hMenu()), (UINT)_idx, MF_BYPOSITION | MF_CHECKED);

	}
	else
	{
		CheckMenuItem(bg::native_cast<HMENU>(_parent->hMenu()), (UINT)_idx, MF_BYPOSITION | MF_UNCHECKED);
	}
}

bool Win32PopUpMenu::isChecked() const
{
	DWORD fdwMenu;
	fdwMenu = GetMenuState(bg::native_cast<HMENU>(_parent->hMenu()), (UINT)_idx, MF_BYPOSITION);
	if (!(fdwMenu & MF_CHECKED))
	{
		return false;
	}
	else
	{
		return true;
	}
}

int Win32PopUpMenu::changeCheckState() const
{
	if (!this->isChecked())
	{
		setCheck(true);
		return 1;
	}
	else
	{
		setCheck(false);
		return 0;
	}
}

void Win32PopUpMenu::setEnabled(bool enable) const
{
	if (enable)
	{
		EnableMenuItem(bg::native_cast<HMENU>(_parent->hMenu()), (UINT)_idx, MF_BYPOSITION | MF_ENABLED);

	}
	else
	{
		EnableMenuItem(bg::native_cast<HMENU>(_parent->hMenu()), (UINT)_idx, MF_BYPOSITION | MF_GRAYED);
	}
}

bool Win32PopUpMenu::isEnabled() const
{
	DWORD fdwMenu;
	fdwMenu = GetMenuState(bg::native_cast<HMENU>(_parent->hMenu()), (UINT)_idx, MF_BYPOSITION);
	if (!(fdwMenu & MF_GRAYED))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Win32PopUpMenu::changeEnabledState() const
{
	if (!this->isEnabled())
	{
		setEnabled(true);
		return 1;
	}
	else
	{
		setEnabled(false);
		return 0;
	}
}

void Win32PopUpMenu::initSubMenus() const
{
	this->eachSubMenu([&](const bg::wnd::PopUpMenu * _subMenu, auto index)
	{

		if (_subMenu->identifier() == -1)
		{
			HMENU hSubMenu = CreateMenu();
			_subMenu->setHMenu(bg::plain_ptr(hSubMenu));
			_subMenu->initSubMenus();
			AppendMenuA(bg::native_cast<HMENU>(_hMenu), _subMenu->flags(), (UINT_PTR)hSubMenu, _subMenu->title().c_str());
		}
		else
		{
			AppendMenuA(bg::native_cast<HMENU>(_hMenu), _subMenu->flags(), _subMenu->identifier(), _subMenu->title().c_str());
		}
	});
	/*menu->eachMenuItem([&](const bg::wnd::PopUpMenuItem & item, auto index) {
		std::string title = item.title.c_str();

		if (item.shortcut.valid()) {
			title += "";
			auto maxSeparation = 30;
			auto separation = maxSeparation - title.length();
			for (auto i = 0; i < separation; ++i) {
				title += " ";
			}
			title += " ";
			if (item.shortcut.modifierMask & bg::base::Keyboard::kCtrlKey) {
				title += "Ctrl+";
			}
			if (item.shortcut.modifierMask & bg::base::Keyboard::kShiftKey) {
				title += "Shift+";
			}
			if (item.shortcut.modifierMask & bg::base::Keyboard::kAltKey) {
				title += "Alt+";
			}
			title += static_cast<char>(item.shortcut.keyCode);
			_shortcutItems.push_back(item);
		}
		AppendMenuA(hmenu, MF_STRING , item.identifier, title.c_str());
	});*/
}



#endif
	
}
}