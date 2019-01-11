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

#include <bg/gui/label.hpp>
#include <bg/tools/poly_list_primitives.hpp>
#include <bg/tools/texture_cache.hpp>
#include <bg/gui/window.hpp>
#include <bg/gui/surface.hpp>
#include <bg/fx/gui_rect.hpp>

namespace bg {
namespace gui {

static const float k_textScale = 250.0f;

Label::Label(bg::base::Context * ctx, Skin * s)
	:View(ctx, s)
	,_text("")
    ,_autoLineBreak(false)
    ,_scrollX(0)
    ,_scrollY(0)
	,_textScale(1.0f)
{
	_renderer = new bg::text::StringRenderer(ctx);
	setBackgroundColor(bg::math::Color::Transparent());
}

Label::~Label() {
}

void Label::updateContent(bg::base::Pipeline * pipeline) {
	if (!setupFont()) return;
	float s = 1.0f;
	if(parentWindow() && parentWindow()->parentSurface()) {
		setContentChanged(parentWindow()->parentSurface()->scaleChanged());
		s = parentWindow()->parentSurface()->scale();
	}
	_renderer->setScale(s * _textScale);

	if (isCaretVisible()) {
		adjustScrollToCaret();
	}
	else {
		_scrollX = 0;
		_scrollY = 0;
	}

	bg::ptr<bg::base::Material> mat = pipeline->effect()->materialPtr();
	bg::math::Vector2 textSize;
	std::vector<bg::math::Vector2> lineSize;
	float lineHeight = _renderer->font()->scaledLineHeight() * k_textScale * s;
    std::string textWithBreaks = "";
    if (_autoLineBreak) {
        _renderer->insertLineBreaks(_text, textWithBreaks, static_cast<float>(size().width()) / k_textScale);
    }
    else {
        textWithBreaks = _text;
    }

    _renderer->getStringMetrics(textWithBreaks, textSize, lineSize);
	textSize.scale(k_textScale * s);
	bg::math::Vector2 translate;

	switch (_renderer->paragraph().horizontalAlignment()) {
	case bg::text::Paragraph::kHAlignLeft:
		translate.x(static_cast<float>(textSize.width() / 2));
		break;
	case bg::text::Paragraph::kHAlignCenter:
		translate.x(static_cast<float>(_size.width() / 2) * s);
		break;
	case bg::text::Paragraph::kHAlignRight:
		translate.x(static_cast<float>(_size.width()) * s - static_cast<float>(textSize.width() / 2));
		break;
	}

	switch (_renderer->paragraph().verticalAlignment()) {
	case bg::text::Paragraph::kVAlignTop:
		translate.y(static_cast<float>(_size.height()) * s - lineHeight);
		break;
	case bg::text::Paragraph::kVAlignCenter:
		if (lineSize.size()==1) {
			translate.y(static_cast<float>(_size.height() / 2) * s - lineHeight);
		}
		else if (lineSize.size()>1) {
			translate.y(static_cast<float>(_size.height() / 2 - textSize.height() / 2) + lineHeight / 2.0f);
		}
		break;
	case bg::text::Paragraph::kVAlignBottom:
		translate.y(-lineHeight);
		break;
	}

	bg::fx::GuiRect * guiRect = dynamic_cast<bg::fx::GuiRect*>(pipeline->effect());

	if (guiRect) {
		guiRect->setFrameSize(_textureFrame->size());
		guiRect->setBorderColor(_skinProperties[_status].borderColor);
		guiRect->setBorderWidth(0);
	}

    // If we _caretPosition!=-1, the user is editting the text. In this case,
    // update the _scrollX and scrollY values to make the caret visible
	pipeline->effect()->setMaterial(_mat.getPtr());
    _mat->setDiffuse(_skinProperties[status()].textColor);
	pipeline->setClearColor(_skinProperties[_status].backgroundColor);
	pipeline->clearBuffers();
	pipeline->effect()->matrixState().modelMatrixStack()
		.push()
		.translate(translate.x() - _scrollX, translate.y() + _scrollY, 0.0f)
		.scale(k_textScale);
	_renderer->draw(pipeline,textWithBreaks,_caretPosition);
	pipeline->effect()->matrixState().modelMatrixStack()
		.pop();
	pipeline->effect()->setMaterial(mat.getPtr());
}

void Label::adjustScrollToCaret() {
	float s = 1.0f;
	_scrollX = 0;
	_scrollY = 0;
	if (parentWindow() && parentWindow()->parentSurface()) {
		setContentChanged(parentWindow()->parentSurface()->scaleChanged());
		s = parentWindow()->parentSurface()->scale();
	}
	float w = static_cast<float>(size().width()) * s;
	//float h = static_cast<float>(size().height()) * s;
	bg::math::Vector2 pos;
	_renderer->getCaretMetrics(_text, _caretPosition, pos);
	pos.scale(k_textScale * s);
	if (pos.width() > w) {
		_scrollX = static_cast<int>(floor(pos.width() / w) * w);
		_scrollX = _scrollX<20 ? 0 : _scrollX - 20;
	}
}

bool Label::setupFont() {
	if (_font.valid()) {
		_renderer->setFont(_font.getPtr());
	}
	else if (parentWindow() && parentWindow()->parentSurface()) {
		_renderer->setFont(parentWindow()->parentSurface()->skinFont());
	}
	return _renderer->font()!=nullptr;
}

ClickableLabel::ClickableLabel(bg::base::Context * ctx, Skin * s)
	:Label(ctx, s)
{

}

ClickableLabel::~ClickableLabel() {

}

void ClickableLabel::mouseUp(const bg::base::MouseEvent & evt) {
	if (_action) _action();
}



}
}
