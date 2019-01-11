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


#ifndef _bg2e_render_shader_input_hpp_
#define _bg2e_render_shader_input_hpp_

#include <bg/base/context_object.hpp>
#include <bg/base/referenced_pointer.hpp>
#include <bg/base/context.hpp>
#include <bg/base/material.hpp>

#include <vector>
#include <bg/system/path.hpp>

#include <bg/wnd/window_controller.hpp>

#include <bg/math/math.hpp>

namespace bg {
namespace render {


class BG2E_EXPORT ShaderInputs : public bg::base::ContextObject,
                                public bg::base::ReferencedPointer
{
public:
    static ShaderInputs * Create(bg::base::Context *);
    
    ShaderInputs(bg::base::Context *);
    
    void create();
    virtual void createLayout() = 0;
    
    enum ShaderInputDestination {
        kInputVertexShader = 0x1 << 0,
        kInputFragmentShader = 0x1 << 1
        // TODO: add more inputs
    };
    
    virtual void addUniformBuffer(uint32_t binding, size_t bufferSize, uint32_t shaderInputDestination) = 0;
    virtual void addSampler(uint32_t binding, uint32_t shaderInputDestination) = 0;
    virtual void mapBufferMemory(uint32_t index, void ** data, size_t size) = 0;
    virtual void unmapBufferMemory(uint32_t index) = 0;
    
protected:
    virtual ~ShaderInputs();
};

}
}

#endif
