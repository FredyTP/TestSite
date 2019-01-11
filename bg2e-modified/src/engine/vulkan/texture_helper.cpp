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

TextureHelper::TextureHelper(vk::PhysicalDevice * phDev, vk::Device * dev)
    :BufferHelper(phDev, dev)
{

    imageViewCreateInfo.viewType = vk::IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.subresourceRange.aspectMask = vk::IMAGE_ASPECT_COLOR_BIT;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;
    
    samplerCreateInfo.magFilter = vk::FILTER_LINEAR;
    samplerCreateInfo.minFilter = vk::FILTER_LINEAR;
    samplerCreateInfo.addressModeU = vk::SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeV = vk::SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeW = vk::SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.anisotropyEnable = true;
    samplerCreateInfo.maxAnisotropy = 16;
    samplerCreateInfo.borderColor = vk::BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerCreateInfo.unnormalizedCoordinates = false;
    samplerCreateInfo.compareEnable = false;
    samplerCreateInfo.compareOp = vk::COMPARE_OP_ALWAYS;
    samplerCreateInfo.mipmapMode = vk::SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.mipLodBias = 0.0f;
    samplerCreateInfo.minLod = 0.0f;
    samplerCreateInfo.maxLod = 0.0f;
}

void TextureHelper::createTexture(vk::CommandPool * cmdPool, vk::Queue * queue, const std::string & imgPath, bg::ptr<vk::Image> & texImage, bg::ptr<vk::DeviceMemory> & texMemory, bg::ptr<vk::ImageView> & imgView, bg::ptr<vk::Sampler> & sampler) {
    bg::ptr<bg::base::Image> image = new bg::base::Image();
    image->load(imgPath);
    if (!image->valid()) {
        throw bg::base::NoSuchFileException("Could not create texture from image at " + imgPath);
    }
    
    createTexture(cmdPool, queue, image.getPtr(), texImage, texMemory, imgView, sampler);
}

void TextureHelper::createTexture(vk::CommandPool *cmdPool, vk::Queue *queue, bg::base::Image *img, bg::ptr<vk::Image> &texImage, bg::ptr<vk::DeviceMemory> &texMemory, bg::ptr<vk::ImageView> &imgView, bg::ptr<vk::Sampler> &sampler) {
    createImageBuffer(cmdPool, queue, img, texImage, texMemory);
    
    imgView = new vk::ImageView();
    imgView->createInfo.image = texImage.getPtr();
    imgView->createInfo.viewType = imageViewCreateInfo.viewType;
    imgView->createInfo.format = texImage->createInfo.format;
    imgView->createInfo.subresourceRange = imageViewCreateInfo.subresourceRange;
    imgView->create(_dev);
    
    sampler = new vk::Sampler();
    sampler->createInfo = samplerCreateInfo;
    sampler->create(_dev);
}

void TextureHelper::createCubemap(
        vk::CommandPool * cmdPool,
        vk::Queue * queue,
        const std::string & posX,
        const std::string & negX,
        const std::string & posY,
        const std::string & negY,
        const std::string & posZ,
        const std::string & negZ,
        bg::ptr<vk::Image> & texImage,
        bg::ptr<vk::DeviceMemory> & texMemory,
        bg::ptr<vk::ImageView> & imgView,
        bg::ptr<vk::Sampler> & sampler)
{
    bg::ptr<bg::base::Image> posXImage = new bg::base::Image();
    bg::ptr<bg::base::Image> negXImage = new bg::base::Image();
    bg::ptr<bg::base::Image> posYImage = new bg::base::Image();
    bg::ptr<bg::base::Image> negYImage = new bg::base::Image();
    bg::ptr<bg::base::Image> posZImage = new bg::base::Image();
    bg::ptr<bg::base::Image> negZImage = new bg::base::Image();
    
    posXImage->load(posX);
    negXImage->load(negX);
    posYImage->load(posY);
    negYImage->load(negY);
    posZImage->load(posZ);
    negZImage->load(negZ);
    if (!posXImage->valid() || !negXImage->valid() ||
        !posYImage->valid() || !negYImage->valid() ||
        !posZImage->valid() || !negZImage->valid())
    {
        throw bg::base::NoSuchFileException("Could not create cubemap image. No such file.");
    }
    createCubemap(
        cmdPool, queue,
        posXImage.getPtr(), negXImage.getPtr(),
        posYImage.getPtr(), negYImage.getPtr(),
        posZImage.getPtr(), negZImage.getPtr(),
        texImage, texMemory, imgView, sampler);
}

void TextureHelper::createCubemap(
    vk::CommandPool * cmdPool,
    vk::Queue * queue,
    bg::base::Image * posX,
    bg::base::Image * negX,
    bg::base::Image * posY,
    bg::base::Image * negY,
    bg::base::Image * posZ,
    bg::base::Image * negZ,
    bg::ptr<vk::Image> & texImage,
    bg::ptr<vk::DeviceMemory> & texMemory,
    bg::ptr<vk::ImageView> & imgView,
    bg::ptr<vk::Sampler> & sampler)
{
    auto w = posX->width();
    auto h = posX->height();
    if (w != h || !bg::math::isPowerOfTwo(w)) {
        throw bg::base::InvalidParameterException("Error creating cubemap: square, power of two image required.");
    }
    if (negX->width() != w || negX->height() != h ||
        posY->width() != w || posY->height() != h ||
        negY->width() != w || negY->height() != h ||
        posZ->width() != w || posZ->height() != h ||
        negZ->width() != w || negZ->height() != h
    ) {
        throw bg::base::InvalidParameterException("Error creating cubemap: all the images must have the same size.");
    }
    
    if (texImage.isNull()) {
        texImage = new vk::Image();
        texImage->createInfo.mipLevels = 1;
        texImage->createInfo.tiling = vk::IMAGE_TILING_OPTIMAL;
        texImage->createInfo.usage = vk::IMAGE_USAGE_SAMPLED_BIT;
        texImage->createInfo.sharingMode = vk::SHARING_MODE_EXCLUSIVE;
    }
    bg::ptr<vk::Buffer> stagingBuffer = new vk::Buffer();
    bg::ptr<vk::DeviceMemory> stagingBufferMemory = new vk::DeviceMemory();
    size_t imageSize = w * h * posX->bytesPerPixel();
    vk::DeviceSize size = imageSize * 6;    // 6 images, one for each layer?
    createBuffer(size, vk::BUFFER_USAGE_TRANSFER_SRC_BIT, vk::SHARING_MODE_EXCLUSIVE, vk::MEMORY_PROPERTY_HOST_VISIBLE_BIT | vk::MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    
    uint8_t * data;
    size_t offset = 0;
    stagingBufferMemory->mapMemory(0, size, 0, reinterpret_cast<void**>(&data));
    memcpy(data, posX->buffer(), static_cast<size_t>(imageSize));
    offset += imageSize;
    memcpy(data + offset, negX->buffer(), static_cast<size_t>(imageSize));
    offset += imageSize;
    memcpy(data + offset, posY->buffer(), static_cast<size_t>(imageSize));
    offset += imageSize;
    memcpy(data + offset, negY->buffer(), static_cast<size_t>(imageSize));
    offset += imageSize;
    memcpy(data + offset, posZ->buffer(), static_cast<size_t>(imageSize));
    offset += imageSize;
    memcpy(data + offset, negZ->buffer(), static_cast<size_t>(imageSize));
    stagingBufferMemory->unmapMemory();
    
    texImage->createInfo.imageType = vk::IMAGE_TYPE_2D;
    texImage->createInfo.flags = vk::IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    texImage->createInfo.arrayLayers = 6;   // One layer for each image
    texImage->createInfo.extent.width = w;
    texImage->createInfo.extent.height = h;
    texImage->createInfo.extent.depth = 1;
    texImage->createInfo.initialLayout = vk::IMAGE_LAYOUT_UNDEFINED;
    texImage->createInfo.samples = vk::SAMPLE_COUNT_1_BIT;
    switch (posX->imageFormat()) {
    case bg::base::Image::kFormatRGB:
        texImage->createInfo.format = vk::FORMAT_R8G8B8_UNORM;
        break;
    case bg::base::Image::kFormatRGBA:
        texImage->createInfo.format = vk::FORMAT_R8G8B8A8_UNORM;
        break;
    case bg::base::Image::kFormatGrayScale:
        texImage->createInfo.format = vk::FORMAT_R8_UNORM;
        break;
    default:
        texImage->createInfo.format = vk::FORMAT_R8G8B8A8_UNORM;
    }
    
    texImage->createInfo.usage = vk::IMAGE_USAGE_TRANSFER_DST_BIT | texImage->createInfo.usage;
    
    texImage->create(_dev);
    
    vk::MemoryRequirements req;
    texImage->geImageMemoryRequirements(req);
    texMemory = new vk::DeviceMemory();
    
    PhysicalDeviceHelper physicalDevHelper(_phDev);
    
    texMemory->allocateInfo.allocationSize = req.size;
    texMemory->allocateInfo.memoryTypeIndex = physicalDevHelper.findMemoryType(req.memoryTypeBits, vk::MEMORY_HEAP_DEVICE_LOCAL_BIT);
    texMemory->allocateMemory(_dev);
    texMemory->bindImageMemory(texImage.getPtr(), 0);
    
    CommandBufferHelper cmdBufferHelper(_dev, cmdPool, queue);
    cmdBufferHelper.beginCmdBuffer();
    cmdBufferHelper.transitionImageLayout(
        texImage.getPtr(),
        texImage->createInfo.format,
        vk::IMAGE_LAYOUT_UNDEFINED, vk::IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    cmdBufferHelper.copyBufferToImage(
        stagingBuffer.getPtr(),
        texImage.getPtr(),
        w, h, 6);
    cmdBufferHelper.transitionImageLayout(
        texImage.getPtr(),
        texImage->createInfo.format,
        vk::IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, vk::IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    cmdBufferHelper.endCmdBuffer();
    
    imgView = new vk::ImageView();
    imgView->createInfo.image = texImage.getPtr();
    imgView->createInfo.viewType = vk::IMAGE_VIEW_TYPE_CUBE;
    imgView->createInfo.format = texImage->createInfo.format;
    imgView->createInfo.subresourceRange = imageViewCreateInfo.subresourceRange;
    imgView->createInfo.subresourceRange.baseArrayLayer = 0;
    imgView->createInfo.subresourceRange.layerCount = 6;
    imgView->create(_dev);
    
    sampler = new vk::Sampler();
    sampler->createInfo = samplerCreateInfo;
    sampler->create(_dev);
}

}
}
}
