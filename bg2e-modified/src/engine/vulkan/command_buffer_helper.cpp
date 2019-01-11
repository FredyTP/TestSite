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

#include <memory>
#include <iostream>
#include <fstream>

#include <bg/log.hpp>

namespace bg {
namespace engine {
namespace vulkan {	

CommandBufferHelper::CommandBufferHelper(vk::Device * device, vk::CommandPool * pool, vk::Queue * queue)
    :_device(device)
    ,_pool(pool)
    ,_queue(queue)
{

}

void CommandBufferHelper::executeCommands(std::function<void(vk::CommandBuffer*)> closure) {
    bg::ptr<vk::CommandBuffer> cmdBuffer;
    vk::CommandBuffer::Allocate(_device, _pool, vk::COMMAND_BUFFER_LEVEL_PRIMARY, cmdBuffer);
    
    cmdBuffer->beginCommandBuffer(vk::COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    closure(cmdBuffer.getPtr());
    cmdBuffer->endCommandBuffer();
    
    _queue->submitWaitIdle({ cmdBuffer.getPtr() });
}

void CommandBufferHelper::beginCmdBuffer() {
    vk::CommandBuffer::Allocate(_device, _pool, vk::COMMAND_BUFFER_LEVEL_PRIMARY, _cmdBuffer);
    _cmdBuffer->beginCommandBuffer(vk::COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
}

void CommandBufferHelper::transitionImageLayout(vk::Image * img, vk::Format fmt, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
    auto closure = [&](vk::CommandBuffer * cmdBuffer) {
        vk::PipelineStageFlags srcStage;
        vk::PipelineStageFlags dstStage;
        
        vk::ImageMemoryBarrier barrier;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = BG_VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = BG_VK_QUEUE_FAMILY_IGNORED;
        barrier.image = img;

		if (newLayout == vk::IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
			barrier.subresourceRange.aspectMask = vk::IMAGE_ASPECT_DEPTH_BIT;

			if (PhysicalDeviceHelper::HasStencilFormat(fmt)) {
				barrier.subresourceRange.aspectMask |= vk::IMAGE_ASPECT_COLOR_BIT;
			}
		}
		else {
			barrier.subresourceRange.aspectMask = vk::IMAGE_ASPECT_COLOR_BIT;
		}

        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        
        if (oldLayout == vk::IMAGE_LAYOUT_UNDEFINED && newLayout == vk::IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = vk::ACCESS_TRANSFER_WRITE_BIT;
            
            srcStage = vk::PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            dstStage = vk::PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == vk::IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = vk::ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = vk::ACCESS_SHADER_READ_BIT;
            
            srcStage = vk::PIPELINE_STAGE_TRANSFER_BIT;
            dstStage = vk::PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = vk::ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | vk::ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            
            srcStage = vk::PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            dstStage = vk::PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }
        else {
            throw bg::base::VulkanEngineException("Unsupported layout transition.");
        }
        
        cmdBuffer->cmdPipelineBarrier(
            srcStage,
            dstStage,
            0, {}, {}, { barrier });
    };
    
    if (_cmdBuffer.valid()) {
        closure(_cmdBuffer.getPtr());
    }
    else {
        executeCommands(closure);
    }
}

void CommandBufferHelper::copyBufferToImage(vk::Buffer * buffer, vk::Image * image, uint32_t width, uint32_t height, uint32_t layers) {
    auto closure = [&](vk::CommandBuffer * cmdBuffer) {
        vk::BufferImageCopy region;
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        
        region.imageSubresource.aspectMask = vk::IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = layers;
        
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            width,
            height,
            1
        };
        
        cmdBuffer->cmdCopyBufferToImage(buffer, image, vk::IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, { region });
    };
    
    if (_cmdBuffer.valid()) {
        closure(_cmdBuffer.getPtr());
    }
    else {
        executeCommands(closure);
    }
}

void CommandBufferHelper::endCmdBuffer() {
    if (_cmdBuffer.isNull()) {
        bg::log(bg::log::kWarning) << "CommandBufferHelper: endCmdBuffer invoqued, bug the command buffer is not valid." << bg::endl;
    }
    _cmdBuffer->endCommandBuffer();
    _queue->submitWaitIdle({ _cmdBuffer.getPtr() });
    _cmdBuffer = nullptr;
}

}
}
}
