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


#ifndef _bg2e_scene_primitive_factory_hpp_
#define _bg2e_scene_primitive_factory_hpp_

#include <bg/math/utils.hpp>
#include <bg/base/context.hpp>
#include <bg/scene/drawable.hpp>

namespace bg {
namespace scene {

class BG2E_EXPORT PrimitiveFactory {
public:
	PrimitiveFactory(bg::base::Context *);
	
	inline Drawable * cube(const std::string & name = "") { return cube(1.0f, name); }
	inline Drawable * cube(float size, const std::string & name = "") { return cube(size, size, size, name); }
	inline Drawable * cube(const bg::math::Vector3 & size, const std::string & name ="") { return cube(size.x(), size.y(), size.z(), name); }
	inline Drawable * cube(const bg::math::Scalar & size, const std::string & name = "") { return cube(size.value(), size.value(), size.value(), name); }
	inline Drawable * cube(const bg::math::Scalar & width, const bg::math::Scalar & height, const bg::math::Scalar & depth, const std::string & name = "")
		{ return cube(width.value(), height.value(), depth.value(), name); }
	inline Drawable * cube(const bg::math::Scalar & width, const bg::math::Scalar & height, float depth, const std::string & name = "")
		{ return cube(width.value(), height.value(), depth, name); }
	inline Drawable * cube(const bg::math::Scalar & width, float height, const bg::math::Scalar & depth, const std::string & name = "")
		{ return cube(width.value(), height, depth.value(), name); }
	inline Drawable * cube(const bg::math::Scalar & width, float height, float depth, const std::string & name = "")
		{ return cube(width.value(), height, depth, name); }
	inline Drawable * cube(float width, const bg::math::Scalar & height, const bg::math::Scalar & depth, const std::string & name = "")
		{ return cube(width, height.value(), depth.value(), name); }
	inline Drawable * cube(float width, const bg::math::Scalar & height, float depth, const std::string & name = "")
		{ return cube(width, height.value(), depth, name); }
	inline Drawable * cube(float width, float height, const bg::math::Scalar & depth, const std::string & name = "")
		{ return cube(width, height, depth.value(), name); }
	Drawable * cube(float width, float height, float depth, const std::string & name = "");
	
	
	inline Drawable * plane(const std::string & name = "") { return plane(1.0f, name); }
	inline Drawable * plane(float size, const std::string & name = "") { return plane(size, size, name); }
	inline Drawable * plane(const bg::math::Scalar & size, const std::string & name = "") { return plane(size.value(), size.value(), name); }
	inline Drawable * plane(const bg::math::Scalar & width, const bg::math::Scalar & depth, const std::string & name = "")
		{ return plane(width.value(), depth.value(), name); }
	inline Drawable * plane(const bg::math::Scalar & width, float depth, const std::string & name = "")
		{ return plane(width.value(), depth, name); }
	inline Drawable * plane(float width, const bg::math::Scalar & depth, const std::string & name = "")
		{ return plane(width, depth.value(), name); }
	Drawable * plane(float width, float depth, const std::string & name = "");
	
	
	inline Drawable * sphere(const std::string & name = "") { return sphere(1.0f, name); }
	inline Drawable * sphere(float radius, const std::string & name = "") { return sphere(radius, 20, name); }
	inline Drawable * sphere(float radius, float subdivisions, const std::string & name = "") { return sphere(radius, subdivisions, subdivisions, name); }
	inline Drawable * sphere(const bg::math::Scalar & radius, const std::string & name = "") { return sphere(radius.value(), 20, name); }
	inline Drawable * sphere(const bg::math::Scalar & radius, float subdivisions, const std::string & name = "") { return sphere(radius.value(), subdivisions, subdivisions, name); }
	inline Drawable * sphere(const bg::math::Scalar & radius, float slices, float stacks, const std::string & name = "") { return sphere(radius.value(), slices, stacks, name); }
	Drawable * sphere(float radius, float slices, float stacks, const std::string & name = "");
	
protected:
	bg::ptr<bg::base::Context> _ctx;
};

}
}

#endif
