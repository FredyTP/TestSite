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

#include <vulkan/vulkan.h>

#include <bg/engine/vulkan/vulkan_includes.hpp>

#include <bg/wnd/window_controller.hpp>
#include <bg/wnd/vk_context.hpp>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace bg {
namespace engine {
namespace vulkan {

RenderManager::RenderManager(bg::wnd::VKContext * ctx)
    :_ctx(ctx)
    ,_currentFrame(0)
    ,_inFlightFrames(0)
{
}

RenderManager::~RenderManager() {
    _syncObjects.destroy();
    _inFlightFrames = 0;
    _currentFrame = 0;
}

void RenderManager::create() {
    _inFlightFrames = _ctx->swapchainSize();
    _currentFrame = 0;
    for (uint32_t i = 0; i < _inFlightFrames; ++i) {
        _syncObjects.imageAvailableSemaphore.push_back(new vk::Semaphore(_ctx->device()));
        _syncObjects.renderFinishedSemaphore.push_back(new vk::Semaphore(_ctx->device()));
        _syncObjects.frameFences.push_back(new vk::Fence(_ctx->device()));
    }
}

void RenderManager::waitForPreviousFrame() {
    std::vector<vulkan::vk::Fence*> fences{ _syncObjects.frameFences[_currentFrame].getPtr() };
    _ctx->device()->waitForFences(fences, true, std::numeric_limits<uint64_t>::max());
    _ctx->device()->resetFences(fences);
}
    
void RenderManager::presentNextFrame() {
    uint32_t imageIndex;
    _ctx->swapchain()->acquireNextImage(std::numeric_limits<uint64_t>::max(), _syncObjects.imageAvailableSemaphore[_currentFrame].getPtr(), nullptr, imageIndex);
    
    std::vector<vk::Semaphore*> signalSemaphore{ _syncObjects.renderFinishedSemaphore[_currentFrame].getPtr() };
    _ctx->graphicsQueue()->submit(
        { { vk::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, _syncObjects.imageAvailableSemaphore[_currentFrame].getPtr() } },
        { _ctx->commandBuffer(imageIndex) },
        signalSemaphore,
        _syncObjects.frameFences[_currentFrame].getPtr());
    
    _ctx->presentQueue()->present(signalSemaphore, { _ctx->swapchain() }, { imageIndex });
    
    _currentFrame = (_currentFrame + 1) % _inFlightFrames;
}

}
}
}
