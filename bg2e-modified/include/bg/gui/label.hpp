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

#ifndef _bg2e_gui_label_hpp_
#define _bg2e_gui_label_hpp_

#include <bg/gui/view.hpp>
#include <bg/text/string_renderer.hpp>

#include <bg/log.hpp>

namespace bg {
namespace gui {

class BG2E_EXPORT Label : public View {
public:
	Label(bg::base::Context *, Skin * s);

	inline void setText(const std::string & t) { setContentChanged(true); _text = t; }
	inline const std::string & text() const { return _text; }

	inline void setFont(bg::text::Font * font) { setContentChanged(true); _font = font; }
	inline const bg::text::Font * font() const { return _font.getPtr(); }
	inline bg::text::Font * font() { setContentChanged(true); return _font.getPtr(); }

	inline bg::text::Paragraph & paragraph() { setContentChanged(true); return _renderer->paragraph(); }
	inline const bg::text::Paragraph & paragraph() const { return _renderer->paragraph(); }
	inline void setParagraph(const bg::text::Paragraph & p) { setContentChanged(true); _renderer->setParagraph(p); }

    inline void setAutoLineBreak(bool alb) { setContentChanged(true); _autoLineBreak = alb; }
    inline bool autoLineBreak() const { return _autoLineBreak; }
    
    inline void setScrollX(int s) { setContentChanged(true); _scrollX = s; }
    inline void setScrollY(int s) { setContentChanged(true); _scrollY = s; }
    inline int scrollX() const { return _scrollX; }
    inline int scrollY() const { return _scrollY; }
    
	virtual void mouseUp(const bg::base::MouseEvent & evt) {}

	inline void setVisibleCaretAtPosition(int64_t pos) { _caretPosition = pos; }
	inline int64_t caretPosition() const { return _caretPosition; }
	inline void hideCaret() { _caretPosition = -1; }
	inline bool isCaretVisible() const { return _caretPosition != -1; }
	void adjustScrollToCaret();

	inline void setTextScale(float s) { _textScale = s; }
	inline float textScale() const { return _textScale; }
	
	inline const bg::text::StringRenderer & stringRenderer() const { return *_renderer.getPtr();  }

protected:
	virtual ~Label();

	std::string _text;
	bg::ptr<bg::text::StringRenderer> _renderer;
	bg::ptr<bg::text::Font> _font;
    bool _autoLineBreak;
    int _scrollX;
    int _scrollY;
	float _textScale;

	int64_t _caretPosition = -1;

	virtual void updateContent(bg::base::Pipeline *);
	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kLabel; }

	bool setupFont();
};

class BG2E_EXPORT ClickableLabel : public Label {
public:
	ClickableLabel(bg::base::Context * ctx, Skin * s);

	virtual void mouseUp(const bg::base::MouseEvent & evt);

	void setAction(std::function<void()> action) { _action = action; }

protected:
	virtual ~ClickableLabel();

	std::function<void()> _action;
};
}
}

#endif
