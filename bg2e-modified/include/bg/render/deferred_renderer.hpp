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

#ifndef _bg2e_render_deferred_renderer_hpp_
#define _bg2e_render_deferred_renderer_hpp_

#include <bg/render/renderer.hpp>
#include <bg/render/deferred_render_layer.hpp>
#include <bg/render/shadow_map.hpp>

namespace bg {
namespace render {

class BG2E_EXPORT DeferredRenderer : public ClassicRenderer {
	friend class Renderer;
public:
	virtual bool isSupported();
	virtual void create();
	
	virtual void draw(bg::scene::Node *, bg::scene::Camera *);
	
	virtual void invalidateBuffers();
	virtual void invalidateTextures();

protected:
	DeferredRenderer(bg::base::Context *);
	virtual ~DeferredRenderer();

	bg::ptr<RenderLayer> _transparentLayer;
	bg::ptr<RenderLayer> _opaqueLayer;
	bg::ptr<ShadowMap> _shadowMap;

	bg::ptr<AmbientOcclusionSettings> _ssaoSettings;
	bg::ptr<RaytracerSettings> _ssrtSettings;
	bg::ptr<DebugRenderSettings> _debugSettings;
};

}
}

#endif
