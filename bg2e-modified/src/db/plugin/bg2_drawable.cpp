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


#include <bg/db/plugin/bg2_drawable.hpp>

#include <bg/db/bg2/bg2_file_reader.hpp>
#include <bg/base/exception.hpp>
#include <bg/log.hpp>
#include <bg/base/material.hpp>
#include <bg/tools/material_tools.hpp>
#include <bg/tools/poly_list_tools.hpp>
#include <bg/tools/drawable_tools.hpp>

#include <bg/scene/node.hpp>
#include <bg/scene/transform.hpp>
#include <bg/scene/chain.hpp>
#include <bg/scene/camera.hpp>

#include <iostream>

namespace bg {
namespace db {
namespace plugin {

Bg2ReaderDelegate::Bg2ReaderDelegate(bg::base::Context * ctx, const bg::system::Path & file)
	:_file(file)
	,_ctx(ctx)
{
	_node = new bg::scene::Node(ctx, _file.fileName() + "-Node");
	_drw = new bg::scene::Drawable(_file.fileName());
	_node->addComponent(_drw.getPtr());
	_node->addComponent(new bg::scene::Transform());
}
	
void Bg2ReaderDelegate::onError(const std::string & error) {
	throw bg::base::ReadFileException(error);
}

void Bg2ReaderDelegate::onWarning(const std::string & warning) {
	bg::log(bg::log::kWarning) << "BG2 file read " << warning << '\n' << bg::endl;
}

void Bg2ReaderDelegate::fileVersion(const FileVersion & version) {
	bg::log() << "bg2 file version " << version.major << "."
			<< version.minor << "."
			<< version.revision << bg::endl;
}

void Bg2ReaderDelegate::metadata(FileMetadata metadata, int value) {
	bg::log() << "metadata: " << value << bg::endl;
}

void Bg2ReaderDelegate::metadata(FileMetadata metadata, const std::string & value) {
	bg::log() << "metadata: " << value << bg::endl;
}

void Bg2ReaderDelegate::metadata(FileMetadata metadata, bool value) {
	bg::log() << "metadata: " << value << bg::endl;
}

void Bg2ReaderDelegate::metadata(FileMetadata metadata, float value) {
	bg::log() << "metadata: " << value << bg::endl;
}

void Bg2ReaderDelegate::polyList(base::PolyList * plist) {
	_drw->addPolyList(plist);
}

void Bg2ReaderDelegate::materials(const std::string & materialsString) {
	bg::tools::MaterialMap materials;
	bg::tools::MaterialTools::LoadMaterialsFromJson(_ctx.getPtr(), materialsString, _file.pathRemovingLastComponent(), materials);
	bg::tools::PolyListPropertiesMap properties;
	bg::tools::PolyListTools::LoadPropertiesFromJson(materialsString, properties);
	
	size_t index = 0;
	for (auto elem : _drw->drawableElements()) {
		std::string name = elem.polyList->materialName();
		if (materials.find(name)!=materials.end()) {
			tools::DrawableTools drwTools(_drw.getPtr());
			drwTools.replaceMaterial(index++, materials[name].getPtr());
		}
		if (properties.find(name)!=properties.end()) {
			auto prop = properties[name];
			elem.polyList->setGroupName(prop.groupName);
			elem.polyList->setVisible(prop.visible);
			// TODO: cullFace
		}
	}
}

void Bg2ReaderDelegate::joint(JointType type, const bg::math::Vector3 & offset, float pitch, float roll, float yaw) {
	bg::scene::ChainJoint * joint = nullptr;
	if (type==bg::db::bg2::Bg2FileReaderDelegate::kJointTypeInput) {
		joint = new bg::scene::InputChainJoint();
	}
	else  if (type==bg::db::bg2::Bg2FileReaderDelegate::kJointTypeOutput) {
		joint = new bg::scene::OutputChainJoint();
	}
	
	if (joint) {
		joint->joint()->setOffset(offset);
		joint->joint()->setPitch(pitch);
		joint->joint()->setYaw(yaw);
		joint->joint()->setRoll(roll);
		_node->addComponent(joint);
	}
}


ReadDrawableBg2::ReadDrawableBg2()
{
	
}

ReadDrawableBg2::~ReadDrawableBg2(){
		
}

bool ReadDrawableBg2::supportFileType(const std::string &extension) {
	return extension=="bg2" || extension=="vwglb";
}

bg::scene::Drawable * ReadDrawableBg2::load(bg::base::Context * ctx, const bg::system::Path & path) {
	bg::ptr<bg::scene::Drawable> result;
	{
		bg2::Bg2FileReader reader(ctx);
		Bg2ReaderDelegate del(ctx, path);
		reader.setDelegate(&del);
		reader.load(path.text());
        bg::ptr<bg::scene::Node> drwNode = del.drawable()->node();
        drwNode->addComponent(new bg::scene::Drawable());  // This will clean the drawable node

		// Try to load the material definition file
		bg2::Bg2MatReader matReader(ctx, path);
		if (matReader.isPresent()) {
			try {
				matReader.load(del.drawable());
			}
			catch (std::runtime_error & msg) {
				bg::log(bg::log::kWarning) << "Error loading material definition file: " << msg.what() << bg::endl;
			}
		}
		result = del.drawable();
	}
	return result.release();
}


WriteDrawableBg2::WriteDrawableBg2() {
	
}

WriteDrawableBg2::~WriteDrawableBg2() {
	
}

bool WriteDrawableBg2::supportFileType(const std::string & extension) {
	return extension=="bg2" || extension=="vwglb";
}

void WriteDrawableBg2::save(bg::base::Context * ctx, const bg::system::Path & path, bg::scene::Drawable * drawable) {
	setContext(ctx);
	writeData(path.text(), drawable);
}

	
}
}
}
