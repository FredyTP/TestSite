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

#ifndef _bg2e_gui_text_input_hpp_
#define _bg2e_gui_text_input_hpp_

#include <bg/gui/label.hpp>
#include <bg/gui/form_field.hpp>
#include <bg/base/exception.hpp>
#include <regex>

namespace bg {
namespace gui {

class TextValidator : public bg::base::ReferencedPointer {
public:
	TextValidator() {}

	void setInitialValue(const std::string & text) {
		if (!isValid(text)) {
			throw bg::base::InvalidParameterException("Text validation exception: Initial values is not a valid value.");
		}
		_lastValidValue = text;
		_lastEditValue = text;
	}

	bool checkFinalValue(std::string & text) {
		bool result = isValid(text);
		if (result) {
			_lastEditValue = text;
			_lastValidValue = text;
		}
		else {
			text = _lastValidValue;
		}
		return result;
	}

	bool checkEditValue(std::string & text) {
		bool result = seemsWillBeValid(text);
		if (result) {
			_lastEditValue = text;
		}
		else {
			text = _lastEditValue;
		}
		return result;
	}

	virtual bool isValid(const std::string & text) = 0;

	// Override this function to test a string that may be valid, but currently it isn't,
	// for example: 0. for a floating point number
	virtual bool seemsWillBeValid(const std::string & text) { return isValid(text); }

protected:
	virtual ~TextValidator() {}

	std::string _lastValidValue;
	std::string _lastEditValue;
};

class RegExTextValidator : public TextValidator {
public:
	RegExTextValidator(std::regex re) :_re(re), _seemsRe(re) {}
	RegExTextValidator(std::regex re, std::regex seemsRe) :_re(re), _seemsRe(seemsRe) {}

	virtual bool isValid(const std::string & text) {
		return static_cast<bool>(std::regex_match(text, _re));
	}

	virtual bool seemsWillBeValid(const std::string & text) {
		return static_cast<bool>(std::regex_match(text, _seemsRe));
	}

public:
	virtual ~RegExTextValidator() {}

	std::regex _re;
	std::regex _seemsRe;
};

class NumericTextValidator : public RegExTextValidator {
public:
	NumericTextValidator() :RegExTextValidator(std::regex("-?\\d+\\.?\\d+"), std::regex("-?\\d*\\.?\\d*")) {}

protected:
	virtual ~NumericTextValidator() {}
};

class BG2E_EXPORT TextInput : public View,
							  public FormField {
public:
	typedef std::function<void(const bg::base::KeyboardEvent &)> KeyClosure;

	TextInput(bg::base::Context *, Skin * s);

	inline void setValidator(TextValidator * v) { _validator = v; }
	inline const TextValidator * validator() const { return _validator.getPtr(); }
	inline TextValidator * validator() { return _validator.getPtr(); }

	inline const std::string & text() const { return _label->text(); }
	inline void setText(const std::string & s) {
		std::string validationString = s;
		if (_validator.valid() && !_validator->checkFinalValue(validationString)) {
			return;
		}
		_label->setText(s); setContentChanged(true); onValueChanged();
	}

	virtual void keyUp(const bg::base::KeyboardEvent & evt);
	virtual void charPress(const bg::base::KeyboardEvent & evt);
	virtual void mouseUp(const bg::base::MouseEvent & evt);

	virtual void focusSet();
	virtual void focusLost();

	inline void setKeyAction(bg::base::Keyboard::KeyCode code, KeyClosure closure) { _keyClosures[code] = closure; }

protected:

	virtual ~TextInput();

	bg::ptr<Label> _label;
	size_t _caretPos = 0;
	bg::ptr<TextValidator> _validator;

	virtual void updateContent(bg::base::Pipeline *);
	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kTextInput; }

	std::map<bg::base::Keyboard::KeyCode, KeyClosure> _keyClosures;
};

}
}

#endif
