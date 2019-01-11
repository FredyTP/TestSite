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



#include <bg/engine/openglCore/pipeline_impl.hpp>

#include <bg/base/definitions.hpp>

#include <bg/engine/ogl_includes.hpp>

#include <bg/base/pipeline.hpp>

namespace bg {
namespace engine {
namespace openglCore {

PipelineImpl::PipelineImpl(bg::base::Pipeline * pipeline)
	:bg::engine::PipelineImpl(pipeline)
{
	
}

PipelineImpl::~PipelineImpl() {
	
}

void PipelineImpl::activate() {
	setViewport();
	setDepthTestEnabled();
	setBlendEnabled();
	setBlendMode();
	setCullFace();
}

void PipelineImpl::setViewport() {
	gl(ctx())->viewport(_pipeline->viewport());
}

void PipelineImpl::clearBuffers(unsigned int buffers) {
	using namespace bg::engine::opengl;
	gl(ctx())->clearColor(_pipeline->clearColor());
	gl(ctx())->clear((buffers & bg::base::ClearBuffers::kColor ? kColorBufferBit : 0) |
					 (buffers & bg::base::ClearBuffers::kDepth ? kDepthBufferBit : 0));
}

void PipelineImpl::setDepthTestEnabled() {
	gl(ctx())->setDepthTestEnabled(_pipeline->depthTest());
}

void PipelineImpl::setBlendEnabled() {
	gl(ctx())->setBlendEnabled(_pipeline->blend());
}

void PipelineImpl::setBlendMode() {
	using namespace bg::engine::opengl;
	switch (_pipeline->blendMode()) {
	case bg::base::BlendMode::kNormal:
		gl(ctx())->blendFunction(kSrcAlpha, kOneMinusSrcAlpha);
		gl(ctx())->blendEquation(kFuncAdd);
		break;
	case bg::base::BlendMode::kMultiply:
		gl(ctx())->blendFunction(kZero, kSrcColor);
		gl(ctx())->blendEquation(kFuncAdd);
		break;
	case bg::base::BlendMode::kAdd:
		gl(ctx())->blendFunction(kOne, kOne);
		gl(ctx())->blendEquation(kFuncAdd);
		break;
	case bg::base::BlendMode::kSubtract:
		gl(ctx())->blendFunction(kOne, kOne);
		gl(ctx())->blendEquation(kFuncSubtract);
		break;
	case bg::base::BlendMode::kAlphaAdd:
		gl(ctx())->blendFunction(kSrcAlpha, kSrcAlpha);
		gl(ctx())->blendEquation(kFuncAdd);
		break;
	case bg::base::BlendMode::kAlphaSubtract:
		gl(ctx())->blendFunction(kSrcAlpha, kSrcAlpha);
		gl(ctx())->blendEquation(kFuncSubtract);
		break;
	}
}

void PipelineImpl::setCullFace() {
	gl(ctx())->setCullFaceEnabled(_pipeline->cullFace());
}

void PipelineImpl::destroy() {

}

}
}
}
