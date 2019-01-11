#include "mesh.hpp"

mesh::mesh::mesh(bg::base::Context * ctx) : bg::base::ContextObject(ctx)
{

}

void mesh::mesh::addVertex(vertex * v)
{
	v->setIndex(vertexCount());
	_vertex.push_back(v);
}

void mesh::mesh::addVertex(bg::math::Vector3 pos)
{
	vertex *v = new vertex(pos);
	this->addVertex(v);
}

void mesh::mesh::addEdge(edge * e)
{
	_edge.push_back(e);
}

void mesh::mesh::addEdge(vertex * v1, vertex * v2)
{
	edge *e = new edge(v1, v2);
	this->addEdge(e);
}

void mesh::mesh::addEdge(uint64_t idx1, uint64_t idx2)
{
	if (idx1 < vertexCount() && idx2 < vertexCount())
	{
		edge * e = new edge(_vertex[idx1].getPtr(), _vertex[idx2].getPtr());
		this->addEdge(e);
	}

}

void mesh::mesh::addFace(face * f)
{
	_face.push_back(f);
}

void mesh::mesh::deleteFace(face * f)
{
	for (int i = 0; i < faceCount(); i++)
	{
		if (_face[i] == f)
		{
			_face[i] = nullptr;
			_face.erase(_face.begin() + i);
		}
	}
	std::cout << "faces: " << faceCount() << std::endl;
}

void mesh::mesh::buildLines()
{
	//FALTA AGREGAR EL MODO KTRIANGLES//
	_plist = new bg::base::PolyList(context());
	_plist->setDrawMode(bg::base::PolyList::kLines);
	for (uint64_t i = 0; i < vertexCount(); i++)
	{
		_plist->addVertex(_vertex[i]->position());
		_plist->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
		_plist->addNormal(bg::math::Vector3(1.0f, 0.0f, 0.0f));
	}
	for (uint64_t i = 0; i < edgeCount(); i++)
	{
		_plist->addIndex(_edge[i]->v1()->index());
		_plist->addIndex(_edge[i]->v2()->index());
	}
	_plist->build();
}

void mesh::mesh::buildTriangles()
{
	_plist = new bg::base::PolyList(context());
	_plist->setDrawMode(bg::base::PolyList::kTriangles);
	for (uint64_t i = 0; i < faceCount(); i++)
	{
		_plist->addVertex(_face[i]->getVertex()[0]->position());
		_plist->addVertex(_face[i]->getVertex()[1]->position());
		_plist->addVertex(_face[i]->getVertex()[2]->position());

		_plist->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
		_plist->addTexCoord0(bg::math::Vector2(1.0f, 0.0f));
		_plist->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));

		_plist->addNormal(_face[i]->normal());
		_plist->addNormal(_face[i]->normal());
		_plist->addNormal(_face[i]->normal());

		_plist->addTriangle(3 * i, 3 * i + 1, 3 * i + 2);
	}
	_plist->build();

}

void mesh::mesh::clear()
{
	_vertex.clear();
	_edge.clear();
	_face.clear();
}

void mesh::mesh::load(bg::base::PolyList * body)
{
	this->clear();
	if (body->drawMode() == bg::base::PolyList::kTriangles)
	{
		std::vector<uint64_t> indexSwap;
		indexSwap.resize(body->vertexCount() / 3);

		for (uint64_t i=0; i < body->vertexCount()/3; i++)
		{
			bg::math::Vector3 vPos(bg::math::Vector3(body->vertexVector()[3 * i],
				body->vertexVector()[3 * i + 1], 
				body->vertexVector()[3 * i + 2]));

			//FINDS IF THE VERTEX WAS ALREADY ADD
			bool found = false;
			vertex *vert = nullptr;
			for (auto &v : _vertex)
			{
				if (v->position() == vPos)
				{
					found = true;
					vert = v.getPtr();
				}
			}
			if (found)
			{
				//IF VERTEX WAS ADD, reference old vertex index to this one
				indexSwap[i] = vert->index();
				//std::cout << "index: " << i << "is now: " << indexSwap[i] << std::endl;
			}
			else
			{
				//IF WASNT, ADD IT
				vertex* vert = new vertex(vPos);
				this->addVertex(vert);
				indexSwap[i] = vert->index();
			}
			
		}
		for (uint64_t i = 0; i < body->indexCount() / 3; i++)
		{
			size_t idx1 = indexSwap[body->indexVector()[3 * i]];
			size_t idx2 = indexSwap[body->indexVector()[3 * i + 1]];
			size_t idx3 = indexSwap[body->indexVector()[3 * i + 2]];

			face *f = new face(_vertex[idx1].getPtr(), _vertex[idx2].getPtr(), _vertex[idx3].getPtr());
			f->calcNormal();
			//f->setNormal(bg::math::Vector3(body->normalVector()[3 * i], body->normalVector()[3 * i + 1], body->normalVector()[3 * i + 2]));
			
			this->addFace(f);

		}
		std::cout << "Faces: " <<faceCount()<< std::endl;
		for (uint64_t i=0; i < body->indexCount()/3; i++)
		{
			size_t idx1 = indexSwap[body->indexVector()[3 * i]];
			size_t idx2 = indexSwap[body->indexVector()[3 * i + 1]];
			size_t idx3 = indexSwap[body->indexVector()[3 * i + 2]];

			bool found1 = false;
			bool found2 = false;
			bool found3 = false;

			for (auto &e : _edge)
			{
				if (e->v1()->index() == idx1 || e->v1()->index() == idx2 || e->v1()->index() == idx3)
				{
					if (e->v2()->index() == idx1 || e->v2()->index() == idx2 || e->v2()->index() == idx3)
					{
						if (e->v1()->index() == idx1 && e->v2()->index() == idx2 || e->v1()->index() == idx2 && e->v2()->index() == idx1)
						{
							found1 = true;
						}
						else if (e->v1()->index() == idx2 && e->v2()->index() == idx3 || e->v1()->index() == idx3 && e->v2()->index() == idx2)
						{
							found2 = true;
						}
						else if (e->v1()->index() == idx3 && e->v2()->index() == idx1 || e->v1()->index() == idx1 && e->v2()->index() == idx3)
						{
							found3 = true;
						}
					}
				}
			}
			if (!found1)
			{
				this->addEdge(idx1, idx2);
			}
			if (!found2)
			{
				this->addEdge(idx2, idx3);
			}
			if (!found3)
			{
				this->addEdge(idx3, idx1);
			}
		}
	}
	std::cout <<"Edges: "<< edgeCount() << std::endl;

	this->buildTriangles();
	
}

void mesh::mesh::setPosition(bg::math::Vector3 newPos)
{
	for (auto &v : _vertex)
	{
		v->setPosition(newPos);
	}
	
}

void mesh::mesh::translate(bg::math::Vector3 translation)
{
	for (auto &v : _vertex)
	{
		v->translate(translation);
	}
}

void mesh::mesh::translate(bg::math::Vector3 direction, float distance)
{
	for (auto &v : _vertex)
	{
		v->translate(direction, distance);
	}
}

void mesh::mesh::scale(bg::math::Vector3 scaleFactors)
{
	bg::math::Vector3 vec;
	for (auto &v : _vertex)
	{
		vec = v->position();
		vec.sub(this->_position);
		vec.set(vec.x()*scaleFactors.x(), vec.y()*scaleFactors.y(), vec.z()*scaleFactors.z());
		vec.add(this->_position);
		v->setPosition(vec);
	}

}

void mesh::mesh::scale(float scale)
{
	this->scale(bg::math::Vector3(scale, scale, scale));
}

void mesh::mesh::extrude(bg::math::Vector3 vec)
{
}

void mesh::mesh::extrude(face * f, bg::math::Vector3 vec)
{
	std::vector<vertex*> newVerts;
	for (auto &v : f->getVertex())
	{
		vertex *vert = new vertex(v->position());
		newVerts.push_back(vert);
		this->addVertex(vert);
	}

}

void mesh::mesh::inset(float scale)
{
}

void mesh::mesh::inset(face * f, float s)
{
}

void mesh::mesh::calcPosition()
{
	bg::math::Vector3 aux;
	for (uint64_t i = 0; i < vertexCount(); i++)
	{
		aux.add(_vertex[i]->position());
	}
	aux.scale(1/(float)vertexCount());
	_position = aux;
}

void mesh::mesh::updateNormals()
{
	for (auto &f : _face)
	{
		f->calcNormal();
	}
}

mesh::mesh::~mesh()
{
	_vertex.clear();
	_edge.clear();
	_face.clear();
}
