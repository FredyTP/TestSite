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


#ifndef _bg2e_render_forward_render_layer_hpp_
#define _bg2e_render_forward_render_layer_hpp_

#include <bg/render/render_layer.hpp>
#include <bg/fx/forward.hpp>
#include <bg/scene/light.hpp>
#include <bg/render/shadow_map.hpp>

namespace bg {
namespace render {

class BG2E_EXPORT ForwardRenderLayer : public RenderLayer {
public:
	ForwardRenderLayer(bg::base::Context *, bg::base::Pipeline * pl);
	
	inline void setLightComponent(bg::scene::Light * light) { if (light) _lightComponent = light; }
	inline const bg::scene::Light * lightComponent() const { return _lightComponent.getPtr(); }
	inline bg::scene::Light * lightComponent() { return _lightComponent.getPtr(); }

	inline void setShadowMap(bg::render::ShadowMap * sm) { _shadowMap = sm; }
	
	virtual void invalidateBuffers();
	virtual void invalidateTextures();

protected:
	virtual ~ForwardRenderLayer();
	
	virtual void performDraw(bg::scene::Node *, bg::scene::Camera *);
	
	bg::ptr<bg::scene::Light> _lightComponent;
	bg::ptr<bg::render::ShadowMap> _shadowMap;
};

}
}

#endif
