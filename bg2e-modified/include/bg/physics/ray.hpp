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


#ifndef _bg2e_physics_ray_hpp_
#define _bg2e_physics_ray_hpp_

#include <bg/math/vector.hpp>
#include <bg/math/matrix.hpp>

#include <bg/scene/node.hpp>
#include <bg/scene/camera.hpp>

namespace bg {
namespace physics {

class InfiniteRay;
class BG2E_EXPORT Ray {
public:
	Ray();
	
	static Ray RayWithPoints(const bg::math::Vector3 & p0, const bg::math::Vector3 & p1) {
		Ray ray;
		ray.setWithPoints(p0, p1);
		return ray;
	}
	
	static Ray RayWithVector(const bg::math::Vector3 & vec, const bg::math::Vector3 & origin, float maxDepth) {
		Ray ray;
		ray.setWithVector(vec, origin, maxDepth);
		return ray;
	}
	
	static Ray RayWithScreenPoint(const bg::math::Position2Di & screenPoint, const bg::math::Matrix4 & projMatrix, const bg::math::Matrix4 & viewMatrix, const bg::math::Viewport & viewport, bool invertYCoord = true) {
		Ray ray;
		ray.setWithScreenPoint(screenPoint, projMatrix, viewMatrix, viewport, invertYCoord);
		return ray;
	}
	
	static Ray RayWithCamera(const bg::math::Position2Di & screenPoint, bg::scene::Camera * cam, bool invertYCoord = true) {
		Ray ray;
		ray.setWithCamera(screenPoint, cam, invertYCoord);
		return ray;
	}
	
	
	Ray & setWithPoints(const bg::math::Vector3 & p0, const bg::math::Vector3 & p1) {
		_start = p0;
		_end = p1;
		calculateVector();
		return (*this);
	}
	
	Ray & setWithVector(const bg::math::Vector3 & vec, const bg::math::Vector3 & origin, float maxDepth) {
		_start = origin;
		_end = origin;
		bg::math::Vector3 vector = vec;
		vector.normalize().scale(maxDepth);
		_end.add(vector);
		calculateVector();
		return (*this);
	}
	
	Ray & setWithScreenPoint(const bg::math::Position2Di & screenPoint, const bg::math::Matrix4 & projMatrix, const bg::math::Matrix4 & viewMatrix, const bg::math::Viewport & viewport, bool invertYCoord = true) {
		bg::math::Position2Di point = screenPoint;
		if (invertYCoord) {
			point.y(viewport.height() - point.y());
		}
		bg::math::Vector4 start = bg::math::Matrix4::Unproject(static_cast<float>(point.x()), static_cast<float>(point.y()), 0.0f, viewMatrix, projMatrix, viewport);
		bg::math::Vector4 end = bg::math::Matrix4::Unproject(static_cast<float>(point.x()), static_cast<float>(point.y()), 1.0f, viewMatrix, projMatrix, viewport);
		_start = start.xyz();
		_end = end.xyz();
		calculateVector();
		return (*this);
	}
	
	Ray & setWithCamera(const bg::math::Position2Di & screenPoint, bg::scene::Camera * cam, bool invertYCoord = true) {
		bg::math::Position2Di point = screenPoint;
		if (invertYCoord) {
			point.y(cam->viewport().height() - point.y());
		}
		bg::math::Vector4 start = bg::math::Matrix4::Unproject(static_cast<float>(point.x()), static_cast<float>(point.y()), 0.0f, cam->viewMatrix(), cam->projection(), cam->viewport());
		bg::math::Vector4 end = bg::math::Matrix4::Unproject(static_cast<float>(point.x()), static_cast<float>(point.y()), 1.0f, cam->viewMatrix(), cam->projection(), cam->viewport());
		_start = start.xyz();
		_end = end.xyz();
		calculateVector();
		return (*this);
	}
	
	inline void operator=(const Ray & ray) {
		_start = ray._start;
		_end = ray._end;
		_vector = ray._vector;
		_magnitude = ray._magnitude;
	}
	
	inline bool operator==(const Ray & ray) const {
		return _start==ray._start && _end==ray._end;
	}

	inline void setStart(const bg::math::Vector3 & s) { _start = s; calculateVector(); }
	inline const bg::math::Vector3 & start() const { return _start; }
	
	inline void setEnd(const bg::math::Vector3 &e) { _end = e; calculateVector(); }
	inline const bg::math::Vector3 & end() const { return _end; }
	
	inline const bg::math::Vector3 & vector() const { return _vector; }
	inline float magnitude() const { return _magnitude; }
	
	inline bg::math::Vector3 pointAtDistance(float d) const {
		bg::math::Vector3 dir = _vector;
		dir.normalize();
		return _start + d * dir;
	}
	
	InfiniteRay infiniteRay();
	
protected:
	bg::math::Vector3 _start;
	bg::math::Vector3 _end;
	bg::math::Vector3 _vector;
	float _magnitude;
	
	void calculateVector() {
		_vector = _end;
		_vector.sub(_start);
		_magnitude = _vector.magnitude();
		_vector.normalize();
	}
};


class BG2E_EXPORT InfiniteRay {
public:
	InfiniteRay();
	InfiniteRay(const bg::math::Vector3 & origin, const bg::math::Vector3 & direction);
	
	inline void setOrigin(const bg::math::Vector3 & a) { _origin = a; }
	inline void setDirection(const bg::math::Vector3 & b) { _direction = b; _direction.normalize(); }
	inline const bg::math::Vector3 & origin() const { return _origin; }
	inline const bg::math::Vector3 & direction() const { return _direction; }
	inline bg::math::Vector3 pointAtDistance(float t) const { return _origin + t * _direction; }
	
	inline Ray rayWithDistance(float t) { return Ray::RayWithVector(_direction, _origin, t); }

protected:
	bg::math::Vector3 _origin;
	bg::math::Vector3 _direction;
};

class RayCastResult : public bg::base::ReferencedPointer {
public:
	RayCastResult(bg::scene::Node * node, const bg::math::Vector3 & point, const bg::math::Vector3 & normal)
		:_node(node), _collisionPoint(point), _collisionNormal(normal)
	{}
	
	inline bg::scene::Node * node() { return _node.getPtr(); }
	inline const bg::scene::Node * node() const { return _node.getPtr(); }
	inline const bg::math::Vector3 & collisionPoint() const { return _collisionPoint; }
	inline const bg::math::Vector3 & collisionNormal() const { return _collisionNormal; }
	
protected:
	virtual ~RayCastResult() {}
	
	bg::ptr<bg::scene::Node> _node;
	bg::math::Vector3 _collisionPoint;
	bg::math::Vector3 _collisionNormal;
};

}
}

#endif
