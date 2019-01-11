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

#ifndef _bg2e_tools_poly_list_primitives_hpp_
#define _bg2e_tools_poly_list_primitives_hpp_

#include <bg/base/poly_list.hpp>
#include <bg/base/context_object.hpp>

namespace bg {
namespace tools {
	
class BG2E_EXPORT PolyListPrimitives : public bg::base::ContextObject {
public:
	enum Alignment {
		kAlignXPlane,
		kAlignYPlane,
		kAlignZPlane
	};
	
	PolyListPrimitives(bg::base::Context *);
	
	bg::base::PolyList * cube(float width, float height, float depth);
	
	bg::base::PolyList * plane(float width, float depth, Alignment align = kAlignYPlane, const bg::math::Vector2 & offset = bg::math::Vector2(0.0f));
	
	bg::base::PolyList * sphere(float radius, float slices, float stacks);
	
};

}
}

#endif
