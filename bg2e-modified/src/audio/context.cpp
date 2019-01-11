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


#include <bg/audio/context.hpp>

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
	
Context::Context(Device * dev)
	:_device(dev)
	,_impl(nullptr)
{
	if (!_device.valid() || !_device->valid()) {
		throw bg::base::AudioCreateException("Could not create audio context: invalid device.");
	}
}

Context::~Context() {
	
}

void Context::create() {
	ALCcontext * ctx = alcCreateContext(_device->impl<ALCdevice>(), nullptr);
	if (!alcMakeContextCurrent(ctx)) {
		throw bg::base::AudioCreateException("Unexpected error creating audio context. Could not set current context.");
	}
	_impl = ctx;
}

bool Context::makeCurrent() {
	if (_impl || alcMakeContextCurrent(impl<ALCcontext>())) {
		return true;
	}
	else {
		return false;
	}
}

void Context::update() {
	if (!_listener.valid()) {
		_listener = new Listener();
	}
	ALfloat orientation[] = {
		_listener->direction().x(), _listener->direction().y(), _listener->direction().z(),
		_listener->up().x(), _listener->up().y(), _listener->up().z(),
	};
	
	alListener3f(AL_POSITION, _listener->position().x(), _listener->position().y(), _listener->position().z());
	alListener3f(AL_VELOCITY, _listener->velocity().x(), _listener->velocity().y(), _listener->velocity().z());
	alListenerfv(AL_ORIENTATION, orientation);
}

void Context::destroy() {
	if (_impl) {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(bg::impl_cast<ALCcontext*>(_impl));
		_impl = nullptr;
	}
}

}
}