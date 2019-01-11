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


#include <bg/physics/intersection.hpp>

namespace bg {
namespace physics {

RayToPlaneIntersection Intersection::RayToPlane(const bg::physics::Ray &ray, const bg::physics::Plane &plane) {
	return RayToPlaneIntersection(ray, plane);
}

RayToPlaneIntersection::RayToPlaneIntersection(const bg::physics::Ray &ray, const bg::physics::Plane &plane) {
	_type = Intersection::kTypePoint;
	bg::math::Vector3 p0(plane.origin());
	bg::math::Vector3 n(plane.normal());
	bg::math::Vector3 l0(ray.start());
	bg::math::Vector3 l(ray.vector());
	float num = p0.sub(l0).dot(n);
	float den = l.dot(n);
	float distance = 0;
	
	if (den==0) {
		_intersects = false;
	}
	else if ((distance=num/den)>ray.magnitude()) {
		_intersects = false;
	}
	else if (distance<0.0f) {	// Intersects behind the ray direction
		_intersects = false;
	}
	else {
		_ray = Ray::RayWithVector(ray.vector(), ray.start(), distance);
		_p0 = _ray.start();
		_p1 = _ray.end();
		_intersects = true;
	}
}

}
}
