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


#include <bg/render/shader_inputs.hpp>
#include <bg/engine/vulkan.hpp>
#include <bg/render/vulkan_shader_inputs.hpp>
#include <bg/engine.hpp>

namespace bg {
namespace render {

ShaderInputs * ShaderInputs::Create(bg::base::Context * ctx) {
    if (bg::Engine::IsVulkan()) {
        return new VulkanShaderInputs(ctx);
    }
    else {
        throw bg::base::CompatibilityException("bg::render::ShaderInputs() is not compatible with the current rendering engine.");
    }
}

ShaderInputs::ShaderInputs(bg::base::Context * ctx)
    :bg::base::ContextObject(ctx)
{

}

ShaderInputs::~ShaderInputs() {

}

void ShaderInputs::create() {
    createLayout();
}

}
}
