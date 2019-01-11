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


#ifndef _bg2e_render_low_overhead_forward_renderer_hpp_
#define _bg2e_render_low_overhead_forward_renderer_hpp_

#include <bg/render/renderer.hpp>
#include <bg/render/render_layer.hpp>
#include <bg/render/update_visitor.hpp>

#include <bg/scene/render_queue_visitor.hpp>

#include <bg/render/low_overhead_pipeline.hpp>

#include <bg/engine/vulkan/vulkan_includes.hpp>

namespace bg {
namespace render {

class LowOverheadForwardRenderer : public Renderer {
public:
    LowOverheadForwardRenderer(bg::base::Context * ctx);
    
    virtual bool isSupported();
    
    inline void setShaderMaterial(ShaderMaterial * mat) { _loPipeline->setShaderMaterial(mat); }
    inline ShaderMaterial * shaderMaterial() { return _loPipeline->shaderMaterial(); }

    virtual void create();
    
    virtual void resize(int w, int h);
    
    virtual void frame(bg::scene::Node * node, float delta);
    
    virtual void draw(bg::scene::Node * node, bg::scene::Camera * camera);
    
    virtual void invalidateTextures();
    
    inline LowOverheadPipeline * pipeline() { return _loPipeline.getPtr(); }
    
protected:
    virtual ~LowOverheadForwardRenderer();
    
    bg::ptr<bg::engine::vulkan::RenderManager> _renderManager;
    bg::ptr<bg::render::LowOverheadPipeline> _loPipeline;
    
    bg::ptr<bg::render::UpdateVisitor> _updateVisitor;
    bg::ptr<bg::scene::RenderQueueVisitor> _renderQueueVisitor;
    bg::ptr<bg::scene::TransformVisitor> _transformVisitor;
    float _delta;
    
    inline bg::wnd::VKContext * vkContext() { return bg::engine::vulkan::context(context()); }
};

}
}

#endif
