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


#include <bg/engine/opengl_es_3.hpp>

#include <bg/engine/ogl_includes.hpp>

#include <bg/engine/openglEs/pipeline_impl.hpp>
#include <bg/engine/openglEs/poly_list_impl.hpp>
#include <bg/engine/openglEs/texture_impl.hpp>
#include <bg/engine/openglEs/render_surface_impl.hpp>
#include <bg/engine/openglEs/opengl_state.hpp>

#include <bg/ios/gles_context.hpp>
#include <bg/android/gles_context.hpp>

#include <sstream>

namespace bg {
namespace engine {

bool OpenGLES3::Supported() {
	
#if BG2E_OPENGL_ES_AVAILABLE==1
	return true;
#else
	return false;
#endif
}

OpenGLES3::OpenGLES3()
	:_multibuffer(false)
	,_floatTextures(kTextureFloatUnsupported)
	,_tessellation(false)
	,_depthTextures(false)
{
	_name = "OpenGL-ES-3";
	
	// TODO: Implementation objects
}
	
void OpenGLES3::initialize(bg::base::Context * ctx) {
    bg::ptr<bg::ios::GLESContext> iosCtx = dynamic_cast<bg::ios::GLESContext*>(ctx);
	bg::ptr<bg::android::GLESContext> androidCtx = dynamic_cast<bg::android::GLESContext*>(ctx);
	std::vector<std::string> extensions;

    if (iosCtx.isNull() && androidCtx.isNull()) {
        throw bg::base::InvalidParameterException("Unexpected context found. Trying to initialize OpenGL ES rendering engine with a non-gl context");
    }
	else if (iosCtx.valid()) {
		iosCtx->glState()->sync();
		iosCtx->glState()->getExtensions(extensions);
	}
	else if (androidCtx.valid()) {
		androidCtx->glState()->sync();
		androidCtx->glState()->getExtensions(extensions);
	}

    for (auto ex : extensions) {
        if (ex=="GL_OES_depth_texture") {
            _depthTextures = true;
        }
        // TODO: other extensions.
    }
    
    // OpenGL ES 3 supports at least:
    _multibuffer = true;
    _floatTextures = kTextureFloatHalf;
    _depthTextures = true;
	_tessellation = false;
}

bool OpenGLES3::supportsFeature(bg::base::Context *, EngineFeature feature) {
	switch (feature) {
		case kMultibuffer:
			return _multibuffer;
		case kFloatTextures:
			return _floatTextures!=FloatTextureType::kTextureFloatUnsupported;
		case kTessellation:
			return _tessellation;
		case kDepthTextures:
			return _depthTextures;
		case kFloatTexture32:
			return _floatTextures==FloatTextureType::kTextureFloat;
		case kFloatTexture16:
			return _floatTextures==FloatTextureType::kTextureFloatHalf;
	}
	return false;
}

bg::engine::PipelineImpl * OpenGLES3::createPipeline(bg::base::Pipeline * pipeline) {
	return new bg::engine::openglEs::PipelineImpl(pipeline);
}

bg::engine::PolyListImpl * OpenGLES3::createPolyList(bg::base::PolyList * plist) {
	return new bg::engine::openglEs::PolyListImpl(plist);
}

bg::engine::TextureImpl * OpenGLES3::createTextureImpl(bg::base::Texture * tex) {
	return new bg::engine::openglEs::TextureImpl(tex);
}

bg::engine::RenderSurfaceImpl * OpenGLES3::createRenderSurfaceImpl(bg::base::RenderSurface * surface,engine::RenderSurfaceImpl::RenderSurfaceTarget target) {
	return new bg::engine::openglEs::RenderSurfaceImpl(surface,target);
}

}
}
