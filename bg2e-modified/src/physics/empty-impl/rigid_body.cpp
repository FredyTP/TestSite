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
}
	
void RigidBody::endSimulation() {
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
}

const bg::math::Vector3 & RigidBody::linearVelocity() {
	static bg::math::Vector3 fooValue;
	return fooValue;
}

const bg::math::Vector3 & RigidBody::angularVelocity() {
	static bg::math::Vector3 fooValue;
	return fooValue;
}

void RigidBody::applyForce(const bg::math::Vector3 &, const bg::math::Vector3 &) {
}

void RigidBody::applyTorque(const bg::math::Vector3 &) {
}

void RigidBody::applyImpulse(const bg::math::Vector3 &, const bg::math::Vector3 &) {
}

void RigidBody::applyCentralForce(const bg::math::Vector3 &) {
}

void RigidBody::applyTorqueImpulse(const bg::math::Vector3 &) {
}

void RigidBody::applyCentralImpulse(const bg::math::Vector3 &) {
}

void RigidBody::setLinearVelocity(const bg::math::Vector3 &) {
}

void RigidBody::setAngularVelocity(const bg::math::Vector3 &) {
}

void RigidBody::addLinearVelocity(const bg::math::Vector3 &) {
}

void RigidBody::addAngularVelocity(const bg::math::Vector3 &) {
}

const bg::math::Vector3 & RigidBody::linearFactor() {
	static bg::math::Vector3 fooValue;
	return fooValue;
}

const bg::math::Vector3 & RigidBody::angularFactor() {
	static bg::math::Vector3 fooValue;
	return fooValue;
}

void RigidBody::setLinearFactor(const bg::math::Vector3 &) {
}

void RigidBody::setAngularFactor(const bg::math::Vector3 &) {
}


}
}
