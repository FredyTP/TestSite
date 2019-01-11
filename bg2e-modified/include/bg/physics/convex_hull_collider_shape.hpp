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

#ifndef _bg2e_physics_convex_hull_collider_shape_hpp_
#define _bg2e_physics_convex_hull_collider_shape_hpp_

#include <bg/physics/collider_shape.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/math/vector.hpp>

namespace bg {
namespace physics {

class BG2E_EXPORT ConvexHullColliderShape : public ColliderShape {
public:
	ConvexHullColliderShape();
	ConvexHullColliderShape(const std::vector<float> & vertexList);
	virtual ~ConvexHullColliderShape();
	
	virtual ColliderShape * clone();
    
	void setVertexList(const bg::math::FloatVector & vertexList);
	void addVertexList(const bg::math::FloatVector & vertexList);
	inline const bg::math::FloatVector & vertexList() const { return _vertexList; }
	void addVertex(const bg::math::Vector3 & v);
	
	void optimizeVertexList();
	
	virtual bg::impl_ptr beginSimulation();

	virtual void draw() {}
	
	virtual void deserialize(bg::db::json::Value * , const bg::system::Path &);
	virtual bg::db::json::Value * serialize(const bg::system::Path &);
	
protected:
	bg::math::FloatVector _vertexList;
};

}
}

#endif
