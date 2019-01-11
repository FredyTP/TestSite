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


#include <bg/db/plugin/bg2_scene.hpp>

#include <bg/system/archive.hpp>

#include <bg/db/json/value.hpp>
#include <bg/db/json/parser.hpp>

#include <bg/db/plugin/bg2_drawable.hpp>
#include <bg/db/plugin/bg2_prefab.hpp>

#include <bg/db/drawable.hpp>

#include <bg/scene/component.hpp>
#include <bg/scene/camera.hpp>

#include <ostream>

namespace bg {
namespace db {
namespace plugin {

class JsonSceneLoader {
public:
	struct Version {
		int major;
		int minor;
		int revision;
	};

	JsonSceneLoader(bg::base::Context * ctx)
		:_context(ctx)
	{
	}

	void open(const bg::system::Path & path) {
		_path = path;
		using namespace bg::db::json;
		_document = bg::db::json::Parser::ParseFile(path);

		if ((*_document)["fileType"]->type() != Value::kString ||
			((*_document)["fileType"]->stringValue()!="vwgl::scene" &&
			(*_document)["fileType"]->stringValue() != "bg2::scene"))
		{
			throw bg::base::PluginException("This file doesn't seems to be a scene file.");
		}
		parseVersion();
		
		_sceneRoot = new bg::scene::Node(_context.getPtr(), "Scene Root");
		parseChildren(_sceneRoot.getPtr(), (*_document)["scene"]);
	}

	inline bg::scene::Node * sceneRoot() { return _sceneRoot.getPtr(); }
	inline bg::scene::Node * releaseScene() { return _sceneRoot.release(); }

protected:
	bg::ptr<bg::db::json::Value> _document;

	void parseVersion() {
		using namespace bg::db::json;
		if(_document.valid() && (*_document)["version"]->type() == Value::kObject) {
			Value & version = *(*_document)["version"];
			_version.major = version["major"]->intValue();
			_version.minor = version["minor"]->intValue();
			_version.revision = version["rev"]->intValue();
		}
	}

	void parseChildren(bg::scene::Node * parent, bg::db::json::Value * children) {
		using namespace bg::db::json;
		if (children && children->type()==Value::kArray) {
			children->eachItem([&](Value * childData) {
				bg::ptr<bg::scene::Node> node = new bg::scene::Node(_context.getPtr(),
																	Value::String((*childData)["name"]));
				node->setSteady(Value::Bool((*childData)["steady"], false));
				node->setEnabled(Value::Bool((*childData)["enabled"], true));
				parent->addChild(node.getPtr());
				Value * components = (*childData)["components"];
				if (components) {
					parseComponents(node.getPtr(), components);
				}
				Value * children = (*childData)["children"];
				if (children) {
					parseChildren(node.getPtr(), children);
				}
			});
		}
	}

	void parseComponents(bg::scene::Node * node, bg::db::json::Value * componentData) {
		using namespace bg::db::json;
		componentData->eachItem([&](Value * componentData) {
			bg::scene::Component * component = bg::scene::Component::Factory(_context.getPtr(), componentData, _path.pathRemovingLastComponent());
			if (component) {
				node->addComponent(component);
			}
		});
	}

	Version _version;
	bg::system::Path _path;
	bg::ptr<bg::base::Context> _context;
	bg::ptr<bg::scene::Node> _sceneRoot;
};

class WriteSceneHelper {
public:
	WriteSceneHelper(bg::base::Context * ctx, std::ostream & stream, const bg::system::Path & path) :_ctx(ctx), _stream(&stream), _dst(path) {}

	void save(bg::scene::Node * node) {
		using namespace bg::db::json;
		using namespace bg::scene;
		_sceneRoot = new Value();
		_sceneRoot->setValue("fileType","vwgl::scene");
		_sceneRoot->setValue("version", Value::ValueMap{ { "major", new Value(2) },
														 { "minor", new Value(0) },
														 { "rev", new Value(0) }});

		Value * scene = new Value(Value::kArray);
		_sceneRoot->setValue("scene",scene);
		node->eachChild([&](Node * child) {
			Value * jsonChild = new Value();
			scene->push(jsonChild);
			writeChildren(child, jsonChild);
			writeNode(child, jsonChild);
		});
 		_sceneRoot->writeToStream(*_stream);
	}

	void writeNode(bg::scene::Node * node, bg::db::json::Value * jsonNode) {
		using namespace bg::db::json;
		using namespace bg::scene;

		jsonNode->setValue("type","Node");
		jsonNode->setValue("name", node->name());
		jsonNode->setValue("enabled", node->enabled());

		Value * components = new Value(Value::kArray);
		jsonNode->setValue("components", components);
		node->eachComponent([&](Component * comp) {
			Value * jsonComp = comp->serialize(_ctx, _dst);
			if (jsonComp) {
				components->push(jsonComp);
			}
		});

		
	}

	void writeChildren(bg::scene::Node * node, bg::db::json::Value * jsonNode, const std::string & key = "children") {
		using namespace bg::db::json;
		using namespace bg::scene;

		Value * jsonChildren = new Value(Value::kArray);
		jsonNode->setValue(key, jsonChildren);
		node->eachChild([&](Node * child) {
			Value * jsonChild = new Value(Value::kArray);
			jsonChildren->push(jsonChild);
			writeNode(child, jsonChild);
			writeChildren(child, jsonChild);
		});
	}

protected:
	bg::base::Context * _ctx;
	bg::system::Path _dst;
	std::ostream * _stream;
	bg::ptr<bg::db::json::Value> _sceneRoot;
};

ReadScene::ReadScene()
{
}

ReadScene::~ReadScene() {
}

bool ReadScene::supportFileType(const std::string & extension) {
	return	extension=="vitscn" ||
			extension=="vitscnj" ||
			extension=="vitbundle" ||
			extension=="bg2scn";
}

bg::scene::Node * ReadScene::load(bg::base::Context * context, const bg::system::Path & path) {
	bg::system::Path jsonFile;
	bg::system::Path tmpDir;

	if (path.extension()=="vitbundle") {
		if (path.isDirectory()) {
			std::string fileName = path.fileName();
			jsonFile = path.pathAddingComponent("scene.vitscnj");
		}
	}
	else if (path.extension()=="vitscnj") {
		jsonFile = path;
	}
	else if (path.extension()=="vitscn" || path.extension()=="bg2scn") {
		bg::system::Path tmpPath = bg::system::Path::TempDir();
		bg::system::Archive archive;
		archive.open(path, bg::system::Archive::kModeUnpack);
		archive.unpackTo(tmpPath, true);
		tmpPath.addComponent(path.fileName());
		tmpDir = tmpPath;
		jsonFile = tmpPath.pathAddingComponent("scene.vitscnj");
	}
	else {
		throw bg::base::ReadFileException("Unexpected error reading scene file: " + path.text());
	}

	if(!jsonFile.exists()) {
		throw bg::base::NoSuchFileException("No such scene file: " + path.text());
	}

	JsonSceneLoader loader(context);
	loader.open(jsonFile);
	bg::scene::Node * result = loader.releaseScene();
	return result;
}

void ReadScene::registerDependencies() {
	NodeLoader::RegisterPlugin(new bg::db::plugin::ReadPrefabBg2());
	DrawableLoader::RegisterPlugin(new bg::db::plugin::ReadDrawableBg2());
}


WriteScene::WriteScene()
{
}

WriteScene::~WriteScene() {
}

bool WriteScene::supportFileType(const std::string & extension) {
	return	extension == "vitscn" ||
			extension == "vitbundle" ||
			extension == "bg2scn";
}

void WriteScene::save(bg::base::Context * ctx, const bg::system::Path & path, bg::scene::Node * node) {
	std::string ext = path.extension();
	bg::system::Path filePath;
	if (ext=="bg2scn" || ext=="vitscn") {
		// package 
		filePath = bg::system::Path::TempDir().pathAddingComponent("scene.vitscnj");
	}
	else if (ext=="vitbundle") {
		// bundle
		filePath = path;
		filePath.create(true);
		filePath = path.pathAddingComponent("scene.vitscnj");
	}

	std::ofstream outFile;
	outFile.open(filePath.text());
	if (!outFile.is_open()) {
		throw bg::base::WriteFileException("Error opening scene file for write");
	}
	WriteSceneHelper writeScene(ctx, outFile, filePath);
	writeScene.save(node);
	outFile.close();

	if (ext=="bg2scn" || ext=="vitscn") {
		bg::system::Archive archive;
		archive.open(path, bg::system::Archive::kModePack);
		filePath.pathRemovingLastComponent()
			.list([&](const bg::system::Path & filePath) {
				archive.addFile(filePath);
			});
		archive.close();
	}
}

void WriteScene::registerDependencies() {
	DrawableWriter::RegisterPlugin(new bg::db::plugin::WriteDrawableBg2());
}


}
}
}
