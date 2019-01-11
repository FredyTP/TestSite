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


#include <bg/tools/material_tools.hpp>

#include <bg/db/json/parser.hpp>
#include <bg/db/json/value.hpp>

#include <bg/db/texture.hpp>

namespace bg {
namespace tools {

MaterialTools::MaterialTools(bg::base::Material * target)
	:ToolBase(target)
{
		
}

MaterialTools & MaterialTools::clone() {
	assertTarget();
	_target = new bg::base::Material(_target.getPtr());
	return *this;
}

MaterialTools & MaterialTools::resources(std::function<void(const std::string & file)> cb) {
	assertTarget();
	std::string f;
	if (!(f = bg::base::Texture::FileName(_target->texture())).empty()) cb (f);
	if (!(f = bg::base::Texture::FileName(_target->lightMap())).empty()) cb (f);
	if (!(f = bg::base::Texture::FileName(_target->normalMap())).empty()) cb (f);
	if (!(f = bg::base::Texture::FileName(_target->shininessMask())).empty()) cb (f);
	if (!(f = bg::base::Texture::FileName(_target->lightEmissionMask())).empty()) cb (f);
	if (!(f = bg::base::Texture::FileName(_target->displacementMap())).empty()) cb (f);
	if (!(f = bg::base::Texture::FileName(_target->reflectionMask())).empty()) cb (f);
	if (!(f = bg::base::Texture::FileName(_target->roughnessMask())).empty()) cb (f);
	return *this;
}

bg::math::Color getColor(bg::db::json::Value * value, const std::string & r, const std::string & g, const std::string & b, const std::string & a) {
	using namespace bg::db::json;
	bg::math::Color color = bg::math::Color::White();
	if (value) {
		color.x(Value::Float((*value)[r], 1.0f));
		color.y(Value::Float((*value)[g], 1.0f));
		color.z(Value::Float((*value)[b], 1.0f));
		color.w(Value::Float((*value)[a], 1.0f));
	}
	return color;
}

bg::math::Vector2 getVec2(bg::db::json::Value * value, const std::string & x, const std::string & y, const bg::math::Vector2 & def = bg::math::Vector2()) {
	using namespace bg::db::json;
	bg::math::Vector2 result;
	if (value) {
		result.x(Value::Float((*value)[x], def.x()));
		result.y(Value::Float((*value)[y], def.y()));
	}
	return result;
}

bg::math::Vector3 getVec3(bg::db::json::Value * value, const std::string & x, const std::string & y, const std::string & z, const bg::math::Vector3 & def = bg::math::Vector3()) {
	using namespace bg::db::json;
	bg::math::Vector3 result;
	if (value) {
		result.x(Value::Float((*value)[x], def.x()));
		result.y(Value::Float((*value)[y], def.y()));
		result.z(Value::Float((*value)[z], def.z()));
	}
	return result;
}

bg::math::Vector4 getVec4(bg::db::json::Value * value, const std::string & x, const std::string & y, const std::string & z, const std::string & w, const bg::math::Vector4 & def = bg::math::Vector4()) {
	using namespace bg::db::json;
	bg::math::Vector4 result;
	if (value) {
		result.x(Value::Float((*value)[x], def.x()));
		result.y(Value::Float((*value)[y], def.y()));
		result.z(Value::Float((*value)[z], def.z()));
		result.w(Value::Float((*value)[w], def.w()));
	}
	return result;
}

bg::base::Texture * loadTexture(bg::base::Context * ctx, const std::string & file, const bg::system::Path & path) {
	if (file=="") return nullptr;
	bg::system::Path filePath(file);
	if (filePath.isAbsolute()) {
		return bg::db::loadTexture(ctx, filePath);
	}
	else {
		return bg::db::loadTexture(ctx, path.pathAddingComponent(file));
	}
}

bg::base::Material * parseMaterialItem(bg::base::Context * ctx, bg::db::json::Value * value, const bg::system::Path & filePath, std::string & outName) {
	using namespace bg::db::json;
	bg::ptr<bg::base::Material> mat = new bg::base::Material();
	
	if (value && value->type()==Value::kObject) {
		outName = Value::String((*value)["name"],"default-material");
		mat->setDiffuse(getColor(value, "diffuseR", "diffuseG", "diffuseB", "diffuseA"));
		mat->setSpecular(getColor(value, "specularR", "specularG", "specularB", "specularA"));
		mat->setShininess(Value::Float((*value)["shininess"]));
		
		
		mat->setRefractionAmount(Value::Float((*value)["refractionAmount"]));
		mat->setReflectionAmount(Value::Float((*value)["reflectionAmount"]));

		mat->setLightEmission(Value::Float((*value)["lightEmission"]));
		
		mat->setTexture(loadTexture(ctx,Value::String((*value)["texture"]),filePath));
		mat->setLightMap(loadTexture(ctx,Value::String((*value)["lightmap"]),filePath));
		mat->setNormalMap(loadTexture(ctx,Value::String((*value)["normalMap"]),filePath));
		mat->setShininessMask(loadTexture(ctx,Value::String((*value)["shininessMask"]),filePath));
		mat->setLightEmissionMask(loadTexture(ctx,Value::String((*value)["lightEmissionMask"]),filePath));
		mat->setReflectionMask(loadTexture(ctx,Value::String((*value)["reflectionMask"]),filePath));
		mat->setRoughnessMask(loadTexture(ctx, Value::String((*value)["roughnessMask"]), filePath));
		
		
		mat->setTextureOffset(getVec2(value,"textureOffsetX","textureOffsetY"));
		mat->setTextureScale(getVec2(value,"textureScaleX","textureScaleY",bg::math::Vector2(1.0f)));
		mat->setLightMapOffset(getVec2(value,"lightmapOffsetX","lightmapOffsetY"));
		mat->setLightMapScale(getVec2(value,"lightmapScaleX","lightmapScaleY",bg::math::Vector2(1.0f)));
		mat->setNormalMapOffset(getVec2(value,"normalMapOffsetX","normalMapOffsetY"));
		mat->setNormalMapScale(getVec2(value,"normalMapScaleX","normalMapScaleY", bg::math::Vector2(1.0f)));
		mat->setCastShadows(Value::Bool((*value)["castShadows"], true));
		mat->setReceiveShadows(Value::Bool((*value)["receiveShadows"], true));
		mat->setAlphaCutoff(Value::Float((*value)["alphaCutoff"],0.5f));
		
		
		mat->setShininessMaskChannel(Value::Int((*value)["shininessMaskChannel"]));
		mat->setInvertShininessMask(Value::Bool((*value)["invertShininessMask"],true));
		
		
		
		mat->setLightEmissionMaskChannel(Value::Int((*value)["lightEmissionMaskChannel"]));
		mat->setInvertLightEmissionMask(Value::Bool((*value)["invertLightEmissionMask"],true));
		
		mat->setDisplacementFactor(Value::Float((*value)["displacementFactor"],0.2f));
		mat->setDisplacementUV(Value::Int((*value)["displacementUV"]));

		
		mat->setTessellationDistances(getVec4(value, "tessDistanceFarthest", "tessDistanceFar", "tessDistanceNear", "tessDistanceNearest",
											  bg::math::Vector4(40.0f,30.0f,15.0f,8.0f)));

		mat->setTessellationLevels(getVec4(value,"tessFarthestLevel","tessFarLevel","tessNearLevel","tessNearestLevel",
										   bg::math::Vector4(1.0f)));
		
		mat->setReflectionMaskChannel(Value::Int((*value)["reflectionMaskChannel"]));
		mat->setInvertReflectionMask(Value::Bool((*value)["invertReflectionMask"],false));

		mat->setRoughness(Value::Float((*value)["roughness"], 0.0f));
		mat->setRoughnessMaskChannel(Value::Int((*value)["roughnessMaskChannel"], 0));
		mat->setInvertRoughnessMask(Value::Bool((*value)["invertRoughnessMask"], false));

		mat->setUnlit(Value::Bool((*value)["unlit"], false));

		mat->setCullFace(Value::Bool((*value)["cullFace"],true));
		
		mat->setProcessAsTransparent(Value::Bool((*value)["processAsTransparent"],false));
	}
	
	return mat.release();
}

MaterialTools & MaterialTools::copyPropertiesOf(bg::base::Material *other, uint32_t properties) {
	using namespace bg::base;
	assertTarget();
	
	if ((properties & Material::kDiffuse)!=0) _target->setDiffuse(other->diffuse());
	if ((properties & Material::kSpecular)!=0) _target->setSpecular(other->specular());
	if ((properties & Material::kShininess)!=0) _target->setShininess(other->shininess());
	if ((properties & Material::kLightEmission)!=0) _target->setLightEmission(other->lightEmission());
	if ((properties & Material::kRefractionAmount)!=0) _target->setRefractionAmount(other->refractionAmount());
	if ((properties & Material::kReflectionAmount)!=0) _target->setReflectionAmount(other->reflectionAmount());
	if ((properties & Material::kTexture)!=0) _target->setTexture(other->texture());
	if ((properties & Material::kLightMap)!=0) _target->setLightMap(other->lightMap());
	if ((properties & Material::kNormalMap)!=0) _target->setNormalMap(other->normalMap());
	if ((properties & Material::kTextureOffset)!=0) _target->setTextureOffset(other->textureOffset());
	if ((properties & Material::kTextureScale)!=0) _target->setTextureScale(other->textureScale());
	if ((properties & Material::kLightMapOffset)!=0) _target->setLightMapOffset(other->lightMapOffset());
	if ((properties & Material::kLightMapScale)!=0) _target->setLightMapScale(other->lightMapScale());
	if ((properties & Material::kNormalMapOffset)!=0) _target->setNormalMapOffset(other->normalMapOffset());
	if ((properties & Material::kNormalMapScale)!=0) _target->setNormalMapScale(other->normalMapScale());
	if ((properties & Material::kCastShadows)!=0) _target->setCastShadows(other->castShadows());
	if ((properties & Material::kReceiveShadows)!=0) _target->setReceiveShadows(other->receiveShadows());
	if ((properties & Material::kAlphaCutoff)!=0) _target->setAlphaCutoff(other->alphaCutoff());
	if ((properties & Material::kShininessMask)!=0) {
		_target->setShininessMask(other->shininessMask());
		_target->setShininessMaskChannel(other->shininessMaskChannel());
		_target->setInvertShininessMask(other->invertShininessMask());
	}
	if ((properties & Material::kLightEmissionMask)!=0) {
		_target->setLightEmissionMask(other->lightEmissionMask());
		_target->setLightEmissionMaskChannel(other->lightEmissionMaskChannel());
		_target->setInvertLightEmissionMask(other->invertLightEmissionMask());
	}
	if ((properties & Material::kDisplacementFactor)!=0) _target->setDisplacementFactor(other->displacementFactor());
	if ((properties & Material::kDisplacementUV)!=0) _target->setDisplacementUV(other->displacementUV());
	if ((properties & Material::kDisplacementMap)!=0) _target->setDisplacementMap(other->displacementMap());
	if ((properties & Material::kTessellationDistances)!=0) _target->setTessellationDistances(other->tessellationDistances());
	if ((properties & Material::kTessellationLevels)!=0) _target->setTessellationLevels(other->tessellationLevels());
	if ((properties & Material::kReflectionMask)!=0) {
		_target->setReflectionMask(other->reflectionMask());
		_target->setReflectionMaskChannel(other->reflectionMaskChannel());
		_target->setInvertReflectionMask(other->invertReflectionMask());
	}
	if ((properties & Material::kCullFace)!=0) {
		_target->setCullFace(other->cullFace());
	}
	if ((properties & Material::kRoughness) != 0) {
		_target->setRoughness(other->roughness());
		_target->setRoughnessMask(other->roughnessMask());
		_target->setRoughnessMaskChannel(other->roughnessMaskChannel());
		_target->setInvertRoughnessMask(other->invertRoughnessMask());
	}
	if ((properties & Material::kUnlit) != 0) {
		_target->setUnlit(other->unlit());
	}
	if ((properties & Material::kProcessAsTransparent)!=0) {
		_target->setProcessAsTransparent(other->processAsTransparent());
	}
	
	return *this;
}

void MaterialTools::LoadMaterialsFromJson(bg::base::Context * ctx, const std::string & jsonString, const bg::system::Path & path, MaterialMap & materials) {
	using namespace bg::db::json;
	bg::ptr<Value> materialData = Parser::ParseString(jsonString);
	std::string name;
	bg::ptr<bg::base::Material> mat;
	
	// The json string contains an array of materials
	if (materialData.valid() && materialData->type()==Value::kArray) {
		for (auto materialItem : materialData->array()) {
			mat = parseMaterialItem(ctx, materialItem.getPtr(), path, name);
			materials[name] = mat.getPtr();
		}
	}
	// The json string contains a single material
	else if (materialData.valid() && materialData->type()==Value::kObject) {
		mat = parseMaterialItem(ctx, materialData.getPtr(), path, name);
		materials[name] = mat.getPtr();
	}
}

}
}
