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


#include <bg/system/system.hpp>

#include <bg/platform.hpp>

extern "C"
{
#if BG2E_WINDOWS==1
#include <Rpc.h>
#pragma comment(lib, "RpcRT4.lib")
#endif
}

namespace bg {
namespace system {

bool isBigEndian() {
    unsigned int v = 0x01020304;
	return reinterpret_cast<unsigned char*>(&v)[0] == 1;;
}

bool isLittleEndian() {
	unsigned int v = 0x01020304;
	return reinterpret_cast<unsigned char*>(&v)[0] == 4;
}

Platform currentPlatform() {
#if BG2E_MAC==1 && BG2E_IOS==0
	return Platform::kMac;
#elif BG2E_WINDOWS==1
	return Platform::kWindows;
#elif BG2E_LINUX==1
	return Platform::kLinux;
#elif BG2E_IOS==1
	return Platform::kiOS;
#elif BG2E_ANDROID==1
	return Platform::kAndroid;
#else
	return Platform::kUnknown;
#endif
}

bool isDesktop() {
	return BG2E_MOBILE==0;
}

bool isMobile() {
	return BG2E_MOBILE==1;
}

#if BG2E_WINDOWS==1
std::string uuid() {
	UUID uuid;
	UuidCreate(&uuid);

	unsigned char * str;
	UuidToStringA(&uuid, &str);
	std::string result((char*) &str);
	RpcStringFreeA(&str);
	return result;
}
#endif

}
}