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

#ifndef _bg2e_physics_collider_hpp_
#define _bg2e_physics_collider_hpp_

#include <bg/export.hpp>
#include <bg/base/referenced_pointer.hpp>
#include <bg/system/path.hpp>
#include <bg/db/json/value.hpp>

#include <bg/physics/world.hpp>

#include <unordered_map>

namespace bg {
namespace physics {
		
class BG2E_EXPORT ColliderShape {
	friend class World;
public:
	static ColliderShape * Factory(bg::db::json::Value *, const bg::system::Path &);
	
	ColliderShape();
	virtual ~ColliderShape();
	
	virtual ColliderShape * clone() = 0;
	
	virtual bg::impl_ptr beginSimulation() = 0;
	virtual void endSimulation();
	
	virtual void draw() = 0;
	
	inline impl_ptr collisionShapeImpl() { return _collisionShapeImpl; }
	inline const impl_ptr collisionShapeImpl() const { return _collisionShapeImpl; }

	inline impl_ptr collisionObject() { return _collisionObject; }
	inline const impl_ptr collisionObject() const { return _collisionObject; }

	inline World * world() { return _world; }
	inline const World * world() const { return _world; }
	
	virtual void deserialize(bg::db::json::Value *, const bg::system::Path &) = 0;
	virtual bg::db::json::Value * serialize(const bg::system::Path &) = 0;

protected:
	bg::impl_ptr _collisionShapeImpl;

	// This objects are not null only during simulation loop.
	bg::impl_ptr _collisionObject;	
	World * _world;

	// This function is called by the World class, when the object is added to the simulation loop
	inline void setSimulationLoopObjects(impl_ptr collisionObject, World * world) { _collisionObject = collisionObject; _world = world; }
};

class BG2E_EXPORT ColliderRegistry {
public:
	static ColliderRegistry * Get() {
		if (s_singleton == nullptr)  {
			s_singleton = new ColliderRegistry();
		}
		return s_singleton;
	}
	
	void registerFactory(const std::string & name, std::function<ColliderShape *()> factory) {
		_factoryMap[name] = factory;
	}
	
	ColliderShape * instantiate(const std::string & name) {
		return _factoryMap.find(name)!=_factoryMap.end() ? _factoryMap[name]() : nullptr;
	}
	
protected:
	ColliderRegistry() {}
	virtual ~ColliderRegistry() {}
	
	static ColliderRegistry * s_singleton;
	std::unordered_map<std::string, std::function<ColliderShape*()> > _factoryMap;
};

template <class T>
class ColliderFactory {
public:
	ColliderFactory(const std::string & name) {
		ColliderRegistry::Get()->registerFactory(name, []() { return new T(); });
	}
};

struct CollisionData {
	bg::scene::Node * nodeA;
	bg::scene::Node * nodeB;
	bg::math::Vector3 posA;
	bg::math::Vector3 posB;
	int lifeTime;
	float appliedImpulse;
	bg::math::Vector3 normal;
};
	
}
}

#endif
