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

#ifndef _bg2e_fx_diffuse_texture_hpp_
#define _bg2e_fx_diffuse_texture_hpp_

#include <bg/base/effect.hpp>
#include <bg/engine/openglCore/shader.hpp>
#include <bg/tools/shader_source.hpp>

namespace bg {
namespace fx {

class BG2E_EXPORT DiffuseTexture : public bg::base::Effect {
public:
	DiffuseTexture(bg::base::Context *);
	
	inline void setUseAlpha(bool a) { _useAlpha = a; }
	inline bool useAlpha() const { return _useAlpha; }

	virtual void activate();
	virtual void bindPolyList(bg::base::PolyList *);
	virtual void unbind();
	
protected:
	virtual ~DiffuseTexture();
	
	void buildGL(bool mobile);
	void activateGL(bool mobile);
	void bindPolyListGL(bg::base::PolyList * plist, bool mobile);
	void unbindGL(bool mobile);
	
	bg::ptr<bg::engine::openglCore::Shader> _glShader;
	bool _useAlpha;
	
	void getVertexShader(bg::tools::ShaderSource &);
	void getFragmentShader(bg::tools::ShaderSource &);
};

}
}

#endif
