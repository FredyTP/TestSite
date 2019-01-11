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


#ifndef _bg2e_render_shadow_map_hpp_
#define _bg2e_render_shadow_map_hpp_

#include <bg/base/context_object.hpp>
#include <bg/base/pipeline.hpp>
#include <bg/fx/shadow_map.hpp>
#include <bg/base/render_surface.hpp>
#include <bg/scene/draw_visitor.hpp>
#include <bg/scene/node.hpp>
#include <bg/scene/camera.hpp>
#include <bg/base/light.hpp>
#include <bg/render/settings.hpp>

namespace bg {
namespace render {

namespace settings {

extern BG2E_EXPORT const SettingsKey kShadowMap;

extern BG2E_EXPORT const SettingsKey kShadowType;
extern BG2E_EXPORT const SettingsKey kShadowColor;
extern BG2E_EXPORT const SettingsKey kShadowMapSize;

}

class BG2E_EXPORT ShadowMap : public bg::base::ContextObject,
							  public bg::base::ReferencedPointer,
							  public RenderSettings {
public:
	enum ShadowType {
		kHardShadows		= 0,
		kSoftShadows		= 1,
		kStratifiedShadows	= 2
	};

	enum ShadowCascade {
		kCascadeNear = 0,
		kCascadeMid,
		kCascadeFar
	};

	struct ShadowCascadeSettings {
		float near;
		float far;
		float frameSize;
		float lightDistance;
		float shadowBias;
		float cameraNear;
		float cameraFar;
		bool blend;
	};

	static std::map<ShadowCascade,ShadowCascadeSettings> CascadeSetup;

	ShadowMap(bg::base::Context *);

	inline void setShadowMapSize(const bg::math::Size2Di & s) {
		_shadowMapSize = s;
		_pipeline->setViewport(bg::math::Viewport(0,0,s.width(),s.height()));
	}
	inline const bg::math::Size2Di & shadowMapSize() const { return _shadowMapSize; }
	inline bg::math::Size2Di & shadowMapSize() { return _shadowMapSize; }

	inline void setShadowType(ShadowType t) { _type = t; }
	inline ShadowType shadowType() const { return _type; }
	
	inline void setShadowColor(const bg::math::Color & c) { _shadowColor = c; }
	inline const bg::math::Color & shadowColor() const { return _shadowColor; }
	inline bg::math::Color & shadowColor() { return _shadowColor; }

	inline bg::base::Texture * texture() { return _renderSurface->texture(0); }
	
	inline bg::math::Matrix4 & tunedViewMatrix() { return _viewMatrix; }

	void update(bg::scene::Node *, bg::scene::Camera *, bg::base::Light  *, const bg::math::Matrix4 &);
	void updateCascade(ShadowCascade c, bg::scene::Node *, bg::scene::Camera *, bg::base::Light  *, const bg::math::Matrix4 &);

	void invalidateBuffers();
	void invalidateTextures();

	// Settings matching the getter functions: shadowMapSize (Size2Di), shadowColor (Vector4), shadowType (int)
	virtual void setSettingValue(const SettingsKey & key, int value);
	virtual void setSettingValue(const SettingsKey & key, const bg::math::Vector4 & value);
	virtual void setSettingValue(const SettingsKey & key, const bg::math::Size2Di & value);

	virtual int intSettingValue(const SettingsKey & key) const;
	virtual const bg::math::Vector4 & vec4SettingValue(const SettingsKey & key) const;
	virtual const bg::math::Size2Di sizeSettingValue(const SettingsKey & key) const;


protected:
	virtual ~ShadowMap();

	bg::ptr<bg::base::Pipeline> _pipeline;
	bg::ptr<bg::fx::ShadowMap> _effect;
	bg::ptr<bg::base::TextureRenderSurface> _renderSurface;
	bg::math::Size2Di _shadowMapSize;
	bg::math::Color _shadowColor;
	ShadowType _type;
	bg::math::Matrix4 _viewMatrix;

	bg::ptr<bg::scene::DrawVisitor> _drawVisitor;

	void renderShadowMap(bg::scene::Node * sceneRoot, bg::scene::Camera * camera, bg::base::Light * light);

};

}
}

#endif
