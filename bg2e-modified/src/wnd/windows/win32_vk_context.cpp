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


#include <bg/wnd/win32_vk_context.hpp>

#include <bg/wnd/window_controller.hpp>

#include <bg/log.hpp>

namespace bg {
namespace wnd {

bool Win32VKContext::createContext() {
    return VKContext::createContext();
}

void Win32VKContext::makeCurrent() {
	VKContext::makeCurrent();
}

void Win32VKContext::swapBuffers() {
	VKContext::swapBuffers();
}

void Win32VKContext::destroy() {
	VKContext::destroy();
}

void Win32VKContext::getRequiredExtensions(std::vector<std::string> & extensions) {
	VKContext::getRequiredExtensions(extensions);
	extensions.push_back(bg::engine::vulkan::vk::KHR_WIN32_SURFACE_EXTENSION_NAME);
}

}
}