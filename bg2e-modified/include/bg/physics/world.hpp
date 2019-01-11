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

#ifndef _bg2e_physics_world_hpp_
#define _bg2e_physics_world_hpp_

#include <bg/export.hpp>

#include <bg/scene/node.hpp>

#include <bg/physics/ray.hpp>

#include <unordered_map>

namespace bg {
namespace scene {

// Forward declaraction of Dynamics component
class Dynamics;

}

namespace physics {

class BG2E_EXPORT World : public bg::base::ReferencedPointer {
	friend bg::scene::Dynamics;
public:
	bg::plain_ptr collisionConfiguration;
	bg::plain_ptr collisionDispatcher;
	bg::plain_ptr overlappingPairCache;
	bg::plain_ptr solver;
	bg::plain_ptr dynamicsWorld;

	World();

    inline void setMinimumFramerate(int fps) { _minFramerate = fps; }
    inline int minimumFramerate() const { return _minFramerate; }
	inline void setTargetFramerate(int fps) { _targetFramerate = fps; }
	inline int targetFramerate() const { return _targetFramerate; }

	void setGravity(const bg::math::Vector3 & g);
	inline const bg::math::Vector3 & gravity() const { return _gravity; }
	
	inline bool isRunning() const { return _isRunning; }
	
	void beginSimulation(bg::scene::Node *);
	void simulationStep(float delta);
	void endSimulation();

	// Use the following functions to add or remove a node to the world during the simulation loop.
	void removeNode(bg::scene::Node *);

	// NOTE: do not add a node that does not belong to the dynamics node
	void addNode(bg::scene::Node *);

	bg::scene::Dynamics * sceneComponent();
	
	
	bg::physics::RayCastResult * rayTest(const bg::physics::Ray & ray);
	
protected:
	virtual ~World();

	std::unordered_map<size_t, bg::ptr<bg::scene::Node> > _colliderMap;
	bg::math::Vector3 _gravity;
	bool _isRunning;
    int _minFramerate = 12;
	int _targetFramerate = 60;
	bg::scene::Dynamics * _dynamics;
};
	
}
}

#endif
