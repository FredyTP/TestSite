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

#ifndef _bg2e_data_tree_node_hpp_
#define _bg2e_data_tree_node_hpp_

#include <bg/base/referenced_pointer.hpp>

#include <functional>
#include <vector>

namespace bg {
namespace data {

template <class T>
class TreeNode : public bg::base::ReferencedPointer {
public:
	typedef std::vector<ptr<T>> NodeVector;

	class Visitor : public bg::base::ReferencedPointer {
	public:
		virtual void visit(T * node) {}

		virtual void didVisit(T * node) {}

	protected:
		virtual ~Visitor() {}
	};

	virtual void accept(Visitor * visitor) {
		if(visitor) {
			visitor->visit(dynamic_cast<T*>(this));
			for(auto c : _children) {
				c->accept(visitor);
			}
			visitor->didVisit(dynamic_cast<T*>(this));
		}
	}

	virtual void acceptReverse(Visitor * visitor) {
		if(visitor) {
			if(_parent) {
				_parent->acceptReverse(visitor);
			}
			visitor->visit(dynamic_cast<T*>(this));
		}
	}

	bool addChild(T * child) {
		if(child && !isAncientOf(child) && child != this) {
			bg::ptr<T> node = child;
			if(node->_parent) {
				node->_parent->removeChild(node.getPtr());
			}
			_children.push_back(node.getPtr());
			node->_parent = dynamic_cast<T*>(this);
			node->addedToNode(dynamic_cast<T*>(this));
			return true;
		}
		return false;
	}

	bool removeChild(T * child) {
		if(child) {
			auto it = std::find(_children.begin(), _children.end(), child);
			if(it != _children.end()) {
				(*it)->_parent = nullptr;
				(*it)->removedFromNode(dynamic_cast<T*>(this));
				_children.erase(it);
				return true;
			}
		}
		return false;
	}

	void clearChildren() {
		for(auto child : _children) {
			child->_parent = nullptr;
			child->removedFromNode(dynamic_cast<T*>(this));
		}
		_children.clear();
	}

	virtual void addedToNode(T *) {}

	virtual void removedFromNode(T *) {}

	inline NodeVector & children() { return _children; }
	inline const NodeVector & children() const { return _children; }

	inline T * parentNode() { return _parent; }

	inline bool haveChild(T * node) { return std::find(_children.begin(), _children.end(), node) != _children.end(); }
	inline bool isAncientOf(T * ancient) {
		return isAncient(dynamic_cast<T*>(this), ancient);
	}
	static bool isAncient(T * node, T * potentialAncient) {
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

	T * nextChildOf(T * node) {
		auto it = std::find(_children.begin(), _children.end(), node);
		auto nextIt = it + 1;
		if(it != _children.end() && nextIt != _children.end()) {
			return (*nextIt).getPtr();
		}
		return nullptr;
	}

	T * prevChildOf(T * node) {
		auto it = std::find(_children.begin(), _children.end(), node);
		auto prevIt = it - 1;
		if(it != _children.begin()) {
			return (*prevIt).getPtr();
		}
		return nullptr;
	}

	bool swapChildren(T * childA, T * childB) {
		auto iterA = std::find(_children.begin(), _children.end(), childA);
		auto iterB = std::find(_children.begin(), _children.end(), childB);

		if(iterA != _children.end() && iterB != _children.end() && iterA != iterB) {
			std::iter_swap(iterA, iterB);
			return true;
		}

		return false;
	}

	bool moveChildNextTo(T * nextToTreeNode, T * movingTreeNode) {
		auto nextToIterator = std::find(_children.begin(), _children.end(), nextToTreeNode);
		auto movingIterator = std::find(_children.begin(), _children.end(), movingTreeNode);
		nextToIterator++;

		if(nextToIterator != _children.end() && movingIterator != _children.end() && nextToIterator != movingIterator) {
			bg::ptr<T> movingTreeNodePtr = movingTreeNode;
			_children.erase(movingIterator);

			_children.insert(nextToIterator, movingTreeNodePtr.getPtr());

			return true;
		}
		return false;
	}

	void eachChild(std::function<void(T *)> lambda) {
		for(auto child : _children) {
			lambda(child.getPtr());
		}
	}

	bool someChild(std::function<bool(T*)> lambda) {
		bool status = false;
		for(auto child : _children) {
			if((status = lambda(child.getPtr())) == true) {
				break;
			}
		}
		return status;
	}

	bool everyChild(std::function<bool(T*)> lambda) {
		bool status = true;
		for(auto child : _children) {
			if((status = lambda(child.getPtr())) == false) {
				break;
			}
		}
		return status;
	}

	T * root() {
		if(_parent) {
			return _parent->root();
		}
		return dynamic_cast<T*>(this);
	}

	virtual void destroyNode() {
		eachChild([&](T * child) {
			child->destroyNode();
		});
		_children.clear();
	}

protected:
	NodeVector _children;
	T * _parent = nullptr;
};

}
}

#endif
