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


#ifndef _bg2e_render_render_layer_hpp_
#define _bg2e_render_render_layer_hpp_

#include <bg/base/pipeline.hpp>
#include <bg/scene/node.hpp>
#include <bg/scene/camera.hpp>
#include <bg/scene/draw_visitor.hpp>
#include <bg/render/settings.hpp>

namespace bg {
namespace render {

namespace settings {

extern BG2E_EXPORT const SettingsKey kDebug;

extern BG2E_EXPORT const SettingsKey kEnabled;
extern BG2E_EXPORT const SettingsKey kBlur;
extern BG2E_EXPORT const SettingsKey kSampleRadius;
extern BG2E_EXPORT const SettingsKey kMaxDistance;
extern BG2E_EXPORT const SettingsKey kColor;
extern BG2E_EXPORT const SettingsKey kQuality;
extern BG2E_EXPORT const SettingsKey kScale;

}

class DebugRenderSettings : public bg::base::ReferencedPointer,
							public RenderSettings {
public:
	DebugRenderSettings() :RenderSettings(settings::kDebug) {}

	inline bool enabled() const { return _enabled; }
	inline void setEnabled(bool e) { _enabled = e; }

	virtual void setSettingValue(const SettingsKey & key, bool value) {
		if (key==settings::kEnabled) {
			_enabled = value;
		}
	}

	virtual bool boolSettingValue(const SettingsKey & key) const {
		if (key==settings::kEnabled) {
			return _enabled;
		}
		else {
			return RenderSettings::boolSettingValue(key);
		}
	}
	
protected:
	virtual ~DebugRenderSettings() {}

	bool _enabled = false;
};

class BG2E_EXPORT RenderLayer : public bg::base::ContextObject, public bg::base::ReferencedPointer {
public:
	RenderLayer(bg::base::Context *, bg::base::Pipeline * pl = nullptr);

	inline bg::base::Pipeline * pipeline() { return _pipeline.getPtr(); }
	inline const bg::base::Pipeline * pipeline() const { return _pipeline.getPtr(); }
	
	inline bg::scene::DrawVisitor * drawVisitor() { return _drawVisitor.getPtr(); }
	inline const bg::scene::DrawVisitor * drawVisitor() const { return _drawVisitor.getPtr(); }
	
	inline bg::base::Pipeline::OpacityLayer opacityLayer() const { return _pipeline->opacityLayer(); }
	
	virtual void draw(bg::scene::Node *, bg::scene::Camera *);
	
	bg::base::Texture * texture() {
		return dynamic_cast<bg::base::TextureRenderSurface*>(_pipeline->renderSurface()) ?
			dynamic_cast<bg::base::TextureRenderSurface*>(_pipeline->renderSurface())->texture() : nullptr;
	}

	virtual void invalidateBuffers() = 0;
	virtual void invalidateTextures() = 0;
	
protected:
	virtual ~RenderLayer();

	ptr<bg::base::Pipeline> _pipeline;
	ptr<bg::scene::DrawVisitor> _drawVisitor;
	
	/*
	 *	Optional: use willDraw() to prepare resources before draw, such as shadow maps
	 *			  deferred render g-buffers
	 */
	virtual void willDraw(bg::scene::Node *, bg::scene::Camera *) {};
	
	/*
	 *	Required: use performDraw() to render the scene into this->_pipeline. The
	 *			  pipeline will be activated automatically before call this function.
	 *			  It's important not to change the current pipeline inside this
	 *			  function.
	 *			  Projection and view will be configured automatically using the pure
	 *			  virtual function effect() to get the matrixState.
	 *
	 */
	virtual void performDraw(bg::scene::Node *, bg::scene::Camera *) = 0;
};

}
}

#endif
