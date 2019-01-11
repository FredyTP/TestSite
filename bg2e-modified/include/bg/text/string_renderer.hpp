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

#ifndef _bg2e_text_string_renderer_hpp_
#define _bg2e_text_string_renderer_hpp_

#include <bg/base/context_object.hpp>
#include <bg/base/pipeline.hpp>

#include <bg/text/font.hpp>
#include <bg/text/paragraph.hpp>

namespace bg {
namespace text {

class BG2E_EXPORT StringRenderer : public bg::base::ContextObject,
								   public bg::base::ReferencedPointer {
public:
	StringRenderer(bg::base::Context * ctx);
	StringRenderer(Font * font, bg::base::Context * ctx);

	inline void setFont(Font * font) { _font = font; }
	inline Font * font() { return _font.getPtr(); }
	inline const Font * font() const { return _font.getPtr(); }

	inline void setScale(float s) { _scale = s; }
	inline float scale() const { return _scale; }
	
	inline void setParagraph(const Paragraph & p) { _paragraph = p; }
	inline const Paragraph & paragraph() const { return _paragraph; }
	inline Paragraph & paragraph() { return _paragraph; }
	
	void draw(bg::base::Pipeline *, const std::string &, int64_t caretPosition = -1);

	void getStringMetrics(const std::string &, bg::math::Vector2 & total, std::vector<bg::math::Vector2> & lines, bool ignoreScale = false) const;
	void getStringMetrics(const std::string &, bg::math::Vector2 & total, bool ignoreScale = false) const;
	void getCaretMetrics(const std::string & text, int64_t caretPosition, bg::math::Vector2 & position, bool ignoreScale = false) const;
    void insertLineBreaks(const std::string & inputString, std::string & outputString, float rectWidth);
    inline void insertLineBreaks(const std::string & inputString, std::string & outputString, int rectWidth) {
        insertLineBreaks(inputString, outputString, static_cast<float>(rectWidth));
    }

protected:
	virtual ~StringRenderer();

	bg::ptr<Font> _font;
	float _scale;
	Paragraph _paragraph;

	void applyLineMetrics(const bg::math::Vector2 & metrics, bg::base::MatrixState & matrixState);
};

}
}

#endif

