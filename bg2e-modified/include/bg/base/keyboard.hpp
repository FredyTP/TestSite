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


#ifndef _bg2e_base_keyboard_hpp_
#define _bg2e_base_keyboard_hpp_

#include <iostream>

#include <bg/system/system.hpp>

namespace bg {
namespace base {

class Keyboard {
public:
	enum KeyModifier {
		kCommandOrControlKey = 1 << 0,
		kAltKey = 1 << 1,
		kCtrlKey = 1 << 2,
		kShiftKey = 1 << 3,
		kCommandKey = 1 << 4
	};

	enum KeyCode {
		kKeyNull = '\0',
		kKeyA = 'A', kKeyB, kKeyC, kKeyD, kKeyE, kKeyF, kKeyG, kKeyH, kKeyI, kKeyJ, kKeyK, kKeyL, kKeyM, kKeyN, kKeyO, kKeyP, kKeyQ, kKeyR, kKeyS, kKeyT, kKeyU, kKeyV, kKeyW, kKeyX, kKeyY, kKeyZ,
		kKey0 = '0', kKey1, kKey2, kKey3, kKey4, kKey5, kKey6, kKey7, kKey8, kKey9,
		kKeyAdd = '+',
		kKeySub = '-',
		kKeyEsc = 27,
		kKeySpace = ' ',
		kKeyDel = 127,
		kKeyAvPag = 200,
		kKeyRePag,
		kKeyStart,
		kKeyEnd,
		kKeyLeft,
		kKeyUp,
		kKeyRight,
		kKeyDown,
		kKeyBack,
		kKeyTab = '\t',
		kKeyReturn = 13
	};
	Keyboard() :_keyMask(0) {}

	inline void clearModifierMask() { _keyMask = 0; }
	inline void setModifierMask(unsigned int mask) { _keyMask = mask; }
	inline void setModifier(KeyModifier mod) { _keyMask = _keyMask | mod; }
	inline void unsetModifier(KeyModifier mod) { _keyMask = _keyMask & ~mod; }
	inline unsigned int getKeyMask() const { return _keyMask; }
	inline bool getModifierStatus(KeyModifier mod) const { return (_keyMask & mod)!=0; }

	inline void setMultibyteCharacter(const std::string & c) { _mbChar = c; }
	inline void setCharacter(char character) { _char = character; }
	inline void setKey(KeyCode key) { _key = key; }
	inline const std::string & multibyteCharacter() const { return _mbChar; }
	inline char character() const { return _char;  }
	inline KeyCode key() const { return _key; }

protected:
	char _char;
	std::string _mbChar;
	KeyCode _key;
	unsigned int _keyMask;

};

struct KeyboardShortcut {
	uint32_t modifierMask;
	Keyboard::KeyCode keyCode;

	KeyboardShortcut() :modifierMask(0), keyCode(Keyboard::kKeyNull) {}
	KeyboardShortcut(uint32_t mod, Keyboard::KeyCode code) :modifierMask(mod), keyCode(code) {
		if (modifierMask & Keyboard::kCommandOrControlKey) {
			modifierMask = modifierMask & (Keyboard::kAltKey | Keyboard::kShiftKey);
			if (bg::system::currentPlatform() == bg::system::kMac) {
				modifierMask |= Keyboard::kCommandKey;
			}
			else {
				modifierMask |= Keyboard::kCtrlKey;
			}
		}
	}

	inline void operator =(const KeyboardShortcut & sc) { modifierMask = sc.modifierMask; keyCode = sc.keyCode; }
	inline bool operator==(const KeyboardShortcut & sc) const {
		return modifierMask == sc.modifierMask && keyCode == sc.keyCode;
	}
	inline bool empty() const { return keyCode==Keyboard::kKeyNull || modifierMask==0; }
	inline bool valid() const { return keyCode!=Keyboard::kKeyNull && modifierMask!=0; }
};

}
}

#endif
