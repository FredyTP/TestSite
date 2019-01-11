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


#include <bg/text/font.hpp>
#include <bg/tools/material_tools.hpp>

namespace bg {
namespace text {

	
Font::Font(const std::string & fontName, float size)
	:_name(fontName)
	,_size(size)
	,_spaceSize(0.0f)
	,_lineHeight(0.0f)
	,_textScale(1.0f)
	,_material(new bg::base::Material())
{
	
}

Font::~Font() {
	
}

void Font::applyMaterial() {
	using namespace bg::base;
	for (auto g : _glyphMap) {
		Glyph * glyph = g.second.getPtr();
		bg::tools::MaterialTools mat(glyph->material());
		mat.copyPropertiesOf(_material.getPtr(),Material::kAllAttributes & ~Material::kTexture);
	}
}

// Clone the font properties and all the glyph textures
Font * Font::clone() {
	bg::ptr<Font> copy = new Font(_name,_size);
	copy->_spaceSize = _spaceSize;
	copy->_lineHeight = _lineHeight;
	copy->_textScale = _textScale;
	bg::tools::MaterialTools matTools(_material.getPtr());
	matTools.clone();
	copy->_material = matTools.target();
	for (auto & g : _glyphMap) {
		copy->_glyphMap[g.first] = g.second->clone();
	}
	return copy.release();
}

// Clone the font properties, but keep the same list of glyph textures
Font * Font::instantiate() {
	bg::ptr<Font> copy = new Font(_name, _size);
	copy->_spaceSize = _spaceSize;
	copy->_lineHeight = _lineHeight;
	copy->_textScale = _textScale;
	bg::tools::MaterialTools matTools(_material.getPtr());
	matTools.clone();
	copy->_material = matTools.target();
	for(auto & g : _glyphMap) {
		copy->_glyphMap[g.first] = g.second.getPtr();
	}
	return copy.release();
}

}
}
