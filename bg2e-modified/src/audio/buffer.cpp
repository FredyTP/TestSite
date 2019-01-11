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


#include <bg/audio/buffer.hpp>
#include <bg/base/exception.hpp>

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

namespace bg {
namespace audio {

Buffer::Buffer()
{
	
}

Buffer::~Buffer() {
	
}

void Buffer::create() {
	if (valid()) {
		destroy();
	}
	alGenBuffers(1,&_buffer);
	if (alGetError()!=AL_NO_ERROR) {
		throw bg::base::AudioException("Unexpected error: Could not create audio buffer descriptor.");
	}
}

void Buffer::setBufferData(unsigned char * data, int size, Format fmt, int sampleRate) {
	if (!valid()) {
		create();
	}
	ALenum format;
	switch (fmt) {
	case kFormatMono8:
		format = AL_FORMAT_MONO8;
		break;
	case kFormatMono16:
		format = AL_FORMAT_MONO16;
		break;
	case kFormatStereo8:
		format = AL_FORMAT_STEREO8;
		break;
	case kFormatStereo16:
		format = AL_FORMAT_STEREO16;
		break;
	default:
		throw bg::base::AudioException("Invalid buffer data format");
	}

	alBufferData(_buffer, format, data, size, sampleRate);
	if (alGetError()!=ALC_NO_ERROR) {
		throw bg::base::AudioException("Unexpected error setting audio buffer data");
	}
	_size = size;
	_format = fmt;
	_sampleRate = sampleRate;
	delete [] data;
}

void Buffer::destroy() {
	if (valid()) {
		alDeleteBuffers(1, &_buffer);
		_buffer = 0;
		_size = 0;
		_format = kFormatUninitialized;
		_sampleRate = 0;
	}
}



}
}