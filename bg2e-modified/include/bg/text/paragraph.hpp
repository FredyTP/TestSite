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


#ifndef _bg2e_text_paragraph_hpp_
#define _bg2e_text_paragraph_hpp_

namespace bg {
namespace text {

class Paragraph {
public:
	enum HorizontalAlignment {
		kHAlignLeft		= 1 << 0,
		kHAlignCenter	= 1 << 1,
		kHAlignRight		= 1 << 2
	};

	enum VerticalAlignment {
		kVAlignTop		= 1 << 0,
		kVAlignCenter	= 1 << 1,
		kVAlignBottom	= 1 << 2
	};

	inline float lineSpacing() const { return _lineSpacing; }
	inline float charSpacing() const { return _charSpacing; }
	inline int tabSize() const { return _tabSize; }
	inline HorizontalAlignment horizontalAlignment() const { return _horzAlign; }
	inline VerticalAlignment verticalAlignment() const { return _vertAlign; }
	
	inline void setLineSpacing(float ls) { _lineSpacing = ls; }
	inline void setCharSpacing(float cs) { _charSpacing = cs; }
	inline void setTabSize(int s) { _tabSize = s; }
	inline void setAlignment(HorizontalAlignment a) { _horzAlign = a; }
	inline void setAlignment(VerticalAlignment a) { _vertAlign = a; }

	inline bool operator==(const Paragraph & other) {
		return	_lineSpacing==other._lineSpacing &&
				_charSpacing==other._charSpacing &&
				_tabSize==other._tabSize &&
				_vertAlign==other._vertAlign &&
				_horzAlign==other._horzAlign;
	}
	
	inline void operator=(const Paragraph & other) {
		_lineSpacing = other._lineSpacing;
		_charSpacing = other._charSpacing;
		_tabSize = other._tabSize;
		_vertAlign = other._vertAlign;
		_horzAlign = other._horzAlign;
	}
	
protected:
	float _lineSpacing = 1.5f;
	float _charSpacing = 1.0f;
	int _tabSize = 4;
	VerticalAlignment  _vertAlign = kVAlignTop;
	HorizontalAlignment _horzAlign = kHAlignLeft;
};
	
}
}


#endif
