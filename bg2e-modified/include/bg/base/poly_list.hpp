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


#ifndef _bg2e_base_poly_list_hpp_
#define _bg2e_base_poly_list_hpp_

#include <bg/base/context_object.hpp>

#include <bg/math/vector.hpp>
#include <bg/math/matrix.hpp>

#include <bg/engine/poly_list_impl.hpp>

#include <bg/base/exception.hpp>

namespace bg {
namespace base {
	
class BG2E_EXPORT PolyList : public ContextObject, public ReferencedPointer {
public:
	enum VertexBufferType {
		kVertex = 0,
		kNormal = 1,
		kTexCoord0 = 2,
		kTexCoord1 = 3,
		kTexCoord2 = 4,
		kTangent = 10,
		kColor = 11,
		kIndex = 100
	};
	
	enum DrawMode {
		kTriangles = 1,
		kLines = 2,
		kTessellation = 3
	};
	
	PolyList(Context * ctx);
	PolyList(Context * ctx, const PolyList * clone);
	
	void setName(const std::string & name) { _name = name; }
	const std::string & name() const { return _name; }
	std::string & name() { return _name; }
	void setGroupName(const std::string & name) { _groupName = name; }
	const std::string & groupName() const { return _groupName; }
	std::string & groupName() { return _groupName; }
	void setMaterialName(const std::string & name) { _materialName = name; }
	const std::string & materialName() const { return _materialName; }
	std::string & materialName() { return _materialName; }
	
	void addVertex(const bg::math::Vector3 & v);
	void addNormal(const bg::math::Vector3 & n);
	void addTexCoord0(const bg::math::Vector2 & tc);
	void addTexCoord1(const bg::math::Vector2 & tc);
	void addTexCoord2(const bg::math::Vector2 & tc);
	void addColor(const bg::math::Color & c);
	void addIndex(unsigned int i);
	void addTriangle(unsigned int v0, unsigned int v1, unsigned int v2);
	unsigned long numberOfIndices() const { return (unsigned long)_index.size(); }
	
	// 2D version of addVertex
	inline void addVertex(const bg::math::Vector2 & v) { addVertex(bg::math::Vector3(v.x(), v.y(), 0.0f)); }
	inline void addVertex(const bg::math::Vector2i & v) { addVertex(bg::math::Vector3(static_cast<float>(v.x()), static_cast<float>(v.y()), 0.0f)); }
	
	
	void addVertexVector(const float * v, size_t size);
	void addNormalVector(const float * v, size_t size);
	void addTexCoord0Vector(const float * v, size_t size);
	void addTexCoord1Vector(const float * v, size_t size);
	void addTexCoord2Vector(const float * v, size_t size);
	void addColorVector(const float * v, size_t size);
	void addIndexVector(const unsigned int * v, size_t size);
	inline void addVertexVector(const std::vector<float> & v) { addVertexVector(v.data(), v.size()); }
	inline void addNormalVector(const std::vector<float> & v) { addNormalVector(v.data(), v.size()); }
	inline void addTexCoord0Vector(const std::vector<float> & v) { addTexCoord0Vector(v.data(), v.size()); }
	inline void addTexCoord1Vector(const std::vector<float> & v) { addTexCoord1Vector(v.data(), v.size()); }
	inline void addTexCoord2Vector(const std::vector<float> & v) { addTexCoord2Vector(v.data(), v.size()); }
	inline void addColorVector(const std::vector<float> & v) { addColorVector(v.data(), v.size()); }
	inline void addIndexVector(const std::vector<unsigned int> & v) { addIndexVector(v.data(), v.size()); }
    
    inline void setData(const std::vector<float> & vertex, const std::vector<float> & norm, const std::vector<float> & texCoord, const std::vector<unsigned int> &index) {
        destroy();
        addVertexVector(vertex.data(), vertex.size());
        addNormalVector(norm.data(), norm.size());
        addTexCoord0Vector(texCoord.data(), texCoord.size());
        addIndexVector(index.data(), index.size());
    }
    
    inline void setData(const std::vector<float> & vertex, const std::vector<float> & norm, const std::vector<float> & texCoord0, const std::vector<float> & texCoord1, const std::vector<unsigned int> &index) {
        destroy();
        addVertexVector(vertex.data(), vertex.size());
        addNormalVector(norm.data(), norm.size());
        addTexCoord0Vector(texCoord0.data(), texCoord0.size());
        addTexCoord1Vector(texCoord1.data(), texCoord1.size());
        addIndexVector(index.data(), index.size());
    }
	
	void build(bool preventStandarizeVectors = false);
	void destroy();
	
	inline float * vertexPointer() { return &_vertex[0]; }
	inline const float * vertexPointer() const { return &_vertex[0]; }
	inline float * normalPointer() { return &_normal[0]; }
	inline const float * normalPointer() const { return &_normal[0]; }
	inline float * texCoord0Pointer() { return &_texCoord0[0]; }
	inline const float * texCoord0Pointer() const { return &_texCoord0[0]; }
	inline float * texCoord1Pointer() { return &_texCoord1[0]; }
	inline const float * texCoord1Pointer() const { return &_texCoord1[0]; }
	inline float * texCoord2Pointer() { return &_texCoord2[0]; }
	inline const float * texCoord2Pointer() const { return &_texCoord2[0]; }
	inline float * colorPointer() { return &_color[0]; }
	inline const  float * colorPointer() const { return &_color[0]; }
	inline float * tangentPointer() { return &_tangent[0]; }
	inline const float * tangentPointer() const { return &_tangent[0]; }
	inline unsigned int * indexPointer() { return &_index[0]; }
	inline const unsigned int * indexPointer() const { return &_index[0]; }
	
	
	inline bg::math::FloatVector & vertexVector() { return _vertex; }
	inline const bg::math::FloatVector & vertexVector() const { return _vertex; }
	inline bg::math::FloatVector & normalVector() { return _normal; }
	inline const bg::math::FloatVector & normalVector() const { return _normal; }
	inline bg::math::FloatVector & texCoord0Vector() { return _texCoord0; }
	inline const bg::math::FloatVector & texCoord0Vector() const { return _texCoord0; }
	inline bg::math::FloatVector & texCoord1Vector() { return _texCoord1; }
	inline const bg::math::FloatVector & texCoord1Vector() const { return _texCoord1; }
	inline bg::math::FloatVector & texCoord2Vector() { return _texCoord2; }
	inline const bg::math::FloatVector & texCoord2Vector() const { return _texCoord2; }
	inline bg::math::FloatVector & colorVector() { return _color; }
	inline const bg::math::FloatVector & colorVector() const { return _color; }
	inline bg::math::FloatVector & tangentVector() { return _tangent; }
	inline const bg::math::FloatVector & tangentVector() const { return _tangent; }
	inline bg::math::UIntVector & indexVector() { return _index; }
	inline const bg::math::UIntVector & indexVector() const { return _index; }
	
	inline unsigned int vertexCount() const { return static_cast<unsigned int>(_vertex.size()); }
	inline unsigned int normalCount() const { return static_cast<unsigned int>(_normal.size()); }
	inline unsigned int texCoord0Count() const { return static_cast<unsigned int>(_texCoord0.size()); }
	inline unsigned int texCoord1Count() const { return static_cast<unsigned int>(_texCoord1.size()); }
	inline unsigned int texCoord2Count() const { return static_cast<unsigned int>(_texCoord2.size()); }
	inline unsigned int colorCount() const { return static_cast<unsigned int>(_color.size()); }
	inline unsigned int tangentCount() const { return static_cast<unsigned int>(_tangent.size()); }
	inline unsigned int indexCount() const { return static_cast<unsigned int>(_index.size()); }
	
	inline void setVisible(bool visible = true) { _visible = visible; }
	inline bool visible() const { return _visible; }
	inline void show() { _visible = true; }
	inline void hide() { _visible = false; }
	
	inline void setDrawMode(DrawMode mode) { _drawMode = mode; }
	inline DrawMode drawMode() const { return _drawMode; }
	
	void draw();
	
	template <class T>
	T * impl() { return dynamic_cast<T*>(_impl.getPtr()); }

protected:
	virtual ~PolyList();
	
	std::string _name;
	std::string _groupName;
	std::string _materialName;
	bg::math::FloatVector _vertex;
	bg::math::FloatVector _normal;
	bg::math::FloatVector _texCoord0;
	bg::math::FloatVector _texCoord1;
	bg::math::FloatVector _texCoord2;
	bg::math::FloatVector _color;
	bg::math::UIntVector _index;
	bg::math::FloatVector _tangent;
	
	DrawMode _drawMode;
	
	ptr<bg::engine::PolyListImpl> _impl;
	
	bool _visible;
	
	void calculateTangent();

	static bool s_standarizeVectors;

	void standarizeVectors();
};

}
}

#endif
