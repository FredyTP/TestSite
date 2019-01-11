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

#ifndef _bg2e_ios_gles_context_hpp_
#define _bg2e_ios_gles_context_hpp_

#include <bg/base/context.hpp>

#include <bg/engine/openglEs/opengl_state.hpp>

namespace bg {
namespace ios {
	
class GLESContext : public bg::base::Context,
					public bg::engine::openglCore::GLStateObject {
public:
	GLESContext(plain_ptr nativeCtx, plain_ptr view);
	
	virtual bool createContext();
	virtual void makeCurrent();
	virtual void swapBuffers();
	virtual void destroy();
	
	inline plain_ptr nativeCtx() { return _nativeCtx; }
	inline plain_ptr view() { return _view; }
    
protected:
	plain_ptr _nativeCtx;
	plain_ptr _view;
};

}
}

#endif
