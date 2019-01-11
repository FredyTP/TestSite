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


#ifndef _bg2e_scene_drawable_hpp_
#define _bg2e_scene_drawable_hpp_

#include <bg/scene/component.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/base/material.hpp>
#include <bg/scene/node.hpp>

#include <vector>
#include <string>
#include <functional>

namespace bg {
namespace scene {

class BG2E_EXPORT Drawable : public Component {
public:
	
	struct DrawableElement {
		bg::ptr<bg::base::PolyList> polyList;
		bg::ptr<bg::base::Material> material;
		bg::math::Matrix4 transform;
		
		inline void operator=(const DrawableElement & e) {
			polyList = e.polyList;
			material = e.material;
			transform = e.transform;
		}
		
		inline bool operator==(const DrawableElement & e) const {
			return  polyList.getPtr()==e.polyList.getPtr() &&
					material.getPtr()==e.material.getPtr() &&
					transform==e.transform;
		}
	};
	
	typedef std::vector<DrawableElement> DrawableElementVector;
	
	/*
	 *	Creates a copy of the node with all its components, except the drawable
	 *	component, that will be an instance (sharing the same polyList)
	 */
	static Node * InstanceNode(Node *);
	
	Drawable();
	Drawable(const std::string & name);
	
	Component * clone();
	Drawable * instance(const std::string & newName);
	
	inline void setName(const std::string & name) { _name = name; }
	inline const std::string & name() const { return _name; }
	inline std::string & name() { return _name; }
	
	inline DrawableElementVector & drawableElements() { return _drawableElements; }
	inline const DrawableElementVector & drawableElements() const { return _drawableElements; }
	
	void addPolyList(bg::base::PolyList * plist, bg::base::Material * mat = nullptr, const bg::math::Matrix4 & trx = bg::math::Matrix4::Identity());
	
	void removePolyList(bg::base::PolyList * plist);
	
	size_t indexOf(bg::base::PolyList * plist) const;
	
	DrawableElementVector::iterator element(size_t index);
	
	const DrawableElementVector::const_iterator element(size_t index) const;
	
	inline bg::base::PolyList * polyList(size_t index) {
		DrawableElementVector::iterator result = element(index);
		if (result!=_drawableElements.end()) {
			return (*result).polyList.getPtr();
		}
		return nullptr;
	}
	
	inline const bg::base::PolyList * polyList(size_t index) const {
		DrawableElementVector::const_iterator result = element(index);
		if (result!=_drawableElements.end()) {
			return (*result).polyList.getPtr();
		}
		return nullptr;
	}
	
	inline bg::base::Material * material(size_t index) {
		DrawableElementVector::iterator result = element(index);
		if (result!=_drawableElements.end()) {
			return (*result).material.getPtr();
		}
		return nullptr;
	}
	
	inline const bg::base::Material * material(size_t index) const  {
		DrawableElementVector::const_iterator result = element(index);
		if (result!=_drawableElements.end()) {
			return (*result).material.getPtr();
		}
		return nullptr;
	}

	inline const bg::math::Matrix4 & transform(size_t index) const {
		DrawableElementVector::const_iterator result = element(index);
		if (result!=_drawableElements.end()) {
			return (*result).transform;
		}
		return s_transformIdentity.identity();
	}
	
	inline bg::math::Matrix4 & transform(size_t index) {
		DrawableElementVector::iterator result = element(index);
		if (result!=_drawableElements.end()) {
			return (*result).transform;
		}
		return s_transformIdentity.identity();
	}

	inline const DrawableElement * find(const std::string & name) const {
		for (auto & drwElem : _drawableElements) {
			if (drwElem.polyList->name() == name) {
				return &drwElem;
			}
		}
		return nullptr;
	}

	inline DrawableElement * find(const std::string & name) {
		for (auto & drwElem : _drawableElements) {
			if (drwElem.polyList->name() == name) {
				return &drwElem;
			}
		}
		return nullptr;
	}
	
	inline void forEach(std::function<void (DrawableElement &)> lambda) {
		for (auto & drwElem : _drawableElements) {
			lambda(drwElem);
		}
	}
	
	inline bool some(std::function<bool (DrawableElement &)> lambda) {
		bool status = false;
		
		for (auto & drwElem : _drawableElements) {
			if ((status=lambda(drwElem))==true) {
				break;
			}
		}
		
		return status;
	}
	
	inline bool every(std::function<bool (DrawableElement &)> lambda) {
		bool status = true;
		
		for (auto & drwElem : _drawableElements) {
			if ((status=lambda(drwElem))==false) {
				break;
			}
		}
		
		return status;
	}
	
	void draw(bg::base::Pipeline * pipeline);
 
 
    virtual void draw(bg::base::RenderQueue * renderQueue, bg::base::MatrixStack & modelMatrixStack);
    
 
    // Low overhead rendering engines
    void update(bg::render::LowOverheadPipeline *, uint32_t currentFrame);
    void recordCommandBuffer(bg::render::LowOverheadPipeline *, uint32_t currentFrame);

	virtual void deserialize(bg::base::Context *, bg::db::json::Value *, const bg::system::Path &);
	virtual bg::db::json::Value * serialize(bg::base::Context *, const bg::system::Path &);

protected:
	virtual ~Drawable();
	
	static bg::math::Matrix4 s_transformIdentity;
	
	std::string _name;
	
	DrawableElementVector _drawableElements;
	
};

}
}

#endif
