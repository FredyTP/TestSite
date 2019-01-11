#pragma once

#include <mesh/meshObject.hpp>
#include <mesh/vertex.hpp>
#include <bg/bg2e.hpp>
#include <vector>

namespace mesh
{
	class face : public meshObject
	{
	public:
		face();
		template <class... Args> 
		face(Args... args)
		{
			for (auto &v : { args... })
			{
				_vertex.push_back(v);
			}
			_sides = _vertex.size();
		}

		virtual void setPosition(bg::math::Vector3 newPos);
		virtual void translate(bg::math::Vector3 translation);
		virtual void translate(bg::math::Vector3 direction, float distance);

		virtual void scale(bg::math::Vector3 scaleFactors);
		virtual void scale(float scale);

		virtual void extrude(bg::math::Vector3 vec);
		virtual void inset(float scale);


		virtual void calcPosition();
		virtual void calcNormal();
		virtual void setNormal(bg::math::Vector3 norm) { _normal = norm; }



		inline bg::math::Vector3 normal() { return _normal; }

		inline std::vector<bg::ptr<vertex> > &getVertex() { return _vertex; }
	protected:
		virtual ~face();
		std::vector<bg::ptr<vertex> > _vertex;
		unsigned int _sides;
		bg::math::Vector3 _normal;
	};

}