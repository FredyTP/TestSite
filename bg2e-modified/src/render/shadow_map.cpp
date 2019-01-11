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


#include <bg/render/shadow_map.hpp>

#include <bg/log.hpp>

namespace bg {
namespace render {

namespace settings {

const SettingsKey kShadowMap = "shadowMap";

const SettingsKey kShadowType = "shadowType";
const SettingsKey kShadowColor = "shadowColor";
const SettingsKey kShadowMapSize = "shadowMapSize";


}

std::map<ShadowMap::ShadowCascade, ShadowMap::ShadowCascadeSettings> ShadowMap::CascadeSetup {
	{ kCascadeNear,{ 0.1f, 40.0f, 3.0f, 20.0f, 0.0025f, 0.1f, 5.0f, false } },
	{ kCascadeMid,{ 10.0f, 200.0f, 15.0f, 150.0f, 0.0025f, 0.2f, 200.0f, false } },
	{ kCascadeFar,{ 10.0f, 200.0f, 50.0f, 150.0f, 0.005f, 50.0f, 600.0f, true  } }
};

ShadowMap::ShadowMap(bg::base::Context * ctx)
	:bg::base::ContextObject(ctx)
	,RenderSettings(settings::kShadowMap)
	,_shadowMapSize(1024)
	,_shadowColor(bg::math::Color::Black())
	,_type(kSoftShadows)
	,_drawVisitor(new bg::scene::DrawVisitor())
{
	_pipeline = new bg::base::Pipeline(ctx);
	_effect = new bg::fx::ShadowMap(ctx);
	_renderSurface = new bg::base::TextureRenderSurface(ctx);
	_renderSurface->create();
	_pipeline->setRenderSurface(_renderSurface.getPtr());
	_pipeline->setEffect(_effect.getPtr());
	_pipeline->setViewport(bg::math::Viewport(0, 0, _shadowMapSize.width(), _shadowMapSize.height()));
	_drawVisitor->setPipeline(_pipeline.getPtr());
}

ShadowMap::~ShadowMap() {
}

void ShadowMap::update(bg::scene::Node * sceneRoot, bg::scene::Camera * camera, bg::base::Light * light, const bg::math::Matrix4 & lightTransform) {
	_effect->setLight(light);
	
	_viewMatrix = lightTransform;
	if (light->type()==bg::base::Light::kTypeDirectional) {
		auto rotation = _viewMatrix.rotation();
		auto cameraPos = camera->modelMatrix().position();
		auto cameraTransform = camera->modelMatrix();
		auto target = cameraPos.add(cameraTransform.forwardVector().scale(-camera->focus()));

		_viewMatrix
			.identity()
			.translate(target)
			.mult(rotation)
			.translate(0.0f, 0.0f, 50.0f)
			.invert();

		light->setProjection(bg::math::Matrix4::Ortho(-camera->focus() * 2.0f, camera->focus() * 2.0f, -camera->focus() * 2.0f, camera->focus() * 2.0f, 0.5f, 100.0f * camera->focus()));
		renderShadowMap(sceneRoot, camera, light);
	}
	else if (light->type()==bg::base::Light::kTypeSpot) {
		float cutoff = light->spotCutoff();
		light->projection().perspective(cutoff*2.0f, 1.0f, 0.5f, 50.0f);
		_viewMatrix.invert();
		renderShadowMap(sceneRoot, camera, light);
	}
	else {
		renderShadowMap(sceneRoot, camera, light);
	}
}

void ShadowMap::updateCascade(ShadowCascade c, bg::scene::Node * sceneRoot, bg::scene::Camera * camera, bg::base::Light * light, const bg::math::Matrix4 & lightTransform) {
	_effect->setLight(light);

	_viewMatrix = lightTransform;
	if(light->type() == bg::base::Light::kTypeDirectional) {
		auto rotation = _viewMatrix.rotation();
		auto cameraPos = camera->modelMatrix().position();
		auto cameraTransform = camera->modelMatrix();
		auto target = cameraPos.add(cameraTransform.forwardVector().scale(-camera->focus()));

		ShadowCascadeSettings & settings = CascadeSetup[c];

		_viewMatrix
			.identity()
			.translate(target)
			.mult(rotation)
			.translate(0.0f, 0.0f, settings.lightDistance)
			.invert();

		light->setProjection(bg::math::Matrix4::Ortho(-settings.frameSize, settings.frameSize, -settings.frameSize, settings.frameSize, settings.near, settings.far));
		renderShadowMap(sceneRoot, camera, light);
	}
	else {
		throw bg::base::InvalidParameterException("Shadow cascades are only compatible with directional lights.");
	}
}

void ShadowMap::invalidateBuffers() {
	_renderSurface->invalidateSurface();
}

void ShadowMap::invalidateTextures() {
	_renderSurface->invalidateTextures();
}

// Settings
void ShadowMap::setSettingValue(const SettingsKey & key, int value) {
	if (key==settings::kShadowType) {
		_type = static_cast<ShadowType>(value);
	}
}

void ShadowMap::setSettingValue(const SettingsKey & key, const bg::math::Vector4 & value) {
	if(key == settings::kShadowColor) {
		_shadowColor = value;
	}
}

void ShadowMap::setSettingValue(const SettingsKey & key, const bg::math::Size2Di & value) {
	if(key == settings::kShadowMapSize) {
		_shadowMapSize = value;
	}
}

int ShadowMap::intSettingValue(const SettingsKey & key) const {
	if(key == settings::kShadowType) {
		return _type;
	}
	else {
		return RenderSettings::intSettingValue(key);
	}
}

const bg::math::Vector4 & ShadowMap::vec4SettingValue(const SettingsKey & key) const {
	if(key == settings::kShadowColor) {
		return _shadowColor;
	}
	else {
		return RenderSettings::vec4SettingValue(key);
	}
}

const bg::math::Size2Di ShadowMap::sizeSettingValue(const SettingsKey & key) const {
	if(key == settings::kShadowMapSize) {
		return _shadowMapSize;
	}
	else {
		return RenderSettings::sizeSettingValue(key);
	}
}

void ShadowMap::renderShadowMap(bg::scene::Node * sceneRoot, bg::scene::Camera * camera, bg::base::Light * light) {
	_effect->setLight(light);
	_effect->matrixState().projectionMatrixStack().set(light->projection());
	_effect->matrixState().viewMatrixStack().set(_viewMatrix);
	_effect->matrixState().modelMatrixStack().identity();

	bg::base::Pipeline::SetCurrent(_pipeline.getPtr());
	_pipeline->setViewport(bg::math::Viewport(0, 0, _shadowMapSize.width(), _shadowMapSize.height()));

	_pipeline->clearBuffers(bg::base::ClearBuffers::kColorDepth);

	sceneRoot->accept(_drawVisitor.getPtr());
}

}
}
