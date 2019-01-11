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


#include <bg/audio/device.hpp>
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

DeviceId kDefaultDeviceId = "";

void Device::EnumerateDevices(DeviceIdVector & result) {
	const ALCchar * devices = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
	ALboolean enumeration;
	enumeration = alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT");
	if(enumeration == AL_FALSE) {
		throw bg::base::AudioException("Unsupported feature: audio device enumeration.");
	}
	else {
		const ALCchar * device = devices, *next = devices + 1;
		size_t len = 0;
		while (device && *device != '\0' && next && *next !='\0') {
			result.push_back(device);
			len = strlen(device);
			device += (len + 1);
			next += (len + 2);
		}
	}
}
	
	Device::Device()
	:_implDevice(nullptr)
{
}

Device::~Device() {
	
}

void Device::open(const DeviceId & device) {
	if (valid()) {
		close();
	}
	
	ALCdevice * dev = nullptr;
	
	if (device.empty()) {
		dev = alcOpenDevice(nullptr);
	}
	else {
		dev = alcOpenDevice(device.c_str());
	}
	
	if (!dev) {
		throw bg::base::AudioCreateException("Could not create audio device.");
	}
	
	_implDevice = dev;
}

void Device::close() {
	if (!valid()) {
		throw bg::base::InvalidStateException("Trying to close an invalid audio device.");
	}
	alcCloseDevice(bg::impl_cast<ALCdevice*>(_implDevice));
}



}
}