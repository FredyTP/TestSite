#pragma once

#include <bg/bg2e.hpp>

#include <mesh/meshObject.hpp>
#include <mesh/vertex.hpp>
#include <mesh/edge.hpp>
#include <mesh/face.hpp>

#include <vector>


namespace mesh
{

	class mesh : public bg::base::ContextObject , public meshObject
	{
	public:
		mesh(bg::base::Context* ctx);
		void addVertex(vertex *v);
		void addVertex(bg::math::Vector3 pos);

		void addEdge(edge *e);
		void addEdge(vertex *v1, vertex *v2);
		void addEdge(uint64_t idx1, uint64_t idx2);

		void addFace(face *f);

		void deleteFace(face *f);



		vertex* getVertex(uint64_t idx) { return _vertex[idx].getPtr(); }
		edge* getEdge(uint64_t idx) { return _edge[idx].getPtr(); }
		face* getFace(uint64_t idx) { return _face[idx].getPtr(); }
		void buildLines();
		void buildTriangles();
		bg::base::PolyList* plist() { return _plist.getPtr(); }

		virtual void clear();

		virtual void load(bg::base::PolyList* body);

		size_t vertexCount() { return _vertex.size(); }
		size_t edgeCount() { return _edge.size(); }
		size_t faceCount() { return _face.size(); }



		//INHERITE FUNCTIONS

		virtual void setPosition(bg::math::Vector3 newPos);
		virtual void translate(bg::math::Vector3 translation);
		virtual void translate(bg::math::Vector3 direction, float distance);

		virtual void scale(bg::math::Vector3 scaleFactors);
		virtual void scale(float scale);

		virtual void extrude(bg::math::Vector3 vec);
		virtual void extrude(face *f, bg::math::Vector3 vec);

		virtual void inset(float scale);
		virtual void inset(face *f, float s);

		virtual void calcPosition();
		virtual void updateNormals();
		
	protected:
		virtual ~mesh();

		std::vector<bg::ptr<vertex> > _vertex;
		std::vector<bg::ptr<edge> > _edge;
		std::vector<bg::ptr<face> > _face;

		bg::ptr<bg::base::PolyList> _plist;

	};
}