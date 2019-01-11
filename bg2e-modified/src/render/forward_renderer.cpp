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


#include <bg/render/forward_renderer.hpp>

#include <bg/scene/drawable.hpp>
#include <bg/scene/light.hpp>

#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/tools/texture_cache.hpp>

#include <bg/log.hpp>

namespace bg {
namespace render {

ForwardRenderer::ForwardRenderer(bg::base::Context * ctx)
	:ClassicRenderer(ctx)
{
}

ForwardRenderer::~ForwardRenderer() {
	
}

bool ForwardRenderer::isSupported() {
	return	(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>() ||
			 bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>());
}

void ForwardRenderer::create() {
	bg::base::Context * ctx = context();
	_effect = new bg::fx::Forward(context());
	pipeline()->setEffect(_effect.getPtr());
	ForwardRenderLayer * opaque = new ForwardRenderLayer(ctx,pipeline());
	_opaqueLayer = opaque;
	ForwardRenderLayer * transp = new ForwardRenderLayer(ctx,pipeline());
	_transparentLayer = transp;
	

	if (_shadowMap.isNull()) _shadowMap = new ShadowMap(ctx);
	_shadowMap->setShadowMapSize(bg::math::Size2Di(1024));
	_shadowMap->setShadowType(ShadowMap::kSoftShadows);
	
	addRenderSetting(_shadowMap.getPtr());
}

void ForwardRenderer::draw(bg::scene::Node * node, bg::scene::Camera * camera) {
	using namespace bg::scene;
	Light * mainLight = nullptr;
	// Loop to find the main light, used to update the shadow map
	for (auto lc : Light::ActiveLights()) {
		if (lc->light()->enabled() && lc->light()->type()!=bg::base::Light::kTypePoint && mainLight==nullptr) {
			mainLight = lc;
			break;
		}
	}
	
	if (mainLight) {
		_shadowMap->update(node, camera, mainLight->light(), mainLight->transform());
		
		dynamic_cast<ForwardRenderLayer*>(_opaqueLayer.getPtr())->setLightComponent(mainLight);
		dynamic_cast<ForwardRenderLayer*>(_opaqueLayer.getPtr())->setShadowMap(_shadowMap.getPtr());
		
		dynamic_cast<ForwardRenderLayer*>(_transparentLayer.getPtr())->setLightComponent(mainLight);
		dynamic_cast<ForwardRenderLayer*>(_transparentLayer.getPtr())->setShadowMap(_shadowMap.getPtr());
	}

	// Loop to set the light data to the forward effect.
	//	IMPORTANT: This loop may be executed AFTER the shadow map update, because lc->transform() isn't 
	//	const and should modify the light projection.
	_effect->lightData().beginFrame();
	for(auto lc : Light::ActiveLights()) {
		if(lc->light()->enabled() && lc->node()->enabled()) {
			_effect->lightData().addLight(lc->light(), lc->transform(), _shadowMap.getPtr());
		}
	}

	
	if (_effect->lightData().numLights()>0) {
		bg::base::Pipeline::SetCurrent(pipeline());
		pipeline()->setClearColor(_clearColor);
		pipeline()->setOpacityLayer(bg::base::Pipeline::kOpacityOpaque);
		pipeline()->setBuffersToClear(bg::base::kColorDepth);
		pipeline()->setBlend(false);
		_opaqueLayer->draw(node, camera);

		pipeline()->setOpacityLayer(bg::base::Pipeline::kOpacityTransparent);
		pipeline()->setBuffersToClear(bg::base::kNone);
		pipeline()->setBlend(true);
		pipeline()->setBlendMode(bg::base::kNormal);
		_transparentLayer->draw(node, camera);
	}

	if (_autoInvalidateTextures) {
		invalidateTextures();
	}
}

void ForwardRenderer::invalidateBuffers() {
	_opaqueLayer->invalidateBuffers();
	_transparentLayer->invalidateBuffers();
	_shadowMap->invalidateBuffers();
}

void ForwardRenderer::invalidateTextures() {
	_opaqueLayer->invalidateTextures();
	_transparentLayer->invalidateTextures();
	_shadowMap->invalidateTextures();
}

}
}
