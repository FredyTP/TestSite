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


#ifndef _bg2e_wnd_directx_context_hpp_
#define _bg2e_wnd_directx_context_hpp_

#include <bg/platform.hpp>

#include <bg/wnd/context.hpp>

namespace bg {
namespace wnd {

class Window;

class BG2E_EXPORT DirectXContext : public wnd::Context {
public:
	DirectXContext(Window * window) :Context(window) {}

	virtual bool createContext();
	virtual void swapBuffers();

	virtual void destroy();

	bg::plain_ptr swapChain() { return _swapChain; }
	bg::plain_ptr device() { return _device; }
	bg::plain_ptr deviceContext() { return _deviceContext; }

protected:
	bg::plain_ptr _swapChain;
	bg::plain_ptr _device;
	bg::plain_ptr _deviceContext;

};

}
}

#endif
