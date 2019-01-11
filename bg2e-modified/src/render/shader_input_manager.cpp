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


#include <bg/render/shader_input_manager.hpp>

#include <bg/engine/vulkan.hpp>
#include <bg/render/vulkan_shader_inputs.hpp>
#include <bg/engine.hpp>

namespace bg {
namespace render {

const std::string ShaderInputManager::kShaderInputManagerId = "bg::render::ShaderInputManager";

ShaderInputManager::ShaderInputManager(bg::base::Context * ctx)
    :ContextObject(ctx)
{

}
   
ShaderInputManager::~ShaderInputManager() {
    for (auto & pl : _plists) {
        if (pl.second->plist) pl.second->plist->onDelete(kShaderInputManagerId, nullptr);
    }
    _plists.clear();
}

ShaderInputManager::ShaderInputPlist * ShaderInputManager::getShaderInput(bg::base::PolyList * plist) {
    InputsIndex::iterator it = _plists.find(plist);
    if (it == _plists.end()) {
        ShaderInputPlist * result = new ShaderInputPlist({
            plist,
            bg::render::ShaderInputs::Create(context()),
            false
        });
        _plists[plist] = result;
        if (plist) {
            plist->onDelete(kShaderInputManagerId,[&](bg::base::ReferencedPointer * item) {
                ShaderInputPlist * toDelete = _plists[item];
                _plists.erase(item);
                delete toDelete;
            });
        }
        if (_initClosure) {
            _initClosure(result);
            result->initialized = true;
        }
        return result;
    }
    return it->second;
}
    
    
}
}
