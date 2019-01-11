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

#ifndef _bg2e_scene_dynamics_hpp_
#define _bg2e_scene_dynamics_hpp_

#include <bg/scene/component.hpp>

#include <bg/physics/world.hpp>

#include <bg/scene/collider.hpp>
#include <bg/scene/rigid_body.hpp>

#include <functional>

namespace bg {
namespace scene {

class BG2E_EXPORT Dynamics : public Component {
public:
	enum SimulationState {
		kStateStopped = 0,
		kStatePlaying = 1,
		kStatePaused  = 2
	};
	
	Dynamics();
	
	Component * clone();

	inline bg::physics::World * world() { return _world.getPtr(); }
	inline const bg::physics::World * world() const { return _world.getPtr(); }
	
	virtual void frame(float delta);
	
	// Iterate all child nodes with a collider component.
	// - collider will never be null
	// - if rigidbody is null, then the object is a static mesh
	// Children without collider will be ignored
	inline void eachShape(std::function<void (Node *,Collider *, RigidBody *)> closure) {
		if (node()) {
			for (auto n : node()->children()) {
				Collider * col = n->component<Collider>();
				if (col) {
					closure(n.getPtr(), col, n->component<RigidBody>());
				}
			}
		}
	}
	
	void play();
	void pause();
	void stop();
	
	inline SimulationState getSimulationState() const { return _state; }
	
	virtual void deserialize(bg::base::Context *, bg::db::json::Value *, const bg::system::Path &);
	virtual bg::db::json::Value * serialize(bg::base::Context *, const bg::system::Path &);
	
protected:
	virtual ~Dynamics();
	
	bg::ptr<bg::physics::World> _world;
	SimulationState _state;
};
	
class BG2E_EXPORT DynamicsVisitor : public NodeVisitor {
public:
	DynamicsVisitor() :_state(Dynamics::kStateStopped) {}
	
	inline void clear() { _dynamics.clear(); }
	
	virtual void visit(bg::scene::Node * node);

	void play();
	void pause();
	void stop();
	
protected:
	std::vector<bg::ptr<Dynamics> > _dynamics;
	
	Dynamics::SimulationState _state;
};

}
}

#endif
