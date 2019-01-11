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

BufferHelper::BufferHelper(vk::PhysicalDevice * phDev, vk::Device * dev)
    :_phDev(phDev)
    ,_dev(dev)
{
}

void BufferHelper::createBuffer(
    vk::DeviceSize size,
    vk::BufferUsageFlags usage,
    vk::SharingMode sharingMode,
    vk::MemoryPropertyFlags properties,
    bg::ptr<vk::Buffer> &buffer,
    bg::ptr<vk::DeviceMemory> &memory)
{
    buffer = new vk::Buffer();
    buffer->createInfo.size = size;
    buffer->createInfo.usage = usage;
    buffer->createInfo.sharingMode = sharingMode;
    buffer->create(_dev);

    vk::MemoryRequirements memReq;
    buffer->getBufferMemoryRequirements(memReq);
    
    PhysicalDeviceHelper physicalDevHelper(_phDev);
    
    memory = new vk::DeviceMemory();
    memory->allocateInfo.allocationSize = memReq.size;
    memory->allocateInfo.memoryTypeIndex = physicalDevHelper.findMemoryType(memReq.memoryTypeBits, properties);
    memory->allocateMemory(_dev);
    memory->bindBufferMemory(buffer.getPtr(), 0);
}

void BufferHelper::mapMemory(vk::DeviceMemory * mem, vk::DeviceSize offset, vk::MemoryMapFlags flags, const void * data, size_t size) {
    void * memData;
    mem->mapMemory(offset, static_cast<vk::DeviceSize>(size), flags, &memData);
    memcpy(memData, data, static_cast<size_t>(size));
    mem->unmapMemory();
}

void BufferHelper::createStagingBuffer(
    vk::CommandPool * cmdPool,
    vk::Queue * queue,
    vk::BufferUsageFlags usage,
    const void * data,
    vk::DeviceSize size,
    bg::ptr<vk::Buffer> & buffer, bg::ptr<vk::DeviceMemory> & memory)
{
    bg::ptr<vk::Buffer> stagingBuffer = new vk::Buffer();
    bg::ptr<vk::DeviceMemory> stagingBufferMemory = new vk::DeviceMemory();
    createBuffer(
        size,
        vk::BUFFER_USAGE_TRANSFER_SRC_BIT,
        vk::SHARING_MODE_EXCLUSIVE,
        vk::MEMORY_PROPERTY_HOST_VISIBLE_BIT | vk::MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory);
    
    mapMemory(stagingBufferMemory.getPtr(), 0, 0, data, size);
    
    createBuffer(
        size,
        vk::BUFFER_USAGE_TRANSFER_DST_BIT | usage,
        vk::SHARING_MODE_EXCLUSIVE,
        vk::MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        buffer, memory);
    
    copyBuffer(cmdPool, queue, stagingBuffer.getPtr(), buffer.getPtr(), size);
    
    stagingBuffer = nullptr;        // Destroy staging buffer
    stagingBufferMemory = nullptr;  // Destroy staging buffer memory
}

void BufferHelper::copyBuffer(vk::CommandPool * cmdPool, vk::Queue * queue, vk::Buffer * src, vk::Buffer * dst, vk::DeviceSize size) {
    CommandBufferHelper cmdBufferHelper(_dev, cmdPool, queue);
    cmdBufferHelper.executeCommands([&](vk::CommandBuffer * cmdBuffer) {
        cmdBuffer->cmdCopyBuffer(size, src, dst);
    });
}

void BufferHelper::createImageBuffer(vk::CommandPool * cmdPool, vk::Queue * queue, bg::base::Image * image, bg::ptr<vk::Image> & textureImage, bg::ptr<vk::DeviceMemory> & textureImageMemory) {
    if (textureImage.isNull()) {
        textureImage = new vk::Image();
        textureImage->createInfo.mipLevels = 1;
        textureImage->createInfo.tiling = vk::IMAGE_TILING_OPTIMAL;
        textureImage->createInfo.usage = vk::IMAGE_USAGE_SAMPLED_BIT;
        textureImage->createInfo.sharingMode = vk::SHARING_MODE_EXCLUSIVE;
    }
    bg::ptr<vk::Buffer> stagingBuffer = new vk::Buffer();
    bg::ptr<vk::DeviceMemory> stagingBufferMemory = new vk::DeviceMemory();
    vk::DeviceSize size = image->width() * image->height() * image->bytesPerPixel();
    createBuffer(size, vk::BUFFER_USAGE_TRANSFER_SRC_BIT, vk::SHARING_MODE_EXCLUSIVE, vk::MEMORY_PROPERTY_HOST_VISIBLE_BIT | vk::MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    
    void * data;
    stagingBufferMemory->mapMemory(0, size, 0, &data);
    memcpy(data, image->buffer(), static_cast<size_t>(size));
    stagingBufferMemory->unmapMemory();
    
    textureImage->createInfo.imageType = vk::IMAGE_TYPE_2D;
    textureImage->createInfo.arrayLayers = 1;
    textureImage->createInfo.extent.width = image->width();
    textureImage->createInfo.extent.height = image->height();
    textureImage->createInfo.extent.depth = 1;
    textureImage->createInfo.initialLayout = vk::IMAGE_LAYOUT_UNDEFINED;
	textureImage->createInfo.samples = vk::SAMPLE_COUNT_1_BIT;
    switch (image->imageFormat()) {
    case bg::base::Image::kFormatRGB:
        textureImage->createInfo.format = vk::FORMAT_R8G8B8_UNORM;
        break;
    case bg::base::Image::kFormatRGBA:
        textureImage->createInfo.format = vk::FORMAT_R8G8B8A8_UNORM;
        break;
    case bg::base::Image::kFormatGrayScale:
        textureImage->createInfo.format = vk::FORMAT_R8_UNORM;
        break;
    default:
        textureImage->createInfo.format = vk::FORMAT_R8G8B8A8_UNORM;
    }
    
    textureImage->createInfo.usage = vk::IMAGE_USAGE_TRANSFER_DST_BIT | textureImage->createInfo.usage;
    
    textureImage->create(_dev);
    
    vk::MemoryRequirements req;
    textureImage->geImageMemoryRequirements(req);
    textureImageMemory = new vk::DeviceMemory();
    
    PhysicalDeviceHelper physicalDevHelper(_phDev);
    
    textureImageMemory->allocateInfo.allocationSize = req.size;
    textureImageMemory->allocateInfo.memoryTypeIndex = physicalDevHelper.findMemoryType(req.memoryTypeBits, vk::MEMORY_HEAP_DEVICE_LOCAL_BIT);
    textureImageMemory->allocateMemory(_dev);
    textureImageMemory->bindImageMemory(textureImage.getPtr(), 0);
    
    CommandBufferHelper cmdBufferHelper(_dev, cmdPool, queue);
    cmdBufferHelper.beginCmdBuffer();
    cmdBufferHelper.transitionImageLayout(
        textureImage.getPtr(),
        textureImage->createInfo.format,
        vk::IMAGE_LAYOUT_UNDEFINED, vk::IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    cmdBufferHelper.copyBufferToImage(
        stagingBuffer.getPtr(),
        textureImage.getPtr(),
        image->width(), image->height());
    cmdBufferHelper.transitionImageLayout(
        textureImage.getPtr(),
        textureImage->createInfo.format,
        vk::IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, vk::IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    cmdBufferHelper.endCmdBuffer();
}

void BufferHelper::createImageBuffer(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, bg::ptr<vk::Image> & image, bg::ptr<vk::DeviceMemory> & imageMemory) {
    image = new vk::Image();
    image->createInfo.imageType = vk::IMAGE_TYPE_2D;
	image->createInfo.flags = 0;
    image->createInfo.extent = { width, height, 1 };
    image->createInfo.mipLevels = 1;
    image->createInfo.arrayLayers = 1;
    image->createInfo.format = format;
    image->createInfo.tiling = tiling;
    image->createInfo.initialLayout = vk::IMAGE_LAYOUT_UNDEFINED;
    image->createInfo.usage = usage;
    image->createInfo.samples = vk::SAMPLE_COUNT_1_BIT;
    image->createInfo.sharingMode = vk::SHARING_MODE_EXCLUSIVE;
    image->create(_dev);

    vk::MemoryRequirements req = image->getImageMemoryRequirements();
    imageMemory = new vk::DeviceMemory();
    imageMemory->allocateInfo.allocationSize = req.size;
    PhysicalDeviceHelper devHelper(_phDev);
    imageMemory->allocateInfo.memoryTypeIndex = devHelper.findMemoryType(req.memoryTypeBits, properties);
    imageMemory->allocateMemory(_dev);
    imageMemory->bindImageMemory(image.getPtr(), 0);
}


}
}
}
