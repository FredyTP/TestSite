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


#include <bg/physics/collider_shape.hpp>

#include "btBulletDynamicsCommon.h"

namespace bg {
namespace physics {

ColliderRegistry * ColliderRegistry::s_singleton = nullptr;

ColliderShape * ColliderShape::Factory(bg::db::json::Value * data, const bg::system::Path & path) {
	using namespace bg::db::json;
	std::string type = Value::String((*data)["shape"]);
	ColliderShape * collider = ColliderRegistry::Get()->instantiate(type);
	if (collider) {
		collider->deserialize(data, path);
	}
	return collider;
}

ColliderShape::ColliderShape()
	:_collisionShapeImpl(nullptr)
	,_collisionObject(nullptr)
	,_world(nullptr)
{
	
}

ColliderShape::~ColliderShape() {
	endSimulation();
}
	
void ColliderShape::endSimulation() {
	if (_collisionShapeImpl!=nullptr) {
		btCollisionShape * shape = bg::impl_cast<btCollisionShape*>(_collisionShapeImpl);
		delete shape;
		_collisionShapeImpl = nullptr;
		_collisionObject = nullptr;
		_world = nullptr;
	}
}
	

}
}
