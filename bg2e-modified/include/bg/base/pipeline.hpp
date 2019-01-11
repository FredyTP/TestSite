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

#ifndef _bg2e_base_pipeline_hpp_
#define _bg2e_base_pipeline_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/base/matrix_state.hpp>
#include <bg/base/render_surface.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/base/texture.hpp>
#include <bg/base/effect.hpp>
#include <bg/base/texture_effect.hpp>

#include <bg/base/definitions.hpp>

#include <bg/engine/pipeline_impl.hpp>

#include <bg/engine.hpp>

namespace bg {
namespace base {

class BG2E_EXPORT Pipeline : public ContextObject, public ReferencedPointer {
public:
	enum OpacityLayer {
		kOpacityNone = 0,
		kOpacityTransparent = 0x1 << 0,
		kOpacityOpaque		= 0x1 << 1,
		kOpacityAll			= 0x3
	};
	
	
	static void SetCurrent(Pipeline * p);
	
	static Pipeline * Current() { return s_currentPipeline.getPtr(); }
	
	
	Pipeline(Context * ctx);
	
	inline bool isCurrent() { return s_currentPipeline.getPtr()==this; }

	inline Effect * effect() { return _effect.getPtr(); }
	inline const Effect * effect() const { return _effect.getPtr(); }
	void setEffect(Effect * e);

	RenderSurface * renderSurface();
	void setRenderSurface(RenderSurface * r);
	
	inline TextureEffect * textureEffect() { return _textureEffect.getPtr(); }
	inline const TextureEffect * textureEffect() const { return _textureEffect.getPtr(); }
	void setTextureEffect(TextureEffect *);
	
	inline void setOpacityLayer(OpacityLayer layer) { _opacityLayer = layer; }
	inline OpacityLayer opacityLayer() const { return _opacityLayer; }
	inline bool shouldDraw(Material * mat) {
		return mat && ((mat->transparent() && (_opacityLayer & kOpacityTransparent)!=0) ||
					   (!mat->transparent() && (_opacityLayer & kOpacityOpaque)!=0));
	}
	
	void draw(PolyList * polyList);
	void drawTexture(Texture * texture);
	void drawTexture(bg::base::TextureRenderSurface * surface);
	void drawTexture(bg::base::TextureVector & textures);
	
	inline bool blend() { return _blend; }
	void setBlend(bool b);
	inline BlendMode blendMode() { return _blendMode; }
	void setBlendMode(BlendMode b);
	inline bg::math::Viewport & viewport() { return _viewport; }
	inline const bg::math::Viewport & viewport() const { return _viewport; }
	void setViewport(const bg::math::Viewport & vp);
	
	/*
	 *	Clear buffers: option 1, use clearBuffers function to clear the specified buffers
	 */
	void clearBuffers(unsigned int buffers = ClearBuffers::kNone);
	
	/*
	 *	Clear buffers: option 2, configure the buffers to clear and perform the action
	 *				   calling clearBuffers() function passing ClearBuffers::kNone
	 */
	inline void setBuffersToClear(unsigned int buffers) { _clearBuffers = buffers; }
	inline unsigned int buffersToClear() const { return _clearBuffers; }
	
	inline bg::math::Color & clearColor() { return _clearColor; }
	inline const bg::math::Color & clearColor() const { return _clearColor; }
	inline void setClearColor(const bg::math::Color & c) { _clearColor = c; }
	
	inline bool depthTest() const { return _depthTest; }
	void setDepthTest(bool e);
	
	inline bool cullFace() const { return _cullFace; }
	void setCullFace(bool c);
	
protected:
	virtual ~Pipeline();
	
	
	static ptr<Pipeline> s_currentPipeline;
	static bool s_deinitializerSet;
	
	ptr<engine::PipelineImpl> _impl;

	ptr<Effect> _effect;
	ptr<TextureEffect> _textureEffect;
	ptr<RenderSurface> _renderSurface;
	
	bool _blend;
	BlendMode _blendMode;
	bg::math::Viewport _viewport;
	bg::math::Color _clearColor;
	bool _depthTest;
	bool _cullFace;
	unsigned int _clearBuffers;
	OpacityLayer _opacityLayer;
};

}
}

#endif
