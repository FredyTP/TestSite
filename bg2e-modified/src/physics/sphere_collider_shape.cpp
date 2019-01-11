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


#include <bg/physics/sphere_collider_shape.hpp>

//#include <bg/manipulation/draw_gizmo_visitor.hpp>

#include "btBulletDynamicsCommon.h"

namespace bg {
namespace physics {

ColliderFactory<SphereColliderShape> sphereColliderShapeFactory("SphereCollider");
	
SphereColliderShape::SphereColliderShape()
	:_radius(1.0f)
{
	
}

SphereColliderShape::SphereColliderShape(float r)
	:_radius(r)
{
	
}
	
SphereColliderShape::SphereColliderShape(const bg::math::Scalar & r)
	:_radius(r.value())
{
	
}

SphereColliderShape::~SphereColliderShape() {
}
	
ColliderShape * SphereColliderShape::clone() {
	return new SphereColliderShape(_radius);
}

impl_ptr SphereColliderShape::beginSimulation() {
	btVector3 pos(0.0f,0.0f,0.0f);
	btScalar rad(_radius);
	_collisionShapeImpl = new btMultiSphereShape(&pos,&rad,1);
	return _collisionShapeImpl;
}

void SphereColliderShape::draw() {
}

void SphereColliderShape::deserialize(bg::db::json::Value * data, const bg::system::Path &) {
	using namespace bg::db::json;
	_radius = Value::Float((*data)["radius"]);
}

bg::db::json::Value * SphereColliderShape::serialize(const bg::system::Path &) {
	using namespace bg::db::json;
	Value * data = new Value();
	data->setValue("shape", "SphereCollider");
	data->setValue("radius", _radius);
	return data;
}

}
}
