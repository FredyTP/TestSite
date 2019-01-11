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


#include <bg/physics/rigid_body.hpp>
#include <bg/physics/world.hpp>

#include "btBulletDynamicsCommon.h"

namespace bg {
namespace physics {

RigidBody::RigidBody()
{
	_isKinematic = false;
}

RigidBody::~RigidBody() {
}

RigidBody * RigidBody::clone() {
	RigidBody * rb = new RigidBody();
	rb->_mass = _mass;
	return rb;
}

void RigidBody::setImpl(bg::plain_ptr impl, World * world) {
	_impl = impl;
	_world = world;
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if(rb) {
		rb->setLinearFactor(btVector3(_linearFactor.x(),
			_linearFactor.y(),
			_linearFactor.z()));
		rb->setAngularFactor(btVector3(_angularFactor.x(),
			_angularFactor.y(),
			_angularFactor.z()));
		if (_isKinematic) {
			rb->setActivationState(DISABLE_DEACTIVATION);
		}
		else {
			rb->setActivationState(ACTIVE_TAG);
		}
	}
}
	
void RigidBody::endSimulation() {
	if (_impl) {
		btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
		delete rb;
	}
	setImpl(nullptr, nullptr);
	_linearVelocity.set(0.0f);
	_angularVelocity.set(0.0f);
}

void RigidBody::serialize(bg::db::json::Value * data) {
	if (data) {
		data->setValue("mass",mass());
		data->setValue("linearFactor",linearFactor());
		data->setValue("angularFactor",angularFactor());
		data->setValue("isKinematic",isKinematic());
	}
}

void RigidBody::deserialize(bg::db::json::Value * data) {
	using namespace bg::db::json;
	setMass(Value::Float((*data)["mass"],1.0));
	setLinearFactor(Value::Vec3((*data)["linearFactor"], bg::math::Vector3(1.0f)));
	setAngularFactor(Value::Vec3((*data)["angularFactor"],bg::math::Vector3(1.0f)));
	setKinematic(Value::Bool((*data)["isKinematic"],false));
}

float RigidBody::mass() {
	return _mass;
}

void RigidBody::setMass(float m) {
	_mass = m;
}

bool RigidBody::isKinematic() {
	return _isKinematic;
}

void RigidBody::setKinematic(bool k) {
	_isKinematic = k;
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if (rb) {
		if (_isKinematic) {
			rb->setActivationState(DISABLE_DEACTIVATION);
		}
		else {
			rb->setActivationState(ACTIVE_TAG);
		}
	}
}

const bg::math::Vector3 & RigidBody::linearVelocity() {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if (rb) {
		_linearVelocity.set(rb->getLinearVelocity().x(),
			rb->getLinearVelocity().y(),
			rb->getLinearVelocity().z());
	}
	return _linearVelocity;
}

const bg::math::Vector3 & RigidBody::angularVelocity() {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if(rb) {
		_angularVelocity.set(rb->getAngularVelocity().x(),
			rb->getAngularVelocity().y(), 
			rb->getAngularVelocity().z());
	}
	return _angularVelocity;
}

void RigidBody::applyForce(const bg::math::Vector3 & force, const bg::math::Vector3 & relPos) {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if (rb) {
		rb->applyForce(btVector3(force.x(), force.y(), force.z()),
					   btVector3(relPos.x(), relPos.y(), relPos.z()));
	}
}

void RigidBody::applyTorque(const bg::math::Vector3 & torque) {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if (rb) {
		rb->applyTorque(btVector3(torque.z(), torque.y(), torque.z()));
	}
}

void RigidBody::applyImpulse(const bg::math::Vector3 & impulse, const bg::math::Vector3 & relPos) {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if (rb) {
		rb->applyImpulse(btVector3(impulse.x(), impulse.y(), impulse.z()),
						 btVector3(relPos.x(), relPos.y(), relPos.z()));
	}
}

void RigidBody::applyCentralForce(const bg::math::Vector3 & force) {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if (rb) {
		rb->applyCentralForce(btVector3(force.x(), force.y(), force.z()));
	}
}

void RigidBody::applyTorqueImpulse(const bg::math::Vector3 & torque) {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if (rb) {
		rb->applyTorqueImpulse(btVector3(torque.x(), torque.y(), torque.z()));
		rb->clearForces();
	}
}

void RigidBody::applyCentralImpulse(const bg::math::Vector3 & impulse) {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if (rb) {
		rb->applyCentralImpulse(btVector3(impulse.x(), impulse.y(), impulse.z()));
	}
}

void RigidBody::setTransform(const bg::math::Matrix4 & trx) {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if(rb) {
		btTransform btMat;
		btMat.setFromOpenGLMatrix(trx.raw());
		rb->setWorldTransform(btMat);
		rb->getMotionState()->setWorldTransform(btMat);
	}
}

void RigidBody::setLinearVelocity(const bg::math::Vector3 & v) {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if(rb) {
		rb->clearForces();
		rb->setLinearVelocity(btVector3(v.x(), v.y(), v.z()));
	}
}

void RigidBody::setAngularVelocity(const bg::math::Vector3 & v) {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if(rb) {
		rb->clearForces();
		rb->setAngularVelocity(btVector3(v.x(), v.y(), v.z()));
	}
}

void RigidBody::addLinearVelocity(const bg::math::Vector3 & v) {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if(rb) {
		btVector3 vel = rb->getLinearVelocity();
		vel.setValue(vel.x() + v.x(), vel.y() + v.y(), vel.z() + v.z());
		rb->clearForces();
		rb->setLinearVelocity(vel);
	}
}

void RigidBody::addAngularVelocity(const bg::math::Vector3 & v) {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if(rb) {
		btVector3 vel = rb->getAngularVelocity();
		vel.setValue(vel.x() + v.x(), vel.y() + v.y(), vel.z() + v.z());
		rb->clearForces();
		rb->setAngularVelocity(vel);
	}
}

const bg::math::Vector3 & RigidBody::linearFactor() {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if(rb) {
		btVector3 factor = rb->getLinearFactor();
		_linearFactor.set(factor.x(), factor.y(), factor.z());
	}
	return _linearFactor;
}

const bg::math::Vector3 & RigidBody::angularFactor() {
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if(rb) {
		btVector3 factor = rb->getAngularFactor();
		_angularFactor.set(factor.x(), factor.y(), factor.z());
	}
	return _angularFactor;
}

void RigidBody::setLinearFactor(const bg::math::Vector3 & k) {
	_linearFactor = k;
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if(rb) {
		rb->setLinearFactor(btVector3(_linearFactor.x(),
			_linearFactor.y(),
			_linearFactor.z()));
	}
}

void RigidBody::setAngularFactor(const bg::math::Vector3 & k) {
	_angularFactor = k;
	btRigidBody * rb = bg::native_cast<btRigidBody*>(_impl);
	if(rb) {
		rb->setAngularFactor(btVector3(_angularFactor.x(),
			_angularFactor.y(),
			_angularFactor.z()));
	}
}


}
}
