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

#ifndef _bg2e_base_matrix_stack_hpp_
#define _bg2e_base_matrix_stack_hpp_

#include <bg/math/matrix.hpp>

#include <vector>

namespace bg {
namespace base {

typedef std::vector<bg::math::Matrix4> MatrixVector;

class MatrixStack {
public:
	
	MatrixStack()
		:_matrix(bg::math::Matrix4::Identity())
		,_changed(false)
	{
	}

	inline bool changed() const { return _changed; }
	inline void setChanged(bool c) { _changed = c; }

	inline MatrixStack & push() {
		_stack.push_back(_matrix);
		return *this;
	}

	inline MatrixStack & set(const bg::math::Matrix4 & m) {
		_matrix = m;
		_changed = true;
		return *this;
	}

	inline MatrixStack & mult(const bg::math::Matrix4 & m) {
		_matrix.mult(m);
		_changed = true;
		return *this;
	}

	inline MatrixStack & identity() {
		_matrix.identity();
		_changed = true;
		return *this;
	}

	inline MatrixStack & translate(const bg::math::Vector3 & tr) {
		_matrix.translate(tr.x(), tr.y(), tr.z());
		_changed = true;
		return *this;
	}
	
	inline MatrixStack & translate(float x, float y, float z) {
		_matrix.translate(x, y, z);
		_changed = true;
		return *this;
	}

	inline MatrixStack & rotate(float alpha, const bg::math::Vector3 & rot) {
		_matrix.rotate(alpha, rot.x(), rot.y(), rot.z());
		_changed = true;
		return *this;
	}
	
	inline MatrixStack & rotate(float alpha, float x, float y, float z) {
		_matrix.rotate(alpha, x, y, z);
		_changed = true;
		return * this;
	}

	inline MatrixStack & scale(const bg::math::Vector3 & s) {
		_matrix.scale(s.x(), s.y(), s.z());
		_changed = true;
		return *this;
	}

	inline MatrixStack & scale(float x, float y, float z) {
		_matrix.scale(x, y, z);
		_changed = true;
		return *this;
	}

	inline MatrixStack & perspective(const bg::math::Scalar & fov, float aspect, float nearPlane, float farPlane) {
		_matrix
			.identity()
			.perspective(fov, aspect,nearPlane,farPlane);
		_changed = true;
		return *this;
	}
	
	inline MatrixStack & perspective(float fov, float aspect, float nearPlane, float farPlane) {
		_matrix
			.identity()
			.perspective(fov, aspect,nearPlane,farPlane);
		_changed = true;
		return *this;
	}

	inline MatrixStack & frustum(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
		_matrix
			.identity()
			.frustum(left,right,bottom,top,nearPlane,farPlane);
		_changed = true;
		return *this;
	}

	inline MatrixStack & ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
		_matrix
			.identity()
			.ortho(left,right,bottom,top,nearPlane,farPlane);
		_changed = true;
		return *this;
	}

	inline MatrixStack & invert() {
		_matrix.invert();
		_changed = true;
		return *this;
	}

	inline bg::math::Matrix4 & matrix() {
		_changed = true;
		return _matrix;
	}
	
	inline const bg::math::Matrix4 & matrix() const {
		return _matrix;
	}

	inline const bg::math::Matrix4 & pop() {
		if (_stack.size()>0) {
			_matrix = _stack.back();
			_stack.pop_back();
			_changed = true;
		}
		return _matrix;
	}

protected:
	bg::math::Matrix4 _matrix;
	MatrixVector _stack;
	bool _changed;
};

}
}

#endif
