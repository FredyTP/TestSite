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


#ifndef _bg2e_manipulation_orbit_node_controller_hpp_
#define _bg2e_manipulation_orbit_node_controller_hpp_

#include <bg/scene/component.hpp>
#include <bg/math/vector.hpp>

namespace bg {
namespace manipulation {


class BG2E_EXPORT OrbitNodeController : public bg::scene::Component {
public:
	struct ActionButtons {
		bool left;
		bool middle;
		bool right;

		inline void operator =(const ActionButtons & other) {
			left = other.left; middle = other.middle; right = other.right;
		}

		inline bool operator==(const ActionButtons & other) const {
			return left == other.left && middle == other.middle && right == other.right;
		}
	};

	OrbitNodeController();

	virtual Component * clone();

	inline void setRotateAction(const ActionButtons & b) { _rotateAction = b; }
	inline void setPanAction(const ActionButtons & b) { _panAction = b; }
	inline void setZoomAction(const ActionButtons & b) { _zoomAction = b; }
	inline ActionButtons & rotateAction() { return _rotateAction; }
	inline const ActionButtons & rotateAction() const { return _rotateAction; }
	inline ActionButtons & panAction() { return _panAction; }
	inline const ActionButtons & panAction() const { return _panAction; }
	inline ActionButtons & zoomAction() { return _zoomAction; }
	inline const ActionButtons & zoomAction() const { return _zoomAction; }

	inline void setRotation(const bg::math::Vector2 & r) { _rotation = r; }
	inline const bg::math::Vector2 & rotation() const { return _rotation; }
	inline bg::math::Vector2 & rotation() { return _rotation; }

	inline void setDistance(float d) { _distance = d; }
	inline void setDistance(const bg::math::Scalar & d) { _distance = d.value(); }
	inline float distance() const { return _distance; }

	inline void setCenter(const bg::math::Vector3 & c) { _center = c; }
	inline const bg::math::Vector3 & center() const { return _center; }
	inline bg::math::Vector3 & center() { return _center; }

	inline void setWheelSpeed(float s) { _wheelSpeed = s; }
	inline float wheelSpeed() const { return _wheelSpeed; }
	
	// This is only used if the parent node have a camera component attached
	inline void setMinCameraFocus(float f) { _minFocus = f; }
	inline void setMinCameraFocus(const bg::math::Scalar & f) { _minFocus = f.value(); }
	inline float minCameraFocus() const { return _minFocus; }

	virtual void frame(float);
	virtual void mouseDown(const bg::base::MouseEvent &);
	virtual void mouseDrag(const bg::base::MouseEvent &);
	virtual void mouseWheel(const bg::base::MouseEvent &);
	virtual void touchStart(const bg::base::TouchEvent & evt);
	virtual void touchMove(const bg::base::TouchEvent & evt);

	void deserialize(bg::db::json::Value * value);
	
protected:
	virtual ~OrbitNodeController();

	ActionButtons _rotateAction;
	ActionButtons _panAction;
	ActionButtons _zoomAction;

	bg::math::Vector2 _rotation;
	float _distance;
	bg::math::Vector3 _center;
	float _rotationSpeed;
	float _forward;
	float _left;
	float _wheelSpeed;
	float _minFocus;

	bg::math::Position2Di _lastPos;
	base::TouchEvent::TouchSet _lastTouch;

	int getAction(const bg::base::MouseEvent &);
	
	void setLastTouch(const bg::base::TouchEvent & evt);
	void clearTouches();
};

}
}

#endif
