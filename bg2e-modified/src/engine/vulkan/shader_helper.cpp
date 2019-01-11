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

#include <memory>
#include <iostream>
#include <fstream>

#include <bg/log.hpp>

namespace bg {
namespace engine {
namespace vulkan {	

ShaderHelper::ShaderHelper(vk::Device * dev)
    :_dev(dev)
{

}

void ShaderHelper::loadFromFile(const std::string & path, vk::ShaderModule::Buffer & result) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    
    if (!file.is_open()) {
        throw bg::base::NoSuchFileException("No such shader file at path " +  path);
    }
    
    size_t fileSize = static_cast<size_t>(file.tellg());
    result.resize(fileSize);
    file.seekg(0);
    file.read(result.data(), fileSize);
    file.close();
}

void ShaderHelper::loadFromFile(const bg::system::Path & path, vk::ShaderModule::Buffer & result) {
    loadFromFile(path.text(), result);
}

vk::ShaderModule * ShaderHelper::loadFromFile(const std::string & path) {
    bg::ptr<vk::ShaderModule> shaderModule = new vk::ShaderModule();
    vk::ShaderModule::Buffer buffer;
    loadFromFile(path, buffer);
    shaderModule->create(_dev, buffer);
    return shaderModule.release();
}

vk::ShaderModule * ShaderHelper::loadFromFile(const bg::system::Path & path) {
    return loadFromFile(path.text());
}

}
}
}
