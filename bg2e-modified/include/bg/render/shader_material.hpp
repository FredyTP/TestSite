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


#ifndef _bg2e_render_shader_material_hpp_
#define _bg2e_render_shader_material_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/base/material.hpp>


namespace bg {
namespace scene {

class BG2E_EXPORT Camera;

}

namespace render {

class LowOverheadPipeline;
class ShaderMaterial : public bg::base::ReferencedPointer {
public:
    ShaderMaterial() {
        
    }
    
    virtual void loadShader() = 0;
    virtual void beginFrame(float delta) = 0;
    virtual void mapInputBuffers(bg::base::PolyList * pl, bg::base::Material * mat) = 0;
    virtual void updateDescriptorSet(bg::base::PolyList * pl, bg::base::Material * mat, uint32_t currentFrame) = 0;
    
    inline void setCamera(bg::scene::Camera * c) { _camera = c; }
    inline const bg::scene::Camera * camera() const { return _camera.getPtr(); }
    inline bg::scene::Camera * camera() { return _camera.getPtr(); }
    
    inline void setPipeline(LowOverheadPipeline * pl) { _pipeline = pl; }
    inline LowOverheadPipeline * pipeline() { return _pipeline; }
    
protected:
    virtual ~ShaderMaterial() {
        
    }
    
    LowOverheadPipeline * _pipeline = nullptr;
    bg::ptr<bg::scene::Camera> _camera;
};

}
}


#endif

