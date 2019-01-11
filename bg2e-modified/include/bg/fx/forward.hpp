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


#ifndef _bg2e_fx_forward_hpp_
#define _bg2e_fx_forward_hpp_

#include <bg/base/light.hpp>
#include <bg/base/effect.hpp>

#include <bg/engine/directx11/shader.hpp>
#include <bg/engine/openglCore/shader.hpp>

#include <bg/render/shadow_map.hpp>

#include <bg/tools/texture_cache.hpp>

namespace bg {
namespace fx {

class BG2E_EXPORT Forward : public bg::base::Effect {
public:
	struct LightData {
		std::vector<bg::math::Vector3> direction;
		std::vector<bg::math::Vector4> ambient;
		std::vector<bg::math::Vector4> diffuse;
		std::vector<bg::math::Vector4> specular;
		std::vector<bg::math::Vector3> attenuation;
		std::vector<bg::math::Vector3> position;
		std::vector<float> spotCutoff;
		std::vector<float> spotExponent;
		std::vector<int> type;

		// Forward render only allows one light to project shadows
		bg::math::Matrix4 shadowProjection;
		bg::math::Matrix4 shadowView;
		bg::ptr<bg::base::Texture> shadowMap;
		float shadowStrength;
		float shadowBias;
		int shadowType;
		bg::math::Color shadowColor;

		bg::base::MatrixState * matrixState = nullptr;
		bg::base::Context * context = nullptr;

		inline void beginFrame() {
			direction.clear();
			ambient.clear();
			diffuse.clear();
			specular.clear();
			attenuation.clear();
			position.clear();
			spotCutoff.clear();
			spotExponent.clear();
			type.clear();
			shadowProjection.identity();
			shadowView.identity();
			shadowMap = nullptr;
			shadowStrength = 1.0f;
			shadowBias = 0.001f;
			shadowType = 0;
			shadowColor = bg::math::Color::Black();
		}

		inline void addLight(bg::base::Light * l, const bg::math::Matrix4 & trx, bg::render::ShadowMap * sm = nullptr) {
			bg::math::Matrix4 lightMatrix = matrixState->viewMatrixStack().matrix();
			lightMatrix.mult(trx);
			bg::math::Vector3 pos = lightMatrix.position();
			bg::math::Vector3 dir = lightMatrix
				.rotation()
				.multVector(l->direction())
				.xyz();
			direction.push_back(dir);
			ambient.push_back(l->ambient());
			diffuse.push_back(l->diffuse());
			specular.push_back(l->specular());
			attenuation.push_back(l->attenuationVector());
			position.push_back(pos);
			spotCutoff.push_back(l->spotCutoff());
			spotExponent.push_back(l->spotExponent());
			type.push_back(l->type());

			if (shadowMap.isNull() && sm && l->type()!=bg::base::Light::kTypePoint) {
				shadowProjection = l->projection();
				shadowView = sm->tunedViewMatrix();
				shadowMap = sm->texture();
				shadowStrength = l->shadowStrength();
				shadowBias = l->shadowBias();
				shadowColor = sm->shadowColor();
				shadowType = sm->shadowType();
			}
		}

		inline int numLights() const { return static_cast<int>(direction.size()); }
	};

	Forward(bg::base::Context *);

	virtual void activate();
	virtual void bindPolyList(bg::base::PolyList *);
	virtual void unbind();

	inline LightData & lightData() { return _lights; }
	inline const LightData & lightData() const { return _lights; }

//	inline void setLight(bg::base::Light * light) { if (light) _light = light; }
//	inline const bg::base::Light * light() const { return _light.getPtr(); }
//	inline bg::base::Light * light() { return _light.getPtr(); }
	
//	inline void setLightTransform(const bg::math::Matrix4 & trx) { _lightTransform = trx; }
//	inline const bg::math::Matrix4 & lightTransform() const { return _lightTransform; }
//	inline bg::math::Matrix4 & lightTransform() { return _lightTransform; }
	
//	inline void setShadowMap(bg::render::ShadowMap * sm) { _shadowMap = sm; }
//	inline bg::render::ShadowMap * shadowMap() { return _shadowMap.getPtr(); }

protected:

	virtual ~Forward();

	void buildDX11();
	void buildGL(bool mobile);

	void activateDX11();
	void activateGL(bool mobile);

	void bindPolyListDX11(bg::base::PolyList * plist);
	void bindPolyListGL(bg::base::PolyList * plist, bool mobile);

	void unbindDX11();
	void unbindGL(bool mobile);


	LightData _lights;

	bg::ptr<bg::engine::directx11::VertexShader> _dxVertexShader;
	bg::ptr<bg::engine::directx11::PixelShader> _dxPixelShader;

	bg::ptr<bg::engine::openglCore::Shader> _glShader;

//	bg::ptr<bg::base::Light> _light;
//	bg::math::Matrix4 _lightTransform;
//	bg::ptr<bg::render::ShadowMap> _shadowMap;

	void getVertexShader(bg::tools::ShaderSource &);
	void getFragmentShader(bg::tools::ShaderSource &);
};

}
}

#endif
