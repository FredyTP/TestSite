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


#include <bg/manipulation/orbit_node_controller.hpp>
#include <bg/scene/transform.hpp>
#include <bg/scene/camera.hpp>

namespace bg {
namespace manipulation {

scene::ComponentFactory<OrbitNodeController> orbitNodeController("OrbitCameraController");

OrbitNodeController::OrbitNodeController()
	:_rotateAction({ true, false, false})	// Left button
	,_panAction({ false, false, true})		// Right button
	,_zoomAction({false, true, false})		// Middle button
	,_distance(5.0f)
	,_rotationSpeed(0.2f)
	,_forward(0.0f)
	,_left(0.0f)
	,_wheelSpeed(1.0f)
	,_minFocus(2.0f)
{
}

OrbitNodeController::~OrbitNodeController() {
}

bg::scene::Component * OrbitNodeController::clone() {
	bg::ptr<OrbitNodeController> cloned = new OrbitNodeController();
	cloned->_rotateAction = _rotateAction;
	cloned->_panAction = _panAction;
	cloned->_zoomAction = _zoomAction;
	cloned->_rotation = _rotation;
	cloned->_distance = _distance;
	cloned->_center = _center;
	cloned->_wheelSpeed = _wheelSpeed;
	cloned->_minFocus = _minFocus;
	cloned->_rotationSpeed = _rotationSpeed;
	cloned->_left = _left;
	return cloned.release();
}

void OrbitNodeController::frame(float) {
	if (transform()) {
		bg::math::Vector3 forward = transform()->matrix().forwardVector();
		bg::math::Vector3 left = transform()->matrix().leftVector();
		forward.scale(_forward);
		left.scale(_left);
		_center.add(forward).add(left);
		
		transform()->matrix()
			.identity()
			.translate(_center)
			.rotate(_rotation.y(), 0.0f, 1.0f, 0.0f)
			.rotate(_rotation.x(), 1.0f, 0.0f, 0.0f)
			.translate(0.0f, 0.0f, _distance);
	}

	if (camera()) {
		camera()->setFocus(_distance>_minFocus ? _distance:_minFocus);
	}
}

void OrbitNodeController::mouseDown(const bg::base::MouseEvent & evt) {
	_lastPos = evt.pos();
}

void OrbitNodeController::mouseDrag(const bg::base::MouseEvent & evt) {
	if (transform()) {
		bg::math::Vector2 delta(static_cast<float>(_lastPos.y() - evt.pos().y()),
								static_cast<float>(_lastPos.x() - evt.pos().x()));
		_lastPos = evt.pos();

		switch (getAction(evt)) {
			case 0:	// rotate
				_rotation.add(delta.scale(0.02f));
				break;
			case 1:	// pan
				{
					bg::math::Vector3 up = transform()->matrix().upVector();
					bg::math::Vector3 left = transform()->matrix().leftVector();

					up.scale(delta.x() * -0.001f * _distance);
					left.scale(delta.y() * -0.001f * _distance);
					_center.add(up).add(left);
				}
				break;
			case 2:	// zoom
				_distance += delta.x() * 0.01f * _distance;
				break;
		}
	}
}

void OrbitNodeController::mouseWheel(const bg::base::MouseEvent & evt) {
	float mult = _distance>0.01f ? _distance:0.01f;
	_distance += evt.delta().y() * -0.1f * mult * _wheelSpeed;
}

void OrbitNodeController::touchStart(const bg::base::TouchEvent & evt) {
	setLastTouch(evt);
}

void OrbitNodeController::touchMove(const bg::base::TouchEvent & evt) {
	if (_lastTouch.size()==evt.touches().size() && transform()) {
		if (_lastTouch.size() == 1) {
			// Rotate
			bg::math::Position2Di last = *_lastTouch[0];
			bg::math::Position2Di t = *evt.touches()[0];
			bg::math::Vector2 delta(static_cast<float>(last.y() - t.y()),
									static_cast<float>(last.x() - t.x()));
			_rotation.add(delta.scale(0.02f));
		}
		else if(_lastTouch.size() == 2) {
			// Pan/zoom
			bg::math::Vector2 l0(static_cast<float>(_lastTouch[0]->x()),
								static_cast<float>(_lastTouch[0]->y()));
			bg::math::Vector2 l1(static_cast<float>(_lastTouch[1]->x()),
								static_cast<float>(_lastTouch[1]->y()));
			bg::math::Vector2 t0(static_cast<float>(evt.touches()[0]->x()),
								static_cast<float>(evt.touches()[0]->y()));
			bg::math::Vector2 t1(static_cast<float>(evt.touches()[1]->x()),
								static_cast<float>(evt.touches()[1]->y()));
			float dist0 = bg::math::round((bg::math::Vector2(l0.x(), l0.y()))
									.sub(bg::math::Vector2(l1.x(), l1.y())).magnitude());
			float dist1 = bg::math::round((bg::math::Vector2(t0.x(), t0.y()))
									.sub(bg::math::Vector2(t1.x(), t1.y())).magnitude());
			bg::math::Vector2 delta(l0.y() - t0.y(), l1.x() - t1.x());
			bg::math::Vector3 up = transform()->matrix().upVector();
			bg::math::Vector3 left = transform()->matrix().leftVector();

			up.scale(delta.x() * -0.001f * _distance);
			left.scale(delta.y() * -0.001f * _distance);
			_center.add(up).add(left);

			_distance += (dist0 - dist1) * 0.005f * _distance;
		}
		setLastTouch(evt);
	}
}

int OrbitNodeController::getAction(const bg::base::MouseEvent & evt) {
	bool left = evt.mouse().getButtonStatus(bg::base::Mouse::kLeftButton);
	bool middle = evt.mouse().getButtonStatus(bg::base::Mouse::kMiddleButton);
	bool right = evt.mouse().getButtonStatus(bg::base::Mouse::kRightButton);

	if (left==_rotateAction.left &&
		middle==_rotateAction.middle &&
		right==_rotateAction.right)
	{
		return 0;
	}
	else if (left == _panAction.left &&
		middle == _panAction.middle &&
		right == _panAction.right)
	{
		return 1;
	}
	else if (left == _zoomAction.left &&
		middle == _zoomAction.middle &&
		right == _zoomAction.right)
	{
		return 2;
	}
	return -1;
}

void OrbitNodeController::setLastTouch(const bg::base::TouchEvent & evt) {
	clearTouches();
	for (auto t : evt.touches()) {
		_lastTouch.push_back(new bg::math::Position2Di(*t));
	}
}

void OrbitNodeController::clearTouches() {
	for (auto t : _lastTouch) {
		delete t;
	}
	_lastTouch.clear();
}

void OrbitNodeController::deserialize(bg::db::json::Value * value) {
	using namespace bg::db::json;
	if (value) {

	}
}

}
}
