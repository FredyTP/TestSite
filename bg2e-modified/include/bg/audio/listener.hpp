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


#ifndef _bg2e_audio_listener_hpp_
#define _bg2e_audio_listener_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/math/vector.hpp>

namespace bg {
namespace audio {

class Listener : public bg::base::ReferencedPointer {
public:
	Listener() :_direction(0.0f, 0.0f, -1.0f), _up(0.0f, 1.0f, 0.0f) {}

	inline void setPosition(const bg::math::Vector3 & pos) { _position = pos; }
	inline void setDirection(const bg::math::Vector3 & dir) { _direction = dir; }
	inline void setUpVector(const bg::math::Vector3 & up) { _up = up; }
	inline void setVelocity(const bg::math::Vector3 & v) { _velocity = v; }
	
	inline bg::math::Vector3 & position() { return _position; }
	inline const bg::math::Vector3 & position() const { return _position; }
	inline bg::math::Vector3 & direction() { return _direction; }
	inline const bg::math::Vector3 & direction() const { return _direction; }
	inline bg::math::Vector3 & up() { return _up; }
	inline const bg::math::Vector3 & up() const { return _up; }
	inline bg::math::Vector3 & velocity() { return _velocity; }
	inline const bg::math::Vector3 & velocity() const { return _velocity; }
	
protected:
	virtual ~Listener() {}
	
	bg::math::Vector3 _position;
	bg::math::Vector3 _direction;
	bg::math::Vector3 _up;
	bg::math::Vector3 _velocity;
};

}
}

#endif
