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


#include <bg/scene/collider.hpp>

#include <bg/physics/collider_shape.hpp>

namespace bg {
namespace scene {

ComponentFactory<Collider> colliderFactory("Collider");
	
Collider::Collider()
	:_shape(nullptr)
{
}

Collider::Collider(bg::physics::ColliderShape * shape)
	:_shape(shape)
{
}

Collider::~Collider() {
	if (_shape) delete _shape;
}

Component * Collider::clone() {
	ptr<Collider> newCollider = new Collider();
	newCollider->_shape = _shape->clone();
	return newCollider.release();
}

void Collider::deserialize(bg::base::Context *, bg::db::json::Value * componentData, const bg::system::Path & path) {
	using namespace bg::db::json;
	bg::physics::ColliderShape * shape = bg::physics::ColliderShape::Factory(componentData, path);
	if (shape) {
		setShape(shape);
	}
}

bg::db::json::Value * Collider::serialize(bg::base::Context *, const bg::system::Path & path) {
	using namespace bg::db::json;
	Value * data = nullptr;
	if (shape()) {
		data = shape()->serialize(path);
	}
	else {
		data = new Value();
	}
	data->setValue("type","Collider");
	return data;
}
		
}
}