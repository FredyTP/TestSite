
#include "vr_model.hpp"
#include <thread>
#include <chrono>

#include <bg/tools/texture_cache.hpp>
#include <bg/scene/camera.hpp>

VRModelFactory::VRModelFactory(bg::base::Context * ctx)
	:ContextObject(ctx)
{
}

bg::scene::Drawable * VRModelFactory::loadModel(const std::string & name) {
	bg::ptr<bg::scene::Drawable> result = nullptr;
	vr::RenderModel_t * vrModel;
	vr::EVRRenderModelError error;
	while (true) {
		error = vr::VRRenderModels()->LoadRenderModel_Async(name.c_str(), &vrModel);
		if (error!=vr::VRRenderModelError_Loading) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	if (error!=vr::VRRenderModelError_None) {
		throw bg::base::InitException("Unable to load render model " + name + ": " +
			vr::VRRenderModels()->GetRenderModelErrorNameFromEnum(error));
	}

	vr::RenderModel_TextureMap_t * pTexture;
	while (true) {
		error = vr::VRRenderModels()->LoadTexture_Async(vrModel->diffuseTextureId, &pTexture);
		if (error!=vr::VRRenderModelError_Loading) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	if (error!=vr::VRRenderModelError_None) {
		vr::VRRenderModels()->FreeRenderModel(vrModel);
		throw bg::base::InitException("Unable to load render texture for model " + name);
	}

	result = new bg::scene::Drawable();
	addPolyList(result.getPtr(), vrModel, pTexture);
	vr::VRRenderModels()->FreeRenderModel(vrModel);
	vr::VRRenderModels()->FreeTexture(pTexture);

	return result.release();
}

void VRModelFactory::addPolyList(bg::scene::Drawable * drw, vr::RenderModel_t * model, vr::RenderModel_TextureMap_t * tex) {
	using namespace bg::math;
	bg::ptr<bg::base::PolyList> plist = new bg::base::PolyList(context());

	for (uint32_t i=0; i<model->unVertexCount; ++i) {
		const vr::RenderModel_Vertex_t & v = model->rVertexData[i];
		plist->addVertex(Vector3(v.vPosition.v[0], v.vPosition.v[1], v.vPosition.v[2]));
		plist->addNormal(Vector3(v.vNormal.v[0],v.vNormal.v[1],v.vNormal.v[2]));
		plist->addTexCoord0(Vector2(v.rfTextureCoord[0],v.rfTextureCoord[1]));
	}

	for (uint32_t i=0; i<model->unTriangleCount * 3; i+=3) {
		plist->addTriangle(static_cast<unsigned int>(model->rIndexData[i]),
						   static_cast<unsigned int>(model->rIndexData[i + 1]),
						   static_cast<unsigned int>(model->rIndexData[i + 2]));
	}

	plist->build();
	bg::ptr<bg::base::Material> mat = new bg::base::Material();
	bg::ptr<bg::base::Texture> texture = new bg::base::Texture(context());
	texture->createWithBuffer(tex->rubTextureMapData, tex->unWidth, tex->unHeight, 4);
	mat->setTexture(texture.getPtr());
	drw->addPolyList(plist.getPtr(), mat.getPtr());
}
