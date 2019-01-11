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

#include <bg/gui/text_input.hpp>
#include <bg/gui/surface.hpp>
#include <bg/gui/window.hpp>

#include <bg/system/utf.hpp>

namespace bg {
namespace gui {

TextInput::TextInput(bg::base::Context * ctx, Skin * s)
	:View(ctx, s)
{
	using namespace bg::base;
	_label = View::Create<Label>(ctx, s);
	addChild(_label.getPtr());

	KeyClosure caretToStart = [&](const KeyboardEvent &) {
		_caretPos = 0;
	};

	KeyClosure caretToEnd = [&](const KeyboardEvent &) {
		_caretPos = bg::system::utf::stringLenght(text());
	};

	KeyClosure caretLeft = [&](const KeyboardEvent &) {
		if (_caretPos > 0) {
			_caretPos--;
		}
	};

	KeyClosure caretRight = [&](const KeyboardEvent &) {
		if (_caretPos < bg::system::utf::stringLenght(text())) {
			_caretPos++;
		}
	};

	KeyClosure escape = [&](const KeyboardEvent &) {
		onEditFinished();
		if (parentSurface()) {
			parentSurface()->clearFocus();
		}
	};

	KeyClosure del = [&](const KeyboardEvent &) {
		if (_caretPos<bg::system::utf::stringLenght(text())) {
			std::string newString;
			using namespace bg::system;
			size_t currentChar = 0;
			utf::iterateString(text(), [&](std::string & mb, UTFCode, UTFCode) {
				if (currentChar != _caretPos) {
					newString += mb;
				}
				++currentChar;
			});
			if (_validator.valid()) {
				_validator->checkEditValue(newString);
			}
			_label->setText(newString);
			onValueChanged();
		}
	};

	_keyClosures[Keyboard::kKeyRePag] = caretToStart;
	_keyClosures[Keyboard::kKeyDown] = caretToStart;
	_keyClosures[Keyboard::kKeyStart] = caretToStart;
	_keyClosures[Keyboard::kKeyAvPag] = caretToEnd;
	_keyClosures[Keyboard::kKeyUp] = caretToEnd;
	_keyClosures[Keyboard::kKeyEnd] = caretToEnd;
	_keyClosures[Keyboard::kKeyLeft] = caretLeft;
	_keyClosures[Keyboard::kKeyRight] = caretRight;
	_keyClosures[Keyboard::kKeyEsc] = escape;
}

TextInput::~TextInput() {
}

void TextInput::updateContent(bg::base::Pipeline * pipeline) {
	bg::math::Size2Di labelSize = size() - bg::math::Size2Di(_skinProperties[_status].padding * 2);
    _label->applySkinProperties(skin());
    _label->setTextColor(status(),_skinProperties[status()].textColor);
	if(_label->size() != labelSize) {
		_label->setSize(labelSize);
		_label->setPosition(bg::math::Position2Di(_skinProperties[_status].padding));
	}
	pipeline->setClearColor(_skinProperties[_status].backgroundColor);
	pipeline->clearBuffers();
}

void TextInput::keyUp(const bg::base::KeyboardEvent & evt) {
	if (_keyClosures.find(evt.keyboard().key()) != _keyClosures.end()) {
		_keyClosures[evt.keyboard().key()](evt);
		setContentChanged(true);
		_label->setContentChanged(true);
		_label->setVisibleCaretAtPosition(_caretPos);
	}
}

void TextInput::charPress(const bg::base::KeyboardEvent & evt) {
	bool insertChar = true;
	bool changeText = false;
	std::string newString;
	size_t strLength = bg::system::utf::stringLenght(text());
	switch (evt.keyboard().key()) {
	case bg::base::Keyboard::kKeyTab:
	case bg::base::Keyboard::kKeyAdd:
	case bg::base::Keyboard::kKeySub:
	case bg::base::Keyboard::kKeySpace:
		break;
	case bg::base::Keyboard::kKeyReturn:
		insertChar = false;
		if (_validator.valid()) {
			std::string finalValue = text();
			_validator->checkFinalValue(finalValue);
			setText(finalValue);
		}
		onEditFinished();
		if (parentSurface()) {
			parentSurface()->clearFocus();
		}
		break;
	case bg::base::Keyboard::kKeyBack:
		if (_caretPos>0 && _caretPos<=strLength) {
			using namespace bg::system;
			--_caretPos;
			changeText = true;
			size_t currentChar = 0;
			utf::iterateString(text(), [&](std::string & mb, UTFCode, UTFCode) {
				if (currentChar!=_caretPos) {
					newString += mb;
				}
				++currentChar;
			});
		}
		insertChar = false;
		break;
	default:
		break;
	}
	if (insertChar) {
		using namespace bg::system;
		changeText = true;
		int currentChar = 0;
		if (_caretPos==strLength) {
			newString = text() + evt.keyboard().character();
		}
		else {
			utf::iterateString(text(), [&](std::string & mb, UTFCode codepoint, UTFCode hex) {
				if(_caretPos == currentChar) {
					newString += evt.keyboard().character();
				}
				newString += mb;
				++currentChar;
			});
		}
		++_caretPos;
	}
	if (changeText) {
		if (_validator.valid()) {
			changeText = _validator->checkEditValue(newString);
		}
		if (changeText) {
			_label->setText(newString);
			_label->setVisibleCaretAtPosition(_caretPos);
			setContentChanged(true);
			onValueChanged();
		}
	}
}

void TextInput::mouseUp(const bg::base::MouseEvent & evt) {
	if (parentSurface()) {
		parentSurface()->setFocus(this);
		_caretPos = bg::system::utf::stringLenght(text());
		_label->setVisibleCaretAtPosition(_caretPos);
		setContentChanged(true);
		_label->setContentChanged(true);
	}
}

void TextInput::focusSet() {
	_label->setVisibleCaretAtPosition(_caretPos);
	setContentChanged(true);
	_label->setContentChanged(true);
}

void TextInput::focusLost() {
	if (_validator.valid()) {
		std::string newText = text();
		_validator->checkFinalValue(newText);
		_label->setText(newText);
	}
	_label->hideCaret();
	setContentChanged(true);
	_label->setContentChanged(true);
	onEditFinished();
}

}
}
