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

#ifndef _bg2e_engine_render_surface_impl_hpp_
#define _bg2e_engine_render_surface_impl_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/base/texture.hpp>

#include <bg/math/vector.hpp>

namespace bg {

namespace base {

class RenderSurface;

}

namespace engine {

class RenderSurfaceImpl : public bg::base::ReferencedPointer {
public:
	enum RenderSurfaceTarget {
		kTargetScreen = 1,
		kTargetTexture
	};
	
	RenderSurfaceImpl(bg::base::RenderSurface * surface, RenderSurfaceTarget target) :_renderSurface(surface), _target(target), _size(512) {}

	virtual bool supportFormat(int) = 0;
	virtual bool supportType(int) = 0;
	virtual int maxOutpuColorPlanes() = 0;
	virtual void invalidateSurface() = 0;
	virtual void invalidateTextures() = 0;
	
	virtual void setSize(const bg::math::Size2Di &) = 0;
	virtual const bg::math::Size2Di & size() const { return _size; }
	virtual void addOutputColorPlane(int type, int format) = 0;
	virtual void create() = 0;
	virtual void setActive() = 0;
	virtual unsigned char * readPixels(const bg::math::Viewport &) = 0;
	virtual void destroy() = 0;
	virtual bg::base::Texture * texture(int colorPlane) = 0;
	
	inline RenderSurfaceTarget target() const { return _target; }
protected:
	virtual ~RenderSurfaceImpl() {}
	
	base::RenderSurface * _renderSurface;
	RenderSurfaceTarget _target;
	bg::math::Size2Di _size;
};

}
}
#endif
