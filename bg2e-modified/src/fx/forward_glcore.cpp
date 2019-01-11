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


#include <bg/fx/forward.hpp>

#include <bg/engine/openglCore/shader.hpp>
#include <bg/engine/openglEs/shader.hpp>

#include <bg/tools/texture_cache.hpp>
#include <bg/tools/shader_source.hpp>

#include <bg/shader/lighting.hpp>
#include <bg/shader/utils.hpp>
#include <bg/shader/color_correction.hpp>

#include <bg/scene/cubemap.hpp>

#include <iostream>

namespace bg {
namespace fx {

static int kMaxLights = 3;

void Forward::buildGL(bool mobile) {
	using namespace bg::tools;
	ShaderSource vshader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	ShaderSource fshader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	getVertexShader(vshader);
	getFragmentShader(fshader);
	_glShader = mobile ? new bg::engine::openglEs::Shader(context()) :
		new bg::engine::openglCore::Shader(context());

	_glShader->attachShader(bg::engine::openglCore::kVertexShader, vshader.text());
	_glShader->attachShader(bg::engine::openglCore::kFragmentShader, fshader.text());
	_glShader->link();
	_glShader->setOutputParameterName(bg::engine::openglCore::ShaderParamType::kShaderOutFragmentLocation, "fs_outColor");

	vshader.initVertexBufferInputs(_glShader.getPtr());

	std::vector<std::string> uniformNames;
	vshader.uniformNames(uniformNames);
	fshader.uniformNames(uniformNames);
	_glShader->initUniforms(uniformNames);
}

void Forward::activateGL(bool mobile) {
	_glShader->setActive();

	bg::math::Matrix4 & model = matrixState().modelMatrixStack().matrix();
	bg::math::Matrix4 & view = matrixState().viewMatrixStack().matrix();
	bg::math::Matrix4 & proj = matrixState().projectionMatrixStack().matrix();
	_glShader->setUniform("inModelMatrix", model);
	_glShader->setUniform("inViewMatrix", view);
	_glShader->setUniform("inProjectionMatrix", proj);
	_glShader->setUniform("inNormalMatrix", matrixState().normalModelViewMatrix());
	
	_glShader->setUniform("inLightProjectionMatrix", _lights.shadowProjection);
	_glShader->setUniform("inLightViewMatrix", _lights.shadowView);

	_glShader->setUniform("inDiffuseColor", _material->diffuse());
	_glShader->setUniform("inSpecularColor", _material->specular());
	
	bg::base::Texture * whiteTexture = tools::TextureCache::WhiteTexture(context());
	bg::base::Texture * normalTexture = tools::TextureCache::NormalTexture(context());
	_glShader->setUniform("inShininess", _material->shininess());
	_glShader->setUniform("inShininessMask", _material->shininessMask() ? _material->shininessMask():whiteTexture, 0);
	_glShader->setUniform("inShininessMaskChannel", base::Material::GetMaskVector(_material->shininessMaskChannel()));
	_glShader->setUniform("inShininessMaskInvert", _material->invertShininessMask());
	
	_glShader->setUniform("inLightEmission", _material->lightEmission());
	_glShader->setUniform("inLightEmissionMask", _material->lightEmissionMask() ? _material->lightEmissionMask():whiteTexture, 1);
	_glShader->setUniform("inLightEmissionMaskChannel", base::Material::GetMaskVector(_material->lightEmissionMaskChannel()));
	_glShader->setUniform("inLightEmissionMaskInvert", _material->invertLightEmissionMask());
	
	_glShader->setUniform("inTexture", _material->texture() ? _material->texture():whiteTexture,2);
	_glShader->setUniform("inTextureOffset", _material->textureOffset());
	_glShader->setUniform("inTextureScale", _material->textureScale());
	_glShader->setUniform("inAlphaCutoff", _material->alphaCutoff());
	
	_glShader->setUniform("inLightMap", _material->lightMap() ? _material->lightMap():whiteTexture, 3);
	_glShader->setUniform("inLightMapOffset", _material->lightMapOffset());
	_glShader->setUniform("inLightMapScale", _material->lightMapScale());
	
	_glShader->setUniform("inNormalMap", _material->normalMap() ? _material->normalMap():normalTexture,4);
	_glShader->setUniform("inNormalMapOffset", _material->normalMapOffset());
	_glShader->setUniform("inNormalMapScale", _material->normalMapScale());
	
	_glShader->setUniform("inReceiveShadows",_material->receiveShadows() && _lights.shadowMap.valid());
	
	bg::base::Texture * cm = bg::scene::Cubemap::Current(context());
	_glShader->setUniform("inCubeMap", cm, 5);
	_glShader->setUniform("inReflection", _material->reflectionAmount());
    _glShader->setUniform("inReflectionMask", _material->reflectionMask() ? _material->reflectionMask() : whiteTexture, 6);
	_glShader->setUniform("inReflectionMaskChannel", bg::base::Material::GetMaskVector(_material->reflectionMaskChannel()));
	_glShader->setUniform("inReflectionMaskInvert", _material->invertReflectionMask());
    
    _glShader->setUniform("inRoughness", _material->roughness());
    _glShader->setUniform("inRoughnessMask", _material->roughnessMask() ? _material->roughnessMask() : whiteTexture, 7);
    _glShader->setUniform("inRoughnessMaskChannel", base::Material::GetMaskVector(_material->roughnessMaskChannel()));
    _glShader->setUniform("inRoughnessMaskInvert", _material->invertRoughnessMask());

    
    _glShader->setUniform("inUnlit", _material->unlit());
	
	// Lighting
	_glShader->setUniform("inLightDirection", _lights.direction);
	_glShader->setUniform("inLightAmbient", _lights.ambient);
	_glShader->setUniform("inLightDiffuse", _lights.diffuse);
	_glShader->setUniform("inLightSpecular", _lights.specular);
	_glShader->setUniform("inLightAttenuation", _lights.attenuation);
	_glShader->setUniform("inLightPosition", _lights.position);
	_glShader->setUniform("inSpotCutoff", _lights.spotCutoff);
	_glShader->setUniform("inSpotExponent", _lights.spotExponent);
	_glShader->setUniform("inLightType", _lights.type);
    _glShader->setUniform("inLightEmissionFactor", 10.0f);

	bg::base::Texture * sm = _lights.shadowMap.valid() ? _lights.shadowMap.getPtr() : bg::tools::TextureCache::WhiteTexture(context());
	_glShader->setUniform("inShadowMap",  sm, 8);
	_glShader->setUniform("inShadowStrength", _lights.shadowStrength);
	_glShader->setUniform("inShadowColor", _lights.shadowColor);
	_glShader->setUniform("inShadowBias", _lights.shadowBias);
	_glShader->setUniform("inShadowType", _lights.shadowType);	// 0:hard, 1:soft, 2:stratified
	_glShader->setUniform("inNumLights", _lights.numLights());
	
	// TODO: color correction
	_glShader->setUniform("inHue", 1.0f);
	_glShader->setUniform("inSaturation", 1.0f);
	_glShader->setUniform("inLightness", 1.0f);
	_glShader->setUniform("inBrightness", 0.5f);
	_glShader->setUniform("inContrast", 0.5f);
}

void Forward::bindPolyListGL(bg::base::PolyList * plist, bool mobile) {
	_glShader->bindPolyList(plist);
}

void Forward::unbindGL(bool mobile) {
	_glShader->clearActive();
	_glShader->unbindPolyList();
}

void Forward::getVertexShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;

	src.precision(kHighp, kFloat);
	src.addShaderInput(Parameter(kFloat3, "inVertex", kRoleVertex));
	src.addShaderInput(Parameter(kFloat3, "inNormal", kRoleNormal));
	src.addShaderInput(Parameter(kFloat2, "inTex0", kRoleTex0));
	src.addShaderInput(Parameter(kFloat2, "inTex1", kRoleTex1));
	src.addShaderInput(Parameter(kFloat3, "inTangent", kRoleTangent));

	src.addUniform({ Parameter(kMatrix4, "inModelMatrix"),
					 Parameter(kMatrix4, "inViewMatrix"),
					 Parameter(kMatrix4, "inProjectionMatrix"),
					 Parameter(kMatrix4, "inNormalMatrix") });

	src.addUniform({ Parameter(kMatrix4, "inLightProjectionMatrix"),
					 Parameter(kMatrix4, "inLightViewMatrix") });

	src.addShaderOutput({ Parameter(kFloat3, "fsPosition"),
						  Parameter(kFloat2, "fsTex0Coord"),
						  Parameter(kFloat2, "fsTex1Coord"),
						  Parameter(kFloat3, "fsNormal"),
						  Parameter(kFloat3, "fsTangent"),
						  Parameter(kFloat3, "fsBitangent"),
						  Parameter(kFloat4, "fsVertexPosFromLight") });

	src.beginFunction(kVoid, "main", {});
	src.functionBody({
         "mat4 ScaleMatrix = mat4(0.5, 0.0, 0.0, 0.0,",
         "						0.0, 0.5, 0.0, 0.0,",
         "						0.0, 0.0, 0.5, 0.0,",
         "						0.5, 0.5, 0.5, 1.0);\n",

         "vec4 viewPos = inViewMatrix * inModelMatrix * vec4(inVertex, 1.0);\n",
         "gl_Position = inProjectionMatrix * viewPos;\n",

         "fsNormal = normalize((inNormalMatrix * vec4(inNormal, 1.0)).xyz);\n",
         "fsTangent = normalize((inNormalMatrix * vec4(inTangent, 1.0)).xyz);\n",
         "fsBitangent = cross(fsNormal,fsTangent);\n",

         "fsVertexPosFromLight = ScaleMatrix * inLightProjectionMatrix * inLightViewMatrix * inModelMatrix * vec4(inVertex,1.0);\n",

         "fsTex0Coord = inTex0;\n",
         "fsTex1Coord = inTex1;\n",
         "fsPosition = viewPos.xyz;\n",
    });
	src.endFunction();
}

void Forward::getFragmentShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;

	src.precision(kHighp, kFloat);

	src.addShaderOutput(Parameter(kFloat4, "fs_outColor", 0));

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
        Parameter(kBool, "inReceiveShadows"),
        Parameter(kSamplerCubemap,"inCubeMap"),
        Parameter(kFloat, "inReflection"),
        Parameter(kSampler2D, "inReflectionMask"),
        Parameter(kFloat4, "inReflectionMaskChannel"),
        Parameter(kBool, "inReflectionMaskInvert"),
        Parameter(kFloat, "inRoughness"),
        Parameter(kSampler2D, "inRoughnessMask"),
        Parameter(kFloat4, "inRoughnessMaskChannel"),
        Parameter(kBool, "inRoughnessMaskInvert"),
        Parameter(kBool, "inUnlit")
    });

	// Lighting inputs
	src.addUniform({
        Parameter(kFloat3, kMaxLights, "inLightDirection"),
        Parameter(kFloat4, kMaxLights, "inLightAmbient"),
        Parameter(kFloat4, kMaxLights, "inLightDiffuse"),
        Parameter(kFloat4, kMaxLights, "inLightSpecular"),
        Parameter(kFloat3, kMaxLights, "inLightPosition"),
        Parameter(kFloat3, kMaxLights, "inLightAttenuation"),
        Parameter(kInt, kMaxLights, "inLightType"),
        Parameter(kFloat, kMaxLights, "inSpotCutoff"),
        Parameter(kFloat, kMaxLights, "inSpotExponent"),
        Parameter(kInt, "inNumLights"),
        Parameter(kFloat, "inLightEmissionFactor")
    });

	// Shadow inputs
	src.addUniform({
       Parameter(kSampler2D, "inShadowMap"),
       Parameter(kFloat, "inShadowStrength"),
       Parameter(kFloat4, "inShadowColor"),
       Parameter(kFloat, "inShadowBias"),
       Parameter(kInt, "inShadowType")
    });

	// Color correction inputs
	src.addUniform({
       Parameter(kFloat, "inHue"),
       Parameter(kFloat, "inSaturation"),
       Parameter(kFloat, "inLightness"),
       Parameter(kFloat, "inBrightness"),
       Parameter(kFloat, "inContrast")
    });

	src.addShaderInput({
        Parameter(kFloat3, "fsPosition"),
        Parameter(kFloat2, "fsTex0Coord"),
        Parameter(kFloat2, "fsTex1Coord"),
        Parameter(kFloat3, "fsNormal"),
        Parameter(kFloat3, "fsTangent"),
        Parameter(kFloat3, "fsBitangent"),
        Parameter(kFloat4, "fsVertexPosFromLight")
    });

	bg::shader::Utils(&src)
			.pack()
			.unpack()
			.random()
			.samplerColor()
			.samplerNormal()
			.combineNormalWithMap()
			.applyTextureMask()
			.specularColor()
            .blurCube();

	bg::shader::Lighting(&src)
			.getDirectionalLight()
			.getPointLight()
			.getSpotLight()
			.getShadowColor()
            .getLight();

	bg::shader::ColorCorrection(&src)
			.rgb2hsv()
			.hsv2rgb()
			.applyContrast()
			.applyBrightness()
			.applySaturation()
			.colorCorrection();

	std::string maxLights = std::to_string(kMaxLights);
	std::string directionalType = std::to_string(bg::base::Light::kTypeDirectional);
	std::string pointType = std::to_string(bg::base::Light::kTypePoint);
	std::string spotType = std::to_string(bg::base::Light::kTypeSpot);
	
	src.beginFunction(kVoid, "main", {});
	src.functionBody({
        "vec4 diffuseColor = samplerColor(inTexture, fsTex0Coord, inTextureOffset, inTextureScale);",
        "vec4 lightmapColor = samplerColor(inLightMap, fsTex1Coord, inLightMapOffset, inLightMapScale);",
        "if (inUnlit && diffuseColor.a >= inAlphaCutoff) {",
        "  fs_outColor = diffuseColor * lightmapColor;",
        "}",
        "else if(diffuseColor.a >= inAlphaCutoff) {",
        "	vec3 normalMap = samplerNormal(inNormalMap, fsTex0Coord, inNormalMapOffset, inNormalMapScale);",
        "	normalMap = combineNormalWithMap(fsNormal, fsTangent, fsBitangent, normalMap);",
        "	vec4 shadowColor = vec4(1.0);",
        "	if(inReceiveShadows) {",
        "		shadowColor = getShadowColor(fsVertexPosFromLight, inShadowMap, inShadowType, inShadowStrength, inShadowBias, inShadowColor);",
        "	}",
        "	vec4 specular = specularColor(inSpecularColor, inShininessMask, fsTex0Coord, inTextureOffset, inTextureScale,",
        "		inShininessMaskChannel, inShininessMaskInvert);",
        "	float lightEmission = applyTextureMask(inLightEmission,",
        "		inLightEmissionMask, fsTex0Coord, inTextureOffset, inTextureScale,",
        "		inLightEmissionMaskChannel, inLightEmissionMaskInvert);",
        "	diffuseColor = diffuseColor * inDiffuseColor * lightmapColor;",
        "	vec4 light = vec4(0.0,0.0,0.0,1.0);",
        "	vec4 shininessColor;",
        "	if (inNumLights>0) {",
        "		light += getLight(inLightAmbient[0], inLightDiffuse[0], inLightSpecular[0], inShininess,"
                                    "inLightDirection[0],inLightPosition[0],inLightAttenuation[0],"
                                    "inSpotCutoff[0],inSpotExponent[0],normalMap,diffuseColor,specular,"
                                    "shadowColor,fsPosition,inLightType[0],shininessColor);",
        "       shininessColor *= shadowColor;",
        "		light.rgb = clamp(light.rgb + (lightEmission * diffuseColor.rgb) + shininessColor.rgb, vec3(0.0), vec3(1.0));"
        "	}",
        "	if (inNumLights>1) {",
        "		light += getLight(inLightAmbient[1], inLightDiffuse[1], inLightSpecular[1], inShininess,"
                                    "inLightDirection[1],inLightPosition[1],inLightAttenuation[1],"
                                    "inSpotCutoff[1],inSpotExponent[1],normalMap,diffuseColor,specular,"
                                    "shadowColor,fsPosition,inLightType[1],shininessColor);",
        "       shininessColor *= shadowColor;",
        "		light.rgb = clamp(light.rgb + (lightEmission * diffuseColor.rgb) + shininessColor.rgb, vec3(0.0), vec3(1.0));"
        "	}",
        "	if (inNumLights>2) {",
        "		light += getLight(inLightAmbient[2], inLightDiffuse[2], inLightSpecular[2], inShininess,"
                                    "inLightDirection[2],inLightPosition[2],inLightAttenuation[2],"
                                    "inSpotCutoff[2],inSpotExponent[2],normalMap,diffuseColor,specular,"
                                    "shadowColor,fsPosition,inLightType[2],shininessColor);",
        "       shininessColor *= shadowColor;",
        "		light.rgb = clamp(light.rgb + (lightEmission * diffuseColor.rgb) + shininessColor.rgb, vec3(0.0), vec3(1.0));"
        "	}",

        "   vec3 cameraPos = vec3(0.0);",
        "   vec3 cameraVector = fsPosition - cameraPos;",
        "   vec3 lookup = reflect(cameraVector,normalMap);",
        "   float dist = distance(fsPosition,cameraPos);",
        "   float maxRough = 50.0;",
        "   float rough = applyTextureMask(inRoughness,inRoughnessMask,fsTex0Coord,inTextureOffset,inTextureScale,inRoughnessMaskChannel,inRoughnessMaskInvert);",
        "   rough = max(inRoughness * 10.0,1.0);",
        "   rough = max(rough*dist,rough);",
        "   float blur = min(rough,maxRough);",
        "   vec3 cubemapColor = blurCube(inCubeMap,lookup,int(blur),vec2(10),dist).rgb;",
        
        "   float reflectionAmount = applyTextureMask(inReflection,inReflectionMask,fsTex0Coord,inTextureOffset,"
                                                    "inTextureScale,inReflectionMaskChannel,inReflectionMaskInvert);",
        
        "   light.rgb = clamp(light.rgb + (lightEmission * diffuseColor.rgb * inLightEmissionFactor), vec3(0.0), vec3(1.0));",
        "   vec3 finalColor = light.rgb * (1.0 - reflectionAmount);",
        "   finalColor += cubemapColor * reflectionAmount * diffuseColor.rgb;",
        "   vec4 result = colorCorrection(vec4(finalColor,1.0),inHue,inSaturation,inLightness,inBrightness,inContrast);",
        "   fs_outColor = vec4(result.rgb,diffuseColor.a);",
        "}",
        "else {",
        "	discard;",
        "}"
     });
	src.endFunction();
}

}
}
