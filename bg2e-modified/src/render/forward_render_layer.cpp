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


#include <bg/render/forward_render_layer.hpp>

#include <bg/scene/light.hpp>

namespace bg {
namespace render {

ForwardRenderLayer::ForwardRenderLayer(bg::base::Context * ctx, bg::base::Pipeline * pl)
	:bg::render::RenderLayer(ctx, pl)
{
}

ForwardRenderLayer::~ForwardRenderLayer() {
	
}

void ForwardRenderLayer::performDraw(bg::scene::Node * sceneRoot, bg::scene::Camera * camera) {
	using namespace bg::base;

	if(opacityLayer() == bg::base::Pipeline::kOpacityTransparent) {
		pipeline()->setBuffersToClear(bg::base::kNone);
		pipeline()->setBlend(true);
		pipeline()->setBlendMode(bg::base::kNormal);
	}
	else {
		pipeline()->setBuffersToClear(bg::base::kColorDepth);
		pipeline()->setBlend(false);
	}

	pipeline()->setViewport(camera->viewport());
	pipeline()->clearBuffers();
	
	sceneRoot->accept(drawVisitor());
}

void ForwardRenderLayer::invalidateBuffers() {
	// ForwardRenderLayer doesn't use framebuffers
}

void ForwardRenderLayer::invalidateTextures() {
}

}
}
