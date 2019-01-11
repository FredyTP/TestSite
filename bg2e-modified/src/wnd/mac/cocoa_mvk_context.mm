
#include <bg/wnd/cocoa_mvk_context.hpp>

#include <bg/wnd/window_controller.hpp>

#import <Cocoa/Cocoa.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_macos.h>

namespace bg {
namespace wnd {

bool CocoaMVKContext::createContext() {
    return VKContext::createContext();
}

void CocoaMVKContext::makeCurrent() {
    VKContext::makeCurrent();
}

void CocoaMVKContext::swapBuffers() {
    VKContext::swapBuffers();
}

void CocoaMVKContext::destroy() {
    VKContext::destroy();
}

void CocoaMVKContext::setCocoaView(bg::plain_ptr view) {
    _cocoaView = view;
}

void CocoaMVKContext::getRequiredLayers(std::vector<std::string> & layers) {
    layers.push_back("MoltenVK");
}

void CocoaMVKContext::getRequiredExtensions(std::vector<std::string> & extensions) {
    VKContext::getRequiredExtensions(extensions);
    extensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
}

}
}
