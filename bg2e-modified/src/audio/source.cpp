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


#include <bg/audio/source.hpp>

#include <bg/base/exception.hpp>

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

#define ASSERT_VALID(impl) if (_impl==0) throw bg::base::InvalidStateException("Invalid source");

namespace bg {
namespace audio {

Source::Source()
{
	
}

Source::~Source() {
	
}

void Source::create() {
	if (valid()) {
		destroy();
	}
	alGenSources(static_cast<ALuint>(1), static_cast<ALuint*>(&_impl));
	alSourcef(static_cast<ALuint>(_impl), AL_PITCH, _pitch);
	alSourcef(static_cast<ALuint>(_impl), AL_GAIN, _gain);
	alSource3f(static_cast<ALuint>(_impl), AL_POSITION, _pos.x(), _pos.y(), _pos.z());
	alSource3f(static_cast<ALuint>(_impl), AL_VELOCITY, _velocity.x(), _velocity.y(), _velocity.z());
	alSourcei(static_cast<ALuint>(_impl), AL_LOOPING, _loop ? AL_TRUE:AL_FALSE);
}

void Source::destroy() {
	if (valid()) {
		alDeleteSources(1, static_cast<ALuint*>(&_impl));
		_impl = 0;
	}
}

void Source::setPosition(const bg::math::Vector3 & p) {
	ASSERT_VALID(_impl);
	_pos = p;
	alSource3f(static_cast<ALuint>(_impl), AL_POSITION, _pos.x(), _pos.y(), _pos.z());
}

void Source::setVelocity(const bg::math::Vector3 & v) {
	ASSERT_VALID(_impl);
	_velocity = v;
	alSource3f(static_cast<ALuint>(_impl), AL_VELOCITY, _velocity.x(), _velocity.y(), _velocity.z());
}

void Source::setPitch(float p) {
	ASSERT_VALID(_impl);
	_pitch = p;
	alSourcef(static_cast<ALuint>(_impl), AL_PITCH, _pitch);
}

void Source::setGain(float g) {
	ASSERT_VALID(_impl);
	_gain = g;
	alSourcef(static_cast<ALuint>(_impl), AL_GAIN, _gain);
}

void Source::setLooping(bool l) {
	ASSERT_VALID(_impl);
	_loop = l;
	alSourcei(static_cast<ALuint>(_impl), AL_LOOPING, _loop ? AL_TRUE:AL_FALSE);
}

void Source::bindBuffer(Buffer * buffer) {
	if (!valid()) {
		throw bg::base::AudioException("Could not bind audio buffer. Invalid source");
	}
	if (!buffer || !buffer->valid()) {
		throw bg::base::AudioException("Could not bind audio buffer. Invalid buffer");
	}
	alSourcei(static_cast<ALuint>(_impl), AL_BUFFER, static_cast<ALuint>(buffer->bufferImpl()));
}

void Source::play() {
	if (!valid()) {
		throw bg::base::AudioException("Could not play audio source. Invalid source");
	}
	alSourcePlay(static_cast<ALuint>(_impl));
}

void Source::pause() {
	if(!valid()) {
		throw bg::base::AudioException("Could not pause audio source. Invalid source");
	}
	alSourcePause(static_cast<ALuint>(_impl));
}

void Source::stop() {
	if(!valid()) {
		throw bg::base::AudioException("Could not stop audio source. Invalid source");
	}
	alSourceStop(static_cast<ALuint>(_impl));
}

void Source::rewind() {
	if(!valid()) {
		throw bg::base::AudioException("Could not rewind audio source. Invalid source");
	}
	alSourceRewind(static_cast<ALuint>(_impl));
}

bool Source::isPlaying() {
	ALint sourceState;
	alGetSourcei(_impl, AL_SOURCE_STATE, &sourceState);
	return sourceState==AL_PLAYING;
}

}
}
