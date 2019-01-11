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


#ifndef _bg2e_audio_source_hpp_
#define _bg2e_audio_source_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/math/vector.hpp>
#include <bg/audio/buffer.hpp>

namespace bg {
namespace audio {

class BG2E_EXPORT Source : public bg::base::ReferencedPointer {
public:
	Source();
	
	void create();
	void destroy();
	
	inline bool valid() const { return _impl>0; }
	
	void setPosition(const bg::math::Vector3 &);
	void setVelocity(const bg::math::Vector3 &);
	void setPitch(float);
	void setGain(float);
	void setLooping(bool);
	
	inline const bg::math::Vector3 & position() const { return _pos; }
	inline const bg::math::Vector3 & velocity() const { return _velocity; }
	inline float pitch() const { return _pitch; }
	inline float gain() const { return _gain; }
	inline bool loop() const { return _loop; }

	void bindBuffer(Buffer * buffer);
	void play();
	void pause();
	void stop();
	void rewind();

	bool isPlaying();
	
protected:
	virtual ~Source();
	
	uint32_t _impl  = 0;
	
	bg::math::Vector3 _pos;
	bg::math::Vector3 _velocity;
	float _pitch = 1.0f;
	float _gain = 1.0f;
	bool _loop = false;
	bg::ptr<Buffer> _buffer;
};

}
}

#endif