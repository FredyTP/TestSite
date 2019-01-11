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


#ifndef _bg2e_physics_intersection_hpp_
#define _bg2e_physics_intersection_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/physics/ray.hpp>
#include <bg/physics/plane.hpp>

namespace bg {
namespace physics {

class RayToPlaneIntersection;

class BG2E_EXPORT Intersection {
public:
	enum Type {
		kTypeNone = 0,
		kTypePoint = 1,
		kTypeLine = 2
	};
	static RayToPlaneIntersection RayToPlane(const Ray & ray, const Plane & plane);

	Intersection() :_type(kTypeNone) {}

	inline Type type() const { return _type; }
	inline const bg::math::Vector3 & startPoint() const { return _p0; }
	inline const bg::math::Vector3 & endPoint() const { return _p1; }
	
	inline bool intersects() const { return false; }

protected:
	Type _type;
	bg::math::Vector3 _p0;
	bg::math::Vector3 _p1;
	
};

class BG2E_EXPORT RayToPlaneIntersection : public Intersection {
public:
	RayToPlaneIntersection(const Ray & ray, const Plane & plane);
	
	inline const Ray & ray() const { return _ray; }
	inline bool intersects() const { return _intersects; }

	inline void operator=(const RayToPlaneIntersection & i) {
		_ray = i._ray;
		_intersects = i._intersects;
	}

protected:
	Ray _ray;
	bool _intersects;
};

}
}

#endif
