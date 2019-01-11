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


#ifndef _bg2e_physics_joint_hpp_
#define _bg2e_physics_joint_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/math/matrix.hpp>
#include <bg/math/vector.hpp>

#include <vector>

namespace bg {
namespace physics {

class Joint : public bg::base::ReferencedPointer {
public:
	Joint() {}

	virtual void applyTransform(bg::math::Matrix4 & modelview) = 0;

protected:
	virtual ~Joint() {}
};

typedef std::vector<bg::ptr<Joint> > JointVector;

class LinkJoint : public Joint {
public:
	enum TransformOrder {
		kOrderTranslateRotate = 1,
		kOrderRotateTranslate
	};

	LinkJoint() :_transform(bg::math::Matrix4::Identity()) {}
	LinkJoint(LinkJoint * clone)
		:_offset(clone->_offset)
		,_eulerRotation(clone->_eulerRotation)
		,_transform(clone->_transform)
		,_order(clone->_order)
	{
	}

	virtual void applyTransform(bg::math::Matrix4 & matrix) { matrix.mult(_transform); }

	inline const bg::math::Vector3 & offset() const { return _offset; }
	inline void setOffset(const bg::math::Vector3 & offset) { _offset = offset; calculateTransform(); }
	
	inline void setEulerRotation(const bg::math::Vector3 & e) { _eulerRotation = e; calculateTransform(); }
	inline const bg::math::Vector3 & eulerRotation() const { return _eulerRotation; }
	
	inline float yaw() const { return _eulerRotation.x(); }
	inline float pitch() const { return _eulerRotation.y(); }
	inline float roll() const { return _eulerRotation.z(); }
	
	inline void setYaw(float x) { _eulerRotation.x(x); calculateTransform(); }
	inline void setPitch(float y) { _eulerRotation.y(y); calculateTransform();  }
	inline void setRoll(float z) { _eulerRotation.z(z); calculateTransform();  }

	inline const bg::math::Matrix4 & transform() const { return _transform; }
	inline bg::math::Matrix4 & transform() { return _transform; }

	inline void setTransformOrder(TransformOrder o) { _order = o; }
    
    void multTransform(bg::math::Matrix4 & dst) {
		if (_order == kOrderTranslateRotate) {
			dst.translate(_offset.x(), _offset.y(), _offset.z());
			multRotation(dst);
		}
		else {
			multRotation(dst);
			dst.translate(_offset.x(), _offset.y(), _offset.z());
		}
	}
	
    void multRotation(bg::math::Matrix4 & dst) {
		dst.rotate(_eulerRotation.z(), 0.0f, 0.0f, 1.0f)
			.rotate(_eulerRotation.y(), 0.0f, 1.0f, 0.0f)
			.rotate(_eulerRotation.x(), 1.0f, 0.0f, 0.0f);
	}

	
protected:
	virtual ~LinkJoint() {}
	
	void calculateTransform() {
		_transform.identity();
		multTransform(_transform);
	}

	bg::math::Vector3 _offset;
	bg::math::Vector3 _eulerRotation;
	bg::math::Matrix4 _transform;
	TransformOrder _order;
};

}
}

#endif
