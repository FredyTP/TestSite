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

#ifndef _bg2e_engine_directx_texture_impl_hpp_
#define _bg2e_engine_directx_texture_impl_hpp_

#include <bg/engine/texture_impl.hpp>
#include <bg/base/texture.hpp>

#include <bg/engine/directx_includes.hpp>

namespace bg {
namespace engine {
namespace directx11 {

class BG2E_EXPORT TextureImpl : public bg::engine::TextureImpl {
public:
	TextureImpl(bg::base::Texture *);
	
	virtual void create(const unsigned char *, int width, int height, int bpp);
	virtual void destroy();

	// TODO: implement directx 11 read texture
	virtual void read(unsigned char *& data, int width, int height, int bpp) { }

	virtual void invalidate() {};

#if BG2E_DIRECTX_AVAILABLE==1
	inline ID3D11Texture2D * dxTexture() { return _dxTexture; }
	inline ID3D11ShaderResourceView * dxTextureView() { return _dxTextureView; }
	inline ID3D11SamplerState * dxSampleState() { return _dxSampleState; }
#else
	inline void * dxTexture() { return nullptr; }
	inline void * dxTextureView() { return nullptr; }
	inline void * dxSampleState() { return nullptr; }
#endif

protected:
	virtual ~TextureImpl();

#if BG2E_DIRECTX_AVAILABLE==1
	ID3D11Texture2D * _dxTexture;
	ID3D11ShaderResourceView * _dxTextureView;
	ID3D11SamplerState * _dxSampleState;
#endif
};

}
}
}

#endif
