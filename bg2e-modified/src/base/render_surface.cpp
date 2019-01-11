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


#include <bg/base/render_surface.hpp>
#include <bg/engine.hpp>

namespace bg {
namespace base {

void RenderSurface::GetTypeAndFormatString(ColorPlane colorPlane, std::string & type, std::string & format) {
	switch (colorPlane.type) {
		case kTypeUndefined:
			type = "kTypeUndefined";
			break;
		case kTypeColor:
			type = "kTypeColor";
			break;
		case kTypeDepth:
			type = "kTypeDepth";
			break;
	}
	
	switch (colorPlane.format) {
		case kFormatUndefined:
			format = "kFormatUndefined";
			break;
		case kFormatUnsignedShort:
			format = "kFormatUnsignedShort";
			break;
		case kFormatFloat:
			format = "kFormatFloat";
			break;
		case kFormatUnsignedByte:
			format = "kFormatUnsignedByte";
			break;
		case kFormatScreen:
			format = "kFormatScreen";
			break;
	}
}

RenderSurface::RenderSurface(Context * ctx)
	:ContextObject(ctx)
{

}

RenderSurface::~RenderSurface() {
	
}

bool RenderSurface::supportFormat(RenderSurfaceFormat fmt) {
	return impl<bg::engine::RenderSurfaceImpl>()->supportFormat(fmt);
}
	
bool RenderSurface::supportType(RenderSurfaceType type) {
	return impl<bg::engine::RenderSurfaceImpl>()->supportType(type);
}

int RenderSurface::maxOutputColorPlanes() {
	return impl<bg::engine::RenderSurfaceImpl>()->maxOutpuColorPlanes();
}

void RenderSurface::invalidateSurface() {
	return impl<bg::engine::RenderSurfaceImpl>()->invalidateSurface();
}

void RenderSurface::invalidateTextures() {
	return impl<bg::engine::RenderSurfaceImpl>()->invalidateTextures();
}

void RenderSurface::setSize(const bg::math::Size2Di & size) {
	impl<bg::engine::RenderSurfaceImpl>()->setSize(size);
}

const bg::math::Size2Di & RenderSurface::size() const {
	return _impl->size();
}

void RenderSurface::create() {
	create(RenderSurface::ColorPlaneSetup{  RenderSurface::ColorPlane(RenderSurface::kTypeColor, RenderSurface::kFormatUnsignedByte),
											RenderSurface::ColorPlane(RenderSurface::kTypeDepth, RenderSurface::kFormatScreen) });
}

void RenderSurface::create(const ColorPlaneSetup & setup) {
	for (auto colorPlane : setup) {
		_impl->addOutputColorPlane(colorPlane.type, colorPlane.format);
	}
	_impl->create();
}

void RenderSurface::setActive() {
	impl<bg::engine::RenderSurfaceImpl>()->setActive();
}

unsigned char * RenderSurface::readPixels(const bg::math::Viewport & vp) {
	return impl<bg::engine::RenderSurfaceImpl>()->readPixels(vp);
}

void RenderSurface::destroy() {
	impl<bg::engine::RenderSurfaceImpl>()->destroy();
}


ScreenRenderSurface::ScreenRenderSurface(Context * ctx)
	:RenderSurface(ctx)
{
	_impl = bg::Engine::Get()->createRenderSurfaceImpl(this, engine::RenderSurfaceImpl::kTargetScreen);
}

ScreenRenderSurface::~ScreenRenderSurface() {
	
}
	

TextureRenderSurface::TextureRenderSurface(Context * ctx)
	:RenderSurface(ctx)
{
	_impl = bg::Engine::Get()->createRenderSurfaceImpl(this, engine::RenderSurfaceImpl::kTargetTexture);
}

TextureRenderSurface::~TextureRenderSurface() {
	
}

Texture * TextureRenderSurface::texture(int colorPlane) {
	return impl<bg::engine::RenderSurfaceImpl>()->texture(colorPlane);
}

}
}
