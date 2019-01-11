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


#include <bg/math/utils.hpp>

#ifdef _WIN32
#include <cmath>
#else
#include <math.h>
#endif
#include <cstdlib>

#include <cfloat>
#include <climits>

namespace bg {
namespace math {

const float kPi = 3.14159265359f;
const float kPiOver180 = 0.01745329251994f;
const float k180OverPi = 57.29577951308233f;
const float kPiOver2 = 1.570796326794897f;
const float kPiOver4 = 0.785398163397448f;
const float k2Pi = 6.283185307179586f;
	
namespace trigonometry {

	float degreesToRadians(float d) {
		return d * kPiOver180;
	}

	float radiansToDegrees(float r) {
		return r * k180OverPi;
	}

	float sin(float f) {
		return sinf(f);
	}

	float cos(float f) {
		return cosf(f);
	}

	float tan(float f) {
		return tanf(f);
	}

	float asin(float f) {
		return asinf(f);
	}

	float acos(float f) {
		return acosf(f);
	}

	float cotan(float i) {
		// TODO: Fix this
		return 1.0f / tanf(i);
	}

	float atan(float i) {
		return atanf(i);
	}

	float atan2(float i, float j) {
		return atan2f(i, j);
	}
	
	const float deg = k180OverPi;
	const float rad = 1.0f;

}

namespace number {
	
	short minValue(short val) {
		return SHRT_MIN;
	}
	
	short maxValue(short val) {
		return SHRT_MAX;
	}
	
	int minValue(int val) {
		return INT_MIN;
	}
	
	int maxValue(int val) {
		return INT_MAX;
	}
	
	long minValue(long val) {
		return LONG_MIN;
	}
	
	long maxValue(long val) {
		return LONG_MAX;
	}
	
	long long minValue(long long val) {
		return LLONG_MIN;
	}
	
	long long maxValue(long long val) {
		return LLONG_MAX;
	}
	
	unsigned short minValue(unsigned short val) {
		return 0;
	}
	
	unsigned short maxValue(unsigned short val) {
		return USHRT_MAX;
	}
	
	unsigned int minValue(unsigned int val) {
		return 0;
	}
	
	unsigned int maxValue(unsigned int val) {
		return UINT_MAX;
	}
	
	unsigned long minValue(unsigned long val) {
		return 0;
	}
	
	unsigned long maxValue(unsigned long val) {
		return ULONG_MAX;
	}
	
	unsigned long long minValue(unsigned long long val) {
		return 0;
	}
	
	unsigned long long maxValue(unsigned long long val) {
		return ULLONG_MAX;
	}
	
	float minValue(float val) {
		return -FLT_MAX;
	}
	
	float maxValue(float val) {
		return FLT_MAX;
	}
	
	double minValue(double val) {
		return -DBL_MAX;
	}
	
	double maxValue(double val) {
		return DBL_MAX;
	}
}

float sqrt(float v) {
	return ::sqrt(v);
}

short max(short a,short b) {
	return a>b ? a:b;
}

unsigned short max(unsigned short a,unsigned short b) {
	return a>b ? a:b;
}
	
int max(int a,int b) {
	return a>b ? a:b;
}
	
unsigned int max(unsigned int a,unsigned int b) {
	return a>b ? a:b;
}
	
long max(long a,long b) {
	return a>b ? a:b;
}
	
unsigned long max(unsigned long a,unsigned long b) {
	return a>b ? a:b;
}
	
long long max(long long a,long long b) {
	return a>b ? a:b;
}
	
unsigned long long max(unsigned long long a,unsigned long long b) {
	return a>b ? a:b;
}
	
float max(float a,float b) {
	return a>b ? a:b;
}
	
double max(double a,double b) {
	return a>b ? a:b;
}
	
short min(short a,short b) {
	return a>b ? b:a;
}
	
unsigned short min(unsigned short a,unsigned short b) {
	return a>b ? b:a;
}
	
int min(int a,int b) {
	return a>b ? b:a;
}
	
unsigned int min(unsigned int a,unsigned int b) {
	return a>b ? b:a;
}
	
long min(long a,long b) {
	return a>b ? b:a;
}
	
unsigned long min(unsigned long a,unsigned long b) {
	return a>b ? b:a;
}
	
long long min(long long a,long long b) {
	return a>b ? b:a;
}
	
unsigned long long min(unsigned long long a,unsigned long long b) {
	return a>b ? b:a;
}
	
float min(float a,float b) {
	return a>b ? b:a;
}
	
double min(double a,double b) {
	return a>b ? b:a;
}

short abs(short a) {
	return a<0 ? -a:a;
}
int abs(int a) {
	return a<0 ? -a:a;
}
long abs(long a) {
	return a<0L ? -a:a;
}
long long abs(long long a) {
	return a<0 ? -a:a;
}
float abs(float a) {
	return a<0.0f ? -a:a;
}
double abs(double a) {
	return a<0.0 ? -a:a;
}

float random() {
	return ((float) rand() / (float)(RAND_MAX));
}

float lerp(float from, float to, float t) {
	return (1.0f - t) * from + t * to;
}

bool isPowerOfTwo(int x) {
	return (x!=0 && (x & (~x + 1))==x);
}

int nextPowerOfTwo(int x) {
	return static_cast<int>(pow(2, ceil(log2(static_cast<double>(x)))));
}

float round(float n) {
	return roundf(n);
}

double round(double n) {
	return ::round(n);
}

namespace distance {
	
const float Km = 0.001f;
const float Hm = 0.01f;
const float Dam = 0.1f;
const float meter = 1.0f;
const float dm = 10.0f;
const float cm = 100.0f;
const float mm = 1000.0f;
const float inch = 39.37f;
const float feet = 3.28084f;
const float yard = 1.09361f;
const float mile = 0.000621371f;
	
}
	
}
}
