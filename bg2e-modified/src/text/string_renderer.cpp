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

#include <bg/text/string_renderer.hpp>

#include <bg/system/utf.hpp>

namespace bg {
namespace text {

StringRenderer::StringRenderer(bg::base::Context * ctx)
	:ContextObject(ctx)
	,_scale(1.0f)
{
}

StringRenderer::StringRenderer(Font * font, bg::base::Context * ctx)
	:ContextObject(ctx)
	,_font(font)
	,_scale(1.0f)
{
}

StringRenderer::~StringRenderer() {
}

void StringRenderer::draw(bg::base::Pipeline * pipeline, const std::string & text, int64_t caretPosition) {
	if (_font.isNull() || !pipeline->effect() || text.empty()) return;
	std::vector<bg::math::Vector2> lineMetrics;
	bg::math::Vector2 paragraphMetrics;
	getStringMetrics(text,paragraphMetrics,lineMetrics,true);

	pipeline->effect()->matrixState()
		.modelMatrixStack()
		.push()
		.scale(bg::math::Vector3(_font->textScale() * _scale));

	if (_paragraph.verticalAlignment()==Paragraph::kVAlignCenter) {
		pipeline->effect()->matrixState()
			.modelMatrixStack()
			.translate(0.0f, paragraphMetrics.height() / 2.0f, 0.0f);
	}
	else if (_paragraph.verticalAlignment()==Paragraph::kVAlignBottom) {
		pipeline->effect()->matrixState()
			.modelMatrixStack()
			.translate(0.0f, paragraphMetrics.height(), 0.0f);
	}
	
	if(_paragraph.horizontalAlignment() == Paragraph::kHAlignLeft) {
		pipeline->effect()->matrixState()
			.modelMatrixStack()
			.translate(-paragraphMetrics.width() / 2.0f, 0.0f, 0.0f);
	}
	else if(_paragraph.horizontalAlignment() == Paragraph::kHAlignRight) {
		pipeline->effect()->matrixState()
			.modelMatrixStack()
			.translate(paragraphMetrics.width() / 2.0f, 0.0f, 0.0f);
	}
	
	int lines = 0;
	float lineHeight = _font->lineHeight();
	bg::math::Matrix4 initialMatrix = pipeline->effect()->matrixState().modelMatrixStack().matrix();
	bg::math::Vector2 & currentMetrics = lineMetrics[lines];
	applyLineMetrics(currentMetrics, pipeline->effect()->matrixState());
	size_t currentChar = 0;
	bg::system::utf::iterateString(text, [&](std::string & mb, bg::system::UTFCode code, bg::system::UTFCode hex) {
		bg::ptr<bg::text::Glyph> glyph;
		if (caretPosition>-1 &&
			static_cast<size_t>(caretPosition)==currentChar &&
			(glyph = _font->getGlyph(0x7C)).valid()	// 0x7C => vertical slash |
		) {
			glyph->draw(pipeline);
		}
		if (bg::system::utf::isControlCode(hex)) {
			switch (hex) {
			case bg::system::utf::kCodeTab:
				if ((glyph = _font->getGlyph(bg::system::utf::kCodeSpace)).valid()) {
					pipeline->effect()->matrixState()
						.modelMatrixStack()
						.translate(glyph->scaledAdvance() *
									_paragraph.charSpacing() *
									static_cast<float>(_paragraph.tabSize()),
								   0.0f,
								   0.0f);
				}
				break;
			case bg::system::utf::kCodeReturn:
			case bg::system::utf::kCodeNewLine: {
				++lines;
				float vSpace = static_cast<float>(lines) * lineHeight * _paragraph.lineSpacing();
				pipeline->effect()->matrixState()
					.modelMatrixStack()
					.set(initialMatrix)
					.translate(bg::math::Vector3(0.0f,
												 -vSpace,
												 0.0f));
				currentMetrics = lineMetrics[lines];
				applyLineMetrics(currentMetrics, pipeline->effect()->matrixState());
				break;
			}
			}
		}
		else if ((glyph = _font->getGlyph(code)).valid()) {
			glyph->draw(pipeline);
			pipeline->effect()->matrixState()
				.modelMatrixStack()
				.translate(glyph->scaledAdvance() * _paragraph.charSpacing(), 0.0f, 0.0f);
		}
		++currentChar;
	});

	// Render caret if is at the end of the string
	bg::ptr<bg::text::Glyph> glyph;
	if(caretPosition==bg::system::utf::stringLenght(text) &&
		(glyph = _font->getGlyph(0x7C)).valid()	// 0x7C => vertical slash |
		) {
		glyph->draw(pipeline);
	}
	pipeline->effect()->matrixState()
		.modelMatrixStack().pop();
}

void StringRenderer::getStringMetrics(const std::string & text, bg::math::Vector2 & total, std::vector<bg::math::Vector2> & lines, bool ignoreScale) const {
	if (_font.isNull()) return;
	float lineWidth = 0.0f;
	float totalHeight = 0.0f;
	float maxWidth = 0.0f;
	float lineHeight = _font->lineHeight() * _paragraph.lineSpacing();
	bg::system::utf::iterateString(text, [&](std::string & mb, bg::system::UTFCode code, bg::system::UTFCode hex) {
		bg::ptr<bg::text::Glyph> glyph;
		if(bg::system::utf::isControlCode(hex)) {
			switch(hex) {
			case bg::system::utf::kCodeTab:
				if((glyph = _font->getGlyph(bg::system::utf::kCodeSpace)).valid()) {
					lineWidth += glyph->scaledAdvance() * _paragraph.charSpacing() * static_cast<float>(_paragraph.tabSize());
				}
				break;
			case bg::system::utf::kCodeReturn:
			case bg::system::utf::kCodeNewLine: {
				lines.push_back(bg::math::Vector2(lineWidth,lineHeight));
				lineWidth = 0.0f;
				break;
			}
			}
		}
		else if((glyph = _font->getGlyph(code)).valid()) {
			lineWidth += glyph->scaledAdvance() * _paragraph.charSpacing();
		}
		maxWidth = lineWidth>maxWidth ? lineWidth : maxWidth;
	});
	if (lineWidth>0.0f) {
		lines.push_back(bg::math::Vector2(lineWidth,lineHeight));
	}

	if (lines.size()==1) {
		totalHeight = lineHeight;	// This will remove the line spacing if the paragraph have only one line
	}
	else {
		totalHeight = static_cast<float>(lines.size()) * lineHeight;
	}
	if (ignoreScale) {
		total.set(maxWidth, totalHeight);
	}
	else {
		total.set(maxWidth * _font->textScale(), totalHeight * _font->textScale());
	}
}

void StringRenderer::getStringMetrics(const std::string & text, bg::math::Vector2 & total, bool ignoreScale) const {
	std::vector<bg::math::Vector2> foo;
	getStringMetrics(text, total, foo, ignoreScale);
}

void StringRenderer::getCaretMetrics(const std::string & text, int64_t caretPosition, bg::math::Vector2 & position, bool ignoreScale) const {
	if (_font.isNull()) return;
	if (caretPosition == -1) {
		position.set(0, 0);
		return;
	}

	float lineHeight = _font->lineHeight() * _paragraph.lineSpacing();
	float caretX = 0.0f;
	float caretY = 0.0f;
	int currentCaretPos = 0;
	bg::system::utf::checkEvery(text, [&](std::string & mb, bg::system::UTFCode code, bg::system::UTFCode hex) -> bool {
		bg::ptr<bg::text::Glyph> glyph;
		if (bg::system::utf::isControlCode(hex)) {
			switch (hex) {
			case bg::system::utf::kCodeTab:
				if ((glyph = _font->getGlyph(bg::system::utf::kCodeSpace)).valid()) {
					caretX += glyph->scaledAdvance() * _paragraph.charSpacing() * static_cast<float>(_paragraph.tabSize());
				}
				break;
			case bg::system::utf::kCodeReturn:
			case bg::system::utf::kCodeNewLine: {
				caretX = 0.0f;
				caretY += lineHeight;
				break;
			}
			}
		}
		else if ((glyph = _font->getGlyph(code)).valid()) {
			caretX += glyph->scaledAdvance() * _paragraph.charSpacing();
		}
		currentCaretPos++;
		return currentCaretPos < caretPosition;
	});

	if (ignoreScale) {
		position.set(caretX, caretY);
	}
	else {
		position.set(caretX * _font->textScale(),caretY * _font->textScale());
	}
}
    
void StringRenderer::insertLineBreaks(const std::string & inputString, std::string & outputString, float rectWidth) {
    outputString = "";
    float currentWidth = 0.0f;
    std::string currentWord = "";
    
    bg::system::utf::iterateString(inputString, [&](std::string & mb, bg::system::UTFCode code, bg::system::UTFCode hex) {
        bg::ptr<bg::text::Glyph> glyph;
        if(bg::system::utf::isControlCode(hex)) {
            switch(hex) {
                case bg::system::utf::kCodeTab:
                    outputString += currentWord + "\t";
                    currentWord = "";
                    currentWidth += glyph->scaledAdvance() * _paragraph.charSpacing() * static_cast<float>(_paragraph.tabSize());
                    break;
                case bg::system::utf::kCodeReturn:
                case bg::system::utf::kCodeNewLine: {
                    outputString += currentWord + "\n";
                    currentWord = "";
                    currentWidth = 0.0f;
                    break;
                }
            }
        }
        else if((glyph = _font->getGlyph(code)).valid()) {
            if (mb==" ") {
                outputString += currentWord + " ";
                currentWord = "";
                currentWidth += glyph->scaledAdvance() * _paragraph.charSpacing();
                
                if (currentWidth>rectWidth) {
                    outputString += "\n" + currentWord;
                    currentWord = "";
                    currentWidth = 0.0f;
                }
            }
            else {
                currentWidth += glyph->scaledAdvance() * _paragraph.charSpacing();
                currentWord += mb;
            }
        }
    });
    
    if (!currentWord.empty() && currentWidth>rectWidth) {
        outputString += "\n" + currentWord;
    }
    else if (!currentWord.empty()) {
        outputString += currentWord;
    }
}

void StringRenderer::applyLineMetrics(const bg::math::Vector2 & metrics, bg::base::MatrixState & matrixState) {
	if(_paragraph.horizontalAlignment() == Paragraph::kHAlignCenter) {
		matrixState
			.modelMatrixStack()
			.translate(-metrics.x() / 2.0f, 0.0f, 0.0f);
	}
	else if(_paragraph.horizontalAlignment() == Paragraph::kHAlignRight) {
		matrixState
			.modelMatrixStack()
			.translate(-metrics.x(), 0.0f, 0.0f);
	}
}

}
}
