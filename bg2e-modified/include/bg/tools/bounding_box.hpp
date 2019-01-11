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


#ifndef _bg2e_tools_bounding_box_hpp_
#define _bg2e_tools_bounding_box_hpp_

#include <bg/export.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/scene/drawable.hpp>
#include <bg/math/matrix.hpp>
#include <bg/math/vector.hpp>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif 

namespace bg {
namespace tools {
	

class BG2E_EXPORT BoundingBox {
public:
	BoundingBox();
	BoundingBox(bg::base::PolyList *, const bg::math::Matrix4 & trx = bg::math::Matrix4::Identity());
	BoundingBox(bg::scene::Drawable *, const bg::math::Matrix4 & trx = bg::math::Matrix4::Identity());
	
	void clear();
	
	const bg::math::Vector3 & min();
	const bg::math::Vector3 & max();
	const bg::math::Vector3 & center();
	const bg::math::Vector3 & size();
	const bg::math::Vector3 & halfSize();
	
	void addPolyList(bg::base::PolyList * polyList, const bg::math::Matrix4 & trx = bg::math::Matrix4::Identity());
	void addDrawable(bg::scene::Drawable * drawable, const bg::math::Matrix4 & trx = bg::math::Matrix4::Identity());

protected:
	bg::math::Vector3 _min;
	bg::math::Vector3 _max;
	bg::math::Vector3 _center;
	bg::math::Vector3 _size;
	bg::math::Vector3 _halfSize;
	bool _dirty;
	
	void update();
};

}
}

#endif
