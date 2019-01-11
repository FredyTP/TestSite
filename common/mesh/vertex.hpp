#pragma once
#include <bg/bg2e.hpp>
#include <mesh/meshObject.hpp>
namespace mesh
{
	class vertex : public meshObject
	{
	public:
		vertex();
		vertex(bg::math::Vector3 pos);
		virtual void setPosition(bg::math::Vector3 newPos);
		virtual void translate(bg::math::Vector3 translation);
		virtual void translate(bg::math::Vector3 direction, float distance);

		virtual void scale(bg::math::Vector3 scaleFactors);
		virtual void scale(float scale);

		virtual void extrude(bg::math::Vector3 vec);
		virtual void inset(float scale);

		virtual void calcPosition();

		inline bg::math::Vector3 position() { return _position; }

		uint64_t index() { return _index; }
		void setIndex(uint64_t idx) { _index = idx; }

	protected:
		virtual ~vertex();
		uint64_t _index;
	};


}