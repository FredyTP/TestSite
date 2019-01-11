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

#include <bg/engine/vulkan/texture_impl.hpp>

#include <bg/wnd/window.hpp>
#include <bg/wnd/vk_context.hpp>
#include <bg/wnd/window_controller.hpp>

namespace bg {
namespace engine {
namespace vulkan {

TextureImpl::TextureImpl(bg::base::Texture * tex)
	:bg::engine::TextureImpl(tex)
{

}

TextureImpl::~TextureImpl() {

}

void TextureImpl::create(const unsigned char * buffer, int width, int height, int bpp) {
	bg::wnd::VKContext * ctx = vulkan::context(_texture->context());
	TextureHelper texHelper(ctx->physicalDevice(), ctx->device());

	switch (_texture->wrapModeU()) {
	case bg::base::Texture::kWrapModeClamp:
		texHelper.samplerCreateInfo.addressModeU = vk::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		break;
	case bg::base::Texture::kWrapModeRepeat:
		texHelper.samplerCreateInfo.addressModeU = vk::SAMPLER_ADDRESS_MODE_REPEAT;
		break;
	}

	switch (_texture->wrapModeV()) {
	case bg::base::Texture::kWrapModeClamp:
		texHelper.samplerCreateInfo.addressModeV = vk::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		break;
	case bg::base::Texture::kWrapModeRepeat:
		texHelper.samplerCreateInfo.addressModeV = vk::SAMPLER_ADDRESS_MODE_REPEAT;
		break;
	}

	switch (_texture->wrapModeW()) {
	case bg::base::Texture::kWrapModeClamp:
		texHelper.samplerCreateInfo.addressModeW = vk::SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		break;
	case bg::base::Texture::kWrapModeRepeat:
		texHelper.samplerCreateInfo.addressModeW = vk::SAMPLER_ADDRESS_MODE_REPEAT;
		break;
	}

	switch (_texture->resizeFilter()) {
	case bg::base::Texture::kFilterLinear:
		texHelper.samplerCreateInfo.minFilter = vk::FILTER_LINEAR;
		texHelper.samplerCreateInfo.magFilter = vk::FILTER_LINEAR;
		break;
	case bg::base::Texture::kFilterNearest:
		texHelper.samplerCreateInfo.minFilter = vk::FILTER_NEAREST;
		texHelper.samplerCreateInfo.magFilter = vk::FILTER_NEAREST;
		break;
	case bg::base::Texture::kFilterMipmap:
		// TODO: Create mipmaps
		texHelper.samplerCreateInfo.minFilter = vk::FILTER_LINEAR;
		texHelper.samplerCreateInfo.magFilter = vk::FILTER_LINEAR;
		break;
	}
	
	bg::ptr<bg::base::Image> image = new bg::base::Image();
	image->setBuffer(const_cast<unsigned char *>(buffer), false);
	image->setWidth(width);
	image->setHeight(height);
	image->setBytesPerPixel(bpp);
	auto format = bg::base::Image::kFormatRGBA;
	if (bpp == 3) {
		format = bg::base::Image::kFormatRGB;
	}
	else if (bpp == 1) {
		format = bg::base::Image::kFormatGrayScale;
	}
	image->setImageFormat(format);

	texHelper.createTexture(ctx->commandPool(), ctx->graphicsQueue(), image.getPtr(), _image, _memory, _imageView, _sampler);
}

void TextureImpl::createCubemap(const std::vector<const unsigned char *> data, int w, int h, int bpp) {
    bg::wnd::VKContext * ctx = vulkan::context(_texture->context());
    TextureHelper texHelper(ctx->physicalDevice(), ctx->device());
    
    switch (_texture->resizeFilter()) {
    case bg::base::Texture::kFilterLinear:
        texHelper.samplerCreateInfo.minFilter = vk::FILTER_LINEAR;
        texHelper.samplerCreateInfo.magFilter = vk::FILTER_LINEAR;
        break;
    case bg::base::Texture::kFilterNearest:
        texHelper.samplerCreateInfo.minFilter = vk::FILTER_NEAREST;
        texHelper.samplerCreateInfo.magFilter = vk::FILTER_NEAREST;
        break;
    case bg::base::Texture::kFilterMipmap:
        // TODO: Create mipmaps
        texHelper.samplerCreateInfo.minFilter = vk::FILTER_LINEAR;
        texHelper.samplerCreateInfo.magFilter = vk::FILTER_LINEAR;
        break;
    }
    
    std::vector<bg::ptr<bg::base::Image>> images;
    for (auto imgData : data) {
        bg::base::Image * img = new bg::base::Image();
        img->setBuffer(const_cast<unsigned char *>(imgData), false);
        img->setWidth(w);
        img->setHeight(h);
        img->setBytesPerPixel(bpp);
        auto format = bg::base::Image::kFormatRGBA;
        if (bpp == 3) {
            format = bg::base::Image::kFormatRGB;
        }
        else if (bpp == 1) {
            format = bg::base::Image::kFormatGrayScale;
        }
        img->setImageFormat(format);
        images.push_back(img);
    }
    
    texHelper.createCubemap(
        ctx->commandPool(), ctx->graphicsQueue(),
        images[0].getPtr(), images[1].getPtr(),
        images[2].getPtr(), images[3].getPtr(),
        images[4].getPtr(), images[5].getPtr(),
        _image, _memory, _imageView, _sampler);
}

void TextureImpl::destroy() {
	_sampler = nullptr;
	_imageView = nullptr;
	_image = nullptr;
	_memory = nullptr;
}

void TextureImpl::read(unsigned char *& buffer, int width, int height, int bpp) {

}

void TextureImpl::invalidate() {

}

}
}
}
