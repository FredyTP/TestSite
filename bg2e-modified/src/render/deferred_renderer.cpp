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


#include <bg/render/deferred_renderer.hpp>

#include <bg/scene/drawable.hpp>
#include <bg/scene/light.hpp>

#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>

namespace bg {
namespace render {

DeferredRenderer::DeferredRenderer(bg::base::Context * ctx)
	:ClassicRenderer(ctx)
{
}

DeferredRenderer::~DeferredRenderer() {

}

bool DeferredRenderer::isSupported() {
	return	(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>() ||
			 bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>()) &&
			bg::Engine::Get()->supportsFeature(context(), bg::kMultibuffer) &&
			(bg::Engine::Get()->supportsFeature(context(), bg::kFloatTextures) ||
			 bg::Engine::Get()->supportsFeature(context(), bg::kFloatTexture16) ||
			 bg::Engine::Get()->supportsFeature(context(), bg::kFloatTexture32));
}

void DeferredRenderer::create() {
	bg::base::Context * ctx = context();
	
	ptr<DeferredRenderLayer> opaqueLayer = new DeferredRenderLayer(ctx);
	opaqueLayer->createOpaqueLayer();
	_opaqueLayer = opaqueLayer.getPtr();
	
	ptr<DeferredRenderLayer> transparentLayer = new DeferredRenderLayer(ctx);
	transparentLayer->createTransparentLayer(opaqueLayer->maps());
	_transparentLayer = transparentLayer.getPtr();
	_transparentLayer->pipeline()->setClearColor(bg::math::Color::Transparent());
	
	_shadowMap = new ShadowMap(ctx);
	_shadowMap->setShadowMapSize(bg::math::Size2Di(1024));
	_shadowMap->setShadowType(ShadowMap::kSoftShadows);
	dynamic_cast<DeferredRenderLayer*>(_transparentLayer.getPtr())->setShadowMap(_shadowMap.getPtr());
	dynamic_cast<DeferredRenderLayer*>(_opaqueLayer.getPtr())->setShadowMap(_shadowMap.getPtr());
	
	addRenderSetting(_shadowMap.getPtr());
	
	_ssaoSettings = new AmbientOcclusionSettings;
	_ssrtSettings = new RaytracerSettings;
	_debugSettings = new DebugRenderSettings;
	
	addRenderSetting(_ssaoSettings.getPtr());
	addRenderSetting(_ssrtSettings.getPtr());
	addRenderSetting(_debugSettings.getPtr());
	
	dynamic_cast<DeferredRenderLayer*>(_transparentLayer.getPtr())->setAmbientOcclusionSettings(_ssaoSettings.getPtr());
	dynamic_cast<DeferredRenderLayer*>(_transparentLayer.getPtr())->setRaytracerSettings(_ssrtSettings.getPtr());
	dynamic_cast<DeferredRenderLayer*>(_transparentLayer.getPtr())->setDebugSettings(_debugSettings.getPtr());
	
	dynamic_cast<DeferredRenderLayer*>(_opaqueLayer.getPtr())->setAmbientOcclusionSettings(_ssaoSettings.getPtr());
	dynamic_cast<DeferredRenderLayer*>(_opaqueLayer.getPtr())->setRaytracerSettings(_ssrtSettings.getPtr());
	dynamic_cast<DeferredRenderLayer*>(_opaqueLayer.getPtr())->setDebugSettings(_debugSettings.getPtr());
}

void DeferredRenderer::draw(bg::scene::Node * node, bg::scene::Camera * camera) {
	using namespace bg::scene;
	
	_textures = std::vector<bg::ptr<bg::base::Texture>>{
		_opaqueLayer->texture(),
		_transparentLayer->texture(),
		dynamic_cast<DeferredRenderLayer*>(_transparentLayer.getPtr())->maps()->normal()
	};
	
    dynamic_cast<DeferredRenderLayer*>(_opaqueLayer.getPtr())->resize(camera);
    dynamic_cast<DeferredRenderLayer*>(_transparentLayer.getPtr())->resize(camera);
	
	dynamic_cast<DeferredRenderLayer*>(_opaqueLayer.getPtr())->setClearColor(_clearColor);
	_opaqueLayer->draw(node, camera);
	_transparentLayer->draw(node, camera);
    
	bg::base::Pipeline::SetCurrent(_pipeline.getPtr());
	_pipeline->setViewport(camera->viewport());
	_pipeline->clearBuffers();
	_pipeline->drawTexture(_textures);

	if(_autoInvalidateTextures) {
		invalidateTextures();
		invalidateBuffers();
	}
}

void DeferredRenderer::invalidateBuffers() {
	_shadowMap->invalidateBuffers();
	_opaqueLayer->invalidateBuffers();
	_transparentLayer->invalidateBuffers();
}

void DeferredRenderer::invalidateTextures() {
	_shadowMap->invalidateTextures();
	_opaqueLayer->invalidateTextures();
	_transparentLayer->invalidateTextures();
}

}
}
