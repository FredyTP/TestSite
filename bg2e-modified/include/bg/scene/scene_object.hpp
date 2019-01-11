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

#ifndef _bg2e_scene_scene_object_hpp_
#define _bg2e_scene_scene_object_hpp_

#include <bg/scene/component.hpp>
#include <bg/base/context_object.hpp>
#include <bg/base/object_registry.hpp>
#include <bg/base/render_queue.hpp>

namespace bg {
namespace scene {


class BG2E_EXPORT SceneObject : public bg::base::ContextObject,
								public bg::base::UniqueIdentifiedObject {
public:
	SceneObject(base::Context *);
	SceneObject(base::Context *, const std::string &);

	inline void setName(const std::string & name) { _name = name; }
	inline const std::string & name() const { return _name; }
	inline std::string & name() { return _name; }

	inline void setSteady(bool s) { _steady = s; }
	inline bool isSteady() const { return _steady; }

	inline void setEnabled() { _enabled = true; }
	inline void setEnabled(bool enabled) { _enabled = enabled; }
	inline void setDisabled() { _enabled = false; }
	inline bool enabled() const { return _enabled; }

	void addComponent(Component * component) {
		if(component) {
			ptr<Component> preventDelete = component;

			if(component->_sceneObject != nullptr) {
				SceneObject * parent = component->_sceneObject;
				parent->removeComponent(component);
				component->removedFromNode(parent);
			}

            if (_componentMap.find(hash(component))!=_componentMap.end() && _componentMap[hash(component)].valid()) {
                _componentMap[hash(component)]->removedFromNode(this);
                _componentMap[hash(component)]->_sceneObject = nullptr;
            }
			_componentMap[hash(component)] = component;
			component->_sceneObject = this;
			component->addedToNode(this);
		}
	}

	void removeComponent(Component * component) {
		ComponentMap::iterator iterator = _componentMap.find(hash(component));
		if(iterator != _componentMap.end()) {
			ptr<Component> preventDelete = component;
			component->_sceneObject = nullptr;
			_componentMap.erase(iterator);
			component->removedFromNode(this);
		}
	}

	template <class T>
	T * component() {
		return dynamic_cast<T*>(_componentMap[typeid(T).hash_code()].getPtr());
	}

	ComponentMap & componentMap() { return _componentMap; }
	const ComponentMap & componentMap() const { return _componentMap; }

	void eachComponent(std::function<void(Component*)> lambda) {
		for (auto comp : _componentMap) {
			if (comp.second.valid()) lambda(comp.second.getPtr());
		}
	}

	bool someComponent(std::function<bool(scene::Component*)> lambda) {
		bool status = false;
		for(auto comp : _componentMap) {
			if(comp.second.valid() && (status = lambda(comp.second.getPtr())) == true) {
				break;
			}
		}
		return status;
	}

	bool everyComponent(std::function<bool(scene::Component*)> lambda) {
		bool status = true;
		for(auto comp : _componentMap) {
			if(comp.second.valid() && (status = lambda(comp.second.getPtr())) == false) {
				break;
			}
		}
		return status;
	}

	virtual void reshape(bg::base::Pipeline *, int, int);
 
    // TODO: Use render queues
	virtual void willDraw(bg::base::Pipeline *);
	virtual void draw(bg::base::Pipeline *);
	virtual void didDraw(bg::base::Pipeline *);
 
 
	virtual void frame(float);
	virtual void destroy();
	virtual void keyUp(const bg::base::KeyboardEvent &);
	virtual void keyDown(const bg::base::KeyboardEvent &);
	virtual void charPress(const bg::base::KeyboardEvent &);
	virtual void mouseDown(const bg::base::MouseEvent &);
	virtual void mouseDrag(const bg::base::MouseEvent &);
	virtual void mouseMove(const bg::base::MouseEvent &);
	virtual void mouseUp(const bg::base::MouseEvent &);
	virtual void mouseWheel(const bg::base::MouseEvent &);
	virtual void touchStart(const bg::base::TouchEvent & evt);
	virtual void touchMove(const bg::base::TouchEvent & evt);
	virtual void touchEnd(const bg::base::TouchEvent & evt);
	virtual void sensorEvent(const bg::base::SensorEvent & evt);
	virtual void customEvent(const bg::base::CustomEvent &);
 
    // Low overhead rendering functions: used in new generation rendering
    // engines (Vulkan/Metal/DirectX 12)
    // TODO: Use render queues
    virtual void willUpdate(bg::render::LowOverheadPipeline *, uint32_t currentFrame);
    virtual void update(bg::render::LowOverheadPipeline *, uint32_t currentFrame);
    virtual void didUpdate(bg::render::LowOverheadPipeline *, uint32_t currentFrame);
    
    virtual void willUpdate(bg::base::MatrixStack & modelMatrixStack);
    virtual void draw(bg::base::RenderQueue *, bg::base::MatrixStack & modelMatrixStack);
    virtual void didUpdate(bg::base::MatrixStack & modelMatrixStack);
    
    
    virtual void recordCommandBuffer(bg::render::LowOverheadPipeline *, uint32_t currentFrame);
	
protected:
	virtual ~SceneObject();

	ComponentMap _componentMap;
	std::string _name;
	bool _enabled;
	bool _initialized;
	bool _steady;

	inline size_t hash(Component * component) const { return typeid(*component).hash_code(); }
};

}
}

#endif
