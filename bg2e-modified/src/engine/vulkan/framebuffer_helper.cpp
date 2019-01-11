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

#include <bg/base/exception.hpp>

#include <bg/wnd/window.hpp>
#include <bg/wnd/window_controller.hpp>
#include <bg/wnd/vk_context.hpp>

#include <memory>
#include <iostream>
#include <fstream>

#include <bg/log.hpp>

namespace bg {
namespace engine {
namespace vulkan {	

FramebufferHelper::FramebufferHelper(bg::wnd::VKContext * ctx)
    :_ctx(ctx)
{

}

void FramebufferHelper::createSwapchainFramebuffers(vk::RenderPass * rp, bool depthBufferEnabled, std::vector<bg::ptr<vk::Framebuffer>> & result) {
    result.clear();
    for (auto imgView : _ctx->swapchainImageViews()) {
        vulkan::vk::Framebuffer * fb = new vulkan::vk::Framebuffer();
        fb->createInfo.renderPass = rp;
        fb->createInfo.attachments.push_back(imgView.getPtr());
        if (depthBufferEnabled) fb->createInfo.attachments.push_back(_ctx->depthImageView());
        fb->createInfo.width = _ctx->swapchain()->extent().width;
        fb->createInfo.height = _ctx->swapchain()->extent().height;
        fb->createInfo.layers = 1;
        fb->create(_ctx->device());
        result.push_back(fb);
    }
}

void FramebufferHelper::createSwapchainFramebuffers(vk::RenderPass * rp, bool depthBufferEnabled, std::vector<vk::Framebuffer*> & result) {
    result.clear();
    for (auto imgView : _ctx->swapchainImageViews()) {
        vulkan::vk::Framebuffer * fb = new vulkan::vk::Framebuffer();
        fb->createInfo.renderPass = rp;
        fb->createInfo.attachments.push_back(imgView.getPtr());
		if (depthBufferEnabled) fb->createInfo.attachments.push_back(_ctx->depthImageView());
        fb->createInfo.width = _ctx->swapchain()->extent().width;
        fb->createInfo.height = _ctx->swapchain()->extent().height;
        fb->createInfo.layers = 1;
        fb->create(_ctx->device());
        result.push_back(fb);
    }
}

}
}
}
