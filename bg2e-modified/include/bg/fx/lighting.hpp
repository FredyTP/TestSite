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

#ifndef _bg2e_fx_lighting_hpp_
#define _bg2e_fx_lighting_hpp_

#include <bg/base/texture_effect.hpp>
#include <bg/base/light.hpp>
#include <bg/engine/openglCore/shader.hpp>

#include <bg/tools/shader_source.hpp>

namespace bg {
namespace fx {

class BG2E_EXPORT Lighting : public bg::base::TextureEffect {
public:
	Lighting(bg::base::Context *);

	inline void beginRenderLights() { _firstLight = true; }
	virtual void activate(bg::base::TextureRenderSurface * surface);
	virtual void bindPolyList(bg::base::PolyList *);
	virtual void unbind();

	inline void setLight(bg::base::Light * l) { _light = l; }
	inline const bg::base::Light * light() const { return _light.getPtr(); }
	inline void setLightTransform(const bg::math::Matrix4 & m) { _lightTransform = m; }
	inline const bg::math::Matrix4 & lightTransform() const { return _lightTransform; }
	inline void setViewMatrix(const bg::math::Matrix4 & m) { _viewMatrix = m; }
	inline const bg::math::Matrix4 & viewMatrix() const { return _viewMatrix; }
	inline void setShadowTexture(bg::base::Texture * st) { _shadowTexture = st; }
	inline const bg::base::Texture * shadowTexture() const { return _shadowTexture.getPtr(); }
	
	// If the position texture is set, it will be used as position gbuffer, instead of
	// using surface->texture(4) in activate() function
	inline void setPositionTexture(bg::base::Texture * pt) { _positionTexture = pt; }
	inline const bg::base::Texture * positionTexture() const { return _positionTexture.getPtr(); }

	inline void setClearColor(const bg::math::Color & c) { _clearColor = c; }
	inline const bg::math::Color & clearColor() const { return _clearColor; }
	
protected:
	virtual ~Lighting();

	void buildGL(bool);
	void activateGL(bool, bg::base::TextureRenderSurface *);
	void bindPolyListGL(bg::base::PolyList *, bool);
	void unbindGL(bool);

	bg::ptr<bg::engine::openglCore::Shader> _directionalShader;
	bg::ptr<bg::engine::openglCore::Shader> _pointShader;
	bg::ptr<bg::engine::openglCore::Shader> _spotShader;
	bg::ptr<bg::base::Light> _light;
	bg::math::Matrix4 _lightTransform;
	bg::math::Matrix4 _viewMatrix;
	bg::ptr<bg::base::Texture> _shadowTexture;
	bg::ptr<bg::base::Texture> _positionTexture;

	bg::math::Color _clearColor;

	bool _firstLight;

	void getVertexShader(bg::tools::ShaderSource &);
	void getDirectionalLightFShader(bg::tools::ShaderSource &);
	void getPointLightFShader(bg::tools::ShaderSource &);
	void getSpotLightFShader(bg::tools::ShaderSource &);
};

}
}

#endif // !_bg2e_fx_lighting_hpp_