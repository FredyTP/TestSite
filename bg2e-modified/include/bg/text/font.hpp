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


#ifndef _BG2E_TEXT_FONT_HPP_
#define _BG2E_TEXT_FONT_HPP_

#include <bg/text/glyph.hpp>
#include <bg/system/utf.hpp>

namespace bg {
namespace text {

class BG2E_EXPORT Font : public bg::base::ReferencedPointer {
public:

	enum FontType {
		kTypeSerif		= 0x1 << 0,
		kTypeSansSerif	= 0x1 << 1,
		kTypeMonospace	= 0x1 << 2
	};
	
	Font(const std::string & fontName, float size = 1.0f);
	
	inline const std::string name() const { return _name; }
	inline float size() const { return _size; }
	
	inline Glyph * getGlyph(bg::system::UTFCode c) { return _glyphMap[c].getPtr(); }
	inline void addGlyph(bg::system::UTFCode c, Glyph * g) { _glyphMap[c] = g; }
	
	inline void setSpaceSize(float size) { _spaceSize = size; }
	inline float spaceSize() const { return _spaceSize; }
	
	inline void setLineHeight(float lh) { _lineHeight = lh; }
	inline float lineHeight() const { return _lineHeight; }
	inline float scaledLineHeight() const { return _lineHeight * _textScale; }
	
	inline void setMaterial(bg::base::Material * mat) { _material = mat; applyMaterial(); }
	inline const bg::base::Material * material() const { return _material.getPtr(); }
	
	inline float textScale() const { return _textScale; }
	inline void setTextScale(float ts) { _textScale = ts; }
	
	// Clone the font properties and all the glyph textures
	Font * clone();

	// Clone the font properties, but keep the same list of glyph textures
	Font * instantiate();

protected:
	virtual ~Font();
	
	std::string _name;
	float _size;
	float _spaceSize;
	float _lineHeight;
	float _textScale;
	
	GlyphMap _glyphMap;
	bg::ptr<bg::base::Material> _material;
	
	void applyMaterial();
};


}
}

#endif
