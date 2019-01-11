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


#include <bg/engine/directx11/render_surface_impl.hpp>
#include <bg/engine/directx_includes.hpp>
#include <bg/base/exception.hpp>

namespace bg {
namespace engine {
namespace directx11 {


RenderSurfaceImpl::RenderSurfaceImpl(bg::base::RenderSurface * surface, RenderSurfaceTarget target)
	:bg::engine::RenderSurfaceImpl(surface,target)
{
	if (_target==kTargetTexture) {
		throw bg::base::DirectXEngineException("Texture render sufaces not implemented in DirectX");
	}
}
	
RenderSurfaceImpl::~RenderSurfaceImpl() {
	
}

bool RenderSurfaceImpl::supportFormat(int fmt) {
	switch (fmt) {
	case bg::base::RenderSurface::kFormatFloat:
		return false;
	case bg::base::RenderSurface::kFormatUnsignedByte:
		return false;
	case bg::base::RenderSurface::kFormatUnsignedShort:
		return false;
	case bg::base::RenderSurface::kFormatUndefined:
		return false;
	default:
		return false;
	}
}

bool RenderSurfaceImpl::supportType(int type) {
	if (type==bg::base::RenderSurface::kTypeColor) {
		return true;
	}
	else if (type==bg::base::RenderSurface::kTypeDepth) {
		return false;
	}
	return false;
}

int RenderSurfaceImpl::maxOutpuColorPlanes() {
	return 1;
}

void RenderSurfaceImpl::invalidateSurface() {
}

void RenderSurfaceImpl::invalidateTextures() {
}

void RenderSurfaceImpl::setSize(const bg::math::Size2Di & size) {
	_size = size;
}

void RenderSurfaceImpl::addOutputColorPlane(int type, int format) {
	_colorPlaneSetup.push_back(base::RenderSurface::ColorPlane(static_cast<base::RenderSurface::RenderSurfaceType>(type),
															   static_cast<base::RenderSurface::RenderSurfaceFormat>(format)));
}

void RenderSurfaceImpl::create() {
}

void RenderSurfaceImpl::setActive() {
}

unsigned char * RenderSurfaceImpl::readPixels(const bg::math::Viewport & vp) {
	return nullptr;
}

void RenderSurfaceImpl::destroy() {
}

bg::base::Texture * RenderSurfaceImpl::texture(int colorPlane) {
	return nullptr;
}

}
}
}