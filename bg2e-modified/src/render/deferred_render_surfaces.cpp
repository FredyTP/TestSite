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

#include <bg/render/deferred_render_surfaces.hpp>

namespace bg {
namespace render {

DeferredRenderSurfaces::DeferredRenderSurfaces(bg::base::Context * ctx, bool memoryOptimized)
	:ContextObject(ctx)
	,_memoryOptimized(memoryOptimized)
	,_type(kTypeUndefined)
{
}

DeferredRenderSurfaces::~DeferredRenderSurfaces() {
}

void DeferredRenderSurfaces::createOpaqueSurfaces() {
	using namespace bg::base;
	_type = kTypeOpaque;
	createCommon();
}

void DeferredRenderSurfaces::createTransparentSurfaces(DeferredRenderSurfaces * opaqueSurfaces) {
	using namespace bg::base;
	_type = kTypeTransparent;
	createCommon();
	_opaqueSurfaces = opaqueSurfaces;
}
	
void DeferredRenderSurfaces::resize(const bg::math::Size2Di & size, bg::base::Quality ssaoQuality, float ssrtScale) {
	if (_memoryOptimized) {
		_gbufferUByteSurface->setSize(size);
	}
	_gbufferFloatSurface->setSize(size);
	_lightingSurface->setSize(size);
	_shadowSurface->setSize(size);
	switch (ssaoQuality) {
	case bg::base::kQualityLow:
		_ssaoSurface->setSize(bg::math::Size2Di(size.x() / 6, size.y() / 6));
		break;
	case bg::base::kQualityMedium:
		_ssaoSurface->setSize(bg::math::Size2Di(size.x() / 4, size.y() / 4));
		break;
	case bg::base::kQualityHigh:
		_ssaoSurface->setSize(bg::math::Size2Di(size.x() / 2, size.y() / 2));
		break;
	case bg::base::kQualityExtreme:
		_ssaoSurface->setSize(size);
		break;
	};
	_ssrtSurface->setSize(bg::math::Size2Di(
		static_cast<int>(static_cast<float>(size.x()) * ssrtScale), 
		static_cast<int>(static_cast<float>(size.y()) * ssrtScale)
	));
	_mixSurface->setSize(size);
}

void DeferredRenderSurfaces::invalidateBuffers() {
	if (_memoryOptimized) {
		_gbufferUByteSurface->invalidateSurface();
	}
	_gbufferFloatSurface->invalidateSurface();
	_lightingSurface->invalidateSurface();
	_shadowSurface->invalidateSurface();
	_ssaoSurface->invalidateSurface();
	_ssrtSurface->invalidateSurface();
	_mixSurface->invalidateSurface();
}

void DeferredRenderSurfaces::invalidateTextures() {
	if(_memoryOptimized) {
		_gbufferUByteSurface->invalidateTextures();
	}
	_gbufferFloatSurface->invalidateTextures();
	_lightingSurface->invalidateTextures();
	_shadowSurface->invalidateTextures();
	_ssaoSurface->invalidateTextures();
	_ssrtSurface->invalidateTextures();
	_mixSurface->invalidateTextures();
}

void DeferredRenderSurfaces::createCommon() {
using namespace bg::base;
	if (_memoryOptimized) {
		_gbufferUByteSurface = new TextureRenderSurface(context());
		_gbufferUByteSurface->create({
			RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatUnsignedByte),	// 0: diffuse
			RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatUnsignedByte),	// 1: specular
			RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatUnsignedByte),	// 2: normal
			RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatUnsignedByte),	// 3: material
			RenderSurface::ColorPlane(RenderSurface::kTypeDepth, RenderSurface::kFormatScreen)
		});
		_gbufferUByteSurface->setResizeOnViewportChanged(false);
		_gbufferFloatSurface = new TextureRenderSurface(context());
		_gbufferFloatSurface->create({
			RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatFloat),	// 0: position
			RenderSurface::ColorPlane(RenderSurface::kTypeDepth, RenderSurface::kFormatScreen)
		});
		_gbufferFloatSurface->setResizeOnViewportChanged(false);
	}
	else {
		_gbufferFloatSurface = new TextureRenderSurface(context());
		_gbufferFloatSurface->create({
			RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatFloat),	// 0: diffuse
			RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatFloat),	// 1: specular
			RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatFloat),	// 2: normal
			RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatFloat),	// 3: material
			RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatFloat),	// 4: position
			RenderSurface::ColorPlane(RenderSurface::kTypeDepth, RenderSurface::kFormatScreen)
		});
		_gbufferFloatSurface->setResizeOnViewportChanged(false);
	}
	
	_lightingSurface = new TextureRenderSurface(context());
	_lightingSurface->create({
		RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatUnsignedByte),	// 0: lighting without shininess
		RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatUnsignedByte),	// 1: shininess
		RenderSurface::ColorPlane(RenderSurface::kTypeDepth, RenderSurface::kFormatScreen)
	});
	_lightingSurface->setResizeOnViewportChanged(false);
	
	_shadowSurface = new TextureRenderSurface(context());
	_shadowSurface->create();
	_shadowSurface->setResizeOnViewportChanged(false);
	
	_ssaoSurface = new TextureRenderSurface(context());
	_ssaoSurface->create();
	_ssaoSurface->setResizeOnViewportChanged(false);
	
	_ssrtSurface = new TextureRenderSurface(context());
	_ssrtSurface->create();
	_ssrtSurface->setResizeOnViewportChanged(false);
	
	_mixSurface = new TextureRenderSurface(context());
	_mixSurface->create();
	_mixSurface->setResizeOnViewportChanged(false);
}

}
}

