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


#ifndef _bg2e_physics_plane_hpp_
#define _bg2e_physics_plane_hpp_

#include <bg/math/vector.hpp>

#include <sstream>

namespace bg {
namespace physics {

class Plane {
public:
	Plane() :_origin(0.0f), _normal(0.0f,1.0f,0.0f) {}
	Plane(const bg::math::Vector3 & normal) :_origin(0.0f), _normal(normal) {}
	Plane(const bg::math::Vector3 & normal, const bg::math::Vector3 & origin) :_origin(origin), _normal(normal) {}
	Plane(const bg::math::Vector3 & p1, const bg::math::Vector3 & p2, const bg::math::Vector3 & p3) {
		bg::math::Vector3 vec1(p1); vec1.sub(p2);
		bg::math::Vector3 vec2(p3); vec2.sub(p1);
		_origin = p1;
		_normal = vec1;
		_normal.cross(vec2).normalize();
	}
	virtual ~Plane() {}

	inline void setNormal(const bg::math::Vector3 & n) { _normal = n; }
	inline const bg::math::Vector3 & normal() const { return _normal; }
	inline bg::math::Vector3 & normal() { return _normal; }
	
	inline void setOrigin(const bg::math::Vector3 & o) { _origin = o; }
	inline const bg::math::Vector3 & origin() const { return _origin; }
	inline bg::math::Vector3 & origin() { return _origin; }
	
	std::string toString() {
		std::stringstream str;
		str << "P0:" << _origin.toString() << ", normal: " << _normal.toString();
		return str.str();
	}
	
	inline bool valid() { return !_origin.isNan() && !_normal.isNan(); }

	void operator=(const Plane & p) { _origin = p._origin; _normal = p._normal; }
	bool operator==(const Plane & p) const { return _origin==p._origin && _normal==p._normal; }
	
protected:
	
	bg::math::Vector3 _origin;
	bg::math::Vector3 _normal;
};

}
}

#endif
