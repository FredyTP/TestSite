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


#include <bg/scene/node.hpp>

#include <bg/scene/audio_listener.hpp>
#include <bg/scene/audio_source.hpp>
#include <bg/scene/transform.hpp>
#include <bg/scene/camera.hpp>
#include <bg/scene/light.hpp>
#include <bg/scene/drawable.hpp>
#include <bg/scene/collider.hpp>
#include <bg/scene/rigid_body.hpp>
#include <bg/scene/dynamics.hpp>

namespace bg {
namespace scene {

std::vector<Node::RemoveNodeHelper> Node::s_removeNodeVector;
std::vector<Node::AddNodeHelper> Node::s_addNodeVector;
std::vector<Node::AddComponentHelper> Node::s_addComponentVector;

void Node::RemoveFromScene(Node * node, std::function<void(Node*)> callback) {
	if (!node) {
		throw  bg::base::NullParameterException("Node::RemoveFromScene(): null node specified.");
	}
	s_removeNodeVector.push_back(Node::RemoveNodeHelper{ node, callback });
}

void Node::AddToScene(Node * node, Node * parent, std::function<void(Node*)> callback) {
	if (!node) {
		throw bg::base::NullParameterException("Node::AddToScene(): null node specified.");
	}
	if (!parent) {
		throw bg::base::NullParameterException("Node::AddToScene(): null parent node specified.");
	}
	s_addNodeVector.push_back(Node::AddNodeHelper{ parent, node, callback });
}

void Node::AddComponent(Component * comp, Node * node, std::function<void(Node*)> callback) {
	if (!comp) {
		throw bg::base::NullParameterException("Node::AddComponent(): null component specified.");
	}
	if (!node) {
		throw bg::base::NullParameterException("Node::AddComponent(): null node specified.");
	}
	s_addComponentVector.push_back(Node::AddComponentHelper{ comp, node, callback });
}

void Node::FlushSceneChanges() {
	for (auto n : s_removeNodeVector) {
		Node * parent = n.node->parent();
		if (parent) {
			parent->removeChild(n.node.getPtr());
			if (n.callback) {
				n.callback(n.node.getPtr());
			}
		}
	}

	for (auto n : s_addNodeVector) {
		n.parent->addChild(n.child.getPtr());
		if (n.callback) {
			n.callback(n.child.getPtr());
		}
	}
	
	for (auto n : s_addComponentVector) {
		n.node->addComponent(n.component.getPtr());
		if (n.callback) {
			n.callback(n.node.getPtr());
		}
	}

	s_removeNodeVector.clear();
	s_addNodeVector.clear();
	s_addComponentVector.clear();
}

Node::Node(bg::base::Context * ctx)
	:SceneObject(ctx)
	,_parent(nullptr)
{

}

Node::Node(bg::base::Context * ctx, const std::string & name)
	:SceneObject(ctx, name)
	,_parent(nullptr)
{

}

Node::~Node()
{
}

bool Node::addChild(scene::Node * child) {
	if(child && !isAncientOf(child) && child != this) {
		bg::ptr<Node> node = child;
		if(node->_parent) {
			node->_parent->removeChild(node.getPtr());
		}
		_children.push_back(node.getPtr());
		node->_parent = this;
		return true;
	}
	return false;
}

bool Node::removeChild(scene::Node * child) {
	if(child) {
		NodeVector::iterator it = std::find(_children.begin(), _children.end(), child);
		if(it != _children.end()) {
			(*it)->_parent = nullptr;
			_children.erase(it);
			return true;
		}
	}
	return false;
}

void Node::clearChildren() {
	for (auto child : _children) {
		child->_parent = nullptr;
	}
	_children.clear();
}

Node * Node::nextChildOf(Node * node) {
	NodeVector::iterator it = std::find(_children.begin(), _children.end(), node);
	NodeVector::iterator nextIt = it + 1;
	if(it != _children.end() && nextIt != _children.end()) {
		return (*nextIt).getPtr();
	}
	return nullptr;
}

Node * Node::prevChildOf(Node * node) {
	NodeVector::iterator it = std::find(_children.begin(), _children.end(), node);
	NodeVector::iterator prevIt = it - 1;
	if(it != _children.begin()) {
		return (*prevIt).getPtr();
	}
	return nullptr;
}

bool Node::swapChildren(Node * childA, Node * childB) {
	NodeVector::iterator iterA = std::find(_children.begin(), _children.end(), childA);
	NodeVector::iterator iterB = std::find(_children.begin(), _children.end(), childB);

	if(iterA != _children.end() && iterB != _children.end() && iterA != iterB) {
		std::iter_swap(iterA, iterB);
		return true;
	}

	return false;
}

bool Node::moveChildNextTo(Node * nextToNode, Node * movingNode) {
	NodeVector::iterator nextToIterator = std::find(_children.begin(), _children.end(), nextToNode);
	NodeVector::iterator movingIterator = std::find(_children.begin(), _children.end(), movingNode);
	nextToIterator++;

	if(nextToIterator != _children.end() && movingIterator != _children.end() && nextToIterator != movingIterator) {
		bg::ptr<Node> movingNodePtr = movingNode;
		_children.erase(movingIterator);

		_children.insert(nextToIterator, movingNodePtr.getPtr());

		return true;
	}
	return false;
}

Node * Node::sceneRoot() {
	if(_parent) {
		return _parent->sceneRoot();
	}
	return this;
}

void Node::destroy() {
	eachChild([&](Node * child) {
		child->destroy();
	});
	_children.clear();
}

AudioListener * Node::audioListener() {
	return component<AudioListener>();
}

AudioSource * Node::audioSource() {
	return component<AudioSource>();
}

Transform * Node::transform() {
	return component<Transform>();
}

Camera * Node::camera() {
	return component<Camera>();
}

Light * Node::light() {
	return component<Light>();
}

Drawable * Node::drawable() {
	return component<Drawable>();
}

Collider * Node::collider() {
	return component<Collider>();
}

RigidBody * Node::rigidBody() {
	return component<RigidBody>();
}

Dynamics * Node::dynamics() {
	return component<Dynamics>();
}

}
}
