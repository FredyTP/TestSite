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


#include <bg/base/material.hpp>

#include <bg/log.hpp>

#include <bg/db/json/value.hpp>

#include <bg/db/texture.hpp>

#include <iostream>

#include <regex>

namespace bg {
namespace base {

namespace bg2mat {
	bg::base::Texture * loadTexture(bg::base::Context * ctx, const std::string & path, const std::string & basePath) {
		if (path.empty()) {
			return nullptr;
		}

		std::regex base64RE("base64", std::regex_constants::ECMAScript | std::regex_constants::icase);
		if (std::regex_search(path, base64RE)) {
			bg::log(bg::log::kWarning) << "The base64 textures are not supported in C++ API" << bg::endl;
			return nullptr;
		}

		bg::system::Path fullPath(basePath);
		fullPath.addComponent(path);
		
		bg::ptr<bg::base::Texture> tex;
		try {
			tex = bg::db::loadTexture(ctx, fullPath);
		}
		catch (std::runtime_error & err) {
			bg::log(bg::log::kWarning) << "Error loading material definition texture: " << err.what() << bg::endl;
		}

		return tex.release();
	}
}

Material * Material::FromMaterialDefinition(bg::base::Context * ctx, bg::db::json::Value * val, const std::string & basePath) {
	using namespace bg::db::json;
	using namespace bg::math;
	bg::ptr<bg::base::Material> mat = new Material();

	mat->setDiffuse(Value::Color((*val)["diffuse"], Color::White()));
	mat->setSpecular(Value::Color((*val)["specular"], Color::White()));

	mat->setShininess(Value::Float((*val)["shininess"], 0.0f));
	mat->setShininessMaskChannel(Value::Int((*val)["shininessMaskChannel"], 0));
	mat->setInvertShininessMask(Value::Bool((*val)["shininessMaskInvert"], false));

	mat->setLightEmission(Value::Float((*val)["lightEmission"], 0.0f));
	mat->setLightEmissionMaskChannel(Value::Int((*val)["lightEmissionMaskChannel"], 0));
	mat->setInvertLightEmissionMask(Value::Bool((*val)["lightEmissionMaskInvert"], false));

	mat->setRefractionAmount(Value::Float((*val)["refractionAmount"], 0.0f));
	mat->setReflectionAmount(Value::Float((*val)["reflectionAmount"], 0.0f));
	mat->setReflectionMaskChannel(Value::Int((*val)["reflectionMaskChannel"], 0));
	mat->setInvertReflectionMask(Value::Bool((*val)["reflectionMaskInvert"], false));

	mat->setTextureOffset(Value::Vec2((*val)["textureOffset"], Vector2(0.0f)));
	mat->setTextureScale(Value::Vec2((*val)["textureScale"], Vector2(1.0f)));

	mat->setNormalMapOffset(Value::Vec2((*val)["normalMapOffset"], Vector2(0.0f)));
	mat->setNormalMapScale(Value::Vec2((*val)["normalMapScale"], Vector2(1.0f)));

	mat->setCullFace(Value::Bool((*val)["cullFace"], true));
	mat->setCastShadows(Value::Bool((*val)["castShadows"], true));
	mat->setReceiveShadows(Value::Bool((*val)["receiveShadows"], true));
	mat->setAlphaCutoff(Value::Float((*val)["alphaCutoff"], 0.5f));

	mat->setRoughness(Value::Float((*val)["roughness"], 0.0f));
	mat->setRoughnessMaskChannel(Value::Int((*val)["roughnessMaskChannel"], 0));
	mat->setInvertRoughnessMask(Value::Bool((*val)["invertRoughnessMask"], false));

	mat->setUnlit(Value::Bool((*val)["unlit"], false));

	// Load texture maps
	mat->setShininessMask(bg2mat::loadTexture(ctx, Value::String((*val)["shininessMask"]), basePath));
	mat->setLightEmissionMask(bg2mat::loadTexture(ctx, Value::String((*val)["lightEmissionMask"]), basePath));
	mat->setReflectionMask(bg2mat::loadTexture(ctx, Value::String((*val)["reflectionMask"]), basePath));
	mat->setTexture(bg2mat::loadTexture(ctx, Value::String((*val)["texture"]), basePath));
	mat->setNormalMap(bg2mat::loadTexture(ctx, Value::String((*val)["normalMap"]), basePath));
	mat->setRoughnessMask(bg2mat::loadTexture(ctx, Value::String((*val)["roughnessMask"]), basePath));
	
	return mat.release();
}

Material::Material()
	:_diffuse(0.9f,0.9f,0.9f,1.0f)
	,_specular(1.0f)
	,_shininess(0.0f)
	,_lightEmission(0.0f)
	,_refractionAmount(0.0f)
	,_reflectionAmount(0.0f)
	,_textureOffset(0.0f)
	,_textureScale(1.0f)
	,_lightMapOffset(0.0f)
	,_lightMapScale(1.0f)
	,_normalMapOffset(0.0f)
	,_normalMapScale(1.0f)
	,_castShadows(true)
	,_receiveShadows(true)
	,_alphaCutoff(0.5f)
	,_shininessMaskChannel(0)
	,_shininessMaskInvert(false)
	,_lightEmissionMaskChannel(0)
	,_lightEmissionMaskInvert(false)
	,_displacementFactor(0.2f)
	,_displacementUV(0)
	,_tessellationDistances(8.0f, 15.0f, 30.0f, 40.0f)
	,_tessellationLevels(1.0f)
	,_reflectionMaskChannel(0)
	,_reflectionMaskInvert(false)
	,_cullFace(true)
	,_unlit(false)
	,_roughness(0.0f)
	,_roughnessMaskChannel(0)
	,_roughnessMaskInvert(false)
	,_processAsTransparent(false)
{
	
}

Material::Material(Material * clone)
	:_diffuse(clone->_diffuse)
	,_specular(clone->_specular)
	,_shininess(clone->_shininess)
	,_lightEmission(clone->_lightEmission)
	,_refractionAmount(clone->_refractionAmount)
	,_reflectionAmount(clone->_reflectionAmount)
	,_texture(clone->_texture)
	,_lightMap(clone->_lightMap)
	,_normalMap(clone->_normalMap)
	,_textureOffset(clone->_textureOffset)
	,_textureScale(clone->_textureScale)
	,_lightMapOffset(clone->_lightMapOffset)
	,_lightMapScale(clone->_lightMapScale)
	,_normalMapOffset(clone->_normalMapOffset)
	,_normalMapScale(clone->_normalMapScale)
	,_castShadows(clone->_castShadows)
	,_receiveShadows(clone->_receiveShadows)
	,_alphaCutoff(clone->_alphaCutoff)
	,_shininessMask(clone->_shininessMask)
	,_shininessMaskChannel(clone->_shininessMaskChannel)
	,_shininessMaskInvert(clone->_shininessMaskInvert)
	,_lightEmissionMask(clone->_lightEmissionMask)
	,_lightEmissionMaskChannel(clone->_lightEmissionMaskChannel)
	,_lightEmissionMaskInvert(clone->_lightEmissionMaskInvert)
	,_displacementFactor(clone->_displacementFactor)
	,_displacementUV(clone->_displacementUV)
	,_displacementMap(clone->_displacementMap)
	,_tessellationDistances(clone->_tessellationDistances)
	,_tessellationLevels(clone->_tessellationLevels)
	,_reflectionMask(clone->_reflectionMask)
	,_reflectionMaskChannel(clone->_reflectionMaskChannel)
	,_reflectionMaskInvert(clone->_reflectionMaskInvert)
	,_cullFace(clone->_cullFace)
	,_unlit(clone->_unlit)
	,_roughness(clone->_roughness)
	,_roughnessMask(clone->_roughnessMask)
	,_roughnessMaskChannel(clone->_roughnessMaskChannel)
	,_roughnessMaskInvert(clone->_roughnessMaskInvert)
	,_processAsTransparent(clone->_processAsTransparent)
{
	
}

Material::~Material() {
}


}
}
