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


#ifndef _bg2e_render_shader_input_manager_hpp_
#define _bg2e_render_shader_input_manager_hpp_

#include <bg/base/context_object.hpp>
#include <bg/base/referenced_pointer.hpp>
#include <bg/base/context.hpp>
#include <bg/base/material.hpp>
#include <bg/render/shader_inputs.hpp>

#include <vector>
#include <bg/system/path.hpp>

#include <bg/wnd/window_controller.hpp>

#include <bg/math/math.hpp>

namespace bg {
namespace render {

class BG2E_EXPORT ShaderInputManager : public bg::base::ContextObject,
                                       public bg::base::ReferencedPointer {
public:
    struct ShaderInputPlist {
        bg::base::PolyList * plist;
        bg::ptr<bg::render::ShaderInputs> shaderInputs;
        bool initialized;
    };
    typedef std::unordered_map<void*, ShaderInputPlist*> InputsIndex;
    
    typedef std::function<void(ShaderInputPlist *)> InitShaderInputClosure;
    
    ShaderInputManager(bg::base::Context * ctx);
    
    ShaderInputPlist * getShaderInput(bg::base::PolyList * plist = nullptr);
    
    inline void onInitShaderInput(InitShaderInputClosure c) { _initClosure = c; }
    
protected:
    virtual ~ShaderInputManager();
    
    InputsIndex _plists;
    InitShaderInputClosure _initClosure;
    
    static const std::string kShaderInputManagerId;
};

}
}

#endif