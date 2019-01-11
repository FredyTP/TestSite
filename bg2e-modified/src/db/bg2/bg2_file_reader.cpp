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


#include <bg/db/bg2/bg2_file_reader.hpp>
#include <bg/db/json/parser.hpp>
#include <bg/db/json/value.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/scene/drawable.hpp>
#include <bg/scene/camera.hpp>

namespace bg {
namespace db {
namespace bg2 {

void readVector(bg::db::json::Value * val, bg::math::Vector2 & result) {
	if (val && (*val)[0] && (*val)[1]) {
		result.set((*val)[0]->floatValue(),
				   (*val)[1]->floatValue());
	}
}

void readVector(bg::db::json::Value * val, bg::math::Vector3 & result) {
	if (val && (*val)[0] && (*val)[1] && (*val)[2]) {
		result.set((*val)[0]->floatValue(),
				   (*val)[1]->floatValue(),
				   (*val)[2]->floatValue());
	}
}

void readVector(bg::db::json::Value * val, bg::math::Vector4 & result) {
	if (val && (*val)[0] && (*val)[1] && (*val)[2] && (*val)[3]) {
		result.set((*val)[0]->floatValue(),
				   (*val)[1]->floatValue(),
				   (*val)[2]->floatValue(),
				   (*val)[3]->floatValue());
	}
}

bool Bg2FileReader::load(const std::string &path) {
	if (_delegate == nullptr) return false;
	_plistMap.clear();
	_componentData = "";

	if (_fileUtils.open(path, bg::base::VwglbUtils::kModeRead)) {
		try {
			std::string materialsString;
			int numberOfPlist;
			readHeader(materialsString,numberOfPlist);

			readPolyList(numberOfPlist);

			_delegate->materials(materialsString);

			_plistMap.clear();
			_fileUtils.close();

			return true;
		}
		catch (std::exception & e) {
			_delegate->onError(e.what());
			_plistMap.clear();
		}
	}
	return false;
}

bool Bg2FileReader::loadHeader(const std::string &path) {
	if(_delegate == nullptr) return false;
	return false;
}
	
void Bg2FileReader::readHeader(std::string & materialsString, int & numberOfPlist) {
	//// File header
	// File endian 0=big endian, 1=little endian
	unsigned char endian;
	_fileUtils.readByte(endian);
	if (endian == 0) _fileUtils.setBigEndian();
	else _fileUtils.setLittleEndian();

	// Version (major, minor, rev)
	Bg2FileReaderDelegate::FileVersion version;
	_fileUtils.readByte(version.major);
	_fileUtils.readByte(version.minor);
	_fileUtils.readByte(version.revision);

	_delegate->fileVersion(version);

	// Header type
	bg::base::VwglbUtils::BlockType btype;
	_fileUtils.readBlock(btype);
	if (btype != bg::base::VwglbUtils::kHeader) {
		throw bg::base::ReadFileException("Bg2 file format exception. Expecting begin of file header.");
	}

	// Number of poly list
	_fileUtils.readInteger(numberOfPlist);
	_delegate->metadata(Bg2FileReaderDelegate::kNumberOfPlist, numberOfPlist);

	// Materials
	bg::base::VwglbUtils::BlockType block;
	_fileUtils.readBlock(block);
	if (block != bg::base::VwglbUtils::kMaterials) {
		throw bg::base::ReadFileException("Bg2 file format exception. Expecting material list.");
	}

	_fileUtils.readString(materialsString);

	// Shadow projectors are deprecated, this block only skips the projector section
	_fileUtils.readBlock(block);
	if (block == bg::base::VwglbUtils::kShadowProjector) {
		std::string fileName;
		float proj[16];
		float trans[16];
		float attenuation;
		_fileUtils.readString(fileName);
		_fileUtils.readFloat(attenuation);
		for (int i = 0; i<16; ++i) {
			_fileUtils.readFloat(proj[i]);
		}
		for (int i = 0; i<16; ++i) {
			_fileUtils.readFloat(trans[i]);
		}
	}
	else {
		_fileUtils.seekForward(-4);
	}

	// Joint list
	// TODO: Support for multiple types of joints, and multiple output joints
	_fileUtils.readBlock(block);
	if (block == bg::base::VwglbUtils::kJoint) {
		using namespace bg::base;
		using namespace bg::db::json;
		std::string jointString;
		math::Vector3 offset;
		float pitch, roll, yaw;
		_fileUtils.readString(jointString);

		ptr<Value> jointData = Parser::ParseString(jointString);
		Value * input = (*jointData)["input"];
		if (input) {
			readVector((*input)["offset"], offset);
			pitch = (*input)["pitch"] ? (*input)["pitch"]->floatValue():0.0f;
			yaw = (*input)["yaw"] ? (*input)["yaw"]->floatValue():0.0f;
			roll = (*input)["roll"] ? (*input)["roll"]->floatValue():0.0f;
			
			_delegate->joint(Bg2FileReaderDelegate::kJointTypeInput, offset, pitch, roll, yaw);
		}


		Value * output = (*jointData)["output"];
		if (output && (*output)[0]) {
			readVector((*(*output)[0])["offset"], offset);
			pitch = (*(*output)[0])["pitch"] ? (*(*output)[0])["pitch"]->floatValue():0.0f;
			yaw = (*(*output)[0])["yaw"] ? (*(*output)[0])["yaw"]->floatValue():0.0f;
			roll = (*(*output)[0])["roll"] ? (*(*output)[0])["roll"]->floatValue():0.0f;
			
			_delegate->joint(Bg2FileReaderDelegate::kJointTypeOutput, offset, pitch, roll, yaw);
		}
	}
	else {
		_fileUtils.seekForward(-4);
	}
}

void Bg2FileReader::readPolyList(int numberOfPlist) {
	using namespace bg::base;
	VwglbUtils::BlockType block;

	_fileUtils.readBlock(block);
	if (block != VwglbUtils::kPolyList) throw ReadFileException("Bg2: File format exception. Expecting poly list");
	for (int i = 0; i < numberOfPlist; ++i) {
		readSinglePolyList(i == numberOfPlist - 1);
	}
}

void Bg2FileReader::readSinglePolyList(bool isLast) {
	using namespace bg::base;
	VwglbUtils::BlockType block;
	bool done = false;
	bool vertexFound = false;
	bool normalFound = false;
	bool tex0Found = false;
	bool tex1Found = false;
	bool tex2Found = false;
	bool indexFound = false;
	std::vector<float> vector;
	std::vector<unsigned int> ivector;
	std::string name;

	ptr<PolyList> plist = new PolyList(_context.getPtr());
	while (!done) {
		vector.clear();
		_fileUtils.readBlock(block);
		switch (block) {
		case VwglbUtils::kPlistName:
			_fileUtils.readString(name);
			plist->setName(name);
			break;
		case VwglbUtils::kMatName:
			_fileUtils.readString(name);
			plist->setMaterialName(name);
			break;
		case VwglbUtils::kVertexArray:
			if (vertexFound) throw ReadFileException("Bg2: File format exception. duplicate vertex array");
			vertexFound = true;
			_fileUtils.readArray(vector);
			plist->addVertexVector(&vector[0], static_cast<int>(vector.size()));
			break;
		case VwglbUtils::kNormalArray:
			if (normalFound) throw ReadFileException("Bg2: File format exception. duplicate normal array");
			normalFound = true;
			_fileUtils.readArray(vector);
			plist->addNormalVector(&vector[0], static_cast<int>(vector.size()));
			break;
		case VwglbUtils::kTexCoord0Array:
			if (tex0Found) throw ReadFileException("Bg2: File format exception. duplicate texcoord0 array");
			tex0Found = true;
			_fileUtils.readArray(vector);
			plist->addTexCoord0Vector(&vector[0], static_cast<int>(vector.size()));
			break;
		case VwglbUtils::kTexCoord1Array:
			if (tex1Found) throw ReadFileException("Bg2: File format exception. duplicate texcoord1 array");
			tex1Found = true;
			_fileUtils.readArray(vector);
			if (vector.size()>0) {
				plist->addTexCoord1Vector(&vector[0], static_cast<int>(vector.size()));
			}
			break;
		case VwglbUtils::kTexCoord2Array:
			if (tex2Found) throw ReadFileException("Bg2: File format exception. duplicate texcoord2 array");
			tex2Found = true;
			_fileUtils.readArray(vector);
			plist->addTexCoord0Vector(&vector[1], static_cast<int>(vector.size()));
			break;
		case VwglbUtils::kIndexArray:
			if (indexFound) throw ReadFileException("Bg2: File format exception. duplicate index array");
			indexFound = true;
			_fileUtils.readArray(ivector);
			plist->addIndexVector(&ivector[0], static_cast<int>(ivector.size()));
			break;
		case VwglbUtils::kPolyList:
		case VwglbUtils::kEnd:
			if (isLast && _fileUtils.readBlock(block) && block==VwglbUtils::kComponents) {
				_fileUtils.readString(_componentData);
			}
			done = true;
			break;
		default:
			throw ReadFileException("Bg2: File format exception. Unexpected poly list member found");
		}
	}

	plist->build();
	_plistMap[plist->materialName()] = plist.getPtr();
	_delegate->polyList(plist.getPtr());
}


bool Bg2MatReader::isPresent() {
	bg::system::Path matFilePath = _modelFilePath.pathRemovingExtension().addExtension("bg2mat");
	return matFilePath.exists();
}

void Bg2MatReader::load(bg::scene::Drawable * drw) {
	using namespace bg::db::json;
	bg::system::Path matFilePath = _modelFilePath.pathRemovingExtension().addExtension("bg2mat");
	std::ifstream fileStream;
	fileStream.open(matFilePath.text(), std::ios::in);
	std::string basePath = matFilePath.pathRemovingLastComponent().text();
	if (fileStream.good()) {
		bg::ptr<Value> doc = new Value();
		doc->loadFromStream(fileStream,false);
		if (doc->type() == Value::kArray) {
			doc->eachItem([&](Value * item) {
				std::string name = Value::String((*item)["name"]);
				auto drawableElement = drw->find(name);
				if (drawableElement) {
					bg::ptr<bg::base::Material> mat = bg::base::Material::FromMaterialDefinition(context(), item, basePath);
					drawableElement->material = mat.getPtr();
				}
			});
		}
		fileStream.close();
	}
}

}
}
}
