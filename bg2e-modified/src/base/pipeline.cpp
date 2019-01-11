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


#include <bg/base/pipeline.hpp>
#include <bg/engine.hpp>

#include <bg/base/render_surface.hpp>

#include <bg/fx/texture.hpp>

#include <bg/engine.hpp>

namespace bg {
namespace base {

ptr<Pipeline> Pipeline::s_currentPipeline;
bool Pipeline::s_deinitializerSet = false;

void Pipeline::SetCurrent(Pipeline * p) {
	if (p==s_currentPipeline) return;
	s_currentPipeline = p;
	if (s_currentPipeline.valid()) {
		s_currentPipeline->renderSurface()->setActive();
		s_currentPipeline->_impl->activate();
	}
	if (!s_deinitializerSet) {
		s_deinitializerSet = true;
		bg::Engine::AddDestroyCallback([&]() {
			SetCurrent(nullptr);
		});
	}
}

Pipeline::Pipeline(Context * ctx)
	:ContextObject(ctx)
	,_blend(false)
	,_blendMode(bg::base::BlendMode::kNormal)
	,_viewport(0,0,200,200)
	,_clearColor(0.0f,0.0f,0.0f,1.0f)
	,_depthTest(true)
	,_cullFace(true)
	,_clearBuffers(ClearBuffers::kColorDepth)
	,_opacityLayer(kOpacityAll)
{
	_impl = Engine::Get()->createPipeline(this);
	_textureEffect = new bg::fx::Texture(ctx);
    if (!s_currentPipeline.valid()) {
        SetCurrent(this);
    }
}

Pipeline::~Pipeline() {

}

void Pipeline::setEffect(Effect * e) {
	_effect = e;
}

void Pipeline::setTextureEffect(TextureEffect * e) {
	_textureEffect = e;
}


RenderSurface * Pipeline::renderSurface() {
	if (!_renderSurface.valid()) {
		setRenderSurface(new bg::base::ScreenRenderSurface(context()));
	}
	return _renderSurface.getPtr();
}

void Pipeline::setRenderSurface(RenderSurface * r) {
	_renderSurface = r;
	if (isCurrent()) {
		_renderSurface->setActive();
	}
}

void Pipeline::draw(PolyList * polyList) {
	if (effect() && polyList && isCurrent()) {
		effect()->activate();
		effect()->bindPolyList(polyList);
		setCullFace(effect()->material().cullFace());
		polyList->draw();
		effect()->unbind();
	}
}

void Pipeline::drawTexture(Texture * texture) {
	if (textureEffect() && texture && isCurrent()) {
		bool dt = depthTest();
		setDepthTest(false);
		textureEffect()->drawTexture(texture);
		setDepthTest(dt);
	}
}

void Pipeline::drawTexture(bg::base::TextureRenderSurface * surface) {
	if (textureEffect() && surface && isCurrent()) {
		bool dt = depthTest();
		setDepthTest(false);
		textureEffect()->drawTexture(surface);
		setDepthTest(dt);
	}
}

void Pipeline::drawTexture(bg::base::TextureVector & textures) {
	if (textureEffect() && textures.size()>0 && isCurrent()) {
		bool dt = depthTest();
		setDepthTest(false);
		textureEffect()->drawTexture(textures);
		setDepthTest(dt);
	}
}

void Pipeline::setBlend(bool b) {
	_blend = b;
	if (isCurrent()) {
		_impl->setBlendEnabled();
	}
}

void Pipeline::setBlendMode(BlendMode b) {
	_blendMode = b;
	if (isCurrent()) {
		_impl->setBlendMode();
	}
}

void Pipeline::setViewport(const bg::math::Viewport & vp) {
	_viewport = vp;
	if (renderSurface()->resizeOnViewportChanged()) {
		renderSurface()->setSize(bg::math::Size2Di(vp.width(),vp.height()));
	}
    if (isCurrent()) {
        _impl->setViewport();
    }
}

void Pipeline::clearBuffers(unsigned int buffers) {
	if (isCurrent()) {
		if(buffers == ClearBuffers::kNone && _clearBuffers != ClearBuffers::kNone) {
			_impl->clearBuffers(_clearBuffers);
		}
		else if(buffers != ClearBuffers::kNone) {
			_impl->clearBuffers(buffers);
		}
	}
}

void Pipeline::setDepthTest(bool e) {
	_depthTest = e;
	if (isCurrent()) {
		_impl->setDepthTestEnabled();
	}
}

void Pipeline::setCullFace(bool c) {
	_cullFace = c;
	if (isCurrent()) {
		_impl->setCullFace();
	}
}

}
}
