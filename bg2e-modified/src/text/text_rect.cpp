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


#include <bg/text/text_rect.hpp>
#include <bg/tools/poly_list_primitives.hpp>
#include <bg/fx/diffuse_texture.hpp>

namespace bg {
namespace text {

TextRect::TextRect(bg::base::Context * ctx, Font * font)
	:ContextObject(ctx)
	,_dirty(true)
{
	build(bg::math::Vector2(1.0f), font);
}

TextRect::TextRect(bg::base::Context * ctx, const bg::math::Vector2 & size, Font * font)
	:ContextObject(ctx)
	,_dirty(true)
{
	build(size, font);
}

TextRect::~TextRect() {
	
}

void TextRect::frame(float) {
	if (_dirty) {
		update();
	}
}

void TextRect::draw(bg::base::Pipeline * pipeline) {
	if (!pipeline->effect()) return;
	if (_dirty) {
		update();
	}

	bg::ptr<bg::base::Material> m = pipeline->effect()->materialPtr();
	pipeline->effect()->setMaterial(_mat.getPtr());
	pipeline->draw(_rect.getPtr());
	pipeline->effect()->setMaterial(m.getPtr());
}

void TextRect::build(const bg::math::Vector2 & size, Font * font) {
	using namespace bg::tools;
	_textureFrame = new bg::tools::TextureFrame(context());
	_textureFrame->onDraw([&](bg::base::Pipeline * pipeline) {
		pipeline->clearBuffers();
		_stringRenderer->draw(pipeline, _text);
	});
	
	_textureFrame->onResize([&](bg::base::Pipeline * pipeline, const bg::math::Size2Di & size) {
		pipeline->setViewport(bg::math::Viewport(0,0,size.width(),size.height()));
		pipeline->effect()->matrixState().projectionMatrixStack()
			.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);
	});
	
	bg::fx::DiffuseTexture * effect = new bg::fx::DiffuseTexture(context());
	effect->setUseAlpha(false);
	_textureFrame->pipeline()->setEffect(effect);
	_textureFrame->pipeline()->setClearColor(bg::math::Color::Transparent());
	
	_rect = PolyListPrimitives(context()).plane(size.width(), size.height(), PolyListPrimitives::kAlignZPlane);
	
	_stringRenderer = new StringRenderer(font, context());
	
	_mat = new bg::base::Material();
	_mat->setTexture(_textureFrame->texture());
}

void TextRect::update() {
	_textureFrame->update();
	_dirty = false;
}

}
}
