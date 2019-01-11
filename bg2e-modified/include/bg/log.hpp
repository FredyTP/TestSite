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

#ifndef _bg2e_base_log_hpp_
#define _bg2e_base_log_hpp_

#include <string>
#include <iostream>
#include <sstream>

#include <bg/export.hpp>
#include <bg/math/vector.hpp>
#include <bg/math/matrix.hpp>
#include <bg/math/quaternion.hpp>
#include <bg/math/utils.hpp>
#include <bg/android/cplusplus_support.hpp>
#include <bg/system/path.hpp>

namespace bg {

class BG2E_EXPORT log {
public:
	enum Level {
		kError = 0,
		kWarning,
		kDebug
	};

	inline log & operator << (char c) { printLog(std::to_string(c)); return *this; }
	inline log & operator << (const std::string & str) { printLog(str); return *this; }
	inline log & operator << (std::string & str) { printLog(str); return *this; }
	inline log & operator << (const char * str) { printLog(str); return *this; }
	inline log & operator << (short c) { printLog(std::to_string(c)); return *this; }
	inline log & operator << (int c) { printLog(std::to_string(c)); return *this; }
	inline log & operator << (long c) { printLog(std::to_string(c)); return *this; }
	inline log & operator << (long long c) { printLog(std::to_string(c)); return *this; }
	inline log & operator << (unsigned char c) { printLog(std::to_string(c)); return *this; }
	inline log & operator << (unsigned short c) { printLog(std::to_string(c)); return *this; }
	inline log & operator << (unsigned int c) { printLog(std::to_string(c)); return *this; }
	inline log & operator << (unsigned long c) { printLog(std::to_string(c)); return *this; }
	inline log & operator << (unsigned long long c) { printLog(std::to_string(c)); return *this; }
	inline log & operator << (float c) { printLog(std::to_string(c)); return *this; }
	inline log & operator << (double c) { printLog(std::to_string(c)); return *this; }

	inline log & operator << (const bg::math::Vector2 & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector3 & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector4 & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector2s & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector3s & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector4s & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector2i & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector3i & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector4i & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector2l & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector3l & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector4l & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector2us & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector3us & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector4us & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector2ui & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector3ui & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector4ui & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector2ul & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector3ul & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector4ul & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector2d & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector3d & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Vector4d & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Bounds2 & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Bounds & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Bounds2s & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Boundss & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Bounds2i & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Boundsi & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Bounds2l & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Boundsl & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Bounds2us & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Boundsus & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Bounds2ui & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Boundsui & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Bounds2ul & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Boundsul & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Bounds2d & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Boundsd & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Matrix3 & v) { printLog(v.toString()); return *this; }
	inline log & operator << (const bg::math::Matrix4 & v) { printLog(v.toString()); return *this; }
 
    inline log & operator << (const bg::math::Scalar & v) { printLog(std::to_string(v.value())); return *this; }

	inline log & operator << (const bg::system::Path & p) { printLog(p.text()); return *this;}

	inline log & operator << (Level l) { _level = l; return *this; }

	log() :_level(kDebug) {}
	log(Level l) :_level(l) {}
	~log();

protected:
	void printLog(const std::string &);

	Level _level = kDebug;
	static Level _currentLevel;
	std::stringstream _out;
};

extern BG2E_EXPORT const std::string endl;

}

#endif
