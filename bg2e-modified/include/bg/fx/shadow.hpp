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


#ifndef _bg2e_fx_shadow_hpp_
#define _bg2e_fx_shadow_hpp_

#include <bg/base/effect.hpp>
#include <bg/base/light.hpp>
#include <bg/engine/openglCore/shader.hpp>
#include <bg/tools/shader_source.hpp>
#include <bg/render/shadow_map.hpp>

namespace bg {
namespace fx {

class BG2E_EXPORT Shadow : public bg::base::Effect {
public:
	Shadow(bg::base::Context *);

	virtual void activate();
	virtual void bindPolyList(bg::base::PolyList *);
	virtual void unbind();

	inline void setLight(bg::base::Light * light) { _light = light; }
	inline const bg::base::Light * light() const { return _light.getPtr(); }
	inline void setLightTransform(const bg::math::Matrix4 & t) { _lightTransform = t; }
	inline const bg::math::Matrix4 & lightTransform() const { return _lightTransform; }
	inline void setShadowMap(bg::render::ShadowMap * sm) { _shadowMap = sm; }
	inline const bg::render::ShadowMap * shadowMap() const { return _shadowMap.getPtr(); }

protected:
	virtual ~Shadow();

	void buildGL(bool);
	void activateGL(bool);

	bg::ptr<bg::engine::openglCore::Shader> _glShader;
	bg::ptr<bg::base::Light> _light;
	bg::math::Matrix4 _lightTransform;
	bg::ptr<bg::render::ShadowMap> _shadowMap;

	void getVertexShader(bg::tools::ShaderSource &);
	void getFragmentShader(bg::tools::ShaderSource &);
};

}
}

#endif
