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

#include <bg/tools/uuid.hpp>

#include <bg/platform.hpp>

extern "C"
{
#if PLATFORM_WINDOWS==1
#include <Rpc.h>
#else
#include <uuid/uuid.h>
#endif
}

namespace bg {
namespace tools {

std::string uuid() {
#if PLATFORM_WINDOWS==1
	UUID uuid;
	UuidCreate (&uuid);

	unsigned char * str;
	UuidToStringA (&uuid, &str);

	std::string s((char*)str);

	RpcStringFreeA (&str);
#else
	uuid_t uuid;
	uuid_generate_random (uuid);
	char s[37];
	uuid_unparse (uuid, s);
#endif
	return s;
}

}
}