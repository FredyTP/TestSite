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


#include <bg/scene/primitive_factory.hpp>

#include <bg/base/poly_list.hpp>

#include <bg/math/utils.hpp>

#include <bg/tools/poly_list_primitives.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace scene {

PrimitiveFactory::PrimitiveFactory(bg::base::Context * ctx)
	:_ctx(ctx)
{
	
}

Drawable * PrimitiveFactory::cube(float width, float height, float depth, const std::string & name) {
	using namespace bg::base;
	ptr<Drawable> drw = new Drawable(name);
	PolyList * plist = bg::tools::PolyListPrimitives(_ctx.getPtr()).cube(width, height, depth);
	drw->addPolyList(plist);
	return drw.release();
}

Drawable * PrimitiveFactory::plane(float width, float depth, const std::string & name) {
	using namespace bg::base;
	ptr<Drawable> drw = new Drawable(name);
	PolyList * plist = bg::tools::PolyListPrimitives(_ctx.getPtr()).plane(width, depth, bg::tools::PolyListPrimitives::kAlignYPlane);
	drw->addPolyList(plist);
	return drw.release();
}

Drawable * PrimitiveFactory::sphere(float radius, float slc, float stacks, const std::string & name) {
	using namespace bg::base;
	ptr<Drawable> drw = new Drawable(name);
	PolyList * plist = bg::tools::PolyListPrimitives(_ctx.getPtr()).sphere(radius, slc, stacks);
	drw->addPolyList(plist);
	return drw.release();
}

}
}
