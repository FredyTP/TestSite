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

#include <bg/db/plugin/obj_drawable.hpp>

#include <bg/scene/drawable.hpp>

#include <bg/base/exception.hpp>

#include <bg/scene/camera.hpp>

#include <fstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <regex>

namespace bg {
namespace db {
namespace plugin {

namespace objutils {

class MtlParser {
public:
	void setPath(const bg::system::Path & p) {
		_path = p;
	}

	void parse(bg::scene::Drawable * drw) {
		if (_path.exists()) {
			// TODO: Parse mtl
		}
	}

protected:
	bg::system::Path _path;
};
class ObjParser {
public:
	enum FaceIndex {
		kIndexVertex	= 1 << 0,
		kIndexTexCoord	= 1 << 1,
		kIndexNormal	= 1 << 2
	};
	struct FaceVertex {
		int v;
		int vt;
		int vn;
		uint32_t faceTypes;
	};
	typedef std::function<void(float, float, float)> Float3Closure;
	typedef std::function<void(float, float)> Float2Closure;
	typedef std::function<void(const std::string &)> StringClosure;
	typedef std::function<void(const std::vector<FaceVertex> &)> FaceClosure;

	ObjParser() {}

	inline void onVector(Float3Closure c) { _vecClosure = c; }
	inline void onNormal(Float3Closure c) { _normClosure = c; }
	inline void onTexCoord(Float2Closure c) { _texCoordClosure = c; }
	inline void onMtlLib(StringClosure c) { _mtlLibClosure = c; }
	inline void onGroup(StringClosure c) { _groupClosure = c; }
	inline void onUseMtl(StringClosure c) { _useMtlClosure = c; }
	inline void onSmoothingGroup(StringClosure c) { _smoothGroupClosure = c; }
	inline void onObjectName(StringClosure c) { _objectClosure = c; }
	inline void onFace(FaceClosure c) { _faceClosure = c; }
	

	void parse(std::ifstream & file) {
		while (!file.eof()) {
			std::string line;
			std::getline(file, line);

			trim(line);

			if (line.length() > 1 && line[0] != '#') {
				switch (line[0]) {
				case 'v':
					parseVertex(line, std::regex("v\\s+([\\d\\.\\-]+)\\s+([\\d\\.\\-]+)\\s+([\\d\\.\\-]+)"), _vecClosure) ||
						parseVertex(line, std::regex("vn\\s+([\\d\\.\\-]+)\\s+([\\d\\.\\-]+)\\s+([\\d\\.\\-]+)"), _normClosure) ||
						parseVertex(line, std::regex("vt\\s+([\\d\\.\\-]+)\\s+([\\d\\.\\-]+)"), _texCoordClosure) ||
						parseVertex(line, std::regex("vt\\s+([\\d\\.\\-]+)\\s+([\\d\\.\\-]+)\\s+([\\d\\.\\-]+)"), _texCoordClosure);
					break;
				case 'm':
					parseStringParam(line, std::regex("mtllib\\s+(.*)"), _mtlLibClosure);
					break;
				case 'g':
					parseStringParam(line, std::regex("g\\s(.*)"), _groupClosure);
					break;
				case 'u':
					parseStringParam(line, std::regex("usemtl\\s+(.*)"), _useMtlClosure);
					break;
				case 's':
					parseStringParam(line, std::regex("s\\s(.*)"), _smoothGroupClosure);
					break;
				case 'f':
					parseFace(line);
					break;
				case 'o':
					parseStringParam(line, std::regex("o\\s(.+)"), _objectClosure);
					break;
				}
			}
		}

		std::cout << "done" << std::endl;
	}

protected:
	Float3Closure _vecClosure;
	Float3Closure _normClosure;
	Float2Closure _texCoordClosure;
	StringClosure _mtlLibClosure;
	StringClosure _groupClosure;
	StringClosure _useMtlClosure;
	StringClosure _smoothGroupClosure;
	StringClosure _objectClosure;
	FaceClosure _faceClosure;

	inline void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
	}

	inline void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	}

	inline void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}

	inline void split(const std::string s, std::vector<std::string> & tokens) {
		std::string buff;
		std::stringstream ss(s);
		while (ss >> buff) tokens.push_back(buff);
	}

	void parseV(const std::smatch & match, float & v0, float & v1) {
		if (match.size() >= 3) {
			v0 = static_cast<float>(atof(match[1].str().c_str()));
			v1 = static_cast<float>(atof(match[2].str().c_str()));
		}
		else {
			throw bg::base::MalformedFileException("Error opening obj file: malformed document syntax.");
		}
	}

	void parseV(const std::smatch & match, float & v0, float & v1, float & v2) {
		if (match.size() >= 4) {
			v0 = static_cast<float>(atof(match[1].str().c_str()));
			v1 = static_cast<float>(atof(match[2].str().c_str()));
			v2 = static_cast<float>(atof(match[3].str().c_str()));
		}
		else {
			throw bg::base::MalformedFileException("Error opening obj file: malformed document syntax.");
		}
	}

	bool parseVertex(const std::string & line, const std::regex re, Float3Closure c) {
		std::smatch match;
		float v0, v1, v2;
		bool status = std::regex_match(line, match, re);

		if (status && c) {
			parseV(match, v0, v1, v2);
			c(v0, v1, v2);
		}
		return status;
	}

	bool parseVertex(const std::string & line, const std::regex re, Float2Closure c) {
		std::smatch match;
		float v0, v1;
		bool status = std::regex_match(line, match, re);

		if (status && c) {
			parseV(match, v0, v1);
			c(v0, v1);
		}
		return status;
	}

	void parseStringParam(const std::string & line, const std::regex & re, StringClosure c) {
		std::smatch match;
		if (std::regex_match(line, match, re) && match.size() == 2 && c) {
			c(match[1]);
		}
	}
	
	void parseFace(const std::string & line) {
		std::regex faceRE("f\\s+(.*)");
		std::smatch match;
		if (std::regex_match(line, match, faceRE) && match.size()>=1 && _faceClosure) {
			//const std::string faces = match[1];
			std::vector<std::string> faces;
			split(match[1], faces);
			std::vector<FaceVertex> faceVector;

			for (auto face : faces) {
				std::regex vtnRE("([\\d\\-]+)/([\\d\\-]*)/([\\d\\-]*)");
				if (std::regex_match(face, match, vtnRE) && match.size()>=4) {
					// Vertex, texcoord, normal
					uint32_t indexes = kIndexVertex;
					if (!match[2].str().empty()) indexes |= kIndexTexCoord;
					if (!match[3].str().empty()) indexes |= kIndexNormal;
					int vert = std::atoi(match[1].str().c_str());
					int texc = std::atoi(match[2].str().c_str());
					int norm = std::atoi(match[3].str().c_str());

					faceVector.push_back({ vert, texc, norm, indexes });
				}
				else if (face.find_first_of('/')==-1) {
					// Single vertex
					faceVector.push_back({ std::atoi(face.c_str()), 0, 0, kIndexVertex });
				}
			}

			_faceClosure(faceVector);
		}
	}
};

}

ReadDrawableObj::ReadDrawableObj() {

}

ReadDrawableObj::~ReadDrawableObj() {

}

bool ReadDrawableObj::supportFileType(const std::string & extension) {
	return extension=="obj";
}

bg::scene::Drawable * ReadDrawableObj::load(bg::base::Context * ctx, const bg::system::Path & path) {
	using namespace bg::scene;
	using namespace bg::base;
	using namespace bg::math;
	bg::ptr<Drawable> result = new Drawable(path.fileName());
	std::vector<Vector3> v;
	std::vector<Vector2> vt;
	std::vector<Vector3> vn;
	bg::system::Path mtlLibPath = path.pathRemovingLastComponent();

	std::ifstream file;
	file.open(path.text());
	if (!file.good()) {
		throw bg::base::NoSuchFileException("Could not open OBJ file. No such file in path " + path.text());
	}

	objutils::ObjParser parser;
	objutils::MtlParser mtlParser;
	bg::ptr<PolyList> plist = new PolyList(ctx);
	result->addPolyList(plist.getPtr());
	bool newPlist = false;
	std::function<void()> checkAddPlist = [&]() {
		if (newPlist) {
			// Build current plist and create another
			plist->build();
			plist = new PolyList(ctx);
			result->addPolyList(plist.getPtr());
			newPlist = false;
		}
	};

	parser.onVector([&](float v0, float v1, float v2) {
		v.push_back({ v0, v1, v2 });
	});

	parser.onNormal([&](float v0, float v1, float v2) {
		vn.push_back({ v0, v1, v2 });
	});

	parser.onTexCoord([&](float v0, float v1) {
		vt.push_back({ v0, v1 });
	});

	parser.onMtlLib([&](const std::string & mtlLib) {
		checkAddPlist();
		if (plist->name().empty()) {
			plist->setName(mtlLib);
		}
		else {
			plist->setMaterialName(mtlLib);
		}
	});

	parser.onObjectName([&](const std::string & objName) {
		checkAddPlist();
		plist->setName(objName);
	});

	parser.onGroup([&](const std::string & grpName) {
		if (plist->name().empty()) {
			plist->setName(grpName);
		}
	});

	parser.onUseMtl([&](const std::string & useMtl) {
		mtlLibPath.addComponent(useMtl);
		mtlParser.setPath(mtlLibPath);
	});

	std::function<void(const objutils::ObjParser::FaceVertex &)> addPoint = [&](const objutils::ObjParser::FaceVertex & face) {
		int vi = face.v;
		int vni = face.vn;
		int vti = face.vt;

		if (vi < 0) {
			vi = static_cast<int>(v.size()) + vi;
		}
		else {
			vi--;
		}

		if (vni < 0) {
			vni = static_cast<int>(vn.size()) + vni;
		}
		else {
			vni--;
		}

		if (vti < 0) {
			vti = static_cast<int>(vt.size()) + vti;
		}
		else {
			vti--;
		}

		Vector3 vertex = v[vi];
		Vector3 normal(0.0f, 1.0f, 0.0f);
		Vector2 texCoord(0.0f, 0.0f);

		if (face.faceTypes & objutils::ObjParser::kIndexTexCoord) {
			texCoord = vt[vti];
		}
		if (face.faceTypes & objutils::ObjParser::kIndexNormal) {
			normal = vn[vni];
		}
		plist->addVertex(vertex);
		plist->addNormal(normal);
		plist->addTexCoord0(texCoord);
		plist->addIndex(plist->indexCount());
	};

	parser.onFace([&](const std::vector<objutils::ObjParser::FaceVertex> & face) {
		newPlist = true;
		auto currentVertex = 0;
		auto sides = face.size();
		if (sides < 3) return;
		while (currentVertex < sides) {
			auto i0 = currentVertex;
			auto i1 = currentVertex + 1;
			auto i2 = currentVertex + 2;
			if (i2 == sides) {
				i2 = 0;
			}
			else if (i1 == sides) {
				i1 = 0;
				i2 = 2;
			}

			const auto & p0 = face[i0];
			const auto & p1 = face[i1];
			const auto & p2 = face[i2];

			addPoint(p0);
			addPoint(p1);
			addPoint(p2);

			currentVertex += 3;
		}
	});
	parser.parse(file);
	plist->build();
	mtlParser.parse(result.getPtr());

	return result.release();
}


}
}
}
