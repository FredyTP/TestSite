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

#include <bg/wnd/window_controller.hpp>
#include <bg/wnd/vk_context.hpp>

#include <memory>
#include <iostream>
#include <fstream>

#include <bg/log.hpp>

namespace bg {
namespace engine {
namespace vulkan {

RenderPassHelper::RenderPassHelper(bg::wnd::VKContext * ctx)
	:_ctx(ctx)
{
}

vk::RenderPass * RenderPassHelper::create(vk::Format depthFormat) {
	bg::ptr<vk::RenderPass> renderPass = new vk::RenderPass();

	vulkan::vk::AttachmentDescription colorAttachment;
	colorAttachment.format = _ctx->swapchain()->format();
	colorAttachment.samples = vulkan::vk::SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = vulkan::vk::ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = vulkan::vk::ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = vulkan::vk::ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = vulkan::vk::ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = vulkan::vk::IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = vulkan::vk::IMAGE_LAYOUT_PRESENT_SRC_KHR;

	vulkan::vk::AttachmentDescription depthAttachment;
	depthAttachment.format = depthFormat;
	depthAttachment.samples = vulkan::vk::SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = vulkan::vk::ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = vulkan::vk::ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = vulkan::vk::ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = vulkan::vk::ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = vulkan::vk::IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = vulkan::vk::IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	vulkan::vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vulkan::vk::PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachments.push_back({ 0, vulkan::vk::IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
	subpass.depthStencilAttachment = { 1, vulkan::vk::IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
	subpass.useDepthStencilAttachment = true;

	renderPass->createInfo.attachments.push_back(colorAttachment);
	renderPass->createInfo.attachments.push_back(depthAttachment);
	renderPass->createInfo.subpasses.push_back(subpass);

	vulkan::vk::SubpassDependency dep;
	dep.srcSubpass = BG_VK_SUBPASS_EXTERNAL;
	dep.dstSubpass = 0;
	dep.srcStageMask = vulkan::vk::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dep.srcAccessMask = 0;
	dep.dstStageMask = vulkan::vk::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dep.dstAccessMask = vulkan::vk::ACCESS_COLOR_ATTACHMENT_READ_BIT | vulkan::vk::ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	renderPass->createInfo.dependencies.push_back(dep);
	renderPass->create(_ctx->device());

	return renderPass.release();
}

vk::RenderPass * RenderPassHelper::create() {
	bg::ptr<vk::RenderPass> renderPass = new vk::RenderPass();

	vulkan::vk::AttachmentDescription colorAttachment;
	colorAttachment.format = _ctx->swapchain()->format();
	colorAttachment.samples = vulkan::vk::SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = vulkan::vk::ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = vulkan::vk::ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = vulkan::vk::ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = vulkan::vk::ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = vulkan::vk::IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = vulkan::vk::IMAGE_LAYOUT_PRESENT_SRC_KHR;

	vulkan::vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vulkan::vk::PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachments.push_back({ 0, vulkan::vk::IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

	renderPass->createInfo.attachments.push_back(colorAttachment);
	renderPass->createInfo.subpasses.push_back(subpass);

	vulkan::vk::SubpassDependency dep;
	dep.srcSubpass = BG_VK_SUBPASS_EXTERNAL;
	dep.dstSubpass = 0;
	dep.srcStageMask = vulkan::vk::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dep.srcAccessMask = 0;
	dep.dstStageMask = vulkan::vk::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dep.dstAccessMask = vulkan::vk::ACCESS_COLOR_ATTACHMENT_READ_BIT | vulkan::vk::ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	renderPass->createInfo.dependencies.push_back(dep);
	renderPass->create(_ctx->device());

	return renderPass.release();
}

vk::RenderPass * RenderPassHelper::create(
    const vk::AttachmentDescription & colorAttachment,
    const vk::AttachmentDescription & depthAttachment,
    const std::vector<vk::SubpassDependency> & subpassDep
) {
    bg::ptr<vk::RenderPass> renderPass = new vk::RenderPass();

    vulkan::vk::SubpassDescription subpass;
    subpass.pipelineBindPoint = vk::PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachments.push_back({ 0, vk::IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
    subpass.depthStencilAttachment = { 1, vk::IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
    subpass.useDepthStencilAttachment = true;

    renderPass->createInfo.attachments.push_back(colorAttachment);
    renderPass->createInfo.attachments.push_back(depthAttachment);
    renderPass->createInfo.subpasses.push_back(subpass);

    for (const auto & dep : subpassDep) {
        renderPass->createInfo.dependencies.push_back(dep);
    }
    renderPass->create(_ctx->device());

    return renderPass.release();
}


}
}
}
