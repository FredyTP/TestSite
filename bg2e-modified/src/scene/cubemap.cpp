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

#include <bg/scene/cubemap.hpp>

#include <bg/tools/texture_cache.hpp>
#include <bg/log.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace scene {

ComponentFactory<Cubemap> cubemapFactory("Cubemap");

bool Cubemap::s_destroyCallbackRegistered;
bg::ptr<bg::base::Texture> Cubemap::s_currentCubemap;

bg::base::Texture * Cubemap::Current(bg::base::Context * context) {
	if (s_currentCubemap.isNull()) {
		SetCurrent(bg::tools::TextureCache::WhiteCubemap(context));
	}
	return s_currentCubemap.getPtr();
}

void Cubemap::SetCurrent(bg::base::Texture * tex) {
	if (!s_destroyCallbackRegistered) {
		s_destroyCallbackRegistered = true;
		bg::Engine::AddDestroyCallback([&]() {
			s_currentCubemap = nullptr;
		});
	}
	
	s_currentCubemap = tex;
}

Cubemap::Cubemap()
{

}

Cubemap::~Cubemap() {

}
		
Component * Cubemap::clone() {
	return nullptr;
}


void Cubemap::loadCubemap(bg::base::Context * context) {
	bg::ptr<bg::base::Image> posX = new bg::base::Image();
	bg::ptr<bg::base::Image> negX = new bg::base::Image();
	bg::ptr<bg::base::Image> posY = new bg::base::Image();
	bg::ptr<bg::base::Image> negY = new bg::base::Image();
	bg::ptr<bg::base::Image> posZ = new bg::base::Image();
	bg::ptr<bg::base::Image> negZ = new bg::base::Image();

	posX->load(getImageFile(bg::base::Texture::kFacePositiveX));
	negX->load(getImageFile(bg::base::Texture::kFaceNegativeX));
	posY->load(getImageFile(bg::base::Texture::kFacePositiveY));
	negY->load(getImageFile(bg::base::Texture::kFaceNegativeY));
	posZ->load(getImageFile(bg::base::Texture::kFacePositiveZ));
	negZ->load(getImageFile(bg::base::Texture::kFaceNegativeZ));

	if (!posX->valid() || !negX->valid() ||
		!posY->valid() || !negY->valid() ||
		!posZ->valid() || !negZ->valid())
	{
        bg::log(bg::log::kWarning) << "Could not load cubemap texure. Error loading cubemap images." << bg::endl;
	}
    else {
        _texture = new bg::base::Texture(context);
        _texture->createCubemap(posX.getPtr(), negX.getPtr(), posY.getPtr(), negY.getPtr(), posZ.getPtr(), negZ.getPtr());
        
        if (s_currentCubemap.isNull()) {
            SetCurrent(_texture.getPtr());
        }
    }
}

void Cubemap::deserialize(bg::base::Context * context, bg::db::json::Value * componentData, const bg::system::Path & path) {
	using namespace bg::db::json;
	using namespace bg::base;
	
	bg::system::Path posX = path.pathAddingComponent(Value::String((*componentData)["positiveX"]));
	bg::system::Path negX = path.pathAddingComponent(Value::String((*componentData)["negativeX"]));
	bg::system::Path posY = path.pathAddingComponent(Value::String((*componentData)["positiveY"]));
	bg::system::Path negY = path.pathAddingComponent(Value::String((*componentData)["negativeY"]));
	bg::system::Path posZ = path.pathAddingComponent(Value::String((*componentData)["positiveZ"]));
	bg::system::Path negZ = path.pathAddingComponent(Value::String((*componentData)["negativeZ"]));

	setImageFile(Texture::kFacePositiveX, posX.text());
	setImageFile(Texture::kFaceNegativeX, negX.text());
	setImageFile(Texture::kFacePositiveY, posY.text());
	setImageFile(Texture::kFaceNegativeY, negY.text());
	setImageFile(Texture::kFacePositiveZ, posZ.text());
	setImageFile(Texture::kFaceNegativeZ, negZ.text());

	try {
		loadCubemap(context);
	}
	catch (std::runtime_error &) {
		// This may fail if the scene is in legacy format
	}
	
}

bg::db::json::Value * Cubemap::serialize(bg::base::Context *, const bg::system::Path & path) {
	using namespace bg::db::json;
	using namespace bg::base;
	Value * data = new Value();

	data->setValue("type", "Cubemap");
	
	bg::system::Path posX(getImageFile(Texture::kFacePositiveX));
	bg::system::Path negX(getImageFile(Texture::kFaceNegativeX));
	bg::system::Path posY(getImageFile(Texture::kFacePositiveY));
	bg::system::Path negY(getImageFile(Texture::kFaceNegativeY));
	bg::system::Path posZ(getImageFile(Texture::kFacePositiveZ));
	bg::system::Path negZ(getImageFile(Texture::kFaceNegativeZ));

	data->setValue("positiveX", posX.lastPathComponent());
	data->setValue("negativeX", negX.lastPathComponent());
	data->setValue("positiveY", posY.lastPathComponent());
	data->setValue("negativeY", negY.lastPathComponent());
	data->setValue("positiveZ", posZ.lastPathComponent());
	data->setValue("negativeZ", negZ.lastPathComponent());

	bg::system::Path dstPath = path.pathRemovingLastComponent();
	posX.copy(dstPath.pathAddingComponent(posX.lastPathComponent()));
	negX.copy(dstPath.pathAddingComponent(negX.lastPathComponent()));
	posY.copy(dstPath.pathAddingComponent(posY.lastPathComponent()));
	negY.copy(dstPath.pathAddingComponent(negY.lastPathComponent()));
	posZ.copy(dstPath.pathAddingComponent(posZ.lastPathComponent()));
	negZ.copy(dstPath.pathAddingComponent(negZ.lastPathComponent()));

	return data;
}

}
}
