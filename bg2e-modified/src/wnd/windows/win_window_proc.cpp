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


#include <bg/wnd/win_window_proc.hpp>

#include <bg/wnd/main_loop.hpp>
#include <bg/wnd/window_controller.hpp>
#include <bg/wnd/win32_window.hpp>
#include <bg/wnd/win32_popup_menu.hpp>
#include <bg/system/energy_saver.hpp>

#include <bg/system/utf.hpp>

#include <iostream>
#include <iomanip>

#include <bg/wnd/popup_menu.hpp>

#pragma comment(lib,"Shcore.lib")

bg::base::Keyboard::KeyCode code(unsigned char character) {
	bg::base::Keyboard::KeyCode keyCode = bg::base::Keyboard::kKeyNull;

	if (character >= 'a' && character <= 'z') {
		keyCode = static_cast<bg::base::Keyboard::KeyCode>(bg::base::Keyboard::kKeyA + (character - 'a'));
	}
	else if (character >= 'A' && character <= 'Z') {
		keyCode = static_cast<bg::base::Keyboard::KeyCode>(bg::base::Keyboard::kKeyA + (character - 'A'));
	}
	else if (character >= '0' && character <= '9') {
		keyCode = static_cast<bg::base::Keyboard::KeyCode>(bg::base::Keyboard::kKey0 + (character - '0'));
	}
	else if (character == VK_ESCAPE) {
		keyCode = bg::base::Keyboard::kKeyEsc;
	}
	else if (character == VK_DELETE) {
		keyCode = bg::base::Keyboard::kKeyDel;
	}
	else if (character == VK_TAB) {
		keyCode = bg::base::Keyboard::kKeyTab;
	}
	else if (character == VK_SPACE) {
		keyCode = bg::base::Keyboard::kKeySpace;
	}
	else if (character == VK_BACK) {
		keyCode = bg::base::Keyboard::kKeyBack;
	}
	else if (character == VK_LEFT) {
		keyCode = bg::base::Keyboard::kKeyLeft;
	}
	else if (character == VK_RIGHT) {
		keyCode = bg::base::Keyboard::kKeyRight;
	}
	else if (character == VK_UP) {
		keyCode = bg::base::Keyboard::kKeyUp;
	}
	else if (character == VK_DOWN) {
		keyCode = bg::base::Keyboard::kKeyDown;
	}
	else if (character == VK_RETURN) {
		keyCode = bg::base::Keyboard::kKeyReturn;
	}
	else if (character == 187) {
		keyCode = bg::base::Keyboard::kKeyAdd;
	}
	else if (character == 189) {
		keyCode = bg::base::Keyboard::kKeySub;
	}

	return keyCode;
}

unsigned int getModifiers(bool altPressed) {
	unsigned int modifiers = 0;
	if (GetKeyState(VK_CONTROL) & 0x8000) {
		modifiers = modifiers | bg::base::Keyboard::kCtrlKey | bg::base::Keyboard::kCommandOrControlKey;
	}
	if (GetKeyState(VK_SHIFT) & 0x8000) {
		modifiers = modifiers | bg::base::Keyboard::kShiftKey;
	}
	if (altPressed) {
		modifiers = modifiers | bg::base::Keyboard::kAltKey;
	}

	return modifiers;
}

void fillKeyboard(bg::base::Keyboard & kb, unsigned long character, bool altPressed) {
	std::string mb = bg::system::utf::multibyteFromCodepoint(character);
	kb.setMultibyteCharacter(mb);
	kb.setCharacter(static_cast<char>(character));
	kb.setKey(code(static_cast<unsigned char>(character)));
	kb.setModifierMask(getModifiers(altPressed));
}

void fillMouseEvent(bg::base::MouseEvent & mouseEvent, bg::wnd::MainLoop * mainLoop) {
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	bg::math::Vector2i pos;
	bg::math::Rect winRect = mainLoop->window()->rect();
	pos.x(cursorPos.x - winRect.x());
	pos.y(cursorPos.y - winRect.y());
	mouseEvent.setPos(pos);
	mouseEvent.mouse().setButtonMask(mainLoop->getMouse().buttonMask());
	mouseEvent.setDelta(bg::math::Vector2());
}
//MENU THINGS
bool findMenuItem(const bg::wnd::PopUpMenu * menu, bg::wnd::MenuItemIdentifier cmd)
{
	if (menu == nullptr) return false;

	int found = false;
	menu->eachSubMenu([&](const bg::wnd::PopUpMenu * _subMenu, auto index) {
		if (_subMenu->identifier() == -1)
		{
			found = findMenuItem(_subMenu, cmd);
		}
		else
		{
			if (_subMenu->identifier() == cmd)
			{
				bg::wnd::WindowController * controller = bg::wnd::MainLoop::Get()->window()->windowController();
				if (controller != nullptr)
					controller->eventHandler()->menuSelected(_subMenu->title(), _subMenu->identifier());
				found = true;
			}
		}
		
	});

	return found;
	return false;
}
LRESULT CALLBACK vwgl_app_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
	bg::wnd::Window * window = mainLoop->window();
	bg::wnd::WindowController * controller = nullptr;
	bg::base::KeyboardEvent kbEvent;
	bg::base::MouseEvent mouseEvent;

	static bool altPressed = false;
	if (window && (controller = window->windowController())) {
		switch (uMsg) {
		case WM_DPICHANGED:
			std::cout << "DPI changed" << std::endl;
			break;
		case WM_ACTIVATE:
			break;
		case WM_SYSCOMMAND:
			if ((wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER) &&
				bg::system::EnergySaver::Get().screenIdleTimerDisabled())
			{
				return 0;
			}
			break;
		case WM_COMMAND: {
			//MENU THINGS
			   bg::wnd::MenuItemIdentifier cmd = static_cast<bg::wnd::MenuItemIdentifier>(LOWORD(wParam));
			  
			   bool found = false;
			   if (controller->eventHandler()) {
					  found = findMenuItem(window->menu(), cmd);
			   }
			   if (!found) {
				   bg::wnd::Win32PopUpMenu::ProcessCommand(cmd);
			   }
			}
			break;
		case WM_CLOSE:
			if (bg::wnd::MainLoop::Get()->notifyQuit()) {
				bg::wnd::MainLoop::Get()->onQuit(nullptr);
				PostQuitMessage(0);
			}
			else {
				return 0;
			}
			break;
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN: {
			if (wParam == VK_MENU) {
				altPressed = true;
			}
			fillKeyboard(kbEvent.keyboard(), static_cast<unsigned char>(wParam), altPressed);

			bool commandSent = false;
			for (auto & item : dynamic_cast<bg::wnd::Win32Window*>(window)->shortcutItems()) {
				uint32_t modMask = kbEvent.keyboard().getKeyMask();
				modMask = modMask & ~bg::base::Keyboard::kCommandOrControlKey;
				if (item.shortcut.keyCode == kbEvent.keyboard().key() &&
					item.shortcut.modifierMask == modMask)
				{
					controller->eventHandler()->menuSelected(item.title, item.identifier);
					commandSent = true;
					break;
				}
			}
			if (!commandSent) {
				controller->keyDown(kbEvent);
			}

			break;
		}
		case WM_CHAR:
			fillKeyboard(kbEvent.keyboard(), static_cast<unsigned long>(wParam), false);
			controller->charPress(kbEvent);
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			if (wParam == VK_MENU) {
				altPressed = false;
			}
			fillKeyboard(kbEvent.keyboard(), static_cast<unsigned char>(wParam), altPressed);
			controller->keyUp(kbEvent);

			break;
		}
		case WM_LBUTTONDOWN:
			mainLoop->mouse().setMouseDown(bg::base::Mouse::kLeftButton);
			fillKeyboard(mouseEvent.keyboard(), '\0', altPressed);
			fillMouseEvent(mouseEvent, mainLoop);
			controller->mouseDown(mouseEvent);
			break;
		case WM_LBUTTONUP:
			mainLoop->mouse().setMouseUp(bg::base::Mouse::kLeftButton);
			fillKeyboard(mouseEvent.keyboard(), '\0', altPressed);
			mouseEvent.mouse().setReleasedButton(bg::base::Mouse::kLeftButton);
			fillMouseEvent(mouseEvent, mainLoop);
			controller->mouseUp(mouseEvent);
			break;
		case WM_RBUTTONDOWN:
			mainLoop->mouse().setMouseDown(bg::base::Mouse::kRightButton);
			fillKeyboard(mouseEvent.keyboard(), '\0', altPressed);
			fillMouseEvent(mouseEvent, mainLoop);
			controller->mouseDown(mouseEvent);
			break;
		case WM_RBUTTONUP:
			mainLoop->mouse().setMouseUp(bg::base::Mouse::kRightButton);
			fillKeyboard(mouseEvent.keyboard(), '\0', altPressed);
			mouseEvent.mouse().setReleasedButton(bg::base::Mouse::kRightButton);
			fillMouseEvent(mouseEvent, mainLoop);
			controller->mouseUp(mouseEvent);
			break;
		case WM_MBUTTONDOWN:
			mainLoop->mouse().setMouseDown(bg::base::Mouse::kMiddleButton);
			fillKeyboard(mouseEvent.keyboard(), '\0', altPressed);
			fillMouseEvent(mouseEvent, mainLoop);
			controller->mouseDown(mouseEvent);
			break;
		case WM_MBUTTONUP:
			mainLoop->mouse().setMouseUp(bg::base::Mouse::kMiddleButton);
			fillKeyboard(mouseEvent.keyboard(), '\0', altPressed);
			mouseEvent.mouse().setReleasedButton(bg::base::Mouse::kMiddleButton);
			fillMouseEvent(mouseEvent, mainLoop);
			controller->mouseUp(mouseEvent);
			break;
		case WM_MOUSEMOVE:
			fillKeyboard(mouseEvent.keyboard(), '\0', altPressed);
			fillMouseEvent(mouseEvent, mainLoop);
			if (mouseEvent.mouse().anyButtonPressed()) {
				controller->mouseDrag(mouseEvent);
			}			
			controller->mouseMove(mouseEvent);
			break;
		case WM_MOUSEWHEEL:
			fillKeyboard(mouseEvent.keyboard(), '\0', altPressed);
			fillMouseEvent(mouseEvent, mainLoop);
			if ((short)HIWORD(wParam) > 0) {
				mouseEvent.setDelta(bg::math::Vector2(0.0f, -1.0f));
			}
			else {
				mouseEvent.setDelta(bg::math::Vector2(0.0f, 1.0f));
			}
			controller->mouseWheel(mouseEvent);
			break;
		case WM_MOUSELEAVE:
			if (mainLoop->mouse().getButtonStatus(bg::base::Mouse::kLeftButton)) {
				mainLoop->mouse().setMouseUp(bg::base::Mouse::kLeftButton);
				fillKeyboard(mouseEvent.keyboard(), '\0', altPressed);
				fillMouseEvent(mouseEvent, mainLoop);
				controller->mouseUp(mouseEvent);
			}

			if (mainLoop->mouse().getButtonStatus(bg::base::Mouse::kRightButton)) {
				mainLoop->mouse().setMouseUp(bg::base::Mouse::kRightButton);
				fillKeyboard(mouseEvent.keyboard(), '\0', altPressed);
				fillMouseEvent(mouseEvent, mainLoop);
				controller->mouseUp(mouseEvent);
			}

			if (mainLoop->mouse().getButtonStatus(bg::base::Mouse::kMiddleButton)) {
				mainLoop->mouse().setMouseUp(bg::base::Mouse::kMiddleButton);
				fillKeyboard(mouseEvent.keyboard(), '\0', altPressed);
				fillMouseEvent(mouseEvent, mainLoop);
				controller->mouseUp(mouseEvent);
			}
			break;
		case WM_SIZE:
			{
				int w = static_cast<int>(LOWORD(lParam));
				int h = static_cast<int>(HIWORD(lParam));
				window->setSize(w, h);
				controller->windowRectChanged(
					window->rect().x(),
					window->rect().y(),
					window->rect().width(),
					window->rect().height()
				);
				controller->reshape(w, h);
			}
			break;
		case WM_MOVE:
			{
				int x = static_cast<int>(LOWORD(lParam));
				int y = static_cast<int>(HIWORD(lParam));
				window->setPosition(x, y);
				controller->windowRectChanged(
					window->rect().x(),
					window->rect().y(),
					window->rect().width(),
					window->rect().height()
				);
			}
			break;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
