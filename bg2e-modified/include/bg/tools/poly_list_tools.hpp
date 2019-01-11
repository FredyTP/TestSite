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


#ifndef _bg2e_tools_poly_list_hpp_
#define _bg2e_tools_poly_list_hpp_

#include <bg/base/poly_list.hpp>
#include <bg/tools/tool_base.hpp>

#include <unordered_map>

namespace bg {
namespace tools {


struct PolyListProperties {
	std::string name;
	std::string groupName;
	bool cullFace;
	bool visible;
	
	inline void operator=(PolyListProperties & other) {
		name = other.name;
		groupName = other.groupName;
		cullFace = other.cullFace;
		visible = other.visible;
	}
	
	inline bool operator==(PolyListProperties & other) {
		return	name == other.name &&
				groupName == other.groupName &&
				cullFace == other.cullFace &&
				visible == other.visible;
	}
};

typedef std::unordered_map<std::string, PolyListProperties> PolyListPropertiesMap;

class BG2E_EXPORT PolyListTools : public ToolBase<bg::base::PolyList>{
public:
	
	PolyListTools(bg::base::PolyList * plist);
	
	PolyListTools & clone();
	PolyListTools & switchUVs(bg::base::PolyList::VertexBufferType uv1, bg::base::PolyList::VertexBufferType uv2);
	PolyListTools & applyTransform(const bg::math::Matrix4 & trx);
	PolyListTools & flipFaces();
	PolyListTools & flipNormals();
	
	static void LoadPropertiesFromJson(const std::string & jsonString, PolyListPropertiesMap & result);
};

}
}

#endif