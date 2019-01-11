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


#ifndef _bg2e_render_renderer_hpp_
#define _bg2e_render_renderer_hpp_

#include <bg/base/context_object.hpp>
#include <bg/scene/node.hpp>
#include <bg/scene/camera.hpp>
#include <bg/scene/frame_visitor.hpp>
#include <bg/render/settings.hpp>
#include <bg/fx/renderer.hpp>

#include <bg/render/render_layer.hpp>

#include <unordered_map>

namespace bg {
namespace render {


class BG2E_EXPORT Renderer : public bg::base::ContextObject, public bg::base::ReferencedPointer {
public:

	enum RenderPath {
		kRenderPathForward = 1,
		kRenderPathDeferred
	};
	
	typedef std::unordered_map<size_t,RenderSettings*> RenderSettingsMap;

	static Renderer * Create(bg::base::Context *, RenderPath path);

	Renderer(bg::base::Context *);

	virtual bool isSupported() = 0;
	virtual void create() = 0;

	inline void setClearColor(const bg::math::Color & cc) { _clearColor = cc; }
	inline const bg::math::Color & clearColor() const { return _clearColor; }
	inline bg::math::Color & clearColor() { return _clearColor; }
	
    virtual void resize(int,int) {}
	virtual void frame(bg::scene::Node *, float);
	virtual void draw(bg::scene::Node *, bg::scene::Camera *) = 0;
	virtual void invalidateTextures() = 0;

	/*
	 *	If set to true, the internal framebuffers will be invalidated after complete the frame drawing. If you
	 *	need to access to the internal buffers contents, you can disable this flag and manually call
	 *	invalidateBuffers(). If you set a texture render surface to this->pipeline(), this call will not affect
	 *	to its framebuffers (you will need to manually call pipeline()->renderSurface()->invalidateBuffers()).
	 *	Set this value to true to optimize the rendering process in system with multiple GPUs (SLI, Crossfire).
	 *
	 *	Default = true
	 */
	inline void setAutoInvalidateTextures(bool i) { _autoInvalidateTextures = i; } 
	
	template <class T>
	T * settings() {
		return dynamic_cast<T*>(_settings[typeid(T).hash_code()]);
	}

	// It's far more efficient to use the above settings() function to configure the render settings, but you can
	// use the settings group name and the settings key to do it directly in the Renderer object
	void setRenderSetting(const std::string & category, const std::string & key, const std::string & value);
	void setRenderSetting(const std::string & category, const std::string & key, bool value);
	void setRenderSetting(const std::string & category, const std::string & key, int value);
	void setRenderSetting(const std::string & category, const std::string & key, float value);
    void setRenderSetting(const std::string & category, const std::string & key, const bg::math::Scalar & value);
	void setRenderSetting(const std::string & category, const std::string & key, const bg::math::Vector2 & value);
	void setRenderSetting(const std::string & category, const std::string & key, const bg::math::Vector3 & value);
	void setRenderSetting(const std::string & category, const std::string & key, const bg::math::Vector4 & value);
	void setRenderSetting(const std::string & category, const std::string & key, const bg::math::Size2Di & value);

	const std::string & stringSettingValue(const std::string & category, const std::string & key) const;
	bool boolSettingValue(const std::string & category, const std::string & key) const;
	int intSettingValue(const std::string & category, const std::string & key) const;
	float floatSettingValue(const std::string & category, const std::string & key) const;
	const bg::math::Vector2 & vec2SettingValue(const std::string & category, const std::string & key) const;
	const bg::math::Vector3 & vec3SettingValue(const std::string & category, const std::string & key) const;
	const bg::math::Vector4 & vec4SettingValue(const std::string & category, const std::string & key) const;
	const bg::math::Size2Di sizeSettingValue(const std::string & category, const std::string & key) const;

protected:
	virtual ~Renderer();
	
	bg::ptr<bg::scene::FrameVisitor> _frame;
	bg::math::Color _clearColor;
	bool _autoInvalidateTextures;
	
	bg::base::TextureVector _textures;

	// Caution with the render settings: the RenderSettingsMap does not
	// retain the object that it stores.
	void addRenderSetting(RenderSettings * s) {
		_settings[hash(s)] = s;
	}

	inline size_t hash(RenderSettings * settings) const { return typeid(*settings).hash_code(); }

	RenderSettingsMap _settings;
	std::string _emptyString;
	bg::math::Vector2 _emptyVec2;
	bg::math::Vector3 _emptyVec3;
	bg::math::Vector4 _emptyVec4;
	bg::math::Size2Di _emptySize;
};

class BG2E_EXPORT ClassicRenderer : public Renderer {
public:
    ClassicRenderer(bg::base::Context * ctx);
    
    inline bg::base::Pipeline * pipeline() { return _pipeline.getPtr(); }
    
protected:
    virtual ~ClassicRenderer();
    
    ptr<bg::base::Pipeline> _pipeline;
    ptr<bg::fx::Renderer> _mixEffect;
};

}
}

#endif
