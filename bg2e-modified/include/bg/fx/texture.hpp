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


#ifndef _bg2e_fx_texture_hpp_
#define _bg2e_fx_texture_hpp_

#include <bg/base/texture_effect.hpp>

#include <bg/engine/directx11/shader.hpp>
#include <bg/engine/openglCore/shader.hpp>

namespace bg {
namespace fx {

class BG2E_EXPORT Texture : public bg::base::TextureEffect {
public:
	Texture(bg::base::Context *);


	virtual void activate(bg::base::Texture *);
	virtual void bindPolyList(bg::base::PolyList *);
	virtual void unbind();

protected:

	virtual ~Texture();

	void buildDX11();
	void buildGL(bool mobile);

	void activateDX11(bg::base::Texture * tex);
	void activateGL(bool mobile, bg::base::Texture * tex);

	bg::ptr<bg::engine::directx11::VertexShader> _dxVertexShader;
	bg::ptr<bg::engine::directx11::PixelShader> _dxPixelShader;

	bg::ptr<bg::engine::openglCore::Shader> _glShader;
};

}
}

#endif
