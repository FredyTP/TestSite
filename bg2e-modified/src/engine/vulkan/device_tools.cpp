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

namespace bg {
namespace engine {
namespace vulkan {

vk::PhysicalDevice * chooseBestDevice(vk::Instance *, vk::PhysicalDevice::Vector & available, const std::vector<std::string> & reqExtensions, vk::SurfaceKHR * surface) {
	bg::ptr<vk::PhysicalDevice> choosed;

	for (auto & dev : available) {
		if (!vk::ExtensionProperties::CheckSupport(dev->extensions, reqExtensions)) {
			continue;
		}

		QueueFamilyIndices indices = QueueFamilyIndices::Find(dev.getPtr(), surface);
		if (!indices.isComplete()) {
			continue;
		}

        std::vector<vk::SurfaceFormatKHR> formats;
        surface->getFormats(dev.getPtr(), formats);
        std::vector<vk::PresentModeKHR> modes;
        surface->getPresentModes(dev.getPtr(), modes);
        if (formats.size()==0 || modes.size()==0) {
            continue;
        }

		if (!choosed) {
			choosed = dev;
		}
		else if (dev->deviceType == vk::PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
			choosed->deviceType != vk::PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			choosed = dev;
		}
		else if (dev->deviceType == vk::PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU &&
			choosed->deviceType != vk::PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
			choosed->deviceType != vk::PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
		{
			choosed = dev;
		}
		else if (dev->deviceType == choosed->deviceType && dev->vramSize > choosed->vramSize) {
			choosed = dev;
		}
	}

	return choosed.release();
}

}
}
}
