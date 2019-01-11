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


#include <bg/tools/poly_list_tools.hpp>

#include <bg/math/vector.hpp>

#include <bg/db/json/parser.hpp>
#include <bg/db/json/value.hpp>

namespace bg {
namespace tools {

PolyListTools::PolyListTools(bg::base::PolyList * plist)
	:ToolBase(plist)
{
}

PolyListTools & PolyListTools::clone() {
	assertTarget();
	_target = new base::PolyList(_target->context(), _target.getPtr());
	return *this;
}

PolyListTools & PolyListTools::switchUVs(bg::base::PolyList::VertexBufferType uv1, bg::base::PolyList::VertexBufferType uv2) {
	assertTarget();
	if (uv1==base::PolyList::kVertex || uv1==base::PolyList::kNormal || uv1==base::PolyList::kTangent ||
		uv2==base::PolyList::kVertex || uv2==base::PolyList::kNormal || uv2==base::PolyList::kTangent ) {
		throw base::PolygonException("Invalid uv specified in switchUVs function. The specified buffer is not a vertex buffer");
	}
	else if ((uv1==base::PolyList::kTexCoord0 || uv2==base::PolyList::kTexCoord0) && _target->texCoord0Vector().size()==0) {
		throw base::PolygonException("Invalid uv. The specified buffer is null");
	}
	else if ((uv1==base::PolyList::kTexCoord1 || uv2==base::PolyList::kTexCoord1) && _target->texCoord1Vector().size()==0) {
		throw base::PolygonException("Invalid uv. The specified buffer is null");
	}
	else if ((uv1==base::PolyList::kTexCoord2 || uv2==base::PolyList::kTexCoord2) && _target->texCoord2Vector().size()==0) {
		throw base::PolygonException("Invalid uv. The specified buffer is null");
	}
	else {
		bg::math::FloatVector & src = uv1==base::PolyList::kTexCoord0 ? _target->texCoord0Vector():(uv1==base::PolyList::kTexCoord1 ? _target->texCoord1Vector():_target->texCoord2Vector());
		bg::math::FloatVector & dst = uv2==base::PolyList::kTexCoord0 ? _target->texCoord0Vector():(uv2==base::PolyList::kTexCoord1 ? _target->texCoord1Vector():_target->texCoord2Vector());
		bg::math::FloatVector tmp = src;
		src = dst;
		dst = tmp;
		_target->build();
	}
	return *this;
}

PolyListTools & PolyListTools::applyTransform(const bg::math::Matrix4 & trx) {
	assertTarget();
	bg::math::Matrix4 transform = trx;
	bg::math::Matrix4 rotation = trx.getMatrix3();
	if (_target->normalVector().size()>0 && _target->normalVector().size() != _target->vertexVector().size()) throw base::MalformedPolyListException("Unexpected number of normal coordinates found in polyList");
	for (int i = 0; i<_target->vertexVector().size() - 2; i += 3) {
		bg::math::Vector4 vertex(_target->vertexVector()[i], _target->vertexVector()[i + 1], _target->vertexVector()[i + 2], 1.0f);
		vertex = transform.multVector(vertex);
		_target->vertexVector()[i] = vertex.x();
		_target->vertexVector()[i + 1] = vertex.y();
		_target->vertexVector()[i + 2] = vertex.z();
		
		if (_target->normalVector().size()>0) {
			bg::math::Vector4 normal(_target->normalVector()[i], _target->normalVector()[i + 1], _target->normalVector()[i + 2], 1.0f);
			normal = rotation.multVector(normal);
			normal.normalize();
			_target->normalVector()[i] = normal.x();
			_target->normalVector()[i + 1] = normal.y();
			_target->normalVector()[i + 2] = normal.z();
		}
	}
	_target->build();
	return *this;
}

PolyListTools & PolyListTools::flipFaces() {
	assertTarget();
	for (int i = 0; (i + 2)<_target->indexVector().size(); i += 3) {
		int a = _target->indexVector()[i];
		int c = _target->indexVector()[i + 2];
		_target->indexVector()[i] = c;
		_target->indexVector()[i + 2] = a;
	}
	_target->build();
	return *this;
}

PolyListTools & PolyListTools::flipNormals() {
	assertTarget();
	for (int i = 0; i<_target->normalVector().size(); i++) {
		_target->normalVector()[i] = -_target->normalVector()[i];
	}
	_target->build();
	return *this;
}

void parseMaterialItem(bg::db::json::Value * value, std::string & outName, std::string & group, bool & visible, bool & cullFace) {
	using namespace bg::db::json;
	
	if (value && value->type()==Value::kObject) {
		outName = Value::String((*value)["name"],"plist-data");
		group = Value::String((*value)["groupName"]);
		visible = Value::Bool((*value)["visible"],true);
		cullFace = Value::Bool((*value)["cullFace"],true);
	}
}

void PolyListTools::LoadPropertiesFromJson(const std::string & jsonString, PolyListPropertiesMap & result) {
	using namespace bg::db::json;
	bg::ptr<Value> plistData = Parser::ParseString(jsonString);
	PolyListProperties prop;
	
	// The json string contains an array of polyList properties
	if (plistData.valid() && plistData->type()==Value::kArray) {
		for (auto plistItem : plistData->array()) {
			parseMaterialItem(plistItem.getPtr(), prop.name, prop.groupName, prop.visible, prop.cullFace);
			result[prop.name] = prop;
		}
	}
	// The json string contains a single poly list properties
	else if (plistData.valid() && plistData->type()==Value::kObject) {
		parseMaterialItem(plistData.getPtr(), prop.name, prop.groupName, prop.visible, prop.cullFace);
		result[prop.name] = prop;
	}
}

}
}
