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

#ifndef _bg2e_scene_component_hpp_
#define _bg2e_scene_component_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/base/mouse_event.hpp>
#include <bg/base/context.hpp>
#include <bg/base/keyboard_event.hpp>
#include <bg/base/touch_event.hpp>
#include <bg/base/pipeline.hpp>
#include <bg/base/sensor_event.hpp>
#include <bg/db/json/value.hpp>
#include <bg/system/path.hpp>
#include <bg/base/custom_event.hpp>
#include <bg/render/low_overhead_pipeline.hpp>
#include <bg/base/render_queue.hpp>

#include <functional>
#include <unordered_map>

namespace bg {
namespace scene {

// Common components forward declarations
class AudioListener;
class AudioSource;
class Transform;
class Camera;
class Light;
class Drawable;
class Collider;
class RigidBody;
class Dynamics;

class SceneObject;
class Node;
class BG2E_EXPORT Component : public base::ReferencedPointer {
	friend class SceneObject;
public:
	static Component * Factory(bg::base::Context *, bg::db::json::Value * componentData, const bg::system::Path & resourcePath);

	Component();

	virtual Component * clone() = 0;

	inline SceneObject * sceneObject() { return _sceneObject; }
	Node * node();

	virtual void init() {}
	virtual void reshape(bg::base::Pipeline *, int, int) {}
	virtual void willDraw(bg::base::Pipeline *) {}
	virtual void draw(bg::base::Pipeline *) {}
	virtual void didDraw(bg::base::Pipeline *) {}
	virtual void frame(float) {}
	virtual void destroy() {}
	virtual void keyUp(const bg::base::KeyboardEvent &) {}
	virtual void keyDown(const bg::base::KeyboardEvent &) {}
	virtual void charPress(const bg::base::KeyboardEvent &) {}
	virtual void mouseDown(const bg::base::MouseEvent &) {}
	virtual void mouseDrag(const bg::base::MouseEvent &) {}
	virtual void mouseMove(const bg::base::MouseEvent &) {}
	virtual void mouseUp(const bg::base::MouseEvent &) {}
	virtual void mouseWheel(const bg::base::MouseEvent &) {}
	virtual void touchStart(const bg::base::TouchEvent &) {}
	virtual void touchMove(const bg::base::TouchEvent &) {}
	virtual void touchEnd(const bg::base::TouchEvent &) {}
	virtual void sensorEvent(const bg::base::SensorEvent &) {}
	virtual void customEvent(const bg::base::CustomEvent &) {}
    
    // Low overhead rendering functions: used in new generation rendering
    // engines (Vulkan/Metal/DirectX 12)
    virtual void willUpdate(bg::render::LowOverheadPipeline *, uint32_t currentFrame) {}
    virtual void update(bg::render::LowOverheadPipeline *, uint32_t currentFrame) {}
    virtual void didUpdate(bg::render::LowOverheadPipeline *, uint32_t currentFrame) {}
    
    virtual void willUpdate(bg::base::MatrixStack & modelMatrixStack) {}
    virtual void draw(bg::base::RenderQueue * renderQueue, bg::base::MatrixStack & modelMatrixStack) {}
    virtual void didUpdate(bg::base::MatrixStack & modelMatrixStack) {}
    
    virtual void recordCommandBuffer(bg::render::LowOverheadPipeline *, uint32_t currentFrame) {}

	inline void setIgnoreSerialize(bool i) { _ignoreSerialize = i; }
	inline bool ignoreSerialize() const { return _ignoreSerialize; }

	virtual void deserialize(bg::base::Context *, bg::db::json::Value *, const bg::system::Path &) {}
	virtual bg::db::json::Value * serialize(bg::base::Context *, const bg::system::Path &) { return nullptr; }

	virtual void addedToNode(SceneObject *) {}
	virtual void removedFromNode(SceneObject *) {}

	bg::base::Context * context();
	
	// Common components getters
	AudioListener * audioListener();
	AudioSource * audioSource();
	Transform * transform();
	Camera * camera();
	Light * light();
	Drawable * drawable();
	Collider * collider();
	RigidBody * rigidBody();
	Dynamics * dynamics();

	inline void checkInit() {
		if (!_initialized) {
			init();
			_initialized = true;
		}
	}

protected:
	virtual ~Component();

	SceneObject * _sceneObject;
	bool _initialized;

	bool _ignoreSerialize;
};

class BG2E_EXPORT ComponentRegistry {
public:
	static ComponentRegistry * Get() {
		if (s_singleton == nullptr)  {
			s_singleton = new ComponentRegistry();
		}
		return s_singleton;
	}

	void registerFactory(const std::string & name, std::function<Component *()> factory) {
		_factoryMap[name] = factory;
	}

	Component * instantiate(const std::string & name) {
		return _factoryMap.find(name)!=_factoryMap.end() ? _factoryMap[name]() : nullptr;
	}

protected:
	ComponentRegistry();
	virtual ~ComponentRegistry();

	static ComponentRegistry * s_singleton;
	std::unordered_map<std::string, std::function<Component*()> > _factoryMap;
};

template <class T>
class ComponentFactory {
public:
	ComponentFactory(const std::string & name) {
		ComponentRegistry::Get()->registerFactory(name, []() { return new T(); });
	}
};

typedef std::unordered_map<size_t, ptr<Component> > ComponentMap;

}
}

#endif
