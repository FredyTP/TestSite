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


#ifndef _bg2e_render_forward_renderer_hpp_
#define _bg2e_render_forward_renderer_hpp_

#include <bg/render/renderer.hpp>
#include <bg/render/render_layer.hpp>

#include <bg/render/forward_render_layer.hpp>
#include <bg/render/shadow_map.hpp>

#include <bg/fx/forward.hpp>

namespace bg {
namespace render {


class BG2E_EXPORT ForwardRenderer : public ClassicRenderer {
	friend class Renderer;
public:
	virtual bool isSupported();
	virtual void create();
	
	void draw(bg::scene::Node *, bg::scene::Camera *);
	virtual void invalidateBuffers();
	virtual void invalidateTextures();

	void setShadowMap(ShadowMap * sm) { _shadowMap = sm; }
	
protected:
	ForwardRenderer(bg::base::Context *);
	virtual ~ForwardRenderer();
	
	bg::ptr<RenderLayer> _transparentLayer;
	bg::ptr<RenderLayer> _opaqueLayer;
	bg::ptr<ShadowMap> _shadowMap;
	bg::ptr<bg::fx::Forward> _effect;
};

}
}

#endif
