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


#ifndef _bg2e_text_text_rect_hpp_
#define _bg2e_text_text_rect_hpp_

#include <bg/text/string_renderer.hpp>
#include <bg/tools/texture_frame.hpp>
#include <bg/base/pipeline.hpp>

namespace bg {
namespace text {

class BG2E_EXPORT TextRect :	public bg::base::ContextObject,
								public bg::base::ReferencedPointer {
public:
	TextRect(bg::base::Context *, Font * font = nullptr);
	TextRect(bg::base::Context *, const bg::math::Vector2 & size, Font * font = nullptr);

	void setTextureSize(const bg::math::Size2Di & size) { _textureFrame->setSize(size); }
	void frame(float);
	void draw(bg::base::Pipeline *);

	inline void setText(const std::string & str) { _dirty = true; _text = str; }
	inline const std::string & text() const { return _text; }
	
	inline Font * font() { _dirty = true; return _stringRenderer->font(); }
	inline const Font * font() const { return _stringRenderer->font(); }
	inline void setFont(Font * font) { _dirty = true; _stringRenderer->setFont(font); }
	
	inline Paragraph & paragraph() { _dirty = true; return _stringRenderer->paragraph(); }
	inline const Paragraph & paragraph() const { return _stringRenderer->paragraph(); }
	inline void setParagraph(const Paragraph & p) { _dirty = true; _stringRenderer->setParagraph(p); }
	
	inline bg::base::Material * material() { return _mat.getPtr(); }
	inline const bg::base::Material * material() const { return _mat.getPtr(); }

protected:
	virtual ~TextRect();
	
	void build(const bg::math::Vector2 & size, Font * font);

	std::string _text;
	bg::ptr<StringRenderer> _stringRenderer;
	bg::ptr<bg::tools::TextureFrame> _textureFrame;
	bg::ptr<bg::base::PolyList> _rect;
	bg::ptr<bg::base::Material> _mat;
	bool _dirty;
	
	void update();
};

}
}


#endif
