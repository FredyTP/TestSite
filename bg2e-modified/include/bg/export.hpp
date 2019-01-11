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

#ifndef _bg2e_export_hpp_
#define _bg2e_export_hpp_

#include <bg/platform.hpp>

namespace bg {
	
typedef void * impl_ptr;
template <class T> T impl_cast(void * nativePtr) { return static_cast<T>(nativePtr); }
	
}


#if BG2E_WINDOWS==0

#define BG2E_EXPORT

#else

#pragma warning( disable: 4251 )
#pragma warning( disable: 4275 )

#if defined( BG2E_STATIC_LIB )
#define BG2E_EXPORT

#elif defined( BG2E_DYNAMIC_LIB )
#define BG2E_EXPORT __declspec(dllexport)

#else
#define BG2E_EXPORT __declspec(dllimport)

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib ,"bg2ed")

#else
#pragma comment(lib, "bg2e")

#endif

#endif

#endif

#endif

#endif	// _bg2e_export_hpp_


