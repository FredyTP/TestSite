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


#include <bg/physics/convex_hull_collider_shape.hpp>

namespace bg {
namespace physics {

ColliderFactory<ConvexHullColliderShape> convexHullColliderShapeFactory("ConvexHullCollider");

ConvexHullColliderShape::ConvexHullColliderShape()
{
	
}
	
ConvexHullColliderShape::ConvexHullColliderShape(const bg::math::FloatVector & vertexList) {
	if (vertexList.size()%3!=0) {
		throw bg::base::InvalidParameterException("ConvexHullColliderShape: Invalid parameter supplied to vertex list creating hull collider shape: the vertex data length is not divisible by three.");
	}
	_vertexList = vertexList;
}
	
ConvexHullColliderShape::~ConvexHullColliderShape() {
}
	
ColliderShape * ConvexHullColliderShape::clone() {
	return new ConvexHullColliderShape();
}
	
void ConvexHullColliderShape::setVertexList(const bg::math::FloatVector & vertexList) {
	if (vertexList.size()%3!=0) {
		throw bg::base::InvalidParameterException("ConvexHullColliderShape: Invalid parameter supplied to vertex list creating hull collider shape: the vertex data length is not divisible by three.");
	}
	_vertexList = vertexList;
}
	
void ConvexHullColliderShape::addVertexList(const bg::math::FloatVector & vertexList) {
	if (vertexList.size()%3!=0) {
		throw bg::base::InvalidParameterException("ConvexHullColliderShape: Invalid parameter supplied to vertex list creating hull collider shape: the vertex data length is not divisible by three.");
	}
	for (auto v : vertexList) {
		_vertexList.push_back(v);
	}
}

void ConvexHullColliderShape::addVertex(const bg::math::Vector3 & v) {
	_vertexList.push_back(v.x());
	_vertexList.push_back(v.y());
	_vertexList.push_back(v.z());
}

void ConvexHullColliderShape::optimizeVertexList() {
	if (_vertexList.size()==0) {
		throw bg::base::InvalidStateException("ConvexHullColliderShape: Error initializing collider simulation. The vertex list is empty.");
	}
	else if (_vertexList.size()%3!=0) {
		throw bg::base::InvalidStateException("ConvexHullColliderShape: Error initializing collider simulation. The vertex list size is not divisible by three.");
	}
}
	
bg::impl_ptr ConvexHullColliderShape::beginSimulation() {
	if (_vertexList.size()==0) {
		throw bg::base::InvalidStateException("ConvexHullColliderShape: Error initializing collider simulation. The vertex list is empty.");
	}
	else if (_vertexList.size()%3!=0) {
		throw bg::base::InvalidStateException("ConvexHullColliderShape: Error initializing collider simulation. The vertex list size is not divisible by three.");
	}
	return nullptr;
}

void ConvexHullColliderShape::deserialize(bg::db::json::Value * data, const bg::system::Path &) {
	using namespace bg::db::json;
	Value::FloatVector((*data)["vertexData"], _vertexList);
}

bg::db::json::Value * ConvexHullColliderShape::serialize(const bg::system::Path &) {
	using namespace bg::db::json;
	Value * data = new Value();
	data->setValue("shape", "ConvexHullCollider");
	data->setValue("vertexData", _vertexList);
	return data;
}

}
}
