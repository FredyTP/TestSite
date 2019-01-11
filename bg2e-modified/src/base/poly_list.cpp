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


#include <bg/base/poly_list.hpp>

#include <bg/engine.hpp>

#include <unordered_map>

namespace bg {
namespace base {

bool PolyList::s_standarizeVectors = true;

PolyList::PolyList(Context * ctx)
	:ContextObject(ctx)
	,_name("")
	,_materialName("")
	,_drawMode(kTriangles)
	,_visible(true)
{
	_impl = bg::Engine::Get()->createPolyList(this);
}

PolyList::PolyList(Context * ctx, const PolyList * clone)
	:ContextObject(ctx)
	,_name("")
	,_materialName("")
	,_drawMode(kTriangles)
	,_visible(true)
{
	_impl = bg::Engine::Get()->createPolyList(this);
	
	bg::math::FloatVector::const_iterator vit;
	bg::math::FloatVector::const_iterator nit;
	bg::math::FloatVector::const_iterator t0it;
	bg::math::FloatVector::const_iterator t1it;
	bg::math::FloatVector::const_iterator t2it;
	bg::math::UIntVector::const_iterator iit;
	
	_name = clone->_name;
	_materialName = clone->_materialName;
	_drawMode = clone->_drawMode;
	
	for (vit=clone->_vertex.begin(); vit!=clone->_vertex.end(); ++vit) {
		_vertex.push_back(*vit);
	}
	for (nit=clone->_normal.begin(); nit!=clone->_normal.end(); ++nit) {
		_normal.push_back(*nit);
	}
	for (t0it=clone->_texCoord0.begin(); t0it!=clone->_texCoord0.end(); ++t0it) {
		_texCoord0.push_back(*t0it);
	}
	for (t1it=clone->_texCoord1.begin(); t1it!=clone->_texCoord1.end(); ++t1it) {
		_texCoord1.push_back(*t1it);
	}
	for (t2it=clone->_texCoord2.begin(); t2it!=clone->_texCoord2.end(); ++t2it) {
		_texCoord2.push_back(*t2it);
	}
	for (iit=clone->_index.begin(); iit!=clone->_index.end(); ++iit) {
		_index.push_back(*iit);
	}
	build();
}

PolyList::~PolyList() {
	destroy();
}

void PolyList::addVertex(const bg::math::Vector3 & v) {
	_vertex.push_back(v.x());
	_vertex.push_back(v.y());
	_vertex.push_back(v.z());
}

void PolyList::addNormal(const bg::math::Vector3 & n) {
	_normal.push_back(n.x());
	_normal.push_back(n.y());
	_normal.push_back(n.z());
}

void PolyList::addTexCoord0(const bg::math::Vector2 & tc) {
	_texCoord0.push_back(tc.x());
	_texCoord0.push_back(tc.y());
}

void PolyList::addTexCoord1(const bg::math::Vector2 & tc) {
	_texCoord1.push_back(tc.x());
	_texCoord1.push_back(tc.y());
}

void PolyList::addTexCoord2(const bg::math::Vector2 & tc) {
	_texCoord2.push_back(tc.x());
	_texCoord2.push_back(tc.y());
}

void PolyList::addColor(const bg::math::Color &c) {
	_color.push_back(c.r());
	_color.push_back(c.g());
	_color.push_back(c.b());
	_color.push_back(c.a());
}

void PolyList::addIndex(unsigned int i) {
	_index.push_back(i);
}

void PolyList::addTriangle(unsigned int v0, unsigned int v1, unsigned int v2) {
	addIndex(v0); addIndex(v1); addIndex(v2);
}

void PolyList::addVertexVector(const float * v, size_t size) {
	_vertex.reserve(size);
	for (int i=0; i<size; ++i) {
		_vertex.push_back(v[i]);
	}
}

void PolyList::addNormalVector(const float * v, size_t size) {
	for (int i=0; i<size; ++i) {
		_normal.push_back(v[i]);
	}
}

void PolyList::addTexCoord0Vector(const float * v, size_t size) {
	for (int i=0; i<size; ++i) {
		_texCoord0.push_back(v[i]);
	}
}

void PolyList::addTexCoord1Vector(const float * v, size_t size) {
	for (int i=0; i<size; ++i) {
		_texCoord1.push_back(v[i]);
	}
}

void PolyList::addTexCoord2Vector(const float * v, size_t size) {
	for (int i=0; i<size; ++i) {
		_texCoord2.push_back(v[i]);
	}
}

void PolyList::addColorVector(const float *v, size_t size) {
	for (int i=0; i<size; ++i) {
		_color.push_back(v[i]);
	}
}

void PolyList::addIndexVector(const unsigned int * v, size_t size) {
	for (int i=0; i<size; ++i) {
		_index.push_back(v[i]);
	}
}

void PolyList::destroy() {
	_vertex.clear();
	_normal.clear();
	_texCoord0.clear();
	_texCoord1.clear();
	_texCoord2.clear();
	_color.clear();
	_index.clear();
	_tangent.clear();
	
	
	if (_impl.valid()) {
		_impl->destroy();
		_impl = nullptr;
	}
}

void PolyList::calculateTangent() {
	_tangent.clear();
	if (_normal.size() == 0) return;	// No normals
	if (_texCoord0.size() == 0 || _vertex.size() == 0) return;
	
	std::unordered_map<int, bg::math::Vector3> generatedIndexes;
	if (_index.size() % 3 == 0) {
		for (int i = 0; i<_index.size() - 2; i += 3) {
			int v0i = _index[i] * 3;
			int v1i = _index[i + 1] * 3;
			int v2i = _index[i + 2] * 3;
			
			int t0i = _index[i] * 2;
			int t1i = _index[i + 1] * 2;
			int t2i = _index[i + 2] * 2;
			
			bg::math::Vector3 v0(_vertex[v0i], _vertex[v0i + 1], _vertex[v0i + 2]);
			bg::math::Vector3 v1(_vertex[v1i], _vertex[v1i + 1], _vertex[v1i + 2]);
			bg::math::Vector3 v2(_vertex[v2i], _vertex[v2i + 1], _vertex[v2i + 2]);
			
			bg::math::Vector2 t0(_texCoord0[t0i], _texCoord0[t0i + 1]);
			bg::math::Vector2 t1(_texCoord0[t1i], _texCoord0[t1i + 1]);
			bg::math::Vector2 t2(_texCoord0[t2i], _texCoord0[t2i + 1]);
			
			bg::math::Vector3 edge1(v1);
			edge1.sub(v0);
			bg::math::Vector3 edge2(v2);
			edge2.sub(v0);
			
			float deltaU1 = t1.x() - t0.x();
			float deltaV1 = t1.y() - t0.y();
			float deltaU2 = t2.x() - t0.x();
			float deltaV2 = t2.y() - t0.y();
			
			float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);
			
			bg::math::Vector3 tangent(f * (deltaV2 * edge1.x() - deltaV1 * edge2.x()),
							f * (deltaV2 * edge1.y() - deltaV1 * edge2.y()),
							f * (deltaV2 * edge1.z() - deltaV1 * edge2.z()));
			tangent.normalize();
			
			if (generatedIndexes.find(v0i) == generatedIndexes.end()) {
				_tangent.push_back(tangent.x());
				_tangent.push_back(tangent.y());
				_tangent.push_back(tangent.z());
				generatedIndexes[v0i] = tangent;
			}
			if (generatedIndexes.find(v1i) == generatedIndexes.end()) {
				_tangent.push_back(tangent.x());
				_tangent.push_back(tangent.y());
				_tangent.push_back(tangent.z());
				generatedIndexes[v1i] = tangent;
			}
			if (generatedIndexes.find(v2i) == generatedIndexes.end()) {
				_tangent.push_back(tangent.x());
				_tangent.push_back(tangent.y());
				_tangent.push_back(tangent.z());
				generatedIndexes[v2i] = tangent;
			}
		}
	}
	else if (_index.size() % 4 == 0) {
		for (int i = 0; i<_index.size() - 3; i += 4) {
			int v0i = _index[i] * 3;
			int v1i = _index[i + 1] * 3;
			int v2i = _index[i + 2] * 3;
			
			int t0i = _index[i] * 2;
			int t1i = _index[i + 1] * 2;
			int t2i = _index[i + 2] * 2;
			
			bg::math::Vector3 v0(_vertex[v0i], _vertex[v0i + 1], _vertex[v0i + 2]);
			bg::math::Vector3 v1(_vertex[v1i], _vertex[v1i + 1], _vertex[v1i + 2]);
			bg::math::Vector3 v2(_vertex[v2i], _vertex[v2i + 1], _vertex[v2i + 2]);
			
			bg::math::Vector2 t0(_texCoord0[t0i], _texCoord0[t0i + 1]);
			bg::math::Vector2 t1(_texCoord0[t1i], _texCoord0[t1i + 1]);
			bg::math::Vector2 t2(_texCoord0[t2i], _texCoord0[t2i + 1]);
			
			bg::math::Vector3 edge1(v1);
			edge1.sub(v0);
			bg::math::Vector3 edge2(v2);
			edge2.sub(v0);
			
			float deltaU1 = t1.x() - t0.x();
			float deltaV1 = t1.y() - t0.y();
			float deltaU2 = t2.x() - t0.x();
			float deltaV2 = t2.y() - t0.y();
			
			float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);
			
			bg::math::Vector3 tangent(f * (deltaV2 * edge1.x() - deltaV1 * edge2.x()),
							f * (deltaV2 * edge1.y() - deltaV1 * edge2.y()),
							f * (deltaV2 * edge1.z() - deltaV1 * edge2.z()));
			tangent.normalize();
			
			_tangent.push_back(tangent.x());
			_tangent.push_back(tangent.y());
			_tangent.push_back(tangent.z());
			
			_tangent.push_back(tangent.x());
			_tangent.push_back(tangent.y());
			_tangent.push_back(tangent.z());
			
			_tangent.push_back(tangent.x());
			_tangent.push_back(tangent.y());
			_tangent.push_back(tangent.z());
			
			_tangent.push_back(tangent.x());
			_tangent.push_back(tangent.y());
			_tangent.push_back(tangent.z());
		}
	}
}


void PolyList::build(bool preventStandarizeVectors) {
    if (_impl.isNull()) {
        _impl = bg::Engine::Get()->createPolyList(this);
    }
	if (!preventStandarizeVectors) {
		standarizeVectors();
	}
	calculateTangent();
	_impl->build();
}

void PolyList::draw() {
	_impl->draw();
}

void PolyList::standarizeVectors() {
	if (s_standarizeVectors) {
		bool addNormals = _normal.size() == 0;
		bool addT0 = _texCoord0.size() == 0;
		bool addT1 = _texCoord1.size() == 0;
		bool addColor = _color.size() == 0;

		for (size_t i = 0; i < _vertex.size(); i+=3) {
			if (addNormals) {
				_normal.push_back(0.0f);
				_normal.push_back(1.0f);
				_normal.push_back(0.0f);
			}
			if (addT0) {
				_texCoord0.push_back(0.0f);
				_texCoord0.push_back(0.0f);
			}
			if (addT1) {
				_texCoord1.push_back(0.0f);
				_texCoord1.push_back(0.0f);
			}
			if (addColor) {
				_color.push_back(1.0f);
				_color.push_back(1.0f);
				_color.push_back(1.0f);
				_color.push_back(1.0f);
			}
		}

		if (_texCoord2.size() > 0) {
			_texCoord2.clear();
		}

		if (!addNormals) {
			for (size_t i = 0; i < _normal.size(); i += 3) {
				auto n = bg::math::Vector3(_normal[i], _normal[i + 1], _normal[i + 2]);
				n.normalize();
				_normal[i] = n.x();
				_normal[i + 1] = n.y();
				_normal[i + 2] = n.z();
			}
		}
	}
}

}
}
