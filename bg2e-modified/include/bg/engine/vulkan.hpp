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

#ifndef _bg2e_vulkan_hpp_
#define _bg2e_vulkan_hpp_

#include <bg/engine.hpp>

#include <bg/base/exception.hpp>

#include <bg/engine/vulkan/vulkan_includes.hpp>

#include <bg/engine/vulkan/poly_list_impl.hpp>
#include <bg/engine/vulkan/texture_impl.hpp>
#include <bg/engine/vulkan/effect_impl.hpp>
#include <bg/engine/vulkan/command_queue_impl.hpp>

namespace bg {
namespace engine {

class BG2E_EXPORT Vulkan : public Engine {
public:
	
	static bool Supported();
	
	Vulkan();
	
	/*
	 *	Throws bg::base::CompatibilityException if the engine is not compatible
	 */
	virtual void initialize(bg::base::Context *);
	
	virtual bool supportsFeature(bg::base::Context *, EngineFeature feature);
		
	virtual bg::engine::PipelineImpl * createPipeline(bg::base::Pipeline *);
	virtual bg::engine::PolyListImpl * createPolyList(bg::base::PolyList *);
	virtual bg::engine::TextureImpl * createTextureImpl(bg::base::Texture *);
	virtual bg::engine::RenderSurfaceImpl * createRenderSurfaceImpl(bg::base::RenderSurface *,engine::RenderSurfaceImpl::RenderSurfaceTarget);
    virtual bg::engine::EffectImpl * createEffectImpl(bg::base::Effect *);
    virtual bg::engine::CommandQueueImpl * createCommandQueueImpl(bg::base::CommandQueue *);
	
protected:

};
	
}
}

#endif	// _bg2e_export_hpp_


