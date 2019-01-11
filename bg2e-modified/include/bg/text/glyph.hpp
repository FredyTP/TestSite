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


#ifndef _BG2E_TEXT_GLYPH_HPP_
#define _BG2E_TEXT_GLYPH_HPP_

#include <bg/base/referenced_pointer.hpp>
#include <bg/base/image.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/base/material.hpp>
#include <bg/base/pipeline.hpp>
#include <bg/math/vector.hpp>
#include <bg/system/utf.hpp>


#include <unordered_map>

namespace bg {
namespace text {
	
class BG2E_EXPORT Glyph : public bg::base::ReferencedPointer {
public:
	Glyph();

	virtual Glyph * clone() = 0;
	
	inline const std::string & character() const { return _character; }
	inline void setCharacter(const std::string & c) { _character = c; }
	inline void setCharacter(char c) { _character = c; }
	
	inline const bg::math::Vector2 & bearing() const { return _bearing; }
	inline void setBearing(const bg::math::Vector2 & b) { _bearing = b; }
	inline bg::math::Vector2 scaledBearing() const {
		return bg::math::Vector2(_bearing.x() * sk_Scale,
								 _bearing.y() * sk_Scale);
	}
	
	inline const bg::math::Bounds2 & bounds() const { return _bounds; }
	inline bg::math::Bounds2 & bounds() { return _bounds; }
	inline void setBounds(const bg::math::Bounds2 & b) { _bounds = b; }
	inline bg::math::Bounds2 scaledBounds() const {
		return bg::math::Bounds2(_bounds.left() * sk_Scale,
								 _bounds.right() * sk_Scale,
								 _bounds.bottom() * sk_Scale,
								 _bounds.top() * sk_Scale);
	}
	inline float scaledBottom() const { return _bounds.bottom() * sk_Scale; }
	inline float scaledLeft() const { return _bounds.left() * sk_Scale; }
	
	inline float advance() const { return _advance; }
	inline void setAdvance(float a) { _advance = a; }
	inline float scaledAdvance() const { return _advance * sk_Scale; }
	
	inline void setMaterial(bg::base::Material * mat) { _material = mat; }
	inline bg::base::Material * material() { return _material.getPtr(); }
	
	inline float width() const { return _bounds.width(); }
	inline float height() const { return _bounds.height(); }
	inline float scaledWidth() const { return _bounds.width() * sk_Scale; }
	inline float scaledHeight() const { return _bounds.height() * sk_Scale; }
	inline bg::math::Vector2 size() const { return bg::math::Vector2(_bounds.width(), _bounds.height()); }
	inline bg::math::Vector2 scaledSize() const { return bg::math::Vector2(_bounds.width() * sk_Scale,
																		   _bounds.height() * sk_Scale); }
	
	inline float scale() const { return sk_Scale; }
	static float Scale() { return sk_Scale; }
	
	inline void operator=(const Glyph & g) {
		_character = g._character;
		_bearing = g._bearing;
		_advance = g._advance;
		_bounds = g._bounds;
	}
	
	virtual void draw(bg::base::Pipeline *) = 0;

protected:
	virtual ~Glyph();

	bg::math::Bounds2 _bounds;
	bg::math::Vector2 _bearing;
	float _advance;
	std::string _character;
	bg::ptr<bg::base::Material> _material;
	
	static const float sk_Scale;
};

class BG2E_EXPORT BitmapGlyph : public Glyph {
public:
	BitmapGlyph();

	virtual Glyph * clone();
	
	inline bg::base::Image * bitmap() { return _bitmap.getPtr(); }
	inline const bg::base::Image * bitmap() const { return _bitmap.getPtr(); }
	inline void setBitmap(bg::base::Image * img) { _bitmap = img; }

	virtual void draw(bg::base::Pipeline *);
	
protected:
	virtual ~BitmapGlyph();
	
	bg::ptr<bg::base::Image> _bitmap;

	static bg::base::PolyList * getQuad(bg::base::Context *);

	static bg::base::ContextObjectRegistry<bg::base::PolyList> s_quad;
};


class BG2E_EXPORT PolyListGlyph : public Glyph {
public:
	PolyListGlyph();
	
	virtual Glyph * clone();

	inline bg::base::PolyList * polyList() { return _plist.getPtr(); }
	inline const bg::base::PolyList * polyList() const { return _plist.getPtr(); }
	inline void setPolyList(bg::base::PolyList * plist) { _plist = plist; }
	
	virtual void draw(bg::base::Pipeline *);
	
protected:
	virtual ~PolyListGlyph();

	bg::ptr<bg::base::PolyList> _plist;
};

typedef std::unordered_map<bg::system::UTFCode, bg::ptr<Glyph>> GlyphMap;



}
}


#endif
