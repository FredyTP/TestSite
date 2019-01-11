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

#ifndef _bg2e_render_deferred_render_layer_hpp_
#define _bg2e_render_deferred_render_layer_hpp_

#include <bg/render/render_layer.hpp>
#include <bg/fx/gbuffer.hpp>
#include <bg/fx/lighting.hpp>
#include <bg/fx/shadow.hpp>
#include <bg/fx/deferred_mix.hpp>
#include <bg/fx/ssrt.hpp>
#include <bg/fx/ssao.hpp>
#include <bg/scene/light.hpp>
#include <bg/render/shadow_map.hpp>
#include <bg/render/deferred_render_surfaces.hpp>
#include <bg/scene/draw_visitor.hpp>

namespace bg {
namespace render {

namespace settings {
	
extern BG2E_EXPORT const SettingsKey kAmbientOcclusion;
extern BG2E_EXPORT const SettingsKey kRaytracer;

extern BG2E_EXPORT const SettingsKey kKernelSize;

extern BG2E_EXPORT const SettingsKey kClearOnViewChanged;

}

class AmbientOcclusionSettings : public bg::base::ReferencedPointer,
	public RenderSettings {
public:
	AmbientOcclusionSettings() :RenderSettings(settings::kAmbientOcclusion) {}

	inline void setSampleRadius(float sr) { _sampleRadius = sr; }
	inline void setKernelSize(int ks) { if(ks<kMaxKernelOffsets) _kernelSize = ks; }
	inline void setColor(const bg::math::Color & c) { _ssaoColor = c; }
	inline void setEnabled(bool e) { _enabled = e; }
	inline void setMaxDistance(float d) { _maxDistance = d; }
	inline void setBlur(int b) { _blur = b; }
	inline void setQuality(bg::base::Quality quality) { _quality = quality; }
		

	inline float sampleRadius() const { return _sampleRadius; }
	inline int kernelSize() const { return _kernelSize; }
	inline const bg::math::Color & color() const { return _ssaoColor; }
	inline bool isEnabled() const { return _enabled; }
	inline float maxDistance() const { return _maxDistance; }
	inline int blur() const { return _blur; }
	inline bg::base::Quality quality() const { return _quality; }


	// Settings values
	virtual void setSettingValue(const SettingsKey & key, bool value) {
		if(key == settings::kEnabled) {
			_enabled = value;
		}
	}

	virtual void setSettingValue(const SettingsKey & key, int value) {
		if(key == settings::kKernelSize) {
			_kernelSize = value;
		}
		else if(key == settings::kBlur) {
			_blur = value;
		}
		else if(key == settings::kSampleRadius) {
			_sampleRadius = static_cast<float>(value);
		}
		else if(key == settings::kMaxDistance) {
			_maxDistance = static_cast<float>(value);
		}
		else if(key == settings::kQuality && value>=bg::base::kQualityLow && value<=bg::base::kQualityExtreme) {
			_quality = static_cast<bg::base::Quality>(value);
		}
	}
	
	virtual void setSettingValue(const SettingsKey & key, float value) {
		if(key == settings::kKernelSize && value >= 8.0f && value <= static_cast<float>(kMaxKernelOffsets)) {
			_kernelSize = static_cast<int>(value);
		}
		else if(key == settings::kBlur) {
			_blur = static_cast<int>(value);
		}
		else if(key == settings::kSampleRadius) {
			_sampleRadius = value;
		}
		else if(key == settings::kMaxDistance) {
			_maxDistance = value;
		}
	}

	virtual void setSettingValue(const SettingsKey & key, const bg::math::Vector4 & value) {
		if(key == settings::kColor) {
			_ssaoColor = value;
		}
	}

	virtual bool boolSettingValue(const SettingsKey & key) const {
		if(key == settings::kEnabled) {
			return _enabled;
		}
		else {
			return RenderSettings::boolSettingValue(key);
		}
	}

	virtual int intSettingValue(const SettingsKey & key) const {
		if(key == settings::kKernelSize) {
			return _kernelSize;
		}
		else if(key == settings::kBlur) {
			return _blur;
		}
		else if(key == settings::kSampleRadius) {
			return static_cast<int>(_sampleRadius);
		}
		else if(key == settings::kMaxDistance) {
			return static_cast<int>(_maxDistance);
		}
		else if (key == settings::kQuality) {
			return static_cast<int>(_quality);
		}
		else {
			return RenderSettings::intSettingValue(key);
		}
	}

	virtual float floatSettingValue(const SettingsKey & key) const {
		if(key == settings::kSampleRadius) {
			return _sampleRadius;
		}
		else if(key == settings::kMaxDistance) {
			return _maxDistance;
		}
		else if(key == settings::kKernelSize) {
			return static_cast<float>(_kernelSize);
		}
		else if(key == settings::kBlur) {
			return static_cast<float>(_blur);
		}
		else {
			return RenderSettings::floatSettingValue(key);
		}
	}

	virtual const bg::math::Vector4 & vec4SettingValue(const SettingsKey & key) const {
		if(key == settings::kColor) {
			return _ssaoColor;
		}
		else {
			return RenderSettings::vec4SettingValue(key);
		}
	}

protected:
	virtual ~AmbientOcclusionSettings() {}

	float _sampleRadius = 0.5f;
	int _kernelSize = 16;
	bg::math::Color _ssaoColor = bg::math::Color::Black();
	bool _enabled = true;
	float _maxDistance = 10000.0f;
	int _blur = 1;
	bg::base::Quality _quality = bg::base::kQualityExtreme;
};

class RaytracerSettings : public bg::base::ReferencedPointer,
	public RenderSettings {
public:
	RaytracerSettings() :RenderSettings(settings::kRaytracer) {}

	inline void setQuality(bg::base::Quality quality) { _quality = quality; }
	inline bg::base::Quality quality() const { return _quality; }
	inline void setScale(float scale) { _scale = scale; }
	inline float scale() const { return _scale; }
	inline void setEnabled(bool e) { _enabled = e; }
	inline bool isEnabled() const { return _enabled; }
	inline void setClearOnViewChanged(bool e) { _clearOnViewChanged = e; }
	inline bool clearOnViewChanged() const { return _clearOnViewChanged; }

	// Settings values
	virtual void setSettingValue(const SettingsKey & key, bool value) {
		if (key==settings::kEnabled) {
			_enabled = value;
		}
		else if (key == settings::kClearOnViewChanged) {
			_clearOnViewChanged = value;
		}
	}
	
	virtual void setSettingValue(const SettingsKey & key, int value) {
		if(key == settings::kQuality && value >= bg::base::kQualityLow && value <= bg::base::kQualityExtreme) {
			_quality = static_cast<bg::base::Quality>(value);
		}
	}

	virtual void setSettingValue(const SettingsKey & key, float value) {
		if (key == settings::kScale && value <= 1.0f && value > 0.0f) {
			_scale = value;
		}
	}

	virtual int intSettingValue(const SettingsKey & key) const {
		if(key == settings::kQuality) {
			return static_cast<int>(_quality);
		}
		else {
			return RenderSettings::intSettingValue(key);
		}
	}

	virtual bool boolSettingValue(const SettingsKey & key) const {
		if ( key==settings::kEnabled ) {
			return _enabled;
		}
		else if (key == settings::kClearOnViewChanged) {
			return _clearOnViewChanged;
		}
		else {
			return RenderSettings::boolSettingValue(key);
		}
	}

	virtual float floatSettingValue(const SettingsKey & key) {
		if (key == settings::kScale) {
			return _scale;
		}
		else {
			return RenderSettings::floatSettingValue(key);
		}
	}

protected:
	virtual ~RaytracerSettings() {}

	bg::base::Quality _quality = bg::base::kQualityExtreme;
	float _scale = 1.0f;
	bool _enabled = true;
	bool _clearOnViewChanged = false;
};

class BG2E_EXPORT DeferredRenderLayer : public RenderLayer {
public:
	DeferredRenderLayer(bg::base::Context *, bg::base::Pipeline * pl = nullptr, bool memoryOptimized = false);

	void createOpaqueLayer();
	void createTransparentLayer(DeferredRenderSurfaces * opaqueMaps);
	
	inline void setClearColor(const bg::math::Color & c) {
		if (_surfaces->type()==DeferredRenderSurfaces::kTypeOpaque) {
			_clearColor = c;
		}
	}

	inline void setShadowMap(ShadowMap * sm) { _shadowMap = sm; }
	inline ShadowMap * shadowMap() { return _shadowMap.getPtr(); }
	inline const ShadowMap * shadowMap() const { return _shadowMap.getPtr(); }

	void draw(bg::scene::Node *, bg::scene::Camera *);

	inline void setAmbientOcclusionSettings(AmbientOcclusionSettings * s) { _ssaoSettings = s; }
	inline void setRaytracerSettings(RaytracerSettings * s) { _ssrtSettings = s; }
	inline void setDebugSettings(DebugRenderSettings * s) { _debugSettings = s; }
	
	inline DeferredRenderSurfaces * maps() { return _surfaces.getPtr(); }
    
    void resize(bg::scene::Camera *);

	virtual void invalidateBuffers();
	virtual void invalidateTextures();

protected:
	virtual ~DeferredRenderLayer();

	virtual void willDraw(bg::scene::Node *, bg::scene::Camera *);
	virtual void performDraw(bg::scene::Node *, bg::scene::Camera *);

	void renderShadowCascade(bg::scene::Node * sceneRoot, bg::scene::Camera * camera, bg::scene::Light * lightComponent, bool firstCascade, ShadowMap::ShadowCascade cascade);

	bg::ptr<DeferredRenderSurfaces> _surfaces;
	
	bg::math::Color _clearColor;

	bg::ptr<bg::fx::GBuffer> _gbufferEffect;
	bg::ptr<bg::base::Pipeline> _gbufferPipeline;
	
	bg::ptr<bg::fx::GBufferFloat> _gbufferFloatEffect;
	bg::ptr<bg::base::Pipeline> _gbufferFloatPipeline;
	bg::ptr<bg::scene::DrawVisitor> _floatDrawVisitor;

	bg::ptr<bg::fx::Lighting> _lightingEffect;
	bg::ptr<bg::base::Pipeline> _lightingPipeline;

	bg::ptr<bg::fx::Shadow> _shadowEffect;
	bg::ptr<bg::base::Pipeline> _shadowPipeline;
	bg::ptr<bg::scene::DrawVisitor> _shadowVisitor;

	bg::ptr<bg::fx::Ssao> _ssaoEffect;
	bg::ptr<bg::base::Pipeline> _ssaoPipeline;

	bg::ptr<bg::fx::Ssrt> _ssrtEffect;
	bg::ptr<bg::base::Pipeline> _ssrtPipeline;

	bg::ptr<bg::fx::DeferredMix> _mixEffect;
	bg::ptr<bg::base::Pipeline> _mixPipeline;

	bg::ptr<ShadowMap> _shadowMap;
	
	bool _memoryOptimized;

	bg::ptr<AmbientOcclusionSettings> _ssaoSettings;
	bg::ptr<RaytracerSettings> _ssrtSettings;
	bg::ptr<DebugRenderSettings> _debugSettings;

	bg::math::Matrix4 _lastViewMatrix;
	
	void createCommon();
};

}
}

#endif
