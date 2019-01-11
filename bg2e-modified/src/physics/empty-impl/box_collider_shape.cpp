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


#include <bg/physics/box_collider_shape.hpp>

namespace bg {
namespace physics {

ColliderFactory<BoxColliderShape> boxColliderShapeFactory("BoxCollider");

BoxColliderShape::BoxColliderShape()
	:_size(1.0f)
{
	
}
	
BoxColliderShape::BoxColliderShape(float width, float height, float depth)
	:_size(width, height, depth)
{
	
}

BoxColliderShape::BoxColliderShape(float width, float height, const bg::math::Scalar & depth)
	:_size(width, height, depth.value())
{
}
	
BoxColliderShape::BoxColliderShape(float width, const bg::math::Scalar & height, float depth)
	:_size(width, height.value(), depth)
{
	
}

BoxColliderShape::BoxColliderShape(float width, const bg::math::Scalar & height, const bg::math::Scalar & depth)
	:_size(width, height.value(), depth.value())
{
	
}

BoxColliderShape::BoxColliderShape(const bg::math::Scalar & width, float height, float depth)
	:_size(width.value(), height, depth)
{
	
}

BoxColliderShape::BoxColliderShape(const bg::math::Scalar & width, float height, const bg::math::Scalar & depth)
	:_size(width.value(), height, depth.value())
{
	
}

BoxColliderShape::BoxColliderShape(const bg::math::Scalar & width, const bg::math::Scalar & height, float depth)
	:_size(width.value(), height.value(), depth)
{
	
}

BoxColliderShape::BoxColliderShape(const bg::math::Scalar & width, const bg::math::Scalar & height, const bg::math::Scalar & depth)
	:_size(width.value(), height.value(), depth.value())
{
	
}
	
BoxColliderShape::BoxColliderShape(const bg::math::Vector3 & size)
	:_size(size)
{
	
}

BoxColliderShape::~BoxColliderShape() {
}
	
ColliderShape * BoxColliderShape::clone() {
	return new BoxColliderShape(_size);
}

bg::impl_ptr BoxColliderShape::beginSimulation() {
	return nullptr;
}

void BoxColliderShape::deserialize(bg::db::json::Value * data, const bg::system::Path &) {
	using namespace bg::db::json;
	_size = Value::Vec3((*data)["size"]);
}

bg::db::json::Value * BoxColliderShape::serialize(const bg::system::Path &) {
	using namespace bg::db::json;
	Value * data = new Value();
	data->setValue("shape", "BoxCollider");
	data->setValue("size", _size);
	return data;
}

}
}
