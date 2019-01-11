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

#ifndef _bg2e_physics_rigid_body_hpp_
#define _bg2e_physics_rigid_body_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/physics/world.hpp>
#include <bg/db/json/value.hpp>

namespace bg {
namespace physics {

class BG2E_EXPORT RigidBody : public bg::base::ReferencedPointer {
public:
	RigidBody();

	RigidBody * clone();

	void setImpl(bg::plain_ptr impl, World * world);
	void endSimulation();
	
	inline bg::plain_ptr impl() { return _impl; }
	template <class T>
	T * impl() { return dynamic_cast<T*>(_impl); }
	inline World * world() { return _world; }

	inline bool simulationRunning() const { return _impl!=nullptr; }

	void serialize(bg::db::json::Value *);
	void deserialize(bg::db::json::Value *);

	float mass();
	void setMass(float m);
	bool isKinematic();
	void setKinematic(bool k);

	const bg::math::Vector3 & linearVelocity();
	const bg::math::Vector3 & angularVelocity();

	void applyForce(const bg::math::Vector3 & force, const bg::math::Vector3 & relPos);
	void applyTorque(const bg::math::Vector3 & torque);
	void applyImpulse(const bg::math::Vector3 & impulse, const bg::math::Vector3 & relPos);
	void applyCentralForce(const bg::math::Vector3 & force);
	void applyTorqueImpulse(const bg::math::Vector3 & torque);
	void applyCentralImpulse(const bg::math::Vector3 & impulse);

	void setTransform(const bg::math::Matrix4 & trx);
	

	void setLinearVelocity(const bg::math::Vector3 & v);
	void setAngularVelocity(const bg::math::Vector3 & v);

	void addLinearVelocity(const bg::math::Vector3 & v);
	void addAngularVelocity(const bg::math::Vector3 & v);

	const bg::math::Vector3 & linearFactor();
	const bg::math::Vector3 & angularFactor();

	void setLinearFactor(const bg::math::Vector3 &);
	void setAngularFactor(const bg::math::Vector3 &);

protected:
	virtual ~RigidBody();

	bg::plain_ptr _impl = nullptr;
	World * _world = nullptr;

	float _mass;
	bool _isKinematic;

	// This values are catch from the physics engine and only have valid values during simulation loop
	bg::math::Vector3 _linearVelocity;
	bg::math::Vector3 _angularVelocity;

	// This values are shared between the physics engine and the graphic engine, and are always valid.
	bg::math::Vector3 _linearFactor = bg::math::Vector3(1.0f);
	bg::math::Vector3 _angularFactor = bg::math::Vector3(1.0f);
};

}
}
#endif
