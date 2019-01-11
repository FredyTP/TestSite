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


#ifndef _bg2e_math_vector_hpp_
#define _bg2e_math_vector_hpp_

#include <bg/export.hpp>

#include <bg/math/utils.hpp>

#include <string>
#include <sstream>
#include <vector>

namespace bg {
namespace math {


template <class T>
class Vector2Generic {
public:
	Vector2Generic<T>() { _v[0] = static_cast<T>(0); _v[1] = static_cast<T>(0); }
	Vector2Generic<T>(T s) { _v[0] = s; _v[1] = s; }
	Vector2Generic<T>(T x, T y) { _v[0] = x; _v[1] = y; }
	Vector2Generic<T>(const T * v) { _v[0] = v[0]; _v[1] = v[1]; }
	Vector2Generic<T>(const Vector2Generic<T> & v) { _v[0] = v._v[0]; _v[1] = v._v[1]; }
	Vector2Generic<T>(const Scalar & s) { _v[0] = static_cast<T>(s.value()); _v[1] = static_cast<T>(s.value()); }
	Vector2Generic<T>(const Scalar & x, const Scalar & y) { _v[0] = static_cast<T>(x.value()); _v[1] = static_cast<T>(y.value()); }

	static Vector2Generic Lerp(const Vector2Generic<T> & from, const Vector2Generic<T> & to, float delta) {
		return Vector2Generic<T>(bg::math::lerp(from._v[0],to._v[0],delta),
								 bg::math::lerp(from._v[1],to._v[1],delta));
	}
	
	inline T * raw() { return _v; }
	inline const T * raw() const { return _v; }
	inline int length() const { return 2; }

	inline float distance(const Vector2Generic<T> & v) {
		Vector2Generic<T> v3(_v[0] - v._v[0],
							 _v[1] - v._v[1]);
		return v3.magnitude();
	}

	inline float magnitude() { return sqrtf(static_cast<float>(_v[0]*_v[0] + _v[1]*_v[1])); }
	inline Vector2Generic<T> & normalize() {
		T m = magnitude();
		_v[0]=_v[0]/m; _v[1]=_v[1]/m;
		return *this;
	}
	inline Vector2Generic<T> & add(const Vector2Generic<T> & v2) {
		_v[0] += v2._v[0];
		_v[1] += v2._v[1];
		return *this;
	}
	inline Vector2Generic<T> & sub(const Vector2Generic<T> & v2) {
		_v[0] -= v2._v[0];
		_v[1] -= v2._v[1];
		return *this;
	}
	inline T dot(const Vector2Generic<T> & v2) const {
		return _v[0] * v2._v[0] + _v[1] * v2._v[1];
	}
	inline Vector2Generic<T> & scale(T scale) {
		_v[0] *= scale; _v[1] *= scale;
		return *this;
	}
	inline Vector2Generic<T> & scale(const Scalar & s) {
		_v[0] *= s.value(); _v[1] *= s.value();
		return *this;
	}

	inline T operator[](int i) const { return _v[i]; }
	inline bool operator==(const Vector2Generic<T> & v) const { return _v[0]==v._v[0] && _v[1]==v._v[1]; }
	inline bool operator<(const Vector2Generic<T> & v) const { return _v[0]<v._v[0] && _v[1]<v._v[1]; }
	inline bool operator>(const Vector2Generic<T> & v) const { return _v[0]>v._v[0] && _v[1]>v._v[1]; }
	inline bool operator<=(const Vector2Generic<T> & v) const { return _v[0]<=v._v[0] && _v[1]<=v._v[1]; }
	inline bool operator>=(const Vector2Generic<T> & v) const { return _v[0]>=v._v[0] && _v[1]>=v._v[1]; }
	inline bool operator!=(const Vector2Generic<T> & v) const { return _v[0]!=v._v[0] || _v[1]!=v._v[1]; }
	inline void operator=(const Vector2Generic & v) { _v[0] = v._v[0]; _v[1] = v._v[1]; }
	inline void operator=(const T * v) { _v[0] = v[0]; _v[1] = v[1]; }

	inline std::string toString() const {
		std::stringstream ss;
		ss	<< '[' << _v[0] << "," << _v[1] << ']';
		return ss.str();
	}

	inline void set(T v) { _v[0] = v; _v[1] = v; }
	inline void set(T x, T y) { _v[0] = x; _v[1] = y; }
	inline void set(const Scalar & v) { _v[0] = static_cast<T>(v.value()); _v[1] = static_cast<T>(v.value()); }
	inline void set(const Scalar & x, const Scalar & y) { _v[0] = static_cast<T>(x.value()); _v[1] = static_cast<T>(y.value()); }

	inline T x() const { return _v[0]; }
	inline T y() const { return _v[1]; }

	inline void x(T v) { _v[0] = v; }
	inline void y(T v) { _v[1] = v; }
	inline void x(const Scalar & v) { _v[0] = static_cast<T>(v.value()); }
	inline void y(const Scalar & v) { _v[1] = static_cast<T>(v.value()); }

	inline T width() const { return _v[0]; }
	inline T height() const { return _v[1]; }

	inline void width(T v) { _v[0] = v; }
	inline void height(T v) { _v[1] = v; }
	inline void width(const Scalar & v) { _v[0] = static_cast<T>(v.value()); }
	inline void height(const Scalar & v) { _v[1] = static_cast<T>(v.value()); }

	inline T aspectRatio() const { return static_cast<T>(_v[0])/static_cast<T>(_v[1]); }

	inline Vector2Generic<T> & clamp(T min, T max) {
		_v[0] = number::clamp<T>(_v[0], min, max);
		_v[1] = number::clamp<T>(_v[1], min, max);
		return *this;
	}
	inline Vector2Generic<T> & clamp(const Scalar & min, const Scalar & max) {
		_v[0] = number::clamp<T>(_v[0], static_cast<T>(min.value()), static_cast<T>(max.value()));
		_v[1] = number::clamp<T>(_v[1], static_cast<T>(min.value()), static_cast<T>(max.value()));
		return *this;
	}
	
	inline bool isNan() {
		return isnan(_v[0]) || isnan(_v[1]);
	}

	inline bool isZero() {
		return _v[0]==0.0f && _v[1]==0.0f;
	}

	inline bool belongsToRect(T left, T top, T width, T height) const {
		return _v[0] >= left && _v[0] <= left + width && _v[1] >= top && _v[1] <= top + height;
	}

	static Vector2Generic<T> Min(const Vector2Generic<T> & v1, const Vector2Generic<T> & v2) {
		Vector2Generic<T> result;
		
		result._v[0] = bg::math::min(v1._v[0], v2._v[0]);
		result._v[1] = bg::math::min(v1._v[1], v2._v[1]);
		
		return result;
	}
	
	static Vector2Generic<T> Max(const Vector2Generic<T> & v1, const Vector2Generic<T> & v2) {
		Vector2Generic<T> result;
		
		result._v[0] = bg::math::max(v1._v[0], v2._v[0]);
		result._v[1] = bg::math::max(v1._v[1], v2._v[1]);
		
		return result;
	}
	
protected:
	T _v[2];
};

template <class T>
inline Vector2Generic<T> operator + (const Vector2Generic<T> & v1, const Vector2Generic<T> & v2) {
	return Vector2Generic<T>(v1.x() + v2.x(), v1.y() + v2.y());
}
template <class T>
inline Vector2Generic<T> operator - (const Vector2Generic<T> & v1, const Vector2Generic<T> & v2) {
	return Vector2Generic<T>(v1.x() - v2.x(), v1.y() - v2.y());
}
template <class T>
inline Vector2Generic<T> operator * (const Vector2Generic<T> & v1, const Vector2Generic<T> & v2) {
	return Vector2Generic<T>(v1.x() * v2.x(), v1.y() * v2.y());
}
template <class T>
inline Vector2Generic<T> operator / (const Vector2Generic<T> & v1, const Vector2Generic<T> & v2) {
	return Vector2Generic<T>(v1.x() / v2.x(), v1.y() / v2.y());
}
template <class T>
inline Vector2Generic<T> operator * (const Vector2Generic<T> & v1, T s) {
	return Vector2Generic<T>(v1.x() * s, v1.y() * s);
}
template <class T>
inline Vector2Generic<T> operator * (T s, const Vector2Generic<T> & v1) {
	return Vector2Generic<T>(v1.x() * s, v1.y() * s);
}
template <class T>
inline Vector2Generic<T> operator / (const Vector2Generic<T> & v1, T s) {
	return Vector2Generic<T>(v1.x() / s, v1.y() / s);
}

typedef Vector2Generic<short> Vector2s;
typedef Vector2Generic<int> Vector2i;
typedef Vector2Generic<long long> Vector2l;
typedef Vector2Generic<unsigned short> Vector2us;
typedef Vector2Generic<unsigned int> Vector2ui;
typedef Vector2Generic<unsigned long long> Vector2ul;
typedef Vector2Generic<float> Vector2;
typedef Vector2Generic<double> Vector2d;

template <class T>
class Vector3Generic {
public:
	Vector3Generic() { _v[0] = static_cast<T>(0); _v[1] = static_cast<T>(0); _v[2] = static_cast<T>(0); }
	Vector3Generic(T s) { _v[0] = s; _v[1] = s; _v[2] = s; }
	Vector3Generic(T x, T y, T z) { _v[0] = x; _v[1] = y; _v[2] = z; }
	Vector3Generic(const T * v) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; }
	Vector3Generic(const Vector3Generic<T> & v) { _v[0] = v._v[0]; _v[1] = v._v[1]; _v[2] = v._v[2]; }
	Vector3Generic(const Vector2Generic<T> & v, T z) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = z; }
	Vector3Generic(const Scalar & s) { _v[0] = static_cast<T>(s.value()); _v[1] = static_cast<T>(s.value()); _v[2] = static_cast<T>(s.value()); }
	Vector3Generic(const Scalar & x, const Scalar & y, const Scalar & z) { _v[0] = static_cast<T>(x.value()); _v[1] = static_cast<T>(y.value()); _v[2] = static_cast<T>(z.value()); }
	
	static Vector3Generic Lerp(const Vector3Generic<T> & from, const Vector3Generic<T> & to, float delta) {
		return Vector3Generic<T>(bg::math::lerp(from._v[0],to._v[0],delta),
								bg::math::lerp(from._v[1],to._v[1],delta),
								bg::math::lerp(from._v[2],to._v[2],delta));
	}
	
	
	inline T * raw() { return _v; }
	inline const T * raw() const { return _v; }
	inline int length() const { return 3; }

	inline float distance(const Vector3Generic<T> & v) {
		Vector3Generic<T> v3(_v[0] - v._v[0],
							 _v[1] - v._v[1],
							 _v[2] - v._v[2]);
		return v3.magnitude();
	}

	inline float magnitude() { return sqrtf(static_cast<float>(_v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2])); }
	inline Vector3Generic<T> & normalize() {
		T m = magnitude();
		_v[0]=_v[0]/m; _v[1]=_v[1]/m; _v[2]=_v[2]/m;
		return *this;
	}
	inline Vector3Generic<T> & add(const Vector3Generic<T> & v2) {
		_v[0] += v2._v[0];
		_v[1] += v2._v[1];
		_v[2] += v2._v[2];
		return *this;
	}
	inline Vector3Generic<T> & sub(const Vector3Generic<T> & v2) {
		_v[0] -= v2._v[0];
		_v[1] -= v2._v[1];
		_v[2] -= v2._v[2];
		return *this;
	}
	inline T dot(const Vector3Generic<T> & v2) const {
		return _v[0] * v2._v[0] + _v[1] * v2._v[1] + _v[2] * v2._v[2];
	}
	inline Vector3Generic<T> & scale(T scale) {
		_v[0] *= scale; _v[1] *= scale; _v[2] *= scale;
		return *this;
	}
	inline Vector3Generic<T> & scale(const Scalar & scale) {
		_v[0] *= static_cast<T>(scale.value()); _v[1] *= static_cast<T>(scale.value()); _v[2] *= static_cast<T>(scale.value());
		return *this;
	}
	inline Vector3Generic<T> & cross(const Vector3Generic<T> & v2) {
		T x = _v[1] * v2._v[2] - _v[2] * v2._v[1];
		T y = _v[2] * v2._v[0] - _v[0] * v2._v[2];
		T z = _v[0] * v2._v[1] - _v[1] * v2._v[0];
		_v[0]=x; _v[1]=y; _v[2]=z;
		return *this;
	}

	inline T operator[](int i) const { return _v[i]; }
	inline bool operator==(const Vector3Generic<T> & v) const { return _v[0]==v._v[0] && _v[1]==v._v[1] && _v[2]==v._v[2]; }
	inline bool operator<(const Vector3Generic<T> & v) const { return _v[0]<v._v[0] && _v[1]<v._v[1] && _v[2]<v._v[2]; }
	inline bool operator>(const Vector3Generic<T> & v) const { return _v[0]>v._v[0] && _v[1]>v._v[1] && _v[2]>v._v[2]; }
	inline bool operator<=(const Vector3Generic<T> & v) const { return _v[0]<=v._v[0] && _v[1]<=v._v[1] && _v[2]<=v._v[2]; }
	inline bool operator>=(const Vector3Generic<T> & v) const { return _v[0]>=v._v[0] && _v[1]>=v._v[1] && _v[2]>=v._v[2]; }
	inline bool operator!=(const Vector3Generic<T> & v) const { return _v[0]!=v._v[0] || _v[1]!=v._v[1] || _v[2]!=v._v[2]; }
	inline void operator=(const Vector3Generic<T> & v) { _v[0] = v._v[0]; _v[1] = v._v[1]; _v[2] = v._v[2]; }
	inline void operator=(const T * v) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; }

	inline Vector3Generic operator+(const Vector3Generic &v) const { return Vector3Generic(_v[0]+v._v[0],_v[1]+v._v[1],_v[2]+v._v[2]); }
	inline Vector3Generic operator-(const Vector3Generic &v) const { return Vector3Generic(_v[0]-v._v[0],_v[1]-v._v[1],_v[2]-v._v[2]); }
	// Cross product operator
	inline Vector3Generic operator^(const Vector3Generic &v) const {
		return Vector3Generic(_v[1] * v._v[2] - _v[2] * v._v[1],
							  _v[2] * v._v[0] - _v[0] * v._v[2],
							  _v[0] * v._v[1] - _v[1] * v._v[0]);
	}
	// Dot product operator (the same operation as dot() function)
	inline T operator*(const Vector3Generic &v) const { return _v[0] * v._v[0] + _v[1] * v._v[1] + _v[2] * v._v[2]; }
	inline Vector3Generic operator/(T scalar) const { return Vector3Generic(_v[0]/scalar,_v[1]/scalar,_v[2]/scalar); }
	inline Vector3Generic operator/(const Scalar & scalar) const { return Vector3Generic(_v[0]/static_cast<T>(scalar.value()),_v[1]/static_cast<T>(scalar.value()),_v[2]/static_cast<T>(scalar.value())); }


	inline std::string toString() const {
		std::stringstream ss;
		ss	<< '[' << _v[0] << "," << _v[1] << "," << _v[2] << ']';
		return ss.str();
	}

	inline void set(T v) { _v[0] = v; _v[1] = v; _v[2] = v; }
	inline void set(T x, T y, T z) { _v[0] = x; _v[1] = y; _v[2] = z; }
	inline void set(const Scalar & v) { _v[0] = static_cast<T>(v.value()); _v[1] = static_cast<T>(v.value()); _v[2] = static_cast<T>(v.value()); }
	inline void set(const Scalar & x, const Scalar & y, const Scalar & z) { _v[0] = static_cast<T>(x.value()); _v[1] = static_cast<T>(y.value()); _v[2] = static_cast<T>(z.value()); }

	inline T x() const { return _v[0]; }
	inline T y() const { return _v[1]; }
	inline T z() const { return _v[2]; }

	inline void x(T v) { _v[0] = v; }
	inline void y(T v) { _v[1] = v; }
	inline void z(T v) { _v[2] = v; }
	inline void x(const Scalar & v) { _v[0] = static_cast<T>(v.value()); }
	inline void y(const Scalar & v) { _v[1] = static_cast<T>(v.value()); }
	inline void z(const Scalar & v) { _v[2] = static_cast<T>(v.value()); }

	inline T r() const { return _v[0]; }
	inline T g() const { return _v[1]; }
	inline T b() const { return _v[2]; }

	inline void r(T v) { _v[0] = v; }
	inline void g(T v) { _v[1] = v; }
	inline void b(T v) { _v[2] = v; }

	inline T width() const { return _v[0]; }
	inline T height() const { return _v[1]; }
	inline T depth() const { return _v[2]; }

	inline void width(T v) { _v[0] = v; }
	inline void height(T v) { _v[1] = v; }
	inline void depth(T v) { _v[2] = v; }
	inline void width(const Scalar & v) { _v[0] = static_cast<T>(v.value()); }
	inline void height(const Scalar & v) { _v[1] = static_cast<T>(v.value()); }
	inline void depth(const Scalar & v) { _v[2] = static_cast<T>(v.value()); }

	inline Vector2Generic<T> xy() const { return Vector2Generic<T>(x(),y()); }
	inline Vector2Generic<T> yz() const { return Vector2Generic<T>(y(),z()); }
	inline Vector2Generic<T> xz() const { return Vector2Generic<T>(x(),z()); }

	inline T aspectRatio() const { return static_cast<T>(_v[0])/static_cast<T>(_v[1]); }

	inline Vector3Generic<T> & clamp(T min, T max) {
		_v[0] = number::clamp<T>(_v[0], min, max);
		_v[1] = number::clamp<T>(_v[1], min, max);
		_v[2] = number::clamp<T>(_v[2], min, max);
		return *this;
	}
	
	inline Vector3Generic<T> & clamp(const Scalar & min, const Scalar & max) {
		_v[0] = number::clamp<T>(_v[0], static_cast<T>(min.value()), static_cast<T>(max.value()));
		_v[1] = number::clamp<T>(_v[1], static_cast<T>(min.value()), static_cast<T>(max.value()));
		_v[2] = number::clamp<T>(_v[2], static_cast<T>(min.value()), static_cast<T>(max.value()));
		return *this;
	}
	
	inline bool isNan() {
#ifdef _WIN32
		return _isnan(_v[0]) || _isnan(_v[1]) || _isnan(_v[2]);
#else
		return isnan(_v[0]) || isnan(_v[1]) || isnan(_v[2]);
#endif
	}

	inline bool isZero() {
		return _v[0] == 0.0f && _v[1] == 0.0f && _v[2] == 0.0f;
	}
	
	static Vector3Generic<T> Min(const Vector3Generic<T> & v1, const Vector3Generic<T> & v2) {
		Vector3Generic<T> result;
		
		result._v[0] = bg::math::min(v1._v[0], v2._v[0]);
		result._v[1] = bg::math::min(v1._v[1], v2._v[1]);
		result._v[2] = bg::math::min(v1._v[2], v2._v[2]);
		
		return result;
	}
	
	static Vector3Generic<T> Max(const Vector3Generic<T> & v1, const Vector3Generic<T> & v2) {
		Vector3Generic<T> result;
		
		result._v[0] = bg::math::max(v1._v[0], v2._v[0]);
		result._v[1] = bg::math::max(v1._v[1], v2._v[1]);
		result._v[2] = bg::math::max(v1._v[2], v2._v[2]);
		
		return result;
	}

protected:
	T _v[3];
};

template <class T>
inline Vector3Generic<T> operator + (const Vector3Generic<T> & v1, const Vector3Generic<T> & v2) {
	return Vector3Generic<T>(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}
template <class T>
inline Vector3Generic<T> operator - (const Vector3Generic<T> & v1, const Vector3Generic<T> & v2) {
	return Vector3Generic<T>(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}
template <class T>
inline Vector3Generic<T> operator * (const Vector3Generic<T> & v1, const Vector3Generic<T> & v2) {
	return Vector3Generic<T>(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}
template <class T>
inline Vector3Generic<T> operator / (const Vector3Generic<T> & v1, const Vector3Generic<T> & v2) {
	return Vector3Generic<T>(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
}
template <class T>
inline Vector3Generic<T> operator * (const Vector3Generic<T> & v1, T s) {
	return Vector3Generic<T>(v1.x() * s, v1.y() * s, v1.z() * s);
}
template <class T>
inline Vector3Generic<T> operator * (T s, const Vector3Generic<T> & v1) {
	return Vector3Generic<T>(v1.x() * s, v1.y() * s, v1.z() * s);
}
template <class T>
inline Vector3Generic<T> operator / (const Vector3Generic<T> & v1, T s) {
	return Vector3Generic<T>(v1.x() / s, v1.y() / s, v1.z() / s);
}

template <class T>
inline T dot(const Vector3Generic<T> & v1, const Vector3Generic<T> & v2) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

template <class T>
inline Vector3Generic<T> normalize(const Vector3Generic<T> & v) {
	Vector3Generic<T> result = v;
	result.normalize();
	return result;
}

template <class T>
inline Vector3Generic<T> cross(const Vector3Generic<T> & u, const Vector3Generic<T> & v) {
	return Vector3Generic<T>(
		u[1] * v[2] - u[2] * v[1],
		u[2] * v[0] - u[0] * v[2],
		u[0] * v[1] - u[1] * v[0]
	);
}
	
typedef Vector3Generic<short> Vector3s;
typedef Vector3Generic<int> Vector3i;
typedef Vector3Generic<long> Vector3l;
typedef Vector3Generic<unsigned short> Vector3us;
typedef Vector3Generic<unsigned int> Vector3ui;
typedef Vector3Generic<unsigned long> Vector3ul;
typedef Vector3Generic<float> Vector3;
typedef Vector3Generic<double> Vector3d;

template <class T>
class Vector4Generic {
public:
	static Vector4Generic Yellow() { return Vector4Generic(1.0f,1.0f,0.0f,1.0f); }
	static Vector4Generic Orange() { return Vector4Generic(1.0f,0.5f,0.0f,1.0f); }
	static Vector4Generic Red() { return Vector4Generic(1.0f,0.0f,0.0f,1.0f); }
	static Vector4Generic Pink() { return Vector4Generic(1.0f,0.5f,0.5f,1.0f); }
	static Vector4Generic Violet() { return Vector4Generic(0.5f,0.0f,1.0f,1.0f); }
	static Vector4Generic Blue() { return Vector4Generic(0.0f,0.0f,1.0f,1.0f); }
	static Vector4Generic Green() { return Vector4Generic(0.0f,1.0f,0.0f,1.0f); }
	static Vector4Generic White() { return Vector4Generic(1.0f,1.0f,1.0f,1.0f); }
	static Vector4Generic LightGray() { return Vector4Generic(0.8f,0.8f,0.8f,1.0f); }
	static Vector4Generic Gray() { return Vector4Generic(0.5f,0.5f,0.5f,1.0f); }
	static Vector4Generic DarkGray() { return Vector4Generic(0.2f,0.2f,0.2f,1.0f); }
	static Vector4Generic Black() { return Vector4Generic(0.0f,0.0f,0.0f,1.0f); }
	static Vector4Generic Brown() { return Vector4Generic(0.4f,0.2f,0.0f,1.0f); }
	static Vector4Generic Transparent() { return Vector4Generic(0.0f); }
	
	static Vector4Generic<T> Lerp(const Vector4Generic<T> & from, const Vector4Generic<T> & to, float delta) {
		return Vector4Generic<T>(bg::math::lerp(from._v[0],to._v[0],delta),
								  bg::math::lerp(from._v[1],to._v[1],delta),
								  bg::math::lerp(from._v[2],to._v[2],delta),
								  bg::math::lerp(from._v[3],to._v[3],delta));
	}


	Vector4Generic() { _v[0] = static_cast<T>(0); _v[1] = static_cast<T>(0); _v[2] = static_cast<T>(0); _v[3] = static_cast<T>(0); }
	Vector4Generic(T s) { _v[0] = s; _v[1] = s; _v[2] = s; _v[3] = s; }
	Vector4Generic(T x, T y, T z, T w) { _v[0]=x; _v[1]=y; _v[2]=z; _v[3]=w; }
	Vector4Generic(const T * v) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; _v[3] = v[3]; }
	Vector4Generic(const Vector4Generic<T> & v) { _v[0] = v._v[0]; _v[1] = v._v[1]; _v[2] = v._v[2]; _v[3] = v._v[3]; }
	Vector4Generic(const Vector3Generic<T> & v, T w) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; _v[3] = w; }
	Vector4Generic(const Vector2 & v, T z, T w) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = z; _v[3] = w; }
	Vector4Generic(const Scalar & s)
		{ _v[0] = static_cast<T>(s.value()); _v[1] = static_cast<T>(s.value()); _v[2] = static_cast<T>(s.value()); _v[3] = static_cast<T>(s.value()); }
	Vector4Generic(const Scalar & x, const Scalar & y, const Scalar & z, const Scalar & w)
		{ _v[0]=static_cast<T>(x.value()); _v[1]=static_cast<T>(y.value()); _v[2]=static_cast<T>(z.value()); _v[3]=static_cast<T>(w.value()); }

	inline T * raw() { return _v; }
	inline const T * raw() const { return _v; }
	inline int length() const { return 4; }
	
	inline float distance(const Vector4Generic<T> & v) {
		Vector4Generic<T> v3(_v[0] - v._v[0],
							 _v[1] - v._v[1],
							 _v[2] - v._v[2],
							 _v[3] - v._v[3]);
		return v3.magnitude();
	}

	inline float magnitude() { return sqrtf(static_cast<float>(_v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2] + _v[3]*_v[3])); }
	inline Vector4Generic & normalize() {
		T m = magnitude();
		_v[0]=_v[0]/m; _v[1]=_v[1]/m; _v[2]=_v[2]/m; _v[3]=_v[3]/m;
		return *this;
	}
	
	inline T dot(const Vector4Generic<T> & v2) const {
		return _v[0] * v2._v[0] + _v[1] * v2._v[1] + _v[2] * v2._v[2] + _v[3] * v2._v[3];
	}

	inline T operator[](int i) const { return _v[i]; }
	inline bool operator==(const Vector4Generic & v) const { return _v[0]==v._v[0] && _v[1]==v._v[1] && _v[2]==v._v[2] && _v[3]==v._v[3]; }
	inline bool operator<(const Vector4Generic<T> & v) const { return _v[0]<v._v[0] && _v[1]<v._v[1] && _v[2]<v._v[2] && _v[3]<v._v[3]; }
	inline bool operator>(const Vector4Generic<T> & v) const { return _v[0]>v._v[0] && _v[1]>v._v[1] && _v[2]>v._v[2] && _v[3]>v._v[3]; }
	inline bool operator<=(const Vector4Generic<T> & v) const { return _v[0]<=v._v[0] && _v[1]<=v._v[1] && _v[2]<=v._v[2] && _v[3]<=v._v[3]; }
	inline bool operator>=(const Vector4Generic<T> & v) const { return _v[0]>=v._v[0] && _v[1]>=v._v[1] && _v[2]>=v._v[2] && _v[3]>=v._v[3]; }
	inline bool operator!=(const Vector4Generic & v) const { return _v[0]!=v._v[0] || _v[1]!=v._v[1] || _v[2]!=v._v[2] || _v[3]!=v._v[3]; }
	inline void operator=(const Vector4Generic & v) { _v[0] = v._v[0]; _v[1] = v._v[1]; _v[2] = v._v[2]; _v[3] = v._v[3]; }
	inline void operator=(const T * v) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; _v[3] = v[3]; }

	inline Vector4Generic operator+(const Vector4Generic &v) const { return Vector4Generic(_v[0]+v._v[0],_v[1]+v._v[1],_v[2]+v._v[2],_v[3]+v._v[3]); }
	inline Vector4Generic operator-(const Vector4Generic &v) const { return Vector4Generic(_v[0]-v._v[0],_v[1]-v._v[1],_v[2]-v._v[2],_v[3]-v._v[3]); }
	inline T operator*(const Vector4Generic &v) const { return _v[0] * v._v[0] + _v[1] * v._v[1] + _v[2] * v._v[2] + + _v[3] * v._v[3]; }
	inline Vector4Generic operator/(T scalar) const { return Vector4Generic(_v[0]/scalar,_v[1]/scalar,_v[2]/scalar,_v[3]/scalar); }

	inline std::string toString() const {
		std::stringstream ss;
		ss	<< '[' << _v[0] << "," << _v[1] << "," << _v[2] << "," << _v[3] << ']';
		return ss.str();
	}

	inline void set(T v) { _v[0] = v; _v[1] = v; _v[2] = v; _v[3] = v; }
	inline void set(T x, T y, T z, T w) { _v[0] = x; _v[1] = y; _v[2] = z; _v[3] = w; }
	inline void set(const Scalar & s)
		{ _v[0] = static_cast<T>(s.value()); _v[1] = static_cast<T>(s.value()); _v[2] = static_cast<T>(s.value()); _v[3] = static_cast<T>(s.value()); }
	inline void set(const Scalar & x, const Scalar & y, const Scalar & z, const Scalar & w)
		{ _v[0] = static_cast<T>(x.value()); _v[1] = static_cast<T>(y.value()); _v[2] = static_cast<T>(z.value()); _v[3] = static_cast<T>(w.value()); }
	
	inline T x() const { return _v[0]; }
	inline T y() const { return _v[1]; }
	inline T z() const { return _v[2]; }
	inline T w() const { return _v[3]; }

	inline void x(T v) { _v[0] = v; }
	inline void y(T v) { _v[1] = v; }
	inline void z(T v) { _v[2] = v; }
	inline void w(T v) { _v[3] = v; }
	
	inline void x(const Scalar & v) { _v[0] = static_cast<T>(v.value()); }
	inline void y(const Scalar & v) { _v[1] = static_cast<T>(v.value()); }
	inline void z(const Scalar & v) { _v[2] = static_cast<T>(v.value()); }
	inline void w(const Scalar & v) { _v[3] = static_cast<T>(v.value()); }

	inline T r() const { return _v[0]; }
	inline T g() const { return _v[1]; }
	inline T b() const { return _v[2]; }
	inline T a() const { return _v[3]; }

	inline void r(T v) { _v[0] = v; }
	inline void g(T v) { _v[1] = v; }
	inline void b(T v) { _v[2] = v; }
	inline void a(T v) { _v[3] = v; }
	

	inline Vector2Generic<T> xy() const { return Vector2Generic<T>(x(),y()); }
	inline Vector2Generic<T> xz() const { return Vector2Generic<T>(x(),z()); }
	inline Vector2Generic<T> xw() const { return Vector2Generic<T>(x(),w()); }
	inline Vector3Generic<T> xyz() const { return Vector3Generic<T>(x(),y(),z()); }

	inline T width() const { return _v[2]; }
	inline T height() const { return _v[3]; }

	inline void width(T v) { _v[2] = v; }
	inline void height(T v) { _v[3] = v; }
	inline void width(const Scalar & v) { _v[2] = static_cast<T>(v.value()); }
	inline void height(const Scalar & v) { _v[3] = static_cast<T>(v.value()); }

	inline float aspectRatio() const { return _v[3]!=static_cast<T>(0) ? static_cast<float>(_v[2])/static_cast<float>(_v[3]):1.0f; }

	inline Vector4Generic<T> & clamp(T min, T max) {
		_v[0] = number::clamp<T>(_v[0], min, max);
		_v[1] = number::clamp<T>(_v[1], min, max);
		_v[2] = number::clamp<T>(_v[2], min, max);
		_v[3] = number::clamp<T>(_v[3], min, max);
		return *this;
	}
	
	inline bool isNan() {
		return isnan(_v[0]) || isnan(_v[1]) || isnan(_v[2]) || isnan(_v[3]);
	}

	inline bool isZero() {
		return _v[0] == 0.0f && _v[1] == 0.0f && _v[2] == 0.0f && _v[3] == 0.0f;
	}

	inline Vector4Generic<T> & scale(T scale) {
		_v[0] *= scale; _v[1] *= scale; _v[2] *= scale; _v[3] *= scale;
		return *this;
	}
	inline Vector4Generic<T> & scale(const Scalar & scale) {
		_v[0] *= static_cast<T>(scale.value());
		_v[1] *= static_cast<T>(scale.value());
		_v[2] *= static_cast<T>(scale.value());
		_v[3] *= static_cast<T>(scale.value());
		return *this;
	}
 
    inline Vector3Generic<T> rgb() const {
        return Vector3Generic<T>(_v[0], _v[1], _v[2]);
    }
    
	// Viewport contains 2D point
	inline bool containsPoint(const Vector2Generic<T> & p) const {
		return _v[0] <= p.x() && _v[0] + _v[2] >= p.x() &&
			_v[1] <= p.y() && _v[1] + _v[3] >= p.y();
	}

	static Vector4Generic<T> Min(const Vector4Generic<T> & v1, const Vector4Generic<T> & v2) {
		Vector4Generic<T> result;
		
		result._v[0] = bg::math::min(v1._v[0], v2._v[0]);
		result._v[1] = bg::math::min(v1._v[1], v2._v[1]);
		result._v[2] = bg::math::min(v1._v[2], v2._v[2]);
		result._v[3] = bg::math::min(v1._v[3], v2._v[3]);
		
		return result;
	}
	
	static Vector4Generic<T> Max(const Vector4Generic<T> & v1, const Vector4Generic<T> & v2) {
		Vector4Generic<T> result;
		
		result._v[0] = bg::math::max(v1._v[0], v2._v[0]);
		result._v[1] = bg::math::max(v1._v[1], v2._v[1]);
		result._v[2] = bg::math::max(v1._v[2], v2._v[2]);
		result._v[3] = bg::math::max(v1._v[3], v2._v[3]);
	
		return result;
	}
	
protected:
	T _v[4];
};

template <class T>
inline Vector4Generic<T> operator + (const Vector4Generic<T> & v1, const Vector4Generic<T> & v2) {
	return Vector4Generic<T>(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z(), v1.w() + v2.w());
}
template <class T>
inline Vector4Generic<T> operator - (const Vector4Generic<T> & v1, const Vector4Generic<T> & v2) {
	return Vector4Generic<T>(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z(), v1.w() - v2.w());
}
template <class T>
inline Vector4Generic<T> operator * (const Vector4Generic<T> & v1, const Vector4Generic<T> & v2) {
	return Vector4Generic<T>(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z(), v1.w() * v2.w());
}
template <class T>
inline Vector4Generic<T> operator / (const Vector4Generic<T> & v1, const Vector4Generic<T> & v2) {
	return Vector4Generic<T>(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z(), v1.w() / v2.w());
}
template <class T>
inline Vector4Generic<T> operator * (const Vector4Generic<T> & v1, T s) {
	return Vector4Generic<T>(v1.x() * s, v1.y() * s, v1.z() * s, v1.w() * s);
}
template <class T>
inline Vector4Generic<T> operator * (T s, const Vector4Generic<T> & v1) {
	return Vector4Generic<T>(v1.x() * s, v1.y() * s, v1.z() * s, v1.w() * s);
}
template <class T>
inline Vector4Generic<T> operator / (const Vector4Generic<T> & v1, T s) {
	return Vector4Generic<T>(v1.x() / s, v1.y() / s, v1.z() / s, v1.w() / s);
}

typedef Vector4Generic<short> Vector4s;
typedef Vector4Generic<int> Vector4i;
typedef Vector4Generic<long> Vector4l;
typedef Vector4Generic<unsigned short> Vector4us;
typedef Vector4Generic<unsigned int> Vector4ui;
typedef Vector4Generic<unsigned long> Vector4ul;
typedef Vector4Generic<float> Vector4;
typedef Vector4Generic<double> Vector4d;

typedef Vector4 Color;
typedef Vector3 Size3D;
typedef Vector2 Size2D;
typedef Vector3i Size3Di;
typedef Vector2i Size2Di;
typedef Vector4i Viewport;
typedef Vector4i Rect;
typedef Vector2 Position2D;
typedef Vector2i Position2Di;

template <class T>
class Bounds2Generic {
public:
	Bounds2Generic() {
		T min = bg::math::number::minValue(static_cast<T>(0));
		T max = bg::math::number::maxValue(static_cast<T>(0));
		_v[0] = min;
		_v[1] = max;
		_v[2] = min;
		_v[3] = max;
	}
	Bounds2Generic(T s) { _v[0] = s; _v[1] = s; _v[2] = s; _v[3] = s; }
	Bounds2Generic(T left, T right, T bottom, T top) { _v[0]=left; _v[1]=right; _v[2]=bottom; _v[3]=top; }
	Bounds2Generic(const T * v) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; _v[3] = v[3]; }
	Bounds2Generic(const Scalar & s)
		{ _v[0] = static_cast<T>(s.value()); _v[1] = static_cast<T>(s.value()); _v[2] = static_cast<T>(s.value()); _v[3] = static_cast<T>(s.value()); }
	Bounds2Generic(const Scalar & left, const Scalar & right, const Scalar & bottom, const Scalar & top)
		{ _v[0] = static_cast<T>(left.value()); _v[1] = static_cast<T>(right.value()); _v[2] = static_cast<T>(bottom.value()); _v[3] = static_cast<T>(top.value()); }
	
	
	inline T * raw() { return _v; }
	inline const T * raw() const { return _v; }
	inline int length() const { return 6; }
	
	inline bool valid() { return !isNan() && left()<right() && bottom()<top(); }
	
	inline T operator[](int i) const { return _v[i]; }
	inline bool operator==(const Bounds2Generic & v) const { return _v[0]==v._v[0] && _v[1]==v._v[1] && _v[2]==v._v[2] && _v[3]==v._v[3]; }
	inline bool operator!=(const Bounds2Generic & v) const { return _v[0]!=v._v[0] || _v[1]!=v._v[1] || _v[2]!=v._v[2] || _v[3]!=v._v[3]; }
	inline void operator=(const Bounds2Generic & v) { _v[0] = v._v[0]; _v[1] = v._v[1]; _v[2] = v._v[2]; _v[3] = v._v[3]; }
	inline void operator=(const T * v) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; _v[3] = v[3]; }
	
	inline std::string toString() const {
		std::stringstream ss;
		ss	<< '[' << _v[0] << "," << _v[1] << "," << _v[2] << "," << _v[3] << ']';
		return ss.str();
	}
	
	inline void set(T v) { _v[0] = v; _v[1] = v; _v[2] = v; _v[3] = v; }
	inline void set(T left, T right, T bottom, T top) { _v[0] = left; _v[1] = right; _v[2] = bottom; _v[3] = top; }
	inline void set(const Scalar & s)
		{ _v[0] = static_cast<T>(s.value()); _v[1] = static_cast<T>(s.value()); _v[2] = static_cast<T>(s.value()); _v[3] = static_cast<T>(s.value()); }
	inline void set(const Scalar & left, const Scalar & right, const Scalar & bottom, const Scalar & top)
		{ _v[0] = static_cast<T>(left.value()); _v[1] = static_cast<T>(right.value()); _v[2] = static_cast<T>(bottom.value()); _v[3] = static_cast<T>(top.value()); }
	
	
	inline T left() const { return _v[0]; }
	inline T right() const { return _v[1]; }
	inline T bottom() const { return _v[2]; }
	inline T top() const { return _v[3]; }
	
	inline void left(T v) { _v[0] = v; }
	inline void right(T v) { _v[1] = v; }
	inline void bottom(T v) { _v[2] = v; }
	inline void top(T v) { _v[3] = v; }
	
	inline void left(const Scalar & v) { _v[0] = static_cast<T>(v.value()); }
	inline void right(const Scalar & v) { _v[1] = static_cast<T>(v.value()); }
	inline void bottom(const Scalar & v) { _v[2] = static_cast<T>(v.value()); }
	inline void top(const Scalar & v) { _v[3] = static_cast<T>(v.value()); }
	
	inline T width() const { return bg::math::abs(right() - left()); }
	inline T height() const { return bg::math::abs(top() - bottom()); }
	
	inline bool isNan() {
		return isnan(_v[0]) || isnan(_v[1]) || isnan(_v[2]) || isnan(_v[3]);
	}
	
	inline bool isInBounds(const Vector2Generic<T> & v) {
		return v.x()>=_v[0] && v.x()<=_v[1] && v.y()>=_v[2] && v.y()<=_v[3];
	}
	
protected:
	T _v[4];
};

typedef Bounds2Generic<short> Bounds2s;
typedef Bounds2Generic<int> Bounds2i;
typedef Bounds2Generic<long> Bounds2l;
typedef Bounds2Generic<unsigned short> Bounds2us;
typedef Bounds2Generic<unsigned int> Bounds2ui;
typedef Bounds2Generic<unsigned long> Bounds2ul;
typedef Bounds2Generic<float> Bounds2;
typedef Bounds2Generic<double> Bounds2d;

template <class T>
class BoundsGeneric {
public:
	BoundsGeneric() {
		T min = bg::math::number::minValue(static_cast<T>(0));
		T max = bg::math::number::maxValue(static_cast<T>(0));
		_v[0] = min;
		_v[1] = max;
		_v[2] = min;
		_v[3] = max;
		_v[4] = min;
		_v[5] = max;
	}
	BoundsGeneric(T s) { _v[0] = s; _v[1] = s; _v[2] = s; _v[3] = s; _v[4] = s; _v[5] = s; }
	BoundsGeneric(T left, T right, T bottom, T top, T back, T front) { _v[0]=left; _v[1]=right; _v[2]=bottom; _v[3]=top; _v[4]=back; _v[5]=front; }
	BoundsGeneric(const T * v) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; _v[3] = v[3]; _v[4] = v[4]; _v[5] = v[5]; }
	BoundsGeneric(const Scalar & s) {
		_v[0] = static_cast<T>(s.value());
		_v[1] = static_cast<T>(s.value());
		_v[2] = static_cast<T>(s.value());
		_v[3] = static_cast<T>(s.value());
		_v[4] = static_cast<T>(s.value());
		_v[5] = static_cast<T>(s.value());
	}
	BoundsGeneric(const Scalar & left, const Scalar & right, const Scalar & bottom, const Scalar & top, const Scalar & back, const Scalar & front) {
		_v[0] = static_cast<T>(left.value());
		_v[1] = static_cast<T>(right.value());
		_v[2] = static_cast<T>(bottom.value());
		_v[3] = static_cast<T>(top.value());
		_v[4] = static_cast<T>(back.value());
		_v[5] = static_cast<T>(front.value());
	}

	
	inline T * raw() { return _v; }
	inline const T * raw() const { return _v; }
	inline int length() const { return 6; }
	
	inline bool valid() { return !isNan() && left()<right() && bottom()<top() && back()<front(); }
	
	inline T operator[](int i) const { return _v[i]; }
	inline bool operator==(const BoundsGeneric & v) const { return _v[0]==v._v[0] && _v[1]==v._v[1] && _v[2]==v._v[2] && _v[3]==v._v[3] && _v[4]==v._v[4] && _v[5]==v._v[5]; }
	inline bool operator!=(const BoundsGeneric & v) const { return _v[0]!=v._v[0] || _v[1]!=v._v[1] || _v[2]!=v._v[2] || _v[3]!=v._v[3] || _v[4]!=v._v[4] || _v[5]!=v._v[5]; }
	inline void operator=(const BoundsGeneric & v) { _v[0] = v._v[0]; _v[1] = v._v[1]; _v[2] = v._v[2]; _v[3] = v._v[3]; _v[4]=v._v[4];  _v[5]=v._v[5]; }
	inline void operator=(const T * v) { _v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; _v[3] = v[3]; _v[4] = v[4]; _v[5] = v[5]; }
	
	inline std::string toString() const {
		std::stringstream ss;
		ss	<< '[' << _v[0] << "," << _v[1] << "," << _v[2] << "," << _v[3] << "," << _v[4] << "," << _v[5] << ']';
		return ss.str();
	}
	
	inline void set(T v) { _v[0] = v; _v[1] = v; _v[2] = v; _v[3] = v; _v[4] = v; _v[5] = v; }
	inline void set(T left, T right, T bottom, T top, T back, T front) { _v[0] = left; _v[1] = right; _v[2] = bottom; _v[3] = top;  _v[4] = back; _v[5] = front; }
	inline void set(const Scalar & s) {
		_v[0] = static_cast<T>(s.value());
		_v[1] = static_cast<T>(s.value());
		_v[2] = static_cast<T>(s.value());
		_v[3] = static_cast<T>(s.value());
		_v[4] = static_cast<T>(s.value());
		_v[5] = static_cast<T>(s.value());
	}
	inline void set(const Scalar & left, const Scalar & right, const Scalar & bottom, const Scalar & top, const Scalar & back, const Scalar & front) {
		_v[0] = static_cast<T>(left.value());
		_v[1] = static_cast<T>(right.value());
		_v[2] = static_cast<T>(bottom.value());
		_v[3] = static_cast<T>(top.value());
		_v[4] = static_cast<T>(back.value());
		_v[5] = static_cast<T>(front.value());
	}

	
	inline T left() const { return _v[0]; }
	inline T right() const { return _v[1]; }
	inline T bottom() const { return _v[2]; }
	inline T top() const { return _v[3]; }
	inline T back() const { return _v[4]; }
	inline T front() const { return _v[5]; }
	
	inline void left(T v) { _v[0] = v; }
	inline void right(T v) { _v[1] = v; }
	inline void bottom(T v) { _v[2] = v; }
	inline void top(T v) { _v[3] = v; }
	inline void back(T v) { _v[4] = v; }
	inline void front(T v) { _v[5] = v; }
	
	inline void left(const Scalar & v) { _v[0] = static_cast<T>(v.value()); }
	inline void right(const Scalar & v) { _v[1] = static_cast<T>(v.value()); }
	inline void bottom(const Scalar & v) { _v[2] = static_cast<T>(v.value()); }
	inline void top(const Scalar & v) { _v[3] = static_cast<T>(v.value()); }
	inline void back(const Scalar & v) { _v[4] = static_cast<T>(v.value()); }
	inline void front(const Scalar & v) { _v[5] = static_cast<T>(v.value()); }
	
	inline T width() const { return bg::math::abs(right() - left()); }
	inline T height() const { return bg::math::abs(top() - bottom()); }
	inline T depth() const { return bg::math::abs(front() - back()); }
	
	inline bool isNan() {
		return isnan(_v[0]) || isnan(_v[1]) || isnan(_v[2]) || isnan(_v[3]) || isnan(_v[4]) || isnan(_v[5]);
	}
	
	inline bool isInBounds(const Vector3Generic<T> & v) {
		return v.x()>=_v[0] && v.x()<=_v[1] && v.y()>=_v[2] && v.y()<=_v[3] && v.z()>=_v[4] && v.z()<=_v[5];
	}
	
protected:
	T _v[6];
};
	
	
typedef BoundsGeneric<short> Boundss;
typedef BoundsGeneric<int> Boundsi;
typedef BoundsGeneric<long> Boundsl;
typedef BoundsGeneric<unsigned short> Boundsus;
typedef BoundsGeneric<unsigned int> Boundsui;
typedef BoundsGeneric<unsigned long> Boundsul;
typedef BoundsGeneric<float> Bounds;
typedef BoundsGeneric<double> Boundsd;

typedef std::vector<float> FloatVector;
typedef std::vector<unsigned int> UIntVector;

}
}

#endif
