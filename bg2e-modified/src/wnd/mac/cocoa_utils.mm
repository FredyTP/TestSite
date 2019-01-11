
#include <bg/wnd/cocoa_utils.h>

#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

bg::base::Keyboard::KeyCode translateCode(char character, unsigned short code) {
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
	else if (character=='+') {
		keyCode = bg::base::Keyboard::kKeyAdd;
	}
	else if (character=='-') {
		keyCode = bg::base::Keyboard::kKeySub;
	}
	else if (code == kVK_Escape) {
		keyCode = bg::base::Keyboard::kKeyEsc;
	}
	else if (code == kVK_ForwardDelete) {
		keyCode = bg::base::Keyboard::kKeyDel;
	}
	else if (code == kVK_Tab) {
		keyCode = bg::base::Keyboard::kKeyTab;
	}
	else if (code == kVK_Space) {
		keyCode = bg::base::Keyboard::kKeySpace;
	}
	else if (code == kVK_Delete) {
		keyCode = bg::base::Keyboard::kKeyBack;
	}
	else if (code == kVK_LeftArrow) {
		keyCode = bg::base::Keyboard::kKeyLeft;
	}
	else if (code == kVK_RightArrow) {
		keyCode = bg::base::Keyboard::kKeyRight;
	}
	else if (code == kVK_UpArrow) {
		keyCode = bg::base::Keyboard::kKeyUp;
	}
	else if (code == kVK_DownArrow) {
		keyCode = bg::base::Keyboard::kKeyDown;
	}
	else if (code == kVK_Return) {
		keyCode = bg::base::Keyboard::kKeyReturn;
	}
	
	return keyCode;
}

unsigned int getModifiers(NSUInteger flags) {
	unsigned int modifiers = 0;
	if ((flags & NSControlKeyMask)!=0) {
		modifiers = modifiers | bg::base::Keyboard::kCtrlKey | bg::base::Keyboard::kCommandOrControlKey;
	}
	if ((flags & NSShiftKeyMask)!=0) {
		modifiers = modifiers | bg::base::Keyboard::kShiftKey;
	}
	if ((flags & NSAlternateKeyMask)!=0) {
		modifiers = modifiers | bg::base::Keyboard::kAltKey;
	}
	if ((flags & NSCommandKeyMask)!=0) {
		modifiers = modifiers | bg::base::Keyboard::kCommandKey | bg::base::Keyboard::kCommandOrControlKey;
	}
	
	return modifiers;
}

void fillKeyboard(bg::base::Keyboard & kb, NSString * characters, unsigned short code, NSUInteger flags) {
    if (characters.length>0) {
        char character = [characters characterAtIndex:0];
        std::string mbChar = [characters UTF8String];
        kb.setMultibyteCharacter(mbChar);
        kb.setCharacter(character);
        kb.setKey(translateCode(character,code));
        kb.setModifierMask(getModifiers(flags));
    }
}

void fillMouseEvent(bg::base::MouseEvent & mouseEvent, bg::wnd::MainLoop * mainLoop, NSPoint point) {
	mouseEvent.setPos(bg::math::Vector2i(point.x, point.y));
	mouseEvent.mouse().setButtonMask(mainLoop->getMouse().buttonMask());
	mouseEvent.setDelta(bg::math::Vector2());
}

