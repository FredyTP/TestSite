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

#ifndef _bg2e_engine_vulkan_texture_impl_hpp_
#define _bg2e_engine_vulkan_texture_impl_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/engine/texture_impl.hpp>

#include <bg/engine/vulkan/vulkan_includes.hpp>

namespace bg {
namespace engine {
namespace vulkan {

class BG2E_EXPORT TextureImpl : public bg::engine::TextureImpl {
public:
	TextureImpl(bg::base::Texture *);

	virtual void create(const unsigned char *, int width, int height, int bpp);
	virtual void createCubemap(const std::vector<const unsigned char *> data, int w, int h, int bpp);
	virtual void destroy();
	virtual void read(unsigned char *& buffer, int width, int height, int bpp);
	virtual void invalidate();

	inline bg::engine::vulkan::vk::Image * image() { return _image.getPtr(); }
	inline bg::engine::vulkan::vk::DeviceMemory * memory() { return _memory.getPtr(); }
	inline bg::engine::vulkan::vk::ImageView * imageView() { return _imageView.getPtr(); }
	inline bg::engine::vulkan::vk::Sampler * sampler() { return _sampler.getPtr(); }

protected:
	virtual ~TextureImpl();

	bg::ptr<bg::engine::vulkan::vk::Image> _image;
	bg::ptr<bg::engine::vulkan::vk::DeviceMemory> _memory;
	bg::ptr<bg::engine::vulkan::vk::ImageView> _imageView;
	bg::ptr<bg::engine::vulkan::vk::Sampler> _sampler;
};

}
}
}

#endif
