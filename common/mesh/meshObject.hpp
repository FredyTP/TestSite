#pragma once

#include <bg/bg2e.hpp>

namespace mesh
{
	class meshObject : public bg::base::ReferencedPointer
	{
	public:
		meshObject();
		virtual void setPosition(bg::math::Vector3 newPos) = 0;
		virtual void translate(bg::math::Vector3 translation) = 0;
		virtual void translate(bg::math::Vector3 direction, float distance) = 0;
		virtual void scale(bg::math::Vector3 scaleFactors) = 0;
		virtual void scale(float scale) = 0;

		virtual void extrude(bg::math::Vector3 vec) = 0;
		virtual void inset(float scale) = 0;
		virtual void calcPosition() = 0;
		
		inline bg::math::Vector3 position() { return _position; }

	protected:
		virtual ~meshObject();
		bg::math::Vector3 _position;

	};


}