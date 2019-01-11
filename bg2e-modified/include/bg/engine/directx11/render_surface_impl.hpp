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

#ifndef _bg2e_engine_directx11_render_surface_impl_hpp_
#define _bg2e_engine_directx11_render_surface_impl_hpp_

#include <bg/engine/render_surface_impl.hpp>
#include <bg/base/render_surface.hpp>

namespace bg {
namespace engine {
namespace directx11 {

class BG2E_EXPORT RenderSurfaceImpl : public bg::engine::RenderSurfaceImpl {
public:
	RenderSurfaceImpl(bg::base::RenderSurface *, RenderSurfaceTarget);
	
	virtual bool supportFormat(int);
	virtual bool supportType(int);
	virtual int maxOutpuColorPlanes();
	virtual void invalidateSurface();
	virtual void invalidateTextures();
	
	virtual void setSize(const bg::math::Size2Di &);
	virtual void addOutputColorPlane(int type, int format);
	virtual void create();
	virtual void setActive();
	virtual unsigned char * readPixels(const bg::math::Viewport &);
	virtual void destroy();
	virtual bg::base::Texture * texture(int colorPlane);
	
protected:
	virtual ~RenderSurfaceImpl();
	
	bg::base::RenderSurface::ColorPlaneSetup _colorPlaneSetup;
};

}
}
}

#endif
