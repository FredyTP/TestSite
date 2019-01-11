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


#include <bg/render/deferred_render_layer.hpp>

#include <bg/scene/light.hpp>
#include <bg/tools/texture_cache.hpp>

#include <bg/engine/opengl_es_3.hpp>

namespace bg {
namespace render {

namespace settings {
	
const SettingsKey kAmbientOcclusion = "ambientOcclusion";
const SettingsKey kRaytracer = "raytracer";

const SettingsKey kKernelSize = "kernelSize";

const SettingsKey kClearOnViewChanged = "clearOnViewChanged";

}

DeferredRenderLayer::DeferredRenderLayer(bg::base::Context * ctx, bg::base::Pipeline * pl, bool memoryOptimized)
	:RenderLayer(ctx, pl)
	,_memoryOptimized(memoryOptimized)
{
	using namespace bg::base;
	
	bg::base::TextureRenderSurface * mixSurface = new bg::base::TextureRenderSurface(context());
	mixSurface->create();
	pipeline()->setRenderSurface(mixSurface);

	// Force optimize memory in mobile devices
	if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>()) {
		_memoryOptimized = true;
	}
}

void DeferredRenderLayer::createOpaqueLayer() {
	using namespace bg::base;
	pipeline()->setOpacityLayer(Pipeline::kOpacityOpaque);
	
	_surfaces = new DeferredRenderSurfaces(context(), _memoryOptimized);
	_surfaces->createOpaqueSurfaces();
	
	createCommon();
}

void DeferredRenderLayer::createTransparentLayer(DeferredRenderSurfaces * opaqueMaps) {
	using namespace bg::base;
	pipeline()->setOpacityLayer(Pipeline::kOpacityTransparent);
	
	_surfaces = new DeferredRenderSurfaces(context(), _memoryOptimized);
	_surfaces->createTransparentSurfaces(opaqueMaps);
	
	createCommon();
	
	_gbufferPipeline->setBlend(true);
	_gbufferPipeline->setBlendMode(bg::base::BlendMode::kNormal);
	_gbufferPipeline->setClearColor(bg::math::Color::Transparent());
	if (_memoryOptimized) {
		_gbufferFloatPipeline->setBlend(true);
		_gbufferFloatPipeline->setBlendMode(bg::base::BlendMode::kNormal);
		_gbufferFloatPipeline->setClearColor(bg::math::Color::Transparent());
	}
	_lightingPipeline->setClearColor(bg::math::Color::Black());
	pipeline()->setClearColor(bg::math::Color::Transparent());
}

DeferredRenderLayer::~DeferredRenderLayer() {
}

void DeferredRenderLayer::draw(bg::scene::Node * node, bg::scene::Camera * camera) {
	bg::base::Effect * effect = _gbufferEffect.getPtr();
	effect->matrixState().projectionMatrixStack()
		.set(camera->projection());
	
	effect->matrixState().viewMatrixStack()
		.identity()
		.mult(camera->viewMatrix());
	
	effect->matrixState().modelMatrixStack()
		.identity();
	
	if (_memoryOptimized) {
		bg::base::Effect * floatEffect = _gbufferFloatEffect.getPtr();
		floatEffect->matrixState().projectionMatrixStack()
			.set(camera->projection());
		
		floatEffect->matrixState().viewMatrixStack()
			.set(effect->matrixState().viewMatrixStack().matrix());
		
		floatEffect->matrixState().modelMatrixStack()
			.identity();
		
	}
	willDraw(node, camera);
	performDraw(node, camera);
}

void DeferredRenderLayer::willDraw(bg::scene::Node *, bg::scene::Camera *) {
}

void DeferredRenderLayer::resize(bg::scene::Camera * c) {
	using namespace bg::base;
	Quality ssaoQ = _ssaoSettings.valid() ? _ssaoSettings->quality() : kQualityExtreme;
	float ssrtScale = _ssrtSettings.valid() ? _ssrtSettings->scale() : 1.0f;
	_surfaces->resize(bg::math::Size2Di(c->viewport().width(), c->viewport().height()), ssaoQ, ssrtScale);
}

void DeferredRenderLayer::invalidateBuffers() {
	_surfaces->invalidateBuffers();
}

void DeferredRenderLayer::invalidateTextures() {
	_surfaces->invalidateTextures();
}

void DeferredRenderLayer::renderShadowCascade(bg::scene::Node * sceneRoot, bg::scene::Camera * camera, bg::scene::Light * lightComponent, bool firstCascade, ShadowMap::ShadowCascade cascade) {
	using namespace bg::base;
	float bias = lightComponent->light()->shadowBias();
	ShadowMap::ShadowCascadeSettings & shadowSettings = ShadowMap::CascadeSetup[cascade];
	bg::scene::ProjectionStrategy * projectionStrategy = camera->projectionStrategy();
	float near = projectionStrategy ? projectionStrategy->near() : 0.1f;
	float far = projectionStrategy ? projectionStrategy->far() : 600.0f;
	lightComponent->light()->setShadowBias(shadowSettings.shadowBias);
	if (projectionStrategy) {
		projectionStrategy->setNear(shadowSettings.cameraNear);
		projectionStrategy->setFar(shadowSettings.cameraFar);
		projectionStrategy->apply();
	}
	_shadowMap->updateCascade(cascade, sceneRoot, camera, lightComponent->light(), lightComponent->transform());
	Pipeline::SetCurrent(_shadowPipeline.getPtr());
	_shadowPipeline->setViewport(camera->viewport());
	_shadowEffect->matrixState().projectionMatrixStack().set(camera->projection());
	_shadowEffect->matrixState().viewMatrixStack().set(camera->viewMatrix());
	if (firstCascade) {
		_shadowPipeline->clearBuffers(ClearBuffers::kColorDepth);
	}
	else {
		_shadowPipeline->clearBuffers(ClearBuffers::kDepth);
	}
    _shadowPipeline->setBlend(shadowSettings.blend);
    _shadowPipeline->setBlendMode(bg::base::kMultiply);
	_shadowEffect->setLight(lightComponent->light());
	_shadowEffect->setLightTransform(lightComponent->transform());
	_shadowEffect->setShadowMap(_shadowMap.getPtr());
	sceneRoot->accept(_shadowVisitor.getPtr());
	lightComponent->light()->setShadowBias(bias);
	if(projectionStrategy) {
		projectionStrategy->setNear(near);
		projectionStrategy->setFar(far);
		projectionStrategy->apply();
	}
}
    
void DeferredRenderLayer::performDraw(bg::scene::Node * sceneRoot, bg::scene::Camera * camera) {
	using namespace bg::base;
    Pipeline::SetCurrent(_gbufferPipeline.getPtr());
	
    _gbufferPipeline->clearBuffers();
	_gbufferPipeline->setViewport(camera->viewport());
    sceneRoot->accept(drawVisitor());
	
	if (_memoryOptimized) {
		Pipeline::SetCurrent(_gbufferFloatPipeline.getPtr());
		_gbufferFloatPipeline->setViewport(camera->viewport());
		_gbufferFloatPipeline->clearBuffers();
		sceneRoot->accept(_floatDrawVisitor.getPtr());
	}

	// Render lights
	_lightingPipeline->clearBuffers(bg::base::kColorDepth);
	_lightingPipeline->setBlend(false);
	_lightingPipeline->setBlendMode(bg::base::BlendMode::kAdd);
	_lightingEffect->beginRenderLights();
	for (auto lightComponent : bg::scene::Light::ActiveLights()) {
		if (lightComponent->light() && lightComponent->light()->enabled() &&
			lightComponent->node() && lightComponent->node()->enabled())
		{
			if (lightComponent->light()->type() == bg::base::Light::kTypeSpot) {
				_shadowMap->update(sceneRoot, camera, lightComponent->light(), lightComponent->transform());
				Pipeline::SetCurrent(_shadowPipeline.getPtr());
				_shadowPipeline->setViewport(camera->viewport());
				_shadowEffect->matrixState().projectionMatrixStack().set(camera->projection());
				_shadowEffect->matrixState().viewMatrixStack().set(camera->viewMatrix());
				_shadowPipeline->clearBuffers(ClearBuffers::kColorDepth);
				_shadowEffect->setLight(lightComponent->light());
				_shadowEffect->setLightTransform(lightComponent->transform());
				_shadowEffect->setShadowMap(_shadowMap.getPtr());
				sceneRoot->accept(_shadowVisitor.getPtr());
			}
			else if (lightComponent->light()->type() == bg::base::Light::kTypeDirectional) {
				//renderShadowCascade(sceneRoot, camera, lightComponent, true, ShadowMap::kCascadeFar);
				renderShadowCascade(sceneRoot, camera, lightComponent, true, ShadowMap::kCascadeMid);
				//renderShadowCascade(sceneRoot, camera, lightComponent, true, ShadowMap::kCascadeNear);
			}

			Pipeline::SetCurrent(_lightingPipeline.getPtr());
			_lightingPipeline->setViewport(camera->viewport());
			_lightingEffect->setClearColor(_clearColor);
			_lightingEffect->setLight(lightComponent->light());
			_lightingEffect->setLightTransform(lightComponent->transform());
			_lightingEffect->setViewMatrix(camera->viewMatrix());
			_lightingEffect->setShadowTexture(_surfaces->shadow());
			_lightingEffect->setPositionTexture(_surfaces->position());
			if (_memoryOptimized) {
				_lightingPipeline->drawTexture(_surfaces->gbufferUByteSurface());
			}
			else {
				_lightingPipeline->drawTexture(_surfaces->gbufferFloatSurface());
			}
			_lightingPipeline->setBlend(true);

			//if (_gbufferPipeline->opacityLayer()==bg::base::Pipeline::kOpacityOpaque)
			//	_surfaces->lighting()->saveAsImage("C:/Users/Fernando/Desktop/lighting.png");
		}
	}

	bool renderSSAO = true;
	bool renderSSRT = true;
	bg::math::Viewport vp = camera->viewport();
	if (_ssaoSettings.valid() && (renderSSAO = _ssaoSettings->isEnabled())) {
		_ssaoEffect->setColor(_ssaoSettings->color());
		_ssaoEffect->setEnabled(_ssaoSettings->isEnabled());
		_ssaoEffect->setKernelSize(_ssaoSettings->kernelSize());
		_ssaoEffect->setSampleRadius(_ssaoSettings->sampleRadius());
		_ssaoEffect->setMaxDistance(_ssaoSettings->maxDistance());
		_mixEffect->setSSAOBlur(_ssaoSettings->blur());
		switch (_ssaoSettings->quality()) {
		case bg::base::kQualityLow:
			vp.set(0, 0, camera->viewport().width() / 6, camera->viewport().height() / 6);
			break;
		case bg::base::kQualityMedium:
			vp.set(0, 0, camera->viewport().width() / 4, camera->viewport().height() / 4);
			break;
		case bg::base::kQualityHigh:
			vp.set(0, 0, camera->viewport().width() / 2, camera->viewport().height() / 2);
			break;
		case bg::base::kQualityExtreme:
			vp = camera->viewport();
			break;
		}
	}
	if (renderSSAO) {
		Pipeline::SetCurrent(_ssaoPipeline.getPtr());
		_ssaoEffect->setProjectionMatrix(camera->projection());
		_ssaoPipeline->setViewport(vp);
		_ssaoPipeline->clearBuffers(bg::base::kColorDepth);
		TextureVector ssaoTex{
			_surfaces->position(),
			_surfaces->normal(),
			_surfaces->diffuse()
		};
		_ssaoPipeline->drawTexture(ssaoTex);
	}

	float ssrtScale = 1.0f;
	bool clearOnViewChanged = false;
	if (_ssrtSettings.valid() && (renderSSRT=_ssrtSettings->isEnabled())) {
		_ssrtEffect->setQuality(_ssrtSettings->quality());
		ssrtScale = _ssrtSettings->scale();
		clearOnViewChanged = _ssrtSettings->clearOnViewChanged();
	}

	if (renderSSRT) {
		vp = camera->viewport();
		vp.set(
			vp.x(), vp.y(),
			static_cast<int>(static_cast<float>(vp.width()) * ssrtScale),
			static_cast<int>(static_cast<float>(vp.height()) * ssrtScale)
		);
		
		Pipeline::SetCurrent(_ssrtPipeline.getPtr());
		_ssrtPipeline->setViewport(vp);
		if (clearOnViewChanged && camera->viewMatrix() != _lastViewMatrix) {
			_ssrtPipeline->clearBuffers(bg::base::kColorDepth);
		}
		else {
			_ssrtPipeline->clearBuffers(bg::base::kDepth);
		}
		_ssrtEffect->setProjectionMatrix(camera->projection());
		if (camera->transform()) {
			bg::math::Matrix4 viewProjection = camera->projection();
			viewProjection.mult(camera->viewMatrix());
			bg::math::Vector3 pos = viewProjection.multVector(camera->transform()->matrix().position()).xyz();
			_ssrtEffect->setCameraPosition(pos);
		}
		TextureVector ssrtTextures{
			_surfaces->position(),
			_surfaces->normal(),
			_surfaces->reflectionColor(),
			_surfaces->material(),
			_surfaces->specular(),
			_surfaces->reflectionDepth()
		};
		_ssrtPipeline->drawTexture(ssrtTextures);
	}
	
    // TODO:  camera DOF, color correction
    Pipeline::SetCurrent(pipeline());
    pipeline()->setViewport(camera->viewport());
    pipeline()->clearBuffers(bg::base::kColorDepth);
	TextureVector mixTextures{
		_surfaces->lighting(),
		_surfaces->diffuse(),
		renderSSAO ? _surfaces->ambientOcclusion() : bg::tools::TextureCache::WhiteTexture(context()),
		renderSSRT ? _surfaces->reflection() : _surfaces->lighting(),
		_surfaces->material(),
		_surfaces->position(),
		_surfaces->specular(),
		_surfaces->shininess()
	};
	_mixEffect->setOpaqueDepth(_surfaces->mixDepthMap());
	_mixEffect->setSSRTScale(ssrtScale);
    pipeline()->drawTexture(mixTextures);

	_lastViewMatrix = camera->viewMatrix();
}

void DeferredRenderLayer::createCommon() {
	using namespace bg::base;
	// G-Buffers
	if (_memoryOptimized) {
		_gbufferEffect = new bg::fx::GBufferUByte(context());
		_gbufferEffect->build();
		_gbufferPipeline = new Pipeline(context());
		_gbufferPipeline->setEffect(_gbufferEffect.getPtr());
		_gbufferPipeline->setRenderSurface(_surfaces->gbufferUByteSurface());
		_gbufferPipeline->setOpacityLayer(opacityLayer());
		drawVisitor()->setPipeline(_gbufferPipeline.getPtr());

		// Floating point gbuffers
		_gbufferFloatEffect = new bg::fx::GBufferFloat(context());
		_gbufferFloatEffect->build();
		_gbufferFloatPipeline = new Pipeline(context());
		_gbufferFloatPipeline->setEffect(_gbufferFloatEffect.getPtr());
		_gbufferFloatPipeline->setRenderSurface(_surfaces->gbufferFloatSurface());
		_gbufferFloatPipeline->setOpacityLayer(opacityLayer());
		_floatDrawVisitor = new bg::scene::DrawVisitor();
		_floatDrawVisitor->setPipeline(_gbufferFloatPipeline.getPtr());
	}
	else {
		_gbufferEffect = new bg::fx::GBuffer(context());
		_gbufferEffect->build();
		_gbufferPipeline = new Pipeline(context());
		_gbufferPipeline->setEffect(_gbufferEffect.getPtr());
		_gbufferPipeline->setRenderSurface(_surfaces->gbufferFloatSurface());
		_gbufferPipeline->setOpacityLayer(opacityLayer());
		drawVisitor()->setPipeline(_gbufferPipeline.getPtr());
	}
	_gbufferPipeline->setClearColor(bg::math::Color::Black());
	
	// Lighting
	_lightingEffect = new bg::fx::Lighting(context());
	_lightingPipeline = new Pipeline(context());
	_lightingPipeline->setTextureEffect(_lightingEffect.getPtr());
	_lightingPipeline->setClearColor(bg::math::Color::White());
	_lightingPipeline->setRenderSurface(_surfaces->lightingSurface());
	_lightingPipeline->setDepthTest(false);
	
	// Shadows
	_shadowEffect = new bg::fx::Shadow(context());
	_shadowPipeline = new Pipeline(context());
	_shadowPipeline->setEffect(_shadowEffect.getPtr());
	_shadowPipeline->setRenderSurface(_surfaces->shadowSurface());
	_shadowVisitor = new bg::scene::DrawVisitor();
	_shadowVisitor->setPipeline(_shadowPipeline.getPtr());

	// SSAO
	_ssaoEffect = new bg::fx::Ssao(context());
	_ssaoPipeline = new Pipeline(context());
	_ssaoPipeline->setTextureEffect(_ssaoEffect.getPtr());
	_ssaoPipeline->setClearColor(bg::math::Color::White());
	_ssaoPipeline->setRenderSurface(_surfaces->ssaoSurface());
	
	// SSRT
	_ssrtEffect = new bg::fx::Ssrt(context());
	_ssrtPipeline = new Pipeline(context());
	_ssrtPipeline->setTextureEffect(_ssrtEffect.getPtr());
	_ssrtPipeline->setClearColor(bg::math::Color::Black());
	_ssrtPipeline->setRenderSurface(_surfaces->ssrtSurface());
	
	// Mix
	_mixEffect = new bg::fx::DeferredMix(context());
	pipeline()->setTextureEffect(_mixEffect.getPtr());
}

}
}
