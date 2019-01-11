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


#include <bg/db/bg2/bg2_file_writer.hpp>
#include <bg/db/json/value.hpp>
#include <bg/scene/chain.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace db {
namespace bg2 {

bool Bg2Writer::writeData(const std::string & path, scene::Drawable * model) {
	using namespace bg::base;
	using namespace bg::scene;
	if(_fileUtils.open(path, VwglbUtils::kModeWrite)) {
		_dstPath.setText(path);
		writeHeader(model);

		model->forEach([&](Drawable::DrawableElement & elem) {
			writePolyList(elem.polyList.getPtr());
		});

		_fileUtils.writeBlock(VwglbUtils::kEnd);

		// The components section is written after the end of the polyList definition
		// to keep the file compatible with previos versions
		writeComponents(model->node(),path);
		_fileUtils.close();
		return true;
	}
	return false;
}

void Bg2Writer::writeHeader(scene::Drawable * drawable) {
	using namespace bg::base;
	_fileUtils.setBigEndian();

	//// Write header
	// File endian 0=big endian, 1=little endian
	_fileUtils.writeByte(0);

	// Version (major, minor, rev)
	_fileUtils.writeByte(1);
	_fileUtils.writeByte(3);
	_fileUtils.writeByte(0);

	// File type
	_fileUtils.writeBlock(VwglbUtils::kHeader);

	// Number of poly list
	_fileUtils.writeInteger(static_cast<int>(drawable->drawableElements().size()));

	// Materials (as json string)
	ensurePolyListNames(drawable);	// Ensure that all poly list have name and material name
	writeMaterial(drawable);
	writeJoints(drawable->node());
}

void Bg2Writer::ensurePolyListNames(scene::Drawable * drawable) {
	using namespace bg::base;
	auto elem = drawable->drawableElements();
	auto it = elem.begin();
	std::unordered_map<std::string, std::string> plistNames;
	std::unordered_map<std::string, std::string> materialNames;

	int plistIndex = 0;
	//	int matIndex = 0;
	for(it = elem.begin(); it != elem.end(); ++it) {
		PolyList * p = (*it).polyList.getPtr();
		std::string plistname = p->name();
		std::string matName = p->materialName();	// vwglb version 1.2.0 does not support sharing materials between polylist
		std::stringstream newplistname;
		std::stringstream newmatname;
		if(plistname == "" || plistNames.find(plistname) != plistNames.end()) {
			newplistname << "polyList_" << plistIndex;
			plistname = newplistname.str();
			p->setName(plistname);
			++plistIndex;
		}
		// vwglb version 1.2.0 does not support sharing materials between polylist
		p->setMaterialName(p->name());
		//		if (matName == "" || materialNames.find(matName) != materialNames.end()) {
		//			newmatname << "material_" << matIndex;
		//			matName = newmatname.str();
		//			p->setMaterialName(matName);
		//			++matIndex;
		//		}
		plistNames[plistname] = plistname;
		materialNames[matName] = matName;
	}
}

void Bg2Writer::writeMaterial(scene::Drawable * drawable, scene::Node * node) {
	using namespace bg::base;
	using namespace bg::scene;
	using namespace bg::db::json;

	ptr<Value> materialArray = new Value(Value::kArray);
	drawable->forEach([&](Drawable::DrawableElement & elem) {
		Value * material = new Value(Value::kObject);
		materialArray->push(material);
		base::PolyList * p = elem.polyList.getPtr();// (*it).getPtr();
		Material * m = elem.material.getPtr();
		if(m) {
			material->setValue("name", p->name());
			material->setValue("class", "GenericMaterial");
			material->setValue("diffuseR", m->diffuse().r());
			material->setValue("diffuseG", m->diffuse().g());
			material->setValue("diffuseB", m->diffuse().b());
			material->setValue("diffuseA", m->diffuse().a());

			material->setValue("specularR", m->specular().r());
			material->setValue("specularG", m->specular().g());
			material->setValue("specularB", m->specular().b());
			material->setValue("specularA", m->specular().a());
			
			material->setValue("shininess", m->shininess());
			material->setValue("refractionAmount", m->refractionAmount());
			material->setValue("reflectionAmount", m->reflectionAmount());
			material->setValue("lightEmission", m->lightEmission());

			material->setValue("textureOffsetX", m->textureOffset().x());
			material->setValue("textureOffsetY", m->textureOffset().y());
			material->setValue("textureScaleX", m->textureScale().x());
			material->setValue("textureScaleY", m->textureScale().y());

			material->setValue("lightmapOffsetX", m->lightMapOffset().x());
			material->setValue("lightmapOffsetY", m->lightMapOffset().y());
			material->setValue("lightmapScaleX", m->lightMapScale().x());
			material->setValue("lightmapScaleY", m->lightMapScale().y());

			material->setValue("normalMapOffsetX", m->normalMapOffset().x());
			material->setValue("normalMapOffsetY", m->normalMapOffset().y());
			material->setValue("normalMapScaleX", m->normalMapScale().x());
			material->setValue("normalMapScaleY", m->normalMapScale().y());


			material->setValue("castShadows", m->castShadows());
			material->setValue("receiveShadows", m->receiveShadows());
			material->setValue("alphaCutoff", m->alphaCutoff());

			material->setValue("shininessMaskChannel", m->shininessMaskChannel());
			material->setValue("invertShininessMask", m->invertShininessMask());
			
			material->setValue("lightEmissionMaskChannel", m->lightEmissionMaskChannel());
			material->setValue("invertLightEmissionMask", m->invertLightEmissionMask());

			material->setValue("displacementFactor", m->displacementFactor());
			material->setValue("displacementUV", m->displacementUV());

			material->setValue("tessDistanceFarthest", m->tessellationDistances().w());

			material->setValue("tessDistanceFar", m->tessellationDistances().z());
			material->setValue("tessDistanceNear",m->tessellationDistances().y());
			material->setValue("tessDistanceNearest", m->tessellationDistances().x());
			material->setValue("tessFarthestLevel", m->tessellationLevels().w());
			material->setValue("tessFarLevel", m->tessellationLevels().z());
			material->setValue("tessNearLevel", m->tessellationLevels().y());
			material->setValue("tessNearestLevel", m->tessellationLevels().x());

			material->setValue("reflectionMaskChannel", m->reflectionMaskChannel());
			material->setValue("invertReflectionMask", m->invertReflectionMask());

			material->setValue("cullFace", m->cullFace());

			// Textures
			writeTexture(material, "texture", m->texture());
			writeTexture(material, "lightmap", m->lightMap());
			writeTexture(material, "normalMap", m->normalMap());
			writeTexture(material, "shininessMask", m->shininessMask());
			writeTexture(material, "lightEmissionMask", m->lightEmissionMask());
			writeTexture(material, "displacementMap", m->displacementMap());
			writeTexture(material, "reflectionMask", m->reflectionMask());
		
			// Visibility and group name
			material->setValue("visible", p->visible());
			material->setValue("groupName", p->groupName());
		}
	});

	_fileUtils.writeBlock(VwglbUtils::kMaterials);
	std::stringstream sstream;
	materialArray->writeToStream(sstream);
	_fileUtils.writeString(sstream.str());
}

void Bg2Writer::writeJoints(scene::Node * node) {
	if (!node) return;
	using namespace bg::base;
	using namespace bg::scene;
	InputChainJoint * inJoint = node->component<InputChainJoint>();
	OutputChainJoint * outJoint = node->component<OutputChainJoint>();

	if(inJoint || outJoint) {
		_fileUtils.writeBlock(VwglbUtils::kJoint);
		std::stringstream joints;
		joints << "{";

		// In joint
		if(inJoint) {
			bg::math::Vector3 offset = inJoint->joint()->offset();
			float pitch = inJoint->joint()->pitch();
			float roll = inJoint->joint()->roll();
			float yaw = inJoint->joint()->yaw();
			joints << "\"input\":{";
			joints << "\"type\":\"LinkJoint\",";
			joints << "\"offset\":[" << offset.x() << "," << offset.y() << "," << offset.z() << "],";
			joints << "\"pitch\":" << pitch << ",\"roll\":" << roll << ",\"yaw\":" << yaw;
			joints << "}";
			if(outJoint) joints << ",";
		}

		// Out joint
		if(outJoint) {
			bg::math::Vector3 offset = outJoint->joint()->offset();
			float pitch = outJoint->joint()->pitch();
			float roll = outJoint->joint()->roll();
			float yaw = outJoint->joint()->yaw();
			joints << "\"output\":[{";
			joints << "\"type\":\"LinkJoint\",";
			joints << "\"offset\":[" << offset.x() << "," << offset.y() << "," << offset.z() << "],";
			joints << "\"pitch\":" << pitch << ",\"roll\":" << roll << ",\"yaw\":" << yaw;
			joints << "}]";
		}

		joints << "}";
		_fileUtils.writeString(joints.str());
	}
}

void Bg2Writer::writeComponents(bg::scene::Node * node, const std::string & path) {
	using namespace bg::base;
	using namespace bg::scene;
	using namespace bg::db::json;

	_fileUtils.writeBlock(VwglbUtils::kComponents);
	std::stringstream comps;
	comps << "[";
	bool first = true;
	node->eachComponent([&](bg::scene::Component * comp) {
		if (dynamic_cast<bg::scene::Drawable*>(comp)) return;
		ptr<Value> compJson = comp->serialize(this->context(), path);
		if (compJson.valid()) {
			if (first) {
				first = false;
			}
			else {
				comps << ",";
			}
			compJson->writeToStream(comps);
//			comps << compJson->stringValue();
			
		}
	});
	comps << "]";
	_fileUtils.writeString(comps.str());
}

void Bg2Writer::writePolyList(bg::base::PolyList * plist) {
	using namespace bg::base;
	_fileUtils.writeBlock(VwglbUtils::kPolyList);
	_fileUtils.writeBlock(VwglbUtils::kPlistName);
	_fileUtils.writeString(plist->name());
	_fileUtils.writeBlock(VwglbUtils::kMatName);
	_fileUtils.writeString(plist->name());	// In vwglb version 1.2.0 does not support sharing materials between polylists

	if(plist->vertexCount()>0) {
		_fileUtils.writeBlock(VwglbUtils::kVertexArray);
		_fileUtils.writeArray(plist->vertexVector());
	}

	if(plist->normalCount()>0) {
		_fileUtils.writeBlock(VwglbUtils::kNormalArray);
		_fileUtils.writeArray(plist->normalVector());
	}

	if(plist->texCoord0Count()>0) {
		_fileUtils.writeBlock(VwglbUtils::kTexCoord0Array);
		_fileUtils.writeArray(plist->texCoord0Vector());
	}

	if(plist->texCoord1Count()>0) {
		_fileUtils.writeBlock(VwglbUtils::kTexCoord1Array);
		_fileUtils.writeArray(plist->texCoord1Vector());
	}

	if(plist->texCoord2Count()>0) {
		_fileUtils.writeBlock(VwglbUtils::kTexCoord2Array);
		_fileUtils.writeArray(plist->texCoord2Vector());
	}

	if(plist->indexCount()>0) {
		_fileUtils.writeBlock(VwglbUtils::kIndexArray);
		_fileUtils.writeArray(plist->indexVector());
	}
}


void Bg2Writer::writeTexture(bg::db::json::Value * material, const std::string & key, bg::base::Texture * tex) {
	if(tex && !tex->fileName().empty()) {
		system::Path filePath = system::Path(tex->fileName());
		std::string textureFile = filePath.lastPathComponent();
		material->setValue(key, textureFile);
		saveTextureFile(filePath.text());
	}
}

void Bg2Writer::saveTextureFile(const bg::system::Path & imagePath) {
	using namespace bg::base;
	std::string fileName = imagePath.lastPathComponent();
	bg::system::Path dstPath = _dstPath.pathRemovingLastComponent()
										.addComponent(fileName);

	if(dstPath != imagePath) {
		std::ifstream inFile(imagePath.text(), std::ios::binary);
		if(inFile.is_open()) {
			std::ofstream dstFile(dstPath.text(), std::ios::binary);
			if(dstFile) {
				dstFile << inFile.rdbuf();
				dstFile.close();
			}
			inFile.close();
		}
	}
}

}
}
}
