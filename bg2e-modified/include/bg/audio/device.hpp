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


#ifndef _bg2e_audio_device_hpp_
#define _bg2e_audio_device_hpp_

#include <bg/base/referenced_pointer.hpp>

#include <vector>
#include <string>

namespace bg {
namespace audio {

typedef std::string DeviceId;
typedef std::vector<DeviceId> DeviceIdVector;

BG2E_EXPORT extern DeviceId kDefaultDeviceId;

class BG2E_EXPORT Device : public bg::base::ReferencedPointer {
public:
	static void EnumerateDevices(DeviceIdVector & result);
	
	Device();
	
	void open(const DeviceId & device = kDefaultDeviceId);
	void close();

	inline bool valid() const { return _implDevice!=nullptr; }
	
	template <class T> inline T * impl() { return bg::impl_cast<T*>(_implDevice); }

protected:
	virtual ~Device();
	
	bg::plain_ptr _implDevice;
};

	
}
}

#endif