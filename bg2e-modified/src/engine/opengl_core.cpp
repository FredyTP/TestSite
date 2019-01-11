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


#include <bg/engine/opengl_core.hpp>

#include <bg/engine/ogl_includes.hpp>

#include <bg/base/exception.hpp>

#include <bg/engine/openglCore/pipeline_impl.hpp>
#include <bg/engine/openglCore/poly_list_impl.hpp>
#include <bg/engine/openglCore/texture_impl.hpp>
#include <bg/engine/openglCore/render_surface_impl.hpp>

#include <bg/engine/openglCore/opengl_state.hpp>

#include <bg/tools/texture_cache.hpp>

namespace bg {
namespace engine {
	
bool OpenGLCore::Supported() {
	
#if BG2E_OPENGL_AVAILABLE==1
	return true;
#else
	return false;
#endif
}

OpenGLCore::OpenGLCore()
	:_version(0)
{
	_name = "OpenGL-core-profile";
}
	
void OpenGLCore::initialize(bg::base::Context * context) {
	_version = bg::engine::opengl::initialize();
	
	if (_version<330) {
		throw bg::base::CompatibilityException("Trying to instantiate a non-compatible rendering engine: OpenGL core profile.");
	}

	bg::engine::openglCore::GLStateObject * stateObject = dynamic_cast<bg::engine::openglCore::GLStateObject*>(context);
	if (!stateObject) {
		throw bg::base::InvalidParameterException("Unexpected context found. Trying to initialize OpenGLCore rendering engine with a non-gl context");
	}

	stateObject->glState()->sync();
}
	
bool OpenGLCore::supportsFeature(bg::base::Context *, bg::EngineFeature feature)  {
	switch (feature) {
		case kMultibuffer:
		case kFloatTextures:
			return true;
		case kTessellation:
			return _version>=400;
		case kDepthTextures:
			return true;
		case kFloatTexture16:
			return false;
		case kFloatTexture32:
			return true;
	}
	return false;
}


bg::engine::PipelineImpl * OpenGLCore::createPipeline(bg::base::Pipeline * pipeline) {
	return new bg::engine::openglCore::PipelineImpl(pipeline);
}

bg::engine::PolyListImpl * OpenGLCore::createPolyList(bg::base::PolyList * plist) {
	return new bg::engine::openglCore::PolyListImpl(plist);
}

bg::engine::TextureImpl * OpenGLCore::createTextureImpl(bg::base::Texture * tex) {
	return new bg::engine::openglCore::TextureImpl(tex);
}

bg::engine::RenderSurfaceImpl * OpenGLCore::createRenderSurfaceImpl(bg::base::RenderSurface * surface,engine::RenderSurfaceImpl::RenderSurfaceTarget target) {
	return new bg::engine::openglCore::RenderSurfaceImpl(surface,target);
}

}
}

