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


#include <bg/text/glyph.hpp>
#include <bg/scene/drawable.hpp>
#include <bg/scene/camera.hpp>

#include <bg/engine.hpp>

#include <bg/tools/material_tools.hpp>

namespace bg {
namespace text {

const float Glyph::sk_Scale = 1.0f / 1000.0f;

Glyph::Glyph()
	:_advance(0)
{
	
}

Glyph::~Glyph() {
	
}

bg::base::PolyList * BitmapGlyph::getQuad(bg::base::Context * ctx) {
	bg::ptr<bg::base::PolyList> plist;
	if (s_quad.find(ctx)) {
		plist = s_quad.get(ctx);
	}
	else {
		plist = new bg::base::PolyList(ctx);
		std::vector<float> vertex {
			 0.0f,  0.0f, 0.0f,
			 1.0f,  0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			 0.0f,  1.0f, 0.0f
		};

		std::vector<float> normal {
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f
		};

		std::vector<float> texCoord {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};

		std::vector<unsigned int> index {
			0, 1, 2,
			2, 3, 0
		};

		plist->addVertexVector(&vertex[0],vertex.size());
		plist->addNormalVector(&normal[0],normal.size());
		plist->addTexCoord0Vector(&texCoord[0],texCoord.size());
		plist->addIndexVector(&index[0],index.size());
		
		plist->build();

		s_quad.set(plist.getPtr());

		bg::Engine::AddDestroyCallback([&]() {
			s_quad.clear();
		});
	}
	return plist.getPtr();
}

bg::base::ContextObjectRegistry<bg::base::PolyList> BitmapGlyph::s_quad;

BitmapGlyph::BitmapGlyph()
{
	
}

BitmapGlyph::~BitmapGlyph() {
	
}

Glyph * BitmapGlyph::clone() {
	bg::ptr<BitmapGlyph> g = new BitmapGlyph();
	g->_bounds = _bounds;
	g->_bearing = _bearing;
	g->_advance = _advance;
	g->_character = _character;

	bg::tools::MaterialTools mat(_material.getPtr());
	g->_material = mat.clone().target();
	g->_bitmap = _bitmap.getPtr();
	
	return g.release();
}

void BitmapGlyph::draw(bg::base::Pipeline * pl) {
	using namespace bg::base;
	PolyList * quad = getQuad(pl->context());
	if (pl->shouldDraw(material())) {
		bg::ptr<bg::base::Material> m = pl->effect()->materialPtr();
		bg::math::Matrix4 s = bg::math::Matrix4::Identity();
		float b = scaledBottom();
		s.translate(scaledLeft(), b, 0.0f)
			.scale(scaledWidth(), scaledHeight(), 1.0f);
		pl->effect()->matrixState().modelMatrixStack().push();
		pl->effect()->matrixState().modelMatrixStack().mult(s);
		pl->effect()->setMaterial(material());
		pl->draw(quad);
		pl->effect()->setMaterial(m.getPtr());
		pl->effect()->matrixState().modelMatrixStack().pop();
	}
}

PolyListGlyph::PolyListGlyph()
{
	
}

PolyListGlyph::~PolyListGlyph() {
	
}

Glyph * PolyListGlyph::clone() {
	bg::ptr<PolyListGlyph> g = new PolyListGlyph();
	g->_bounds = _bounds;
	g->_bearing = _bearing;
	g->_advance = _advance;
	g->_character = _character;

	bg::tools::MaterialTools mat(_material.getPtr());
	g->_material = mat.clone().target();

	return g.release();
}

void PolyListGlyph::draw(bg::base::Pipeline * pl) {
	using namespace bg;
	using namespace bg::scene;
	
	if (_plist.valid() && pl->shouldDraw(material())) {
		bg::ptr<bg::base::Material> m = pl->effect()->materialPtr();
		pl->effect()->setMaterial(material());
		pl->draw(_plist.getPtr());
		pl->effect()->setMaterial(m.getPtr());
	}
}

}
}
