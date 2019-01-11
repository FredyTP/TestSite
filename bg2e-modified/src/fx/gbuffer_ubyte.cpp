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


#include <bg/fx/gbuffer.hpp>

#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/engine/openglCore/shader.hpp>
#include <bg/engine/openglEs/shader.hpp>

#include <bg/tools/texture_cache.hpp>
#include <bg/tools/shader_source.hpp>

#include <bg/shader/utils.hpp>


namespace bg {
namespace fx {

GBufferUByte::GBufferUByte(bg::base::Context * ctx)
	:GBuffer(ctx)
{
}

GBufferUByte::~GBufferUByte() {
}

void GBufferUByte::activateGL(bool mobile) {
	_glShader->setActive();

	bg::math::Matrix4 & model = matrixState().modelMatrixStack().matrix();
	bg::math::Matrix4 & view = matrixState().viewMatrixStack().matrix();
	bg::math::Matrix4 & proj = matrixState().projectionMatrixStack().matrix();

	_glShader->setUniform("inModelMatrix", model);
	_glShader->setUniform("inViewMatrix", view);
	_glShader->setUniform("inProjectionMatrix", proj);
	_glShader->setUniform("inNormalMatrix", matrixState().normalModelViewMatrix());

	int textureUnit = 0;
	bg::base::Texture * whiteTexture = tools::TextureCache::WhiteTexture(context());
	bg::base::Texture * normalTexture = tools::TextureCache::NormalTexture(context());
	_glShader->setUniform("inDiffuseColor", _material->diffuse());
	_glShader->setUniform("inSpecularColor", _material->specular());

	_glShader->setUniform("inShininess", _material->shininess());
	_glShader->setUniform("inShininessMask", _material->shininessMask() ? _material->shininessMask() : whiteTexture, textureUnit++);
	_glShader->setUniform("inShininessMaskChannel", base::Material::GetMaskVector(_material->shininessMaskChannel()));
	_glShader->setUniform("inShininessMaskInvert", _material->invertShininessMask());

	_glShader->setUniform("inLightEmission", _material->lightEmission());
	_glShader->setUniform("inLightEmissionMask", _material->lightEmissionMask() ? _material->lightEmissionMask() : whiteTexture, textureUnit++);
	_glShader->setUniform("inLightEmissionMaskChannel", base::Material::GetMaskVector(_material->lightEmissionMaskChannel()));
	_glShader->setUniform("inLightEmissionMaskInvert", _material->invertLightEmissionMask());

	_glShader->setUniform("inTexture", _material->texture() ? _material->texture() : whiteTexture, textureUnit++);
	_glShader->setUniform("inTextureOffset", _material->textureOffset());
	_glShader->setUniform("inTextureScale", _material->textureScale());

	_glShader->setUniform("inAlphaCutoff", _material->alphaCutoff());

	_glShader->setUniform("inLightMap", _material->lightMap() ? _material->lightMap() : whiteTexture, textureUnit++);
	_glShader->setUniform("inLightMapOffset", _material->lightMapOffset());
	_glShader->setUniform("inLightMapScale", _material->lightMapScale());

	_glShader->setUniform("inNormalMap", _material->normalMap() ? _material->normalMap() : normalTexture, textureUnit++);
	_glShader->setUniform("inNormalMapOffset", _material->normalMapOffset());
	_glShader->setUniform("inNormalMapScale", _material->normalMapScale());

	_glShader->setUniform("inReflection", _material->reflectionAmount());
	_glShader->setUniform("inReflectionMask", _material->reflectionMask() ? _material->reflectionMask() : whiteTexture, textureUnit++);
	_glShader->setUniform("inReflectionMaskChannel", base::Material::GetMaskVector(_material->reflectionMaskChannel()));
	_glShader->setUniform("inReflectionMaskInvert", _material->invertReflectionMask());

	_glShader->setUniform("inRoughness", _material->roughness());
	_glShader->setUniform("inRoughnessMask", _material->roughnessMask() ? _material->roughnessMask() : whiteTexture, textureUnit++);
	_glShader->setUniform("inRoughnessMaskChannel", base::Material::GetMaskVector(_material->roughnessMaskChannel()));
	_glShader->setUniform("inRoughnessMaskInvert", _material->invertRoughnessMask());

	_glShader->setUniform("inUnlit", _material->unlit());


	_glShader->setUniform("inCastShadows", _material->castShadows());
}

void GBufferUByte::getVertexShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	
	src.precision(kHighp, kFloat);
	src.addShaderInput(Parameter(kFloat3, "inVertex", kRoleVertex));
	src.addShaderInput(Parameter(kFloat3, "inNormal", kRoleNormal));
	src.addShaderInput(Parameter(kFloat2, "inTex0", kRoleTex0));
	src.addShaderInput(Parameter(kFloat2, "inTex1", kRoleTex1));
	src.addShaderInput(Parameter(kFloat3, "inTangent", kRoleTangent));
	
	src.addUniform({
		Parameter(kMatrix4, "inModelMatrix"),
		Parameter(kMatrix4, "inViewMatrix"),
		Parameter(kMatrix4, "inProjectionMatrix"),
		Parameter(kMatrix4, "inNormalMatrix")
	});
	
	src.addShaderOutput({
		Parameter(kFloat3, "fsPosition"),
		Parameter(kFloat2, "fsTex0Coord"),
		Parameter(kFloat2, "fsTex1Coord"),
		Parameter(kFloat3, "fsNormal"),
		Parameter(kFloat3, "fsTangent"),
		Parameter(kFloat3, "fsBitangent")
	});
	
	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		
		// TODO: 	Multiply view matrix in tesselation evaluation shader, when tessellation is implemented
		//"vec4 viewPos = inModelMatrix * vec4(inVertex, 1.0);",
		//"gl_Position = inProjectionMatrix * inViewMatrix * viewPos;"
		"vec4 viewPos = inViewMatrix * inModelMatrix * vec4(inVertex, 1.0);",
		"gl_Position = inProjectionMatrix * viewPos;",
		
		"fsNormal = normalize((inNormalMatrix * vec4(inNormal, 1.0)).xyz);",
		"fsTangent = normalize((inNormalMatrix * vec4(inTangent, 1.0)).xyz);",
		"fsBitangent = cross(fsNormal, fsTangent);",
		
		"fsTex0Coord = inTex0;",
		"fsTex1Coord = inTex1;",
		"fsPosition = viewPos.xyz;"
	});
	src.endFunction();
}

void GBufferUByte::getFragmentShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	
	src.precision(kHighp, kFloat);
	
	//src.requireExtension("GL_EXT_draw_buffers");
	
	
	src.addShaderOutput(Parameter(kFloat4, "out_Diffuse", 0));
	src.addShaderOutput(Parameter(kFloat4, "out_Specular", 1));
	src.addShaderOutput(Parameter(kFloat4, "out_Normal", 2));
	src.addShaderOutput(Parameter(kFloat4, "out_Material", 3));
	
	src.addUniform({
		Parameter(kFloat4, "inDiffuseColor"),
		Parameter(kFloat4, "inSpecularColor"),
		Parameter(kFloat, "inShininess"),
		Parameter(kSampler2D, "inShininessMask"),
		Parameter(kFloat4, "inShininessMaskChannel"),
		Parameter(kBool, "inShininessMaskInvert"),
		Parameter(kFloat, "inLightEmission"),
		Parameter(kSampler2D, "inLightEmissionMask"),
		Parameter(kFloat4, "inLightEmissionMaskChannel"),
		Parameter(kBool, "inLightEmissionMaskInvert"),
		Parameter(kSampler2D, "inTexture"),
		Parameter(kFloat2, "inTextureOffset"),
		Parameter(kFloat2, "inTextureScale"),
		Parameter(kFloat, "inAlphaCutoff"),
		Parameter(kSampler2D, "inLightMap"),
		Parameter(kFloat2, "inLightMapOffset"),
		Parameter(kFloat2, "inLightMapScale"),
		Parameter(kSampler2D, "inNormalMap"),
		Parameter(kFloat2, "inNormalMapOffset"),
		Parameter(kFloat2, "inNormalMapScale"),
		Parameter(kFloat, "inReflection"),
		Parameter(kSampler2D, "inReflectionMask"),
		Parameter(kFloat4, "inReflectionMaskChannel"),
		Parameter(kBool, "inReflectionMaskInvert"),
		Parameter(kBool, "inCastShadows"),
		Parameter(kBool, "inReceiveShadows"),
		Parameter(kFloat, "inRoughness"),
		Parameter(kSampler2D, "inRoughnessMask"),
		Parameter(kFloat4, "inRoughnessMaskChannel"),
		Parameter(kBool, "inRoughnessMaskInvert"),
		Parameter(kBool, "inUnlit")
	});
	
	src.addShaderInput({
		Parameter(kFloat3, "fsPosition"),
		Parameter(kFloat2, "fsTex0Coord"),
		Parameter(kFloat2, "fsTex1Coord"),
		Parameter(kFloat3, "fsNormal"),
		Parameter(kFloat3, "fsTangent"),
		Parameter(kFloat3, "fsBitangent")
	});
	
	bg::shader::Utils(&src)
	.samplerColor()
	.samplerNormal()
	.combineNormalWithMap()
	.applyTextureMask()
	.specularColor();
	
	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"vec4 lightMap = samplerColor(inLightMap,fsTex1Coord,inLightMapOffset,inLightMapScale);",
		"vec4 diffuse = samplerColor(inTexture,fsTex0Coord,inTextureOffset,inTextureScale) * inDiffuseColor * lightMap;",
		"if (diffuse.a >= inAlphaCutoff) {",
		"	vec3 normal = samplerNormal(inNormalMap,fsTex0Coord,inNormalMapOffset,inNormalMapScale);",
		"	normal = combineNormalWithMap(fsNormal,fsTangent,fsBitangent,normal);",
		"	vec4 specular = specularColor(inSpecularColor,inShininessMask,fsTex0Coord,inTextureOffset,"
										 "inTextureScale,inShininessMaskChannel,inShininessMaskInvert);",
		"	float lightEmission = applyTextureMask(inLightEmission,"
										"inLightEmissionMask,fsTex0Coord,inTextureOffset,inTextureScale,"
										"inLightEmissionMaskChannel, inLightEmissionMaskInvert);",
		"	float reflectionMask = applyTextureMask(inReflection,"
												"inReflectionMask,fsTex0Coord,inTextureOffset,inTextureScale,"
												"inReflectionMaskChannel,inReflectionMaskInvert);",
		"	float roughnessMask = applyTextureMask(inRoughness,"
												"inRoughnessMask,fsTex0Coord,inTextureOffset,inTextureScale,"
												"inRoughnessMaskChannel,inRoughnessMaskInvert);",
		"	out_Diffuse = diffuse;",
		"	out_Specular = vec4(specular.rgb,roughnessMask);",
		"	if (!gl_FrontFacing) {",
		"		normal *= -1.0;",
		"	}",
		"	out_Normal = vec4(normal * 0.5 + 0.5, inUnlit ? 0.0 : 1.0);",
		"	out_Material = vec4(lightEmission,inShininess/255.0,reflectionMask,float(inCastShadows));",
		"}",
		"else {",
		"	discard;",
		"}"
	});
	src.endFunction();
}

}
}
