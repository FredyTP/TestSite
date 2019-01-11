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

#ifndef _bg2e_engine_openglCore_texture_impl_hpp_
#define _bg2e_engine_openglCore_texture_impl_hpp_

#include <bg/engine/texture_impl.hpp>
#include <bg/base/texture.hpp>
#include <bg/engine/ogl_impl.hpp>
#include <bg/engine/openglCore/opengl_state.hpp>

namespace bg {
namespace engine {
namespace openglCore {

class BG2E_EXPORT TextureImpl : public bg::engine::TextureImpl {
public:
	TextureImpl(bg::base::Texture *);
	
	virtual void create(const unsigned char *, int width, int height, int bpp);
	virtual void createCubemap(const std::vector<const unsigned char *> data, int w, int h, int bpp);
	virtual void destroy();
	virtual void read(unsigned char *& buffer, int width, int height, int bpp);
	
	static void ClearActive();
	void setActive(int textureUnit);
	void bindTexture2D();
	
	// Use this to create the texture manually
	inline void setTextureName(uint32_t name) { _textureName = name; }
	inline uint32_t textureName() const { return _textureName; }
	
	virtual void invalidate();

protected:
	virtual ~TextureImpl();
	
	uint32_t _textureName;
};

}
}
}

#endif
