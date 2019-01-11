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


#ifndef _bg2e_math_utils_hpp_
#define _bg2e_math_utils_hpp_

#include <bg/export.hpp>

#include <vector>

#define _USE_MATH_DEFINES
#ifdef _WIN32
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#else
#include <math.h>
#endif

namespace bg {
namespace math {
	
BG2E_EXPORT extern const float kPi;
BG2E_EXPORT extern const float kPiOver2;
BG2E_EXPORT extern const float kPiOver4;
BG2E_EXPORT extern const float k2Pi;
BG2E_EXPORT extern const float kPiOver180;
BG2E_EXPORT extern const float k180OverPi;

namespace trigonometry {

	BG2E_EXPORT extern float degreesToRadians(float d);
	BG2E_EXPORT extern float radiansToDegrees(float r);
	BG2E_EXPORT extern float sin(float f);
	BG2E_EXPORT extern float cos(float f);
	BG2E_EXPORT extern float tan(float f);
	BG2E_EXPORT extern float asin(float f);
	BG2E_EXPORT extern float acos(float f);
	BG2E_EXPORT extern float cotan(float i);
	BG2E_EXPORT extern float atan(float i);
	BG2E_EXPORT extern float atan2(float i, float j);
	
	BG2E_EXPORT extern const float deg;
	BG2E_EXPORT extern const float rad;
}

namespace number {
	
	BG2E_EXPORT extern  short minValue(short val);
	BG2E_EXPORT extern  short maxValue(short val);
	BG2E_EXPORT extern  int minValue(int val);
	BG2E_EXPORT extern  int maxValue(int val);
	BG2E_EXPORT extern  long minValue(long val);
	BG2E_EXPORT extern  long maxValue(long val);
	BG2E_EXPORT extern  long long minValue(long long val);
	BG2E_EXPORT extern  long long maxValue(long long val);
	BG2E_EXPORT extern  unsigned short minValue(unsigned short val);
	BG2E_EXPORT extern  unsigned short maxValue(unsigned short val);
	BG2E_EXPORT extern  unsigned int minValue(unsigned int val);
	BG2E_EXPORT extern  unsigned int maxValue(unsigned int val);
	BG2E_EXPORT extern  unsigned long minValue(unsigned long val);
	BG2E_EXPORT extern  unsigned long maxValue(unsigned long val);
	BG2E_EXPORT extern  unsigned long long minValue(unsigned long long val);
	BG2E_EXPORT extern  unsigned long long maxValue(unsigned long long val);
	BG2E_EXPORT extern  float minValue(float val);
	BG2E_EXPORT extern  float maxValue(float val);
	BG2E_EXPORT extern  double minValue(double val);
	BG2E_EXPORT extern  double maxValue(double val);
	
	template <class T> T clamp(T val, T min, T max) { return (val<min ? min:(val>max ? max:val)); }
}

	
BG2E_EXPORT extern  float sqrt(float v);
BG2E_EXPORT extern  short max(short a,short b);
BG2E_EXPORT extern  unsigned short max(unsigned short a,unsigned short b);
BG2E_EXPORT extern  int max(int a,int b);
BG2E_EXPORT extern  unsigned int max(unsigned int a,unsigned int b);
BG2E_EXPORT extern  long max(long a,long b);
BG2E_EXPORT extern  unsigned long max(unsigned long a,unsigned long b);
BG2E_EXPORT extern  long long max(long long a,long long b);
BG2E_EXPORT extern  unsigned long long max(unsigned long long a,unsigned long long b);
BG2E_EXPORT extern  float max(float a,float b);
BG2E_EXPORT extern  double max(double a,double b);
BG2E_EXPORT extern  short min(short a,short b);
BG2E_EXPORT extern  unsigned short min(unsigned short a,unsigned short b);
BG2E_EXPORT extern  int min(int a,int b);
BG2E_EXPORT extern  unsigned int min(unsigned int a,unsigned int b);
BG2E_EXPORT extern  long min(long a,long b);
BG2E_EXPORT extern  unsigned long min(unsigned long a,unsigned long b);
BG2E_EXPORT extern  long long min(long long a,long long b);
BG2E_EXPORT extern  unsigned long long min(unsigned long long a,unsigned long long b);
BG2E_EXPORT extern  float min(float a,float b);
BG2E_EXPORT extern  double min(double a,double b);
BG2E_EXPORT extern  short abs(short a);
BG2E_EXPORT extern  int abs(int a);
BG2E_EXPORT extern  long abs(long a);
BG2E_EXPORT extern  long long abs(long long a);
BG2E_EXPORT extern  float abs(float a);
BG2E_EXPORT extern  double abs(double a);
BG2E_EXPORT extern  float random();
BG2E_EXPORT extern  float lerp(float from, float to, float t);
BG2E_EXPORT extern  bool isPowerOfTwo(int x);
BG2E_EXPORT extern  int nextPowerOfTwo(int x);
BG2E_EXPORT extern	float round(float n);
BG2E_EXPORT extern	double round(double n);

namespace distance {

BG2E_EXPORT extern const float Km;
BG2E_EXPORT extern const float Hm;
BG2E_EXPORT extern const float Dam;
BG2E_EXPORT extern const float meter;
BG2E_EXPORT extern const float dm;
BG2E_EXPORT extern const float cm;
BG2E_EXPORT extern const float mm;
BG2E_EXPORT extern const float inch;
BG2E_EXPORT extern const float feet;
BG2E_EXPORT extern const float yard;
BG2E_EXPORT extern const float mile;

}

class Scalar {
public:
	Scalar() :_value(0.0f) {}
	template <class T>
	Scalar(T v, float factor = distance::meter) :_value(static_cast<float>(v) / factor) {}
	
	inline float value() const { return _value; }
	
	template <class T>
	inline void setValue(T v, float factor = distance::meter) { _value = static_cast<float>(v) / factor; }

	inline Scalar & operator =(const Scalar & s) { _value = s._value; return *this; }
	inline bool operator ==(const Scalar & s) { return _value == s._value; }
	
protected:
	float _value;
};


inline Scalar operator + (const Scalar & v1, const Scalar & v2) {
	return Scalar(v1.value() + v2.value());
}

inline Scalar operator - (const Scalar & v1, const Scalar & v2) {
	return Scalar(v1.value() - v2.value());
}

inline Scalar operator * (const Scalar & v1, const Scalar & v2) {
	return Scalar(v1.value() * v2.value());
}

inline Scalar operator / (const Scalar & v1, const Scalar & v2) {
	return Scalar(v1.value() / v2.value());
}


}
}

#endif
