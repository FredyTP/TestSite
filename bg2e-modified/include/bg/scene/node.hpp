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

#ifndef _bg2e_scene_node_hpp_
#define _bg2e_scene_node_hpp_


#include <bg/scene/scene_object.hpp>
#include <bg/scene/node_visitor.hpp>
#include <bg/base/context.hpp>

#include <vector>
#include <algorithm>
#include <functional>

namespace bg {
namespace scene {

// Forward declaration for common components
class AudioListener;
class AudioSource;
class Transform;
class Camera;
class Light;
class Drawable;
class Collider;
class RigidBody;
class Dynamics;
class BG2E_EXPORT Node : public SceneObject, public bg::base::ReferencedPointer {
public:
	typedef std::vector<ptr<Node> > NodeVector;
	
	// To remove or add nodes to a scene inside a life cycle function such as
	// frame(), draw(), keyUp(), etc, use the following functions. This functions removes
	// or adds the object asynchronously when is safe to do it.
	static void RemoveFromScene(Node *, std::function<void(Node*)> callback = nullptr);
	static void AddToScene(Node * node, Node * parent, std::function<void(Node*)> callback = nullptr);
	static void AddComponent(Component * comp, Node * node, std::function<void(Node*)> callback = nullptr);
	
	// This function must be called after the frame() function, and flushes the
	// object modification structures to add and remove the objects
	static void FlushSceneChanges();
	
	Node(bg::base::Context *);
	Node(bg::base::Context *, const std::string & );
	
	
	bool addChild(Node * child);
	bool removeChild(Node * child);
	void clearChildren();
	
	inline NodeVector & children() { return _children; }
	inline const NodeVector & children() const { return _children; }
	
	inline Node * parent() { return _parent; }
	
	inline bool haveChild(scene::Node * node) { return std::find(_children.begin(), _children.end(), node) != _children.end(); }
	inline bool isAncientOf(scene::Node * ancient) {
		return isAncient(this, ancient);
	}
	static bool isAncient(scene::Node * node, scene::Node * potentialAncient) {
		if(!node || !potentialAncient) {
			return false;
		}
		else if(node->_parent == potentialAncient) {
			return true;
		}
		else {
			return isAncient(node->_parent, potentialAncient);
		}
	}
	
	scene::Node * nextChildOf(scene::Node * node);
	scene::Node * prevChildOf(scene::Node * node);
	
	bool swapChildren(scene::Node * childA, scene::Node * childB);
	
	bool moveChildNextTo(scene::Node * nextToNode, scene::Node * movingNode);
	
	void eachChild(std::function<void(scene::Node*)> lambda) {
		for (auto child : _children) {
			lambda(child.getPtr());
		}
	}
	
	bool someChild(std::function<bool(scene::Node*)> lambda) {
		bool status = false;
		for (auto child : _children) {
			if ((status = lambda(child.getPtr()))==true) {
				break;
			}
		}
		return status;
	}
	
	bool everyChild(std::function<bool(scene::Node*)> lambda) {
		bool status = true;
		for(auto child : _children) {
			if((status = lambda(child.getPtr()))==false) {
				break;
			}
		}
		return status;
	}
	
	virtual void accept(NodeVisitor * visitor) {
		if (visitor && enabled()) {
			visitor->visit(this);
			for (auto c : _children) {
				c->accept(visitor);
			}
			visitor->didVisit(this);
		}
	}
	
	virtual void acceptReverse(NodeVisitor * visitor) {
		if (visitor && enabled()) {
			if(_parent) {
				_parent->acceptReverse(visitor);
			}
			visitor->visit(this);
		}
	}
	
	Node * sceneRoot();
	
	virtual void destroy();
	
	struct RemoveNodeHelper {
		bg::ptr<Node> node;
		std::function<void(Node*)> callback = nullptr;
		
		RemoveNodeHelper(Node * n, std::function<void(Node*)> cb) :node(n), callback(cb) {}
	};
	struct AddNodeHelper {
		bg::ptr<Node> parent;
		bg::ptr<Node> child;
		std::function<void(Node*)> callback = nullptr;
		
		AddNodeHelper(Node * p, Node * c, std::function<void(Node*)> cb) :parent(p), child(c), callback(cb) {}
	};
	
	struct AddComponentHelper {
		bg::ptr<Component> component;
		bg::ptr<Node> node;
		std::function<void(Node*)> callback = nullptr;
		
		AddComponentHelper(Component * c, Node * n, std::function<void(Node*)> cb) :component(c), node(n), callback(cb) {}
	};
	
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
	
protected:
	virtual ~Node();
	
	NodeVector _children;
	Node * _parent;
	
	static std::vector<RemoveNodeHelper> s_removeNodeVector;
	static std::vector<AddNodeHelper> s_addNodeVector;
	static std::vector<AddComponentHelper> s_addComponentVector;
};

}
}

#endif
