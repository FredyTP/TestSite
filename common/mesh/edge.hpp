#pragma once

#include <bg/bg2e.hpp>
#include <mesh/vertex.hpp>
#include <mesh/meshObject.hpp>
#include <vector>

namespace mesh
{
	class edge : public meshObject
	{
	public:
		edge();
		edge(vertex *v1, vertex *v2);
		
		virtual void setPosition(bg::math::Vector3 newPos);
		virtual void translate(bg::math::Vector3 translation);
		virtual void translate(bg::math::Vector3 direction, float distance);

		virtual void scale(bg::math::Vector3 scaleFactors);
		virtual void scale(float scale);

		virtual void extrude(bg::math::Vector3 vec);
		virtual void inset(float scale);

		virtual void calcPosition();

		void setVertex(vertex *v1,vertex *v2);
		inline vertex * v1() { return _v1.getPtr(); }
		inline vertex * v2() { return _v2.getPtr(); }



	protected:
		virtual ~edge();

		bg::ptr<vertex> _v1;
		bg::ptr<vertex> _v2;
	};
}