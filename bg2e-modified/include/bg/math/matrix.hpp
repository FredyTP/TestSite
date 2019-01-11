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


#ifndef _bg_math_matrix_hpp_
#define _bg_math_matrix_hpp_

#include <bg/math/utils.hpp>
#include <bg/math/vector.hpp>

#include <string>
#include <sstream>

#ifdef _MSC_VER
#ifndef isnan
#define isnan(x) _isnan(x)
#endif
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif
#endif

namespace bg {
namespace math {

class Matrix3 {
public:
	Matrix3() {
		_m[0] = 0.0f; _m[1] = 0.0f; _m[2] = 0.0f;
		_m[3] = 0.0f; _m[4] = 0.0f; _m[5] = 0.0f;
		_m[6] = 0.0f; _m[7] = 0.0f; _m[8] = 0.0f;
	}
	Matrix3(float s) {
		_m[0] = s; _m[1] = s; _m[2] = s;
		_m[3] = s; _m[4] = s; _m[5] = s;
		_m[6] = s; _m[7] = s; _m[8] = s;
	}
	Matrix3(float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22) {
		_m[0] = m00; _m[1] = m01; _m[2] = m02;
		_m[3] = m10; _m[4] = m11; _m[5] = m12;
		_m[6] = m20; _m[7] = m21; _m[8] = m22;
	}
	Matrix3(const Vector3 & r1, const Vector3 & r2, const Vector3 & r3) {
		_m[0] = r1[0]; _m[1] = r1[1]; _m[2] = r1[2];
		_m[3] = r2[0]; _m[4] = r2[1]; _m[5] = r2[2];
		_m[6] = r3[0]; _m[7] = r3[1]; _m[8] = r3[2];
	}
	Matrix3(const float * m) {
		_m[0] = m[0]; _m[1] = m[1]; _m[2] = m[2];
		_m[3] = m[3]; _m[4] = m[4]; _m[5] = m[5];
		_m[6] = m[6]; _m[7] = m[7]; _m[8] = m[8];
	}
	Matrix3(const Matrix3 & m) {
		_m[0] = m._m[0]; _m[1] = m._m[1]; _m[2] = m._m[2];
		_m[3] = m._m[3]; _m[4] = m._m[4]; _m[5] = m._m[5];
		_m[6] = m._m[6]; _m[7] = m._m[7]; _m[8] = m._m[8];
	}
	
	inline bool isZero() const {
		return	_m[0]==0.0f && _m[1]==0.0f && _m[2]==0.0f &&
		_m[3]==0.0f && _m[4]==0.0f && _m[5]==0.0f &&
		_m[6]==0.0f && _m[7]==0.0f && _m[8]==0.0f;
	}
	
	inline bool isIdentity() const {
		return	_m[0]==1.0f && _m[1]==0.0f && _m[2]==0.0f &&
		_m[3]==0.0f && _m[4]==1.0f && _m[5]==0.0f &&
		_m[6]==0.0f && _m[7]==0.0f && _m[8]==1.0f;
	}
	
	static Matrix3 Identity() {
		Matrix3 i;
		i._m[0] = 1.0f; i._m[1] = 0.0f; i._m[2] = 0.0f;
		i._m[3] = 0.0f; i._m[4] = 1.0f; i._m[5] = 0.0f;
		i._m[6] = 0.0f; i._m[7] = 0.0f; i._m[8] = 1.0f;
		return i;
	}
	
	inline Matrix3 & identity() {
		_m[0] = 1.0f; _m[1] = 0.0f; _m[2] = 0.0f;
		_m[3] = 0.0f; _m[4] = 1.0f; _m[5] = 0.0f;
		_m[6] = 0.0f; _m[7] = 0.0f; _m[8] = 1.0f;
		
		return *this;
	}
	
	inline const float * row(int i) const { return _m + (i*3); }
	inline void setRow(int i, const Vector3 & r) {  _m[i*3]=r[0]; _m[i*3+1]=r[1]; _m[i*3+2]=r[2]; }
	inline void set(float s) {
		_m[ 0]=s; _m[ 1]=s; _m[ 2]=s;
		_m[ 3]=s; _m[ 4]=s; _m[ 5]=s;
		_m[ 6]=s; _m[ 7]=s; _m[ 8]=s;
	}
	
	inline const float * raw() const { return _m; }
	inline int length() const { return 9; }
	
	inline Matrix3 & transpose() {
		Vector3 r0(_m[0], _m[3], _m[6]);
		Vector3 r1(_m[1], _m[4], _m[7]);
		Vector3 r2(_m[2], _m[5], _m[8]);
		
		setRow(0, r0);
		setRow(1, r1);
		setRow(2, r2);
		
		return *this;
	}
	
	inline float operator[](int i) const { return _m[i]; }
	inline void operator=(const float * m) {
		_m[0] = m[0]; _m[1] = m[1]; _m[2] = m[2];
		_m[3] = m[3]; _m[4] = m[4]; _m[5] = m[5];
		_m[6] = m[6]; _m[7] = m[7]; _m[8] = m[8];
	}
	inline void operator=(const Matrix3 & m) {
		_m[0] = m._m[0]; _m[1] = m._m[1]; _m[2] = m._m[2];
		_m[3] = m._m[3]; _m[4] = m._m[4]; _m[5] = m._m[5];
		_m[6] = m._m[6]; _m[7] = m._m[7]; _m[8] = m._m[8];
	}
	inline bool operator==(const Matrix3 & m) const {
		return	_m[0] == m._m[0] && _m[1] == m._m[1]  && _m[2] == m._m[2] &&
			_m[3] == m._m[3] && _m[4] == m._m[4]  && _m[5] == m._m[5] &&
			_m[6] == m._m[6] && _m[7] == m._m[7]  && _m[8] == m._m[8];
	}
	inline bool operator!=(const Matrix3 & m) const {
		return	_m[0] != m._m[0] || _m[1] != m._m[1] || _m[2] != m._m[2] ||
				_m[3] != m._m[3] || _m[4] != m._m[4] || _m[5] != m._m[5] ||
				_m[6] != m._m[6] || _m[7] != m._m[7] || _m[8] != m._m[8];
	}
	
	inline std::string toString() const {
		std::stringstream ss;
		
		ss	<< '[' << _m[0] << "," << _m[1] << "," << _m[2] << "," << std::endl << ' '
		<< _m[3] << "," << _m[4] << "," << _m[5] << "," << std::endl << ' '
		<< _m[6] << "," << _m[7] << "," << _m[8] << ']';
		
		return ss.str();
	}
	
	inline Matrix3 & mult(const Matrix3 & rMatrix) {
		//		_m[0] _m[1] _m[2]		rm[0] rm[1] rm[2]
		//		_m[3] _m[4] _m[5]		rm[3] rm[4] rm[5]
		//		_m[6] _m[7] _m[8]		rm[6] rm[7] rm[8]
		
		const float * rm = _m;
		const float * lm = rMatrix.raw();
		Matrix3 res;
		
		res._m[0] = lm[0] * rm[0] + lm[1] * rm[1] + lm[2] * rm[2];
		res._m[1] = lm[0] * rm[1] + lm[1] * rm[4] + lm[2] * rm[7];
		res._m[2] = lm[0] * rm[2] + lm[1] * rm[5] + lm[2] * rm[8];
		
		res._m[3] = lm[3] * rm[0] + lm[4] * rm[3] + lm[5] * rm[6];
		res._m[4] = lm[3] * rm[1] + lm[4] * rm[4] + lm[5] * rm[7];
		res._m[5] = lm[3] * rm[2] + lm[4] * rm[5] + lm[5] * rm[8];
		
		res._m[6] = lm[6] * rm[0] + lm[7] * rm[3] + lm[8] * rm[6];
		res._m[7] = lm[6] * rm[1] + lm[7] * rm[4] + lm[8] * rm[7];
		res._m[8] = lm[6] * rm[2] + lm[7] * rm[5] + lm[8] * rm[8];
		
		(*this) = res;
		return *this;
	}
	
	inline Matrix3 & mult(float s) {
		_m[0] *= s; _m[1] *= s; _m[2] *= s;
		_m[3] *= s; _m[4] *= s; _m[5] *= s;
		_m[6] *= s; _m[7] *= s; _m[8] *= s;
		return *this;
	}
	
	void set00(float v) {  _m[0] = v; }
	void set01(float v) {  _m[1] = v; }
	void set02(float v) {  _m[2] = v; }
	
	void set10(float v) {  _m[3] = v; }
	void set11(float v) {  _m[4] = v; }
	void set12(float v) {  _m[5] = v; }
	
	void set20(float v) {  _m[6] = v; }
	void set21(float v) {  _m[7] = v; }
	void set22(float v) {  _m[8] = v; }
	
	float get00() const { return _m[0]; }
	float get01() const { return _m[1]; }
	float get02() const { return _m[2]; }
	
	float get10() const { return _m[3]; }
	float get11() const { return _m[4]; }
	float get12() const { return _m[5]; }
	
	float get20() const { return _m[6]; }
	float get21() const { return _m[7]; }
	float get22() const { return _m[8]; }
	
	inline Vector3 eulerAngles() const {
		float rx = trigonometry::atan2(get12(), get22());
		float c2 = sqrt(get00() * get00() + get01() * get01());
		float ry = trigonometry::atan2(-get02(),c2);
		float s1 = trigonometry::sin(rx);
		float c1 = trigonometry::cos(rx);
		float rz = trigonometry::atan2(s1 * get20() - c1 * get10(), c1 * get11() - s1 * get21());
		return Vector3(rx,ry,rz);
	}
	
	inline bool isNan() const {
		return	!isnan(_m[0]) && !isnan(_m[1]) && !isnan(_m[2]) &&
		!isnan(_m[3]) && !isnan(_m[4]) && !isnan(_m[5]) &&
		!isnan(_m[6]) && !isnan(_m[7]) && !isnan(_m[8]);
	}

protected:
	float _m[9];
};

class Matrix4 {
public:
	Matrix4() {
		_m[ 0] = 0.0f; _m[ 1] = 0.0f; _m[ 2] = 0.0f; _m[ 3] = 0.0f;
		_m[ 4] = 0.0f; _m[ 5] = 0.0f; _m[ 6] = 0.0f; _m[ 7] = 0.0f;
		_m[ 8] = 0.0f; _m[ 9] = 0.0f; _m[10] = 0.0f; _m[11] = 0.0f;
		_m[12] = 0.0f; _m[13] = 0.0f; _m[14] = 0.0f; _m[15] = 0.0f;
	}
	Matrix4(float s) {
		_m[ 0] = s; _m[ 1] = s; _m[ 2] = s; _m[ 3] = s;
		_m[ 4] = s; _m[ 5] = s; _m[ 6] = s; _m[ 7] = s;
		_m[ 8] = s; _m[ 9] = s; _m[10] = s; _m[11] = s;
		_m[12] = s; _m[13] = s; _m[14] = s; _m[15] = s;
	}
	Matrix4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) {
		_m[ 0] = m00; _m[ 1] = m01; _m[ 2] = m02; _m[ 3] = m03;
		_m[ 4] = m10; _m[ 5] = m11; _m[ 6] = m12; _m[ 7] = m13;
		_m[ 8] = m20; _m[ 9] = m21; _m[10] = m22; _m[11] = m23;
		_m[12] = m30; _m[13] = m31; _m[14] = m32; _m[15] = m33;
	}
	Matrix4(const Vector4 & r1, const Vector4 & r2, const Vector4 & r3, const Vector4 & r4) {
		_m[ 0] = r1[0]; _m[ 1] = r1[1]; _m[ 2] = r1[2]; _m[ 3] = r1[3];
		_m[ 4] = r2[0]; _m[ 5] = r2[1]; _m[ 6] = r2[2]; _m[ 7] = r2[3];
		_m[ 8] = r3[0]; _m[ 9] = r3[1]; _m[10] = r3[2]; _m[11] = r3[3];
		_m[12] = r4[0]; _m[13] = r4[1]; _m[14] = r4[2]; _m[15] = r4[3];
	}
	Matrix4(const float * m) {
		_m[ 0] = m[ 0]; _m[ 1] = m[ 1]; _m[ 2] = m[ 2]; _m[ 3] = m[ 3];
		_m[ 4] = m[ 4]; _m[ 5] = m[ 5]; _m[ 6] = m[ 6]; _m[ 7] = m[ 7];
		_m[ 8] = m[ 8]; _m[ 9] = m[ 9]; _m[10] = m[10]; _m[11] = m[11];
		_m[12] = m[12]; _m[13] = m[13]; _m[14] = m[14]; _m[15] = m[15];
	}
	Matrix4(const Matrix4 & m) {
		_m[ 0] = m._m[ 0]; _m[ 1] = m._m[ 1]; _m[ 2] = m._m[ 2]; _m[ 3] = m._m[ 3];
		_m[ 4] = m._m[ 4]; _m[ 5] = m._m[ 5]; _m[ 6] = m._m[ 6]; _m[ 7] = m._m[ 7];
		_m[ 8] = m._m[ 8]; _m[ 9] = m._m[ 9]; _m[10] = m._m[10]; _m[11] = m._m[11];
		_m[12] = m._m[12]; _m[13] = m._m[13]; _m[14] = m._m[14]; _m[15] = m._m[15];
	}
	Matrix4(const Matrix3 & m) {
		_m[ 0] = m[ 0]; _m[ 1] = m[ 1]; _m[ 2] = m[ 2]; _m[ 3] = 0.0f;
		_m[ 4] = m[ 3]; _m[ 5] = m[ 4]; _m[ 6] = m[ 5]; _m[ 7] = 0.0f;
		_m[ 8] = m[ 6]; _m[ 9] = m[ 7]; _m[10] = m[ 8]; _m[11] = 0.0f;
		_m[12] = 0.0f;  _m[13] = 0.0f;  _m[14] = 0.0f;  _m[15] = 1.0f;
	}
	
	inline bool isZero() const {
		return	_m[ 0]==0.0f && _m[ 1]==0.0f && _m[ 2]==0.0f && _m[ 3]==0.0f &&
		_m[ 4]==0.0f && _m[ 5]==0.0f && _m[ 6]==0.0f && _m[ 7]==0.0f &&
		_m[ 8]==0.0f && _m[ 9]==0.0f && _m[10]==0.0f && _m[11]==0.0f &&
		_m[12]==0.0f && _m[13]==0.0f && _m[14]==0.0f && _m[15]==0.0f;
	}
	
	inline bool isIdentity() const {
		return	_m[ 0]==1.0f && _m[ 1]==0.0f && _m[ 2]==0.0f && _m[ 3]==0.0f &&
		_m[ 4]==0.0f && _m[ 5]==1.0f && _m[ 6]==0.0f && _m[ 7]==0.0f &&
		_m[ 8]==0.0f && _m[ 9]==0.0f && _m[10]==1.0f && _m[11]==0.0f &&
		_m[12]==0.0f && _m[13]==0.0f && _m[14]==0.0f && _m[15]==1.0f;
	}
	
	inline Matrix4 & identity() {
		
		(*this) = Matrix4::Identity();
		return *this;
	}
	
	static Vector4 Unproject(float x, float y, float depth, const Matrix4 & mvMat, const Matrix4 & pMat,const Viewport & viewport) {
		Matrix4 mvp(pMat);
		mvp.mult(mvMat);
		mvp.invert();
		
		Vector4 vin(((x - static_cast<float>(viewport.y())) / static_cast<float>(viewport.width())) * 2.0f - 1.0f,
					((y - static_cast<float>(viewport.x())) / static_cast<float>(viewport.height())) * 2.0f - 1.0f,
					depth * 2.0f - 1.0f,
					1.0f);
		
		Vector4 result = mvp.multVector(vin);
		if (result[3]==0.0) {
			result.set(0.0f);
		}
		else {
			result.set(result.x()/result.w(),
					   result.y()/result.w(),
					   result.z()/result.w(),
					   result.w()/result.w());
		}
		
		return result;
	}
	
	static Matrix4 Identity() {
		Matrix4 i;
		i._m[ 0] = 1.0f; i._m[ 1] = 0.0f; i._m[ 2] = 0.0f; i._m[ 3] = 0.0f;
		i._m[ 4] = 0.0f; i._m[ 5] = 1.0f; i._m[ 6] = 0.0f; i._m[ 7] = 0.0f;
		i._m[ 8] = 0.0f; i._m[ 9] = 0.0f; i._m[10] = 1.0f; i._m[11] = 0.0f;
		i._m[12] = 0.0f; i._m[13] = 0.0f; i._m[14] = 0.0f; i._m[15] = 1.0f;
		return i;
	}
	
	static Matrix4 Perspective(const Scalar & fovy, float aspect, float nearPlane, float farPlane) {
		return Matrix4::Perspective(trigonometry::radiansToDegrees(fovy.value()), aspect, nearPlane, farPlane);
	}
	
	static Matrix4 Perspective(float fovy, float aspect, float nearPlane, float farPlane) {
		float fovy2 = trigonometry::tan(fovy * static_cast<float>(kPi) / 360.0f) * nearPlane;
		float fovy2aspect = fovy2 * aspect;
		
		return Matrix4::Frustum(-fovy2aspect,fovy2aspect,-fovy2,fovy2,nearPlane,farPlane);
	}
	
	static Matrix4 Frustum(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
		Matrix4 res;
		double A = static_cast<double>(right-left);
		double B = static_cast<double>(top-bottom);
		double C = static_cast<double>(farPlane-nearPlane);
		
		res.setRow(0, Vector4(static_cast<float>(nearPlane*2.0/A),	0.0f,	0.0f,	0.0f));
		res.setRow(1, Vector4(0.0f,	static_cast<float>(nearPlane*2.0/B),	0.0f,	0.0f));
		res.setRow(2, Vector4((right+left)/static_cast<float>(A),	(top+bottom)/static_cast<float>(B),	static_cast<float>((-farPlane-nearPlane)/C),	-1.0f));
		res.setRow(3, Vector4(0.0f,	0.0f,	static_cast<float>((-farPlane*nearPlane*2.0)/C),	0.0f));
		
		return res;
	}
	
	static Matrix4 Ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
		Matrix4 p;
		
		float m = right-left;
		float l = top-bottom;
		float k=farPlane-nearPlane;;
		
		p._m[0] = 2/m; p._m[1] = 0;   p._m[2] = 0;     p._m[3] = 0;
		p._m[4] = 0;   p._m[5] = 2/l; p._m[6] = 0;     p._m[7] = 0;
		p._m[8] = 0;   p._m[9] = 0;   p._m[10] = -2/k; p._m[11]=0;
		p._m[12]=-(left+right)/m; p._m[13] = -(top+bottom)/l; p._m[14] = -(farPlane+nearPlane)/k; p._m[15]=1;
		
		
		return p;
	}
	
	static Matrix4 LookAt(const Vector3 & p_eye, const Vector3 & p_center, const Vector3 & p_up) {
		Vector3  f = normalize(p_center - p_eye);
		Vector3  u = normalize(p_up);
		Vector3  s = normalize(cross(f, u));
		u = cross(s, f);

		Matrix4 result;
		result.set00(s.x());
		result.set10(s.y());
		result.set20(s.z());
		result.set01(u.x());
		result.set11(u.y());
		result.set21(u.z());
		result.set02(-f.x());
		result.set12(-f.y());
		result.set22(-f.z());
		result.set30(-dot(s, p_eye));
		result.set31(-dot(u, p_eye));
		result.set32(dot(f, p_eye));
		result.set33(1.0f);
		return result;
	}
	
	static Matrix4 Translation(float x, float y, float z) {
		Matrix4 t;
		t._m[ 0] = 1.0f; t._m[ 1] = 0.0f; t._m[ 2] = 0.0f; t._m[ 3] = 0.0f;
		t._m[ 4] = 0.0f; t._m[ 5] = 1.0f; t._m[ 6] = 0.0f; t._m[ 7] = 0.0f;
		t._m[ 8] = 0.0f; t._m[ 9] = 0.0f; t._m[10] = 1.0f; t._m[11] = 0.0f;
		t._m[12] =    x; t._m[13] =    y; t._m[14] =    z; t._m[15] = 1.0f;
		return t;
	}
	
	static Matrix4 Translation(const Scalar & x, const Scalar & y, const Scalar & z) {
		Matrix4 t;
		t._m[ 0] = 1.0f; t._m[ 1] = 0.0f; t._m[ 2] = 0.0f; t._m[ 3] = 0.0f;
		t._m[ 4] = 0.0f; t._m[ 5] = 1.0f; t._m[ 6] = 0.0f; t._m[ 7] = 0.0f;
		t._m[ 8] = 0.0f; t._m[ 9] = 0.0f; t._m[10] = 1.0f; t._m[11] = 0.0f;
		t._m[12] = x.value(); t._m[13] = y.value(); t._m[14] = z.value(); t._m[15] = 1.0f;
		return t;
	}
	
	static Matrix4 Translation(const Vector3 & t) {
		return Translation(t.x(), t.y(), t.z());
	}
	
	static Matrix4 Rotation(const Scalar & alpha, float x, float y, float z) {
		return Rotation(alpha.value(), x, y, z);
	}
	
	static Matrix4 Rotation(float alpha, float x, float y, float z) {
		Vector3 axis(x,y,z);
		axis.normalize();
		Matrix4 rot;
		
		rot.identity();
		
		float cosAlpha = cosf(alpha);
		float acosAlpha = 1.0f - cosAlpha;
		float sinAlpha = sinf(alpha);
		
		rot._m[0] = axis.x() * axis.x() * acosAlpha + cosAlpha;
		rot._m[1] = axis.x() * axis.y() * acosAlpha + axis.z() * sinAlpha;
		rot._m[2] = axis.x() * axis.z() * acosAlpha - axis.y() * sinAlpha;
		
		rot._m[4] = axis.y() * axis.x() * acosAlpha - axis.z() * sinAlpha;
		rot._m[5] = axis.y() * axis.y() * acosAlpha + cosAlpha;
		rot._m[6] = axis.y() * axis.z() * acosAlpha + axis.x() * sinAlpha;
		
		rot._m[8] = axis.z() * axis.x() * acosAlpha + axis.y() * sinAlpha;
		rot._m[9] = axis.z() * axis.y() * acosAlpha - axis.x() * sinAlpha;
		rot._m[10] = axis.z() * axis.z() * acosAlpha + cosAlpha;
		
		return rot;
	}
	
	static Matrix4 Scale(float x, float y, float z) {
		Matrix4 s;
		
		s._m[ 0] =    x; s._m[ 1] = 0.0f; s._m[ 2] = 0.0f; s._m[ 3] = 0.0f;
		s._m[ 4] = 0.0f; s._m[ 5] =    y; s._m[ 6] = 0.0f; s._m[ 7] = 0.0f;
		s._m[ 8] = 0.0f; s._m[ 9] = 0.0f; s._m[10] =    z; s._m[11] = 0.0f;
		s._m[12] = 0.0f; s._m[13] = 0.0f; s._m[14] = 0.0f; s._m[15] = 1.0f;
		
		return s;
	}
	
	static Matrix4 Scale(const Vector3 & s) {
		return Scale(s.x(), s.y(), s.z());
	}
	
	inline Matrix4 & perspective(float fovy, float aspect, float nearPlane, float farPlane) {
		*this = Matrix4::Perspective(fovy, aspect, nearPlane, farPlane);
		return *this;
	}
	
	inline Matrix4 & perspective(const Scalar & fovy, float aspect, float nearPlane, float farPlane) {
		*this = Matrix4::Perspective(fovy, aspect, nearPlane, farPlane);
		return *this;
	}
	
	inline Matrix4 & frustum(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
		*this = Matrix4::Frustum(left, right, bottom, top, nearPlane, farPlane);
		return *this;
	}
	
	inline Matrix4 & ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
		*this = Matrix4::Ortho(left, right, bottom, top, nearPlane, farPlane);
		return *this;
	}
	
	inline Matrix4 & lookAt(const Vector3 & origin, const Vector3 & target, const Vector3 & up) {
		*this = Matrix4::LookAt(origin,target,up);
		return *this;
	}
	
	inline Matrix4 & translate(float x, float y, float z) {
		mult(Matrix4::Translation(x, y, z));
		return *this;
	}
	
	inline Matrix4 & translate(const Scalar & x, const Scalar & y, const Scalar & z) {
		mult(Matrix4::Translation(x, y, z));
		return *this;
	}
	
	inline Matrix4 & translate(const Vector3 & t) {
		mult(Matrix4::Translation(t));
		return *this;
	}
	
	inline Matrix4 & rotate(const Scalar & alpha, float x, float y, float z) {
		mult(Matrix4::Rotation(alpha.value(), x, y, z));
		return *this;
	}
	
	inline Matrix4 & rotate(float alpha, float x, float y, float z) {
		mult(Matrix4::Rotation(alpha, x, y, z));
		return *this;
	}
	
	inline Matrix4 & scale(float x, float y, float z) {
		mult(Matrix4::Scale(x, y, z));
		return *this;
	}
	
	inline Matrix4 & scale(const Vector3 & s) {
		mult(Matrix4::Scale(s));
		return *this;
	}
	
	inline const float * row(int i) const { return _m + (i*4); }
	inline void setRow(int i, const Vector4 & r) { _m[i*4]=r[0]; _m[i*4+1]=r[1]; _m[i*4+2]=r[2]; _m[i*4+3]=r[3]; }
	inline void set(float s) {
		_m[ 0]=s; _m[ 1]=s; _m[ 2]=s; _m[ 3]=s;
		_m[ 4]=s; _m[ 5]=s; _m[ 6]=s; _m[ 7]=s;
		_m[ 8]=s; _m[ 9]=s; _m[10]=s; _m[11]=s;
		_m[12]=s; _m[13]=s; _m[14]=s; _m[15]=s;
	}
	
	inline Matrix3 getMatrix3() const {
		Matrix3 res(_m[ 0],_m[ 1],_m[ 2],
					_m[ 4],_m[ 5],_m[ 6],
					_m[ 8],_m[ 9],_m[10]);
		return res;
	}
	
	inline const float * raw() const { return _m; }
	inline int length() const { return 15; }
	
	inline float operator[](int i) const { return _m[i]; }
	inline void operator=(const float * m) {
		_m[ 0] = m[ 0]; _m[ 1] = m[ 1]; _m[ 2] = m[ 2]; _m[ 3] = m[ 3];
		_m[ 4] = m[ 4]; _m[ 5] = m[ 5]; _m[ 6] = m[ 6]; _m[ 7] = m[ 7];
		_m[ 8] = m[ 8]; _m[ 9] = m[ 9]; _m[10] = m[10]; _m[11] = m[11];
		_m[12] = m[12]; _m[13] = m[13]; _m[14] = m[14]; _m[15] = m[15];
	}
	inline void operator=(const Matrix4 & m) {
		_m[ 0] = m._m[ 0]; _m[ 1] = m._m[ 1]; _m[ 2] = m._m[ 2]; _m[ 3] = m._m[ 3];
		_m[ 4] = m._m[ 4]; _m[ 5] = m._m[ 5]; _m[ 6] = m._m[ 6]; _m[ 7] = m._m[ 7];
		_m[ 8] = m._m[ 8]; _m[ 9] = m._m[ 9]; _m[10] = m._m[10]; _m[11] = m._m[11];
		_m[12] = m._m[12]; _m[13] = m._m[13]; _m[14] = m._m[14]; _m[15] = m._m[15];
	}
	inline void operator=(const Matrix3 & m) {
		_m[ 0] = m[ 0]; _m[ 1] = m[ 1]; _m[ 2] = m[ 2]; _m[ 3] = 0.0f;
		_m[ 4] = m[ 3]; _m[ 5] = m[ 4]; _m[ 6] = m[ 5]; _m[ 7] = 0.0f;
		_m[ 8] = m[ 6]; _m[ 9] = m[ 7]; _m[10] = m[ 8]; _m[11] = 0.0f;
		_m[12] = 0.0f;  _m[13] = 0.0f;  _m[14] = 0.0f;  _m[15] = 1.0f;
	}
	inline bool operator==(const Matrix4 & m) const {
		return	_m[ 0]==m._m[ 0] && _m[ 1]==m._m[ 1] && _m[ 2]==m._m[ 2] && _m[ 3]==m._m[ 3] &&
		_m[ 4]==m._m[ 4] && _m[ 5]==m._m[ 5] && _m[ 6]==m._m[ 6] && _m[ 7]==m._m[ 7] &&
		_m[ 8]==m._m[ 8] && _m[ 9]==m._m[ 9] && _m[10]==m._m[10] && _m[11]==m._m[11] &&
		_m[12]==m._m[12] && _m[13]==m._m[13] && _m[14]==m._m[14] && _m[15]==m._m[15];
	}
	inline bool operator!=(const Matrix4 & m) const {
		return	_m[0]  != m._m[0]  || _m[1]  != m._m[1]  || _m[2]  != m._m[2]  || _m[3]  != m._m[3]  ||
				_m[4]  != m._m[4]  || _m[5]  != m._m[5]  || _m[6]  != m._m[6]  || _m[7]  != m._m[7]  ||
				_m[8]  != m._m[8]  || _m[9]  != m._m[9]  || _m[10] != m._m[10] || _m[11] != m._m[11] ||
				_m[12] != m._m[12] || _m[13] != m._m[13] || _m[14] != m._m[14] || _m[15] != m._m[15];
	}
	
	inline std::string toString() const {
		std::stringstream ss;
		
		ss	<< '[' << _m[ 0] << ',' << _m[ 1] << ',' << _m[ 2] << ',' << _m[ 3] << ',' << std::endl << ' '
		<< _m[ 4] << ',' << _m[ 5] << ',' << _m[ 6] << ',' << _m[ 7] << ',' << std::endl << ' '
		<< _m[ 8] << ',' << _m[ 9] << ',' << _m[10] << ',' << _m[11] << ',' << std::endl << ' '
		<< _m[12] << ',' << _m[13] << ',' << _m[14] << ',' << _m[15] << ']';
		
		return ss.str();
	}
	
	inline Matrix4 & mult(const Matrix4 & rMatrix) {
		//		_m[ 0] _m[ 1] _m[ 2] _m[ 3] 		rm[ 0] rm[ 1] rm[ 2] rm[ 3]
		//		_m[ 4] _m[ 5] _m[ 6] _m[ 7]			rm[ 4] rm[ 5] rm[ 6] rm[ 7]
		//		_m[ 8] _m[ 9] _m[10] _m[11]			rm[ 8] rm[ 9] rm[10] rm[11]
		//		_m[12] _m[13] _m[14] _m[15]			rm[12] rm[13] rm[14] rm[15]
		const float * rm = _m;
		const float * lm = rMatrix.raw();
		Matrix4 res;
		
		res._m[ 0] = lm[ 0] * rm[ 0] + lm[ 1] * rm[ 4] + lm[ 2] * rm[ 8] + lm[ 3] * rm[12];
		res._m[ 1] = lm[ 0] * rm[ 1] + lm[ 1] * rm[ 5] + lm[ 2] * rm[ 9] + lm[ 3] * rm[13];
		res._m[ 2] = lm[ 0] * rm[ 2] + lm[ 1] * rm[ 6] + lm[ 2] * rm[10] + lm[ 3] * rm[14];
		res._m[ 3] = lm[ 0] * rm[ 3] + lm[ 1] * rm[ 7] + lm[ 2] * rm[11] + lm[ 3] * rm[15];
		
		res._m[ 4] = lm[ 4] * rm[ 0] + lm[ 5] * rm[ 4] + lm[ 6] * rm[ 8] + lm[ 7] * rm[12];
		res._m[ 5] = lm[ 4] * rm[ 1] + lm[ 5] * rm[ 5] + lm[ 6] * rm[ 9] + lm[ 7] * rm[13];
		res._m[ 6] = lm[ 4] * rm[ 2] + lm[ 5] * rm[ 6] + lm[ 6] * rm[10] + lm[ 7] * rm[14];
		res._m[ 7] = lm[ 4] * rm[ 3] + lm[ 5] * rm[ 7] + lm[ 6] * rm[11] + lm[ 7] * rm[15];
		
		res._m[ 8] = lm[ 8] * rm[ 0] + lm[ 9] * rm[ 4] + lm[10] * rm[ 8] + lm[11] * rm[12];
		res._m[ 9] = lm[ 8] * rm[ 1] + lm[ 9] * rm[ 5] + lm[10] * rm[ 9] + lm[11] * rm[13];
		res._m[10] = lm[ 8] * rm[ 2] + lm[ 9] * rm[ 6] + lm[10] * rm[10] + lm[11] * rm[14];
		res._m[11] = lm[ 8] * rm[ 3] + lm[ 9] * rm[ 7] + lm[10] * rm[11] + lm[11] * rm[15];
		
		res._m[12] = lm[12] * rm[ 0] + lm[13] * rm[ 4] + lm[14] * rm[ 8] + lm[15] * rm[12];
		res._m[13] = lm[12] * rm[ 1] + lm[13] * rm[ 5] + lm[14] * rm[ 9] + lm[15] * rm[13];
		res._m[14] = lm[12] * rm[ 2] + lm[13] * rm[ 6] + lm[14] * rm[10] + lm[15] * rm[14];
		res._m[15] = lm[12] * rm[ 3] + lm[13] * rm[ 7] + lm[14] * rm[11] + lm[15] * rm[15];
		
		(*this) = res;
		return *this;
	}
	
	inline Matrix4 & mult(float s) {
		
		_m[ 0] *= s; _m[ 1] *= s; _m[ 2] *= s; _m[ 3] *= s;
		_m[ 4] *= s; _m[ 5] *= s; _m[ 6] *= s; _m[ 7] *= s;
		_m[ 8] *= s; _m[ 9] *= s; _m[10] *= s; _m[11] *= s;
		_m[12] *= s; _m[13] *= s; _m[14] *= s; _m[15] *= s;
		return *this;
	}
	
	inline Vector4 multVector(const Vector3 & vec) const {
		Vector4 res;
		float x=vec[0]; float y=vec[1]; float z=vec[2]; float w=1.0f;
		
		res.x(_m[0]*x + _m[4]*y + _m[8] *z + _m[12] * w);
		res.y(_m[1]*x + _m[5]*y + _m[9] *z + _m[13] * w);
		res.z(_m[2]*x + _m[6]*y + _m[10]*z + _m[14] * w);
		res.w(_m[3]*x + _m[7]*y + _m[11]*z + _m[15] * w);
		
		return res;
	}
	
	inline Vector4 multVector(const Vector4 & vec) const {
		Vector4 res;
		float x=vec[0]; float y=vec[1]; float z=vec[2]; float w=vec[3];
		
		res.x(_m[0]*x + _m[4]*y + _m[8] *z + _m[12] * w);
		res.y(_m[1]*x + _m[5]*y + _m[9] *z + _m[13] * w);
		res.z(_m[2]*x + _m[6]*y + _m[10]*z + _m[14] * w);
		res.w(_m[3]*x + _m[7]*y + _m[11]*z + _m[15] * w);
		
		return res;
	}
	
	inline Matrix4 & invert() {
		float inv[16];
		
		inv[0] = _m[5] * _m[10] * _m[15] - _m[5] * _m[11] * _m[14] - _m[9] * _m[6] * _m[15] + _m[9] * _m[7] * _m[14] + _m[13] * _m[6] * _m[11] - _m[13] * _m[7] * _m[10];
		inv[4] = -_m[4] * _m[10] * _m[15] + _m[4] * _m[11] * _m[14] + _m[8] * _m[6] * _m[15] - _m[8] * _m[7] * _m[14] - _m[12] * _m[6] * _m[11] + _m[12] * _m[7] * _m[10];
		inv[8] = _m[4] * _m[9] * _m[15] - _m[4] * _m[11] * _m[13] - _m[8] * _m[5] * _m[15] + _m[8] * _m[7] * _m[13] + _m[12] * _m[5] * _m[11] - _m[12] * _m[7] * _m[9];
		inv[12] = -_m[4] * _m[9] * _m[14] + _m[4] * _m[10] * _m[13] + _m[8] * _m[5] * _m[14] - _m[8] * _m[6] * _m[13] - _m[12] * _m[5] * _m[10] + _m[12] * _m[6] * _m[9];
		inv[1] = -_m[1] * _m[10] * _m[15] + _m[1] * _m[11] * _m[14] + _m[9] * _m[2] * _m[15] - _m[9] * _m[3] * _m[14] - _m[13] * _m[2] * _m[11] + _m[13] * _m[3] * _m[10];
		inv[5] = _m[0] * _m[10] * _m[15] - _m[0] * _m[11] * _m[14] - _m[8] * _m[2] * _m[15] + _m[8] * _m[3] * _m[14] + _m[12] * _m[2] * _m[11] - _m[12] * _m[3] * _m[10];
		inv[9] = -_m[0] * _m[9] * _m[15] + _m[0] * _m[11] * _m[13] + _m[8] * _m[1] * _m[15] - _m[8] * _m[3] * _m[13] - _m[12] * _m[1] * _m[11] + _m[12] * _m[3] * _m[9];
		inv[13] = _m[0] * _m[9] * _m[14] - _m[0] * _m[10] * _m[13] - _m[8] * _m[1] * _m[14] + _m[8] * _m[2] * _m[13] + _m[12] * _m[1] * _m[10] - _m[12] * _m[2] * _m[9];
		inv[2] = _m[1] * _m[6] * _m[15] - _m[1] * _m[7] * _m[14] - _m[5] * _m[2] * _m[15] + _m[5] * _m[3] * _m[14] + _m[13] * _m[2] * _m[7] - _m[13] * _m[3] * _m[6];
		inv[6] = -_m[0] * _m[6] * _m[15] + _m[0] * _m[7] * _m[14] + _m[4] * _m[2] * _m[15] - _m[4] * _m[3] * _m[14] - _m[12] * _m[2] * _m[7] + _m[12] * _m[3] * _m[6];
		inv[10] = _m[0] * _m[5] * _m[15] - _m[0] * _m[7] * _m[13] - _m[4] * _m[1] * _m[15] + _m[4] * _m[3] * _m[13] + _m[12] * _m[1] * _m[7] - _m[12] * _m[3] * _m[5];
		inv[14] = -_m[0] * _m[5] * _m[14] + _m[0] * _m[6] * _m[13] + _m[4] * _m[1] * _m[14] - _m[4] * _m[2] * _m[13] - _m[12] * _m[1] * _m[6] + _m[12] * _m[2] * _m[5];
		inv[3] = -_m[1] * _m[6] * _m[11] + _m[1] * _m[7] * _m[10] + _m[5] * _m[2] * _m[11] - _m[5] * _m[3] * _m[10] - _m[9] * _m[2] * _m[7] + _m[9] * _m[3] * _m[6];
		inv[7] = _m[0] * _m[6] * _m[11] - _m[0] * _m[7] * _m[10] - _m[4] * _m[2] * _m[11] + _m[4] * _m[3] * _m[10] + _m[8] * _m[2] * _m[7] - _m[8] * _m[3] * _m[6];
		inv[11] = -_m[0] * _m[5] * _m[11] + _m[0] * _m[7] * _m[9] + _m[4] * _m[1] * _m[11] - _m[4] * _m[3] * _m[9] - _m[8] * _m[1] * _m[7] + _m[8] * _m[3] * _m[5];
		inv[15] = _m[0] * _m[5] * _m[10] - _m[0] * _m[6] * _m[9] - _m[4] * _m[1] * _m[10] + _m[4] * _m[2] * _m[9] + _m[8] * _m[1] * _m[6] - _m[8] * _m[2] * _m[5];
		
		float det = _m[0] * inv[0] + _m[1] * inv[4] + _m[2] * inv[8] + _m[3] * inv[12];
		
		if (det==0.0f) {
			set(0.0f);
		}
		else {
			det = 1.0f / det;
		
			_m[0]  = inv[0]  * det;
			_m[1]  = inv[1]  * det;
			_m[2]  = inv[2]  * det;
			_m[3]  = inv[3]  * det;
			_m[4]  = inv[4]  * det;
			_m[5]  = inv[5]  * det;
			_m[6]  = inv[6]  * det;
			_m[7]  = inv[7]  * det;
			_m[8]  = inv[8]  * det;
			_m[9]  = inv[9]  * det;
			_m[10] = inv[10] * det;
			_m[11] = inv[11] * det;
			_m[12] = inv[12] * det;
			_m[13] = inv[13] * det;
			_m[14] = inv[14] * det;
			_m[15] = inv[15] * det;
		}
		
		return *this;
	}
	
	inline Matrix4 & transpose() {
		Vector4 r0(_m[0], _m[4], _m[ 8], _m[12]);
		Vector4 r1(_m[1], _m[5], _m[ 9], _m[13]);
		Vector4 r2(_m[2], _m[6], _m[10], _m[14]);
		Vector4 r3(_m[3], _m[7], _m[11], _m[15]);
		
		setRow(0, r0);
		setRow(1, r1);
		setRow(2, r2);
		setRow(3, r3);
		return *this;
	}
	
	inline Matrix4 & setPosition(float x, float y, float z) {
		_m[12] = x; _m[13] = y; _m[14] = z;
		return *this;
	}
	
	inline Matrix4 & setPosition(const Vector3 & pos) {
		_m[12] = pos.x(); _m[13] = pos.y(); _m[14] = pos.z();
		return *this;
	}
	
	inline Matrix4 & setPosition(const Scalar & x, const Scalar & y, const Scalar & z) {
		_m[12] = x.value(); _m[13] = y.value(); _m[14] = z.value();
		return *this;
	}
	
	inline Vector3 position() const {
		return Vector3(_m[12], _m[13], _m[14]);
		
	}
	
	inline Matrix4 rotation() const {
		return Matrix4(
					   _m[0], _m[1], _m[ 2], 0.0f,
					   _m[4], _m[5], _m[ 6], 0.0f,
					   _m[8], _m[9], _m[10], 0.0f,
					   0.0f,  0.0f,  0.0f,   1.0f
					   );
	}
	
	inline Vector3 scale() const {
		return Vector3(
					   Vector3(_m[0], _m[1], _m[ 2]).magnitude(),
					   Vector3(_m[4], _m[5], _m[ 6]).magnitude(),
					   Vector3(_m[8], _m[9], _m[10]).magnitude()
					   );
	}
	
	inline void resetScale() {
		Vector3 rx(_m[0], _m[1], _m[ 2]);
		Vector3 ry(_m[4], _m[5], _m[ 6]);
		Vector3 rz(_m[8], _m[9], _m[10]);
		rx.normalize();
		ry.normalize();
		rz.normalize();
		
		set00(rx.x()); set01(rx.y()); set02(rx.z());
		set10(ry.x()); set11(ry.y()); set12(ry.z());
		set20(rz.x()); set21(rz.y()); set22(rz.z());
	}
	
	inline void setScale(Vector3 s) {
		resetScale();
		scale(s.x(), s.y(), s.z());
	}
	
	inline Matrix4 resetRotation() {
		Vector3 scale = this->scale();
		_m[0] = scale.x(); _m[1] = 0.0f; _m[ 2] = 0.0f;
		_m[4] = 0.0f; _m[5] = scale.y(); _m[ 6] = 0.0f;
		_m[8] = 0.0f; _m[9] = 0.0f; _m[10] = scale.z();
		return *this;
	}
	
	inline Vector3 transformDirection(const Vector3 & dir) const {
		Vector3 direction = dir;
		Matrix4 trx = *this;
		trx.setRow(3, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		direction = trx.multVector(direction).xyz();
		direction.normalize();
		return direction;
	}
	
	inline Vector3 forwardVector() const {
		return transformDirection(Vector3(0.0f, 0.0f, 1.0f));
	}
	
	inline Vector3 rightVector() const {
		return transformDirection(Vector3(1.0f, 0.0f, 0.0f));
	}
	
	inline Vector3 upVector() const {
		return transformDirection(Vector3(0.0f, 1.0f, 0.0f));
	}
	
	inline Vector3 backwardVector() const {
		return transformDirection(Vector3(0.0f, 0.0f, -1.0f));
	}
	
	inline Vector3 leftVector() const {
		return transformDirection(Vector3(-1.0f, 0.0f, 0.0f));
	}
	
	inline Vector3 downVector() const {
		return transformDirection(Vector3(0.0f, -1.0f, 0.0f));
	}
	
	inline Vector3 eulerAngles() const {
		float rx = trigonometry::atan2(get12(), get22());
		float c2 = sqrt(get00() * get00() + get01() * get01());
		float ry = trigonometry::atan2(-1.0f * get02(),c2);
		float s1 = trigonometry::sin(rx);
		float c1 = trigonometry::cos(rx);
		float rz = atan2(s1 * get20() - c1 * get10(), c1 * get11() - s1 * get21());
		return Vector3(rx,ry,rz);
	}
	
	inline void set00(float v) {  _m[0] = v; }
	inline void set01(float v) {  _m[1] = v; }
	inline void set02(float v) {  _m[2] = v; }
	inline void set03(float v) {  _m[3] = v; }
	
	inline void set10(float v) {  _m[4] = v; }
	inline void set11(float v) {  _m[5] = v; }
	inline void set12(float v) {  _m[6] = v; }
	inline void set13(float v) {  _m[7] = v; }
	
	inline void set20(float v) {  _m[8] = v; }
	inline void set21(float v) {  _m[9] = v; }
	inline void set22(float v) {  _m[10] = v; }
	inline void set23(float v) {  _m[11] = v; }
	
	inline void set30(float v) {  _m[12] = v; }
	inline void set31(float v) {  _m[13] = v; }
	inline void set32(float v) {  _m[14] = v; }
	inline void set33(float v) {  _m[15] = v; }
	
	inline float get00() const { return _m[0]; }
	inline float get01() const { return _m[1]; }
	inline float get02() const { return _m[2]; }
	inline float get03() const { return _m[3]; }
	
	inline float get10() const { return _m[4]; }
	inline float get11() const { return _m[5]; }
	inline float get12() const { return _m[6]; }
	inline float get13() const { return _m[7]; }
	
	inline float get20() const { return _m[8]; }
	inline float get21() const { return _m[9]; }
	inline float get22() const { return _m[10]; }
	inline float get23() const { return _m[11]; }
	
	inline float get30() const { return _m[12]; }
	inline float get31() const { return _m[13]; }
	inline float get32() const { return _m[14]; }
	inline float get33() const { return _m[15]; }
	
	inline bool isNan() const {
		return	isnan(_m[ 0]) || isnan(_m[ 1]) || isnan(_m[ 2]) || isnan(_m[ 3]) ||
		isnan(_m[ 4]) || isnan(_m[ 5]) || isnan(_m[ 6]) || isnan(_m[ 7]) ||
		isnan(_m[ 8]) || isnan(_m[ 9]) || isnan(_m[10]) || isnan(_m[11]) ||
		isnan(_m[12]) || isnan(_m[13]) || isnan(_m[14]) || isnan(_m[15]);
	}
	
	void projectionValues(float & /* left */, float & /* right */, float & /* bottom */, float & /* top */, float & /* near */, float & /* far */) {
		
	}
	
	void orthoValues(float & left, float & right, float & bottom, float & top, float & near, float & far)  const {
		near   =  (1+get23())/get22();
		far    = -(1-get23())/get22();
		bottom =  (1-get13())/get11();
		top    = -(1+get13())/get11();
		left   = -(1+get03())/get00();
		right  =  (1-get03())/get00();
	}
	
	void perspectiveValues(float & left, float & right, float & bottom, float & top, float & near, float & far) const {
		near   = get23()/(get22()-1);
		far    = get23()/(get22()+1);
		bottom = near * (get12()-1)/get11();
		top    = near * (get12()+1)/get11();
		left   = near * (get02()-1)/get00();
		right  = near * (get02()+1)/get00();
	}
	
	//inline void setUnchanged(bool unchanged = true) { _unchanged = unchanged; }
	//inline bool unchanged() const { return _unchanged; }

	// Vulkan utility function: multiply the second row, second column by -1
	inline void toVulkanProjection() { _m[5] *= -1.0f; }

protected:
	float _m[16];
	//bool 
};

}
}

#endif
