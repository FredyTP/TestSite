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

#ifndef _bg2e_math_quaternion_hpp_
#define _bg2e_math_quaternion_hpp_

#include <bg/math/vector.hpp>
#include <bg/math/matrix.hpp>

namespace bg {
namespace math {

class Quaternion : public Vector4 {
public:
	Quaternion() :Vector4() {}
	Quaternion(float alpha, float x, float y, float z) {
		_v[0] = x * sinf(alpha/2.0f);
		_v[1] = y * sinf(alpha/2.0f);
		_v[2] = z * sinf(alpha/2.0f);
		_v[3] = cosf(alpha/2.0f);
	}
	Quaternion(const Quaternion & q) {
		_v[0] = q._v[0];
		_v[1] = q._v[1];
		_v[2] = q._v[2];
		_v[3] = q._v[3];
	}
	Quaternion(const Matrix3 & m) {
		float w = sqrtf(1.0f + m[0] + m[4] + m[8]) / 2.0f;
		float w4 = 4.0f * w;
		
		_v[0] =	(m[7] - m[5]) / w;
		_v[1] =	(m[2] - m[6]) / w4;
		_v[2] = (m[3] - m[1]) / w4;
		_v[3] = w;
	}
	Quaternion(const Matrix4 & m) {
		float w = sqrtf(1.0f + m[0] + m[5] + m[10]) / 2.0f;
		float w4 = 4.0f * w;
		
		_v[0] =	(m[9] - m[6]) / w;
		_v[1] =	(m[2] - m[8]) / w4;
		_v[2] = (m[4] - m[1]) / w4;
		_v[3] = w;
	}
	
	static Quaternion MakeWithMatrix(const Matrix3 & m) {
		return Quaternion(m);
	}
	
	static Quaternion MakeWithMatrix(const Matrix4 & m) {
		return Quaternion(m);
	}
	
	inline Matrix4 getMatrix4() const {
		Matrix4 m = Matrix4::Identity();
		m.setRow(0, Vector4(1.0f - 2.0f*_v[1]*_v[1] - 2.0f*_v[2]*_v[2], 2.0f*_v[0]*_v[1] - 2.0f*_v[2]*_v[3], 2.0f*_v[0]*_v[2] + 2.0f*_v[1]*_v[3], 0.0f));
		m.setRow(1, Vector4(2.0f*_v[0]*_v[1] + 2.0f*_v[2]*_v[3], 1.0f - 2.0f*_v[0]*_v[0] - 2.0f*_v[2]*_v[2], 2.0f*_v[1]*_v[2] - 2.0f*_v[0]*_v[3], 0.0f));
		m.setRow(2, Vector4(2.0f*_v[0]*_v[2] - 2.0f*_v[1]*_v[3], 2.0f*_v[1]*_v[2] + 2.0f*_v[0]*_v[3], 1.0f - 2.0f*_v[0]*_v[0] - 2.0f*_v[1]*_v[1], 0.0f));
		return m;
	}
	
	inline Matrix3 getMatrix3() const {
		Matrix3 m = Matrix3::Identity();
		m.setRow(0, Vector3(1.0f - 2.0f*_v[1]*_v[1] - 2.0f*_v[2]*_v[2], 2.0f*_v[0]*_v[1] - 2.0f*_v[2]*_v[3], 2.0f*_v[0]*_v[2] + 2.0f*_v[1]*_v[3]));
		m.setRow(1, Vector3(2.0f*_v[0]*_v[1] + 2.0f*_v[2]*_v[3], 1.0f - 2.0f*_v[0]*_v[0] - 2.0f*_v[2]*_v[2], 2.0f*_v[1]*_v[2] - 2.0f*_v[0]*_v[3]));
		m.setRow(2, Vector3(2.0f*_v[0]*_v[2] - 2.0f*_v[1]*_v[3], 2.0f*_v[1]*_v[2] + 2.0f*_v[0]*_v[3], 1.0f - 2.0f*_v[0]*_v[0] - 2.0f*_v[1]*_v[1]));
		return m;
	}
};


}
}

#endif
