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

#include <bg/scene/skybox.hpp>

#include <bg/tools/texture_cache.hpp>
#include <bg/log.hpp>
#include <bg/scene/camera.hpp>

#include <bg/scene/node.hpp>

namespace bg {
namespace scene {

ComponentFactory<Skybox> SkyboxFactory("Skybox");
    

Skybox::Skybox()
{
}

Skybox::~Skybox() {

}
		
Component * Skybox::clone() {
	return nullptr;
}


void Skybox::loadSkybox(bg::base::Context * context) {
    using namespace bg::base;
	bg::ptr<Image> posX = new Image();
	bg::ptr<Image> negX = new Image();
	bg::ptr<Image> posY = new Image();
	bg::ptr<Image> negY = new Image();
	bg::ptr<Image> posZ = new Image();
	bg::ptr<Image> negZ = new Image();

	posX->load(getImageFile(Texture::kFacePositiveX));
	negX->load(getImageFile(Texture::kFaceNegativeX));
	posY->load(getImageFile(Texture::kFacePositiveY));
	negY->load(getImageFile(Texture::kFaceNegativeY));
	posZ->load(getImageFile(Texture::kFacePositiveZ));
	negZ->load(getImageFile(Texture::kFaceNegativeZ));

	if (!posX->valid() || !negX->valid() ||
		!posY->valid() || !negY->valid() ||
		!posZ->valid() || !negZ->valid())
	{
        bg::log(bg::log::kWarning) << "Could not load Skybox texure. Error loading Skybox images." << bg::endl;
	}
    else {
        createCube(context);
        
        _textures.push_back(Texture::TextureWithImage(context, posX.getPtr(),Texture::kWrapModeClamp));
        _textures.push_back(Texture::TextureWithImage(context, negX.getPtr(),Texture::kWrapModeClamp));
        _textures.push_back(Texture::TextureWithImage(context, posY.getPtr(),Texture::kWrapModeClamp));
        _textures.push_back(Texture::TextureWithImage(context, negY.getPtr(),Texture::kWrapModeClamp));
        _textures.push_back(Texture::TextureWithImage(context, posZ.getPtr(),Texture::kWrapModeClamp));
        _textures.push_back(Texture::TextureWithImage(context, negZ.getPtr(),Texture::kWrapModeClamp));
    }
}

void Skybox::draw(bg::base::Pipeline * pipeline) {
    using namespace bg::base;
    if (pipeline->opacityLayer()==bg::base::Pipeline::kOpacityNone
        || !node()->enabled()
        || _textures.size()!=6
    ) {
        return;
    }
    else if (pipeline->shouldDraw(_material.getPtr())) {
        bg::ptr<Material> curMaterial = pipeline->effect()->materialPtr();
        pipeline->effect()->setMaterial(_material.getPtr());
        bg::base::MatrixState & state = pipeline->effect()->matrixState();
        state.viewMatrixStack().push();
        state.modelMatrixStack().push();
        
        state.viewMatrixStack().matrix().setPosition(0.0f, 0.0f, 0.0f);
        
		auto dt = pipeline->depthTest();
		pipeline->setDepthTest(false);
        auto scale = 5.0f;
        state.modelMatrixStack().scale(scale,scale,scale);
        
        size_t index = 0;
        for (auto & elem : _cube) {
            Texture * t = _textures[index++].getPtr();
            _material->setTexture(t);
            pipeline->draw(elem.getPtr());
        }
        
		pipeline->setDepthTest(dt);
        state.modelMatrixStack().pop();
        state.viewMatrixStack().pop();
        pipeline->effect()->setMaterial(curMaterial.getPtr());
    }
}
    
void Skybox::deserialize(bg::base::Context * context, bg::db::json::Value * componentData, const bg::system::Path & path) {
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
		loadSkybox(context);
	}
	catch (std::runtime_error &) {
		// This may fail if the scene is in legacy format
	}
	
}

bg::db::json::Value * Skybox::serialize(bg::base::Context *, const bg::system::Path & path) {
	using namespace bg::db::json;
	using namespace bg::base;
	Value * data = new Value();

	data->setValue("type", "Skybox");
	
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
    
void Skybox::createCube(bg::base::Context * ctx) {
    using namespace bg::base;
    
    static std::vector<float> backFace       = {  0.5f,-0.5f,-0.5f, -0.5f,-0.5f,-0.5f, -0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f };
    static std::vector<float> rightFace      = {  0.5f,-0.5f, 0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f };
    static std::vector<float> frontFace      = { -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
    static std::vector<float> leftFace       = { -0.5f,-0.5f,-0.5f, -0.5f,-0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f };
    static std::vector<float> topFace        = { -0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f };
    static std::vector<float> bottomFace     = {  0.5f,-0.5f, 0.5f, -0.5f,-0.5f, 0.5f, -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f };
    
    static std::vector<float> backFaceNorm   = { 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f };
    static std::vector<float> rightFaceNorm  = {-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f };
    static std::vector<float> frontFaceNorm  = { 0.0f, 0.0f,-1.0f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,-1.0f };
    static std::vector<float> leftFaceNorm   = { 1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f };
    static std::vector<float> topFaceNorm    = { 0.0f,-1.0f, 0.0f,  0.0f,-1.0f, 0.0f,  0.0f,-1.0f, 0.0f,  0.0f,-1.0f, 0.0f };
    static std::vector<float> bottomFaceNorm = { 0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f };
    
    static float uv0 = 0.0f;
    static float uv1 = 1.0f;
    static std::vector<float> backFaceUV     = { uv1,uv0, uv0,uv0, uv0,uv1, uv1,uv1 };
    static std::vector<float> rightFaceUV    = { uv1,uv0, uv0,uv0, uv0,uv1, uv1,uv1 };
    static std::vector<float> frontFaceUV    = { uv1,uv0, uv0,uv0, uv0,uv1, uv1,uv1 };
    static std::vector<float> leftFaceUV     = { uv1,uv0, uv0,uv0, uv0,uv1, uv1,uv1 };
    static std::vector<float> topFaceUV      = { uv1,uv0, uv0,uv0, uv0,uv1, uv1,uv1 };
    static std::vector<float> bottomFaceUV   = { uv1,uv0, uv0,uv0, uv0,uv1, uv1,uv1 };
    
    static std::vector<uint32_t> index = { 2,1,0, 0,3,2 };
    
    PolyList * backPlist    = new PolyList(ctx);
    PolyList * rightPlist   = new PolyList(ctx);
    PolyList * frontPlist   = new PolyList(ctx);
    PolyList * leftPlist    = new PolyList(ctx);
    PolyList * topPlist     = new PolyList(ctx);
    PolyList * bottomPlist  = new PolyList(ctx);
    
    backPlist->setData(backFace, backFaceNorm, backFaceUV, index);
    backPlist->build();
    
    rightPlist->setData(rightFace, rightFaceNorm, rightFaceUV, index);
    rightPlist->build();
    
    frontPlist->setData(frontFace, frontFaceNorm, frontFaceUV, index);
    frontPlist->build();
    
    leftPlist->setData(leftFace, leftFaceNorm, leftFaceUV, index);
    leftPlist->build();
    
    topPlist->setData(topFace, topFaceNorm, topFaceUV, index);
    topPlist->build();
    
    bottomPlist->setData(bottomFace, bottomFaceNorm, bottomFaceUV, index);
    bottomPlist->build();
    
    _cube.push_back(leftPlist);
    _cube.push_back(rightPlist);
    _cube.push_back(topPlist);
    _cube.push_back(bottomPlist);
    _cube.push_back(frontPlist);
    _cube.push_back(backPlist);
    
    _material = new Material();
    _material->setReceiveShadows(false);
    _material->setCastShadows(false);
    _material->setUnlit(true);
}

}
}
