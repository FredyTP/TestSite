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


#include <bg/engine/vulkan.hpp>

#include <bg/base/exception.hpp>

#if BG2E_WITH_VULKAN_SUPPORT==1

#include <vulkan/vulkan.h>

#endif

#include <bg/wnd/window_controller.hpp>

#include <bg/engine/vulkan/poly_list_impl.hpp>

namespace bg {
namespace engine {
	
bool Vulkan::Supported() {
#if BG2E_WITH_VULKAN_SUPPORT==1
	return true;
#else
    return false;
#endif
}

Vulkan::Vulkan()
{
	_name = "Vulkan 1.0";
}
	
void Vulkan::initialize(bg::base::Context * context) {
#if BG2E_WITH_VULKAN_SUPPORT==1
    // TODO: Implement initialization
#else
	throw bg::base::CompatibilityException("Trying to instantiate a non-compatible rendering engine: Vulkan 1.0.");
#endif
}
	
bool Vulkan::supportsFeature(bg::base::Context *, bg::EngineFeature feature)  {
	return false;
}


bg::engine::PipelineImpl * Vulkan::createPipeline(bg::base::Pipeline * pipeline) {
	return nullptr;
}

bg::engine::PolyListImpl * Vulkan::createPolyList(bg::base::PolyList * plist) {
    return new vulkan::PolyListImpl(plist);
}

bg::engine::TextureImpl * Vulkan::createTextureImpl(bg::base::Texture * tex) {
    return new vulkan::TextureImpl(tex);
}

bg::engine::RenderSurfaceImpl * Vulkan::createRenderSurfaceImpl(bg::base::RenderSurface * surface,engine::RenderSurfaceImpl::RenderSurfaceTarget target) {
    return nullptr;
}

bg::engine::EffectImpl * Vulkan::createEffectImpl(bg::base::Effect * effect) {
    return new vulkan::EffectImpl(effect);
}

bg::engine::CommandQueueImpl * Vulkan::createCommandQueueImpl(bg::base::CommandQueue * q) {
    return new vulkan::CommandQueueImpl(q);
}

}
}

