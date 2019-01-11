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


#ifndef _bg2e_render_low_overhead_pipeline_hpp_
#define _bg2e_render_low_overhead_pipeline_hpp_

#include <bg/base/context_object.hpp>
#include <bg/base/referenced_pointer.hpp>
#include <bg/base/context.hpp>
#include <bg/base/material.hpp>

#include <vector>
#include <bg/system/path.hpp>

#include <bg/wnd/window_controller.hpp>

#include <bg/render/shader_inputs.hpp>
#include <bg/render/shader_input_manager.hpp>

#include <bg/render/shader_material.hpp>

namespace bg {
namespace render {

class BG2E_EXPORT LowOverheadPipeline : public bg::base::ReferencedPointer,
                                        public bg::base::ContextObject
{
public:
    enum OpacityLayer {
        kOpacityNone        = 0,
        kOpacityTransparent = 0x1 << 0,
        kOpacityOpaque      = 0x1 << 1,
        kOpacityAll         = 0x3
    };
    typedef uint32_t OpacityMask;
    
    enum BlendFactor {
        kBlendFactorZero = 0,
        kBlendFactorOne,
        kBlendFactorSrcColor,
        kBlendFactorOneMinusSrcColor,
        kBlendFactorDstColor,
        kBlendFactorOneMinusDstColor,
        kBlendFactorSrcAlpha,
        kBlendFactorOneMinusSrcAlpha,
        kBlendFactorDstAlpha,
        kBlendFactorOneMinusDstAlpha,
        kBlendFactorConstantColor,
        kBlendFactorOneMinusConstantColor,
        kBlendFactorConstantAlpha,
        kBlendFactorOneMinusConstantAlpha,
        kBlendFactorSrcAlphaSaturate
    };
    
    enum BlendOperation {
        kBlendOpAdd = 1,
        kBlendOpSubtract,
        kBlendOpReverseSubtract,
        kBlendOpMin,
        kBlendOpMax
    };
    
    struct BlendOptions {
        bool blendEnabled = false;
        BlendFactor srcColorBlendFactor = kBlendFactorSrcAlpha;
        BlendFactor dstColorBlendFactor = kBlendFactorOneMinusSrcAlpha;
        BlendOperation colorBlendOp = kBlendOpAdd;
        BlendFactor srcAlphaBlendFactor = kBlendFactorSrcAlpha;
        BlendFactor dstAlphaBlendFactor = kBlendFactorOneMinusSrcAlpha;
        BlendOperation alphaBlendOp = kBlendOpAdd;
        
        BlendOptions(bool e,
            BlendFactor sc, BlendFactor dc, BlendOperation cop,
            BlendFactor sa, BlendFactor da, BlendOperation aop)
            :blendEnabled(e), srcColorBlendFactor(sc), dstColorBlendFactor(dc), colorBlendOp(cop)
            ,srcAlphaBlendFactor(sa), dstAlphaBlendFactor(da), alphaBlendOp(aop)
        {}
        
        inline void operator = (const BlendOptions & other) {
            blendEnabled = other.blendEnabled;
            srcColorBlendFactor = other.srcColorBlendFactor;
            dstColorBlendFactor = other.dstColorBlendFactor;
            colorBlendOp = other.colorBlendOp;
            srcAlphaBlendFactor = other.srcAlphaBlendFactor;
            dstAlphaBlendFactor = other.dstAlphaBlendFactor;
            alphaBlendOp = other.alphaBlendOp;
        }
        
        static BlendOptions NoBlend() {
            return BlendOptions{
                false,
                kBlendFactorSrcAlpha, kBlendFactorOneMinusSrcAlpha, kBlendOpAdd,
                kBlendFactorSrcAlpha, kBlendFactorOneMinusSrcAlpha, kBlendOpAdd
            };
        }
        
        static BlendOptions AlphaBlend() {
            return BlendOptions{
                true,
                kBlendFactorSrcAlpha, kBlendFactorOneMinusSrcAlpha, kBlendOpAdd,
                kBlendFactorSrcAlpha, kBlendFactorOneMinusSrcAlpha, kBlendOpAdd
            };
        }
    };
    
    static LowOverheadPipeline * Create(bg::base::Context *, const BlendOptions & opt = BlendOptions::NoBlend());
    
    enum PrimitiveTopology {
        kTopologyTriangleList = 1,
        kTopologyPointList,
        kTopologyLineList
    };
    
    enum CullFaceMode {
        kCullModeFront = 1,
        kCullModeBack,
        kCullModeOff
    };
    
    enum ShaderStage {
        kShaderStageVertex = 1,
        // TODO: Add all the shader stages
        kShaderStageFragment
    };
    
    LowOverheadPipeline(bg::base::Context * ctx, const BlendOptions &);
    
    // Opacity layers functions:
    inline void setOpacityLayer(OpacityMask layer) { _opacityLayer = layer; }
    inline OpacityMask opacityLayer() const { return _opacityLayer; }
    inline bool shouldDraw(bg::base::Material * mat) {
        return mat && ((mat->transparent() && (_opacityLayer & kOpacityTransparent)!=0) ||
                       (!mat->transparent() && (_opacityLayer & kOpacityOpaque)!=0));
    }
    
    inline void clearShaders() { _shaders.clear(); }
    inline void addShader(const bg::system::Path & path, const std::string & entryPoint, ShaderStage shaderType) {
        _shaders.push_back({ path, entryPoint, shaderType });
    }

    virtual void resize(int, int) = 0;
    virtual void check() = 0;
    virtual void mapInputBuffer(uint32_t bufferIndex, bg::base::PolyList * pl, void * memData, size_t size) = 0;
    virtual void drawPolyList(bg::base::PolyList * plist, uint32_t currentFrame) = 0;
    
    void mapInputBuffer(bg::base::PolyList * pl, bg::base::Material * mat);
    void updateShaderInputs(bg::base::PolyList * pl, bg::base::Material * mat, uint32_t currentFrame);
    

    inline void setPrimitiveTopology(PrimitiveTopology topology) { _primitiveTopology = topology; }
    inline PrimitiveTopology primitiveTopology() const { return _primitiveTopology; }
    inline void setCullMode(CullFaceMode mode) { _cullMode = mode; }
    inline CullFaceMode cullMode() const { return _cullMode; }
    
    inline void setShaderMaterial(ShaderMaterial * sm) {
        _shaderMaterial = sm;
        _shaderMaterial->setPipeline(this);
        _shaderMaterial->loadShader();
    }
    inline ShaderMaterial * shaderMaterial() { return _shaderMaterial.getPtr(); }
    inline ShaderInputManager * shaderInputManager() { return _shaderInputManager.getPtr(); }
    
    inline bg::base::MatrixState * matrixState() { return _matrixState.getPtr(); }
    
    virtual void destroy() {};
    
    template <class T>
    T * cast() { return dynamic_cast<T*>(this); }
    
protected:
    virtual ~LowOverheadPipeline() {
        destroy();
    }
    
    PrimitiveTopology _primitiveTopology;
    CullFaceMode _cullMode;
    
    struct ShaderModuleData {
        bg::system::Path shaderPath;
        std::string entryPoint;
        ShaderStage shaderType;
    };
    std::vector<ShaderModuleData> _shaders;
    
    bg::ptr<ShaderInputManager> _shaderInputManager;
    
    bg::ptr<ShaderMaterial> _shaderMaterial;
    
    bg::ptr<bg::base::MatrixState> _matrixState;
    
    OpacityMask _opacityLayer;
    
    BlendOptions _blendOptions;
};

}
}

#endif
