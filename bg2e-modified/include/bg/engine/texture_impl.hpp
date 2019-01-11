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

#ifndef _bg2e_engine_texture_impl_hpp_
#define _bg2e_engine_texture_impl_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/engine/ogl_impl.hpp>

namespace bg {
namespace base {

// Texture forward declaration
class Texture;

}

namespace engine {

class BG2E_EXPORT TextureImpl : public bg::base::ReferencedPointer,
								public bg::engine::OpenGLImpl {
public:
	TextureImpl(bg::base::Texture *);
	
	virtual void create(const unsigned char *, int width, int height, int bpp) = 0;
	virtual void createCubemap(const std::vector<const unsigned char *> data, int w, int h, int bpp) {
		throw bg::base::CompatibilityException("This rendering engine does not support cubemaps"); 
	}
	virtual void destroy() = 0;
	virtual void read(unsigned char *& buffer, int width, int height, int bpp) = 0;
	virtual void invalidate() = 0;
	
protected:
	virtual ~TextureImpl();
	
	base::Texture * _texture;
};

}
}
#endif
