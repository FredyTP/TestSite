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


#include <bg/shader/lighting.hpp>

#include <bg/engine.hpp>
#include <bg/engine/directx11.hpp>
#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>

#include <bg/base/light.hpp>
#include <bg/base/exception.hpp>

namespace bg {
namespace shader {

Lighting::Lighting(bg::tools::ShaderSource * src)
	:Library(src)
{
	if(bg::Engine::IsDX11()) {
		throw bg::base::EngineException("bg::shader::Lighting not implemented for DirectX 11 engine");
	}
}

Lighting & Lighting::beckmannDistribution() {
    using namespace bg::tools;
    using namespace bg::tools::shaderTypes;
    shaderSource().beginFunction(kFloat, "beckmannDistribution", {
        Parameter(kFloat,"x"),
        Parameter(kFloat,"roughness")
    });
    
    shaderSource().functionBody({
        "float NdotH = max(x,0.0001);",
        "float cos2Alpha = NdotH * NdotH;",
        "float tan2Alpha = (cos2Alpha - 1.0) / cos2Alpha;",
        "float roughness2 = roughness * roughness;",
        "float denom = 3.141592653589793 * roughness2 *",
        "return exp(tan2Alpha / roughness2) / denom;"
    });
    shaderSource().endFunction();
    return *this;
}

Lighting & Lighting::beckmannSpecular() {
    using namespace bg::tools;
    using namespace bg::tools::shaderTypes;
    shaderSource().beginFunction(kFloat, "beckmannSpecular", {
        Parameter(kFloat3,"lightDirection"),
        Parameter(kFloat3,"viewDirection"),
        Parameter(kFloat3,"surfaceNormal"),
        Parameter(kFloat,"roughness")
    });
    shaderSource().functionBody({
        "return beckmannDistribution(dot(surfaceNormal,normalize(lightDirection + viewDirection)),roughness);"
    });
    shaderSource().endFunction();
    return *this;
}

Lighting & Lighting::getLight() {
    using namespace bg::tools;
    using namespace bg::tools::shaderTypes;
    shaderSource().beginFunction(kFloat4, "getLight", {
        Parameter(kFloat4,"ambient"),
        Parameter(kFloat4,"diffuse"),
        Parameter(kFloat4,"specular"),
        Parameter(kFloat,"shininess"),
        Parameter(kFloat3,"direction"),
        Parameter(kFloat3,"position"),
        Parameter(kFloat3,"attenuation"),
        Parameter(kFloat,"spotCutoff"),
        Parameter(kFloat,"spotExponent"),
        Parameter(kFloat3,"normal"),
        Parameter(kFloat4,"matDiffuse"),
        Parameter(kFloat4,"matSpecular"),
        Parameter(kFloat4,"shadowColor"),
        Parameter(kFloat3,"targetPosition"),
        Parameter(kInt,"type"),
		Parameter(kFloat4Out,"shininessColor")
    });
    
    shaderSource().functionBody({
        "if (type==" + std::to_string(bg::base::Light::kTypeDirectional) + ") {",
        "    vec4 lightShadow = max(shadowColor,ambient);"
        "    return getDirectionalLight(ambient, diffuse, specular, shininess,",
        "                                        -direction, targetPosition, normal,",
        "                                        matDiffuse, matSpecular, lightShadow, shininessColor);",
        "}",
        "else if (type==" + std::to_string(bg::base::Light::kTypePoint) + ") {",
        "    return getPointLight(ambient,diffuse,specular,shininess,position,",
        "                                    attenuation.x, attenuation.y, attenuation.z,",
        "                                    targetPosition,normal,vec4(matDiffuse.rgb,1.0),matSpecular,shininessColor);",
        "}",
        "else if (type==" + std::to_string(bg::base::Light::kTypeSpot) + ") {",
        "    vec4 lightShadow = max(shadowColor,ambient);",
        "    return getSpotLight(ambient,diffuse,specular,shininess,position,direction,",
        "                                        attenuation.x, attenuation.y, attenuation.z,",
        "                                        spotCutoff,spotExponent,",
        "                                        targetPosition,normal,vec4(matDiffuse.rgb,1.0),matSpecular,lightShadow,shininessColor);",
        "}"
    });
    shaderSource().endFunction();
    
    return *this;
}

Lighting & Lighting::getDirectionalLight() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "getDirectionalLight", {
		Parameter(kFloat4, "ambient"),
		Parameter(kFloat4 ,"diffuse"),
		Parameter(kFloat4, "specular"),
		Parameter(kFloat, "shininess"),
		Parameter(kFloat3, "direction"),
		Parameter(kFloat3, "vertexPos"),
		Parameter(kFloat3, "normal"),
		Parameter(kFloat4, "matDiffuse"),
		Parameter(kFloat4, "matSpecular"),
		Parameter(kFloat4, "shadowColor"),
		Parameter(kFloat4Out, "shininessColor")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"vec3 color = ambient.rgb * matDiffuse.rgb;",
			"vec3 diffuseWeight = max(0.0, dot(normal, direction)) * diffuse.rgb;",
			"diffuseWeight.r = min(diffuseWeight.r,shadowColor.r);",
			"diffuseWeight.g = min(diffuseWeight.g,shadowColor.g);",
			"diffuseWeight.b = min(diffuseWeight.b,shadowColor.b);",
			"color += diffuseWeight * matDiffuse.rgb;",
			"shininessColor = vec4(0.0,0.0,0.0,1.0);",
			"if(shininess>0.0) {",
			"	vec3 eyeDirection = normalize(-vertexPos);",
			"	vec3 reflectionDirection = normalize(reflect(-direction, normal));"
			"	float specularWeight = clamp(pow(max(dot(reflectionDirection, eyeDirection), 0.0), shininess), 0.0, 1.0);",
			"	vec3 specularColor = specularWeight * pow(shadowColor.rgb, vec3(10.0));",
			"	shininessColor = vec4(specularColor * specular.rgb * matSpecular.rgb, 1.0);",
			"}",
			"return vec4(color, 1.0);"
		});
	}

	shaderSource().endFunction();
	return *this;
}

Lighting & Lighting::getPointLight() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "getPointLight",{
		Parameter(kFloat4, "ambient"),
		Parameter(kFloat4 ,"diffuse"),
		Parameter(kFloat4, "specular"),
		Parameter(kFloat, "shininess"),
		Parameter(kFloat3, "position"),
		Parameter(kFloat, "constAtt"),
		Parameter(kFloat, "linearAtt"),
		Parameter(kFloat, "expAtt"),
		Parameter(kFloat3, "vertexPos"),
		Parameter(kFloat3, "normal"),
		Parameter(kFloat4, "matDiffuse"),
		Parameter(kFloat4, "matSpecular"),
		Parameter(kFloat4Out, "shininessColor")
	});

	if (bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"vec3 pointToLight = position - vertexPos;",
			"float distance = length(pointToLight);",
			"vec3 lightDir = normalize(pointToLight);",
			"float attenuation = 1.0 / (constAtt + linearAtt * distance + expAtt * distance * distance);",
			"vec3 color = ambient.rgb * matDiffuse.rgb;",
			"vec3 diffuseWeight = max(0.0,dot(normal,lightDir)) * diffuse.rgb * attenuation;",
			"color += diffuseWeight * matDiffuse.rgb;",
			"shininessColor = vec4(0.0,0.0,0.0,1.0);",
			"if (shininess>0.0) {",
			"	vec3 eyeDirection = normalize(-vertexPos);",
			"	vec3 reflectionDirection = normalize(reflect(-lightDir, normal));",
			"	float specularWeight = clamp(pow(max(dot(reflectionDirection,eyeDirection),0.0), shininess), 0.0, 1.0);",
			"	shininessColor = vec4(specularWeight * specular.rgb * matSpecular.rgb * attenuation,1.0);",
			"}",
			"return vec4(color,1.0);"
		});
	}

	shaderSource().endFunction();
	return *this;
}

Lighting & Lighting::getSpotLight() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "getSpotLight", {
		Parameter(kFloat4, "ambient"),
		Parameter(kFloat4 ,"diffuse"),
		Parameter(kFloat4, "specular"),
		Parameter(kFloat, "shininess"),
		Parameter(kFloat3, "position"),
		Parameter(kFloat3, "direction"),
		Parameter(kFloat, "constAtt"),
		Parameter(kFloat, "linearAtt"),
		Parameter(kFloat, "expAtt"),
		Parameter(kFloat, "spotCutoff"),
		Parameter(kFloat, "spotExponent"),
		Parameter(kFloat3, "vertexPos"),
		Parameter(kFloat3, "normal"),
		Parameter(kFloat4, "matDiffuse"),
		Parameter(kFloat4, "matSpecular"),
		Parameter(kFloat4, "shadowColor"),
		Parameter(kFloat4Out, "shininessColor")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"vec4 matAmbient = vec4(1.0);",
			"vec3 s = normalize(position - vertexPos);",
			"float angle = acos(dot(-s, direction));",
			"float cutoff = radians(clamp(spotCutoff,0.0,90.0));",
			"float distance = length(position - vertexPos);"
			"float attenuation = 1.0 / (constAtt + linearAtt * distance + expAtt * distance * distance);"
			"if (angle<cutoff) {",
			"	float spotFactor = pow(dot(-s, direction), spotExponent);",
			"	vec3 v = normalize(vec3(-vertexPos));",
			"	vec3 h = normalize(v + s);",
			"	vec3 diffuseAmount = matDiffuse.rgb * diffuse.rgb * max(dot(s, normal), 0.0);",
			"	shininessColor = vec4(0.0,0.0,0.0,1.0);",
			"	if (shininess>0.0) {",
			"		shininessColor = vec4(matSpecular.rgb * specular.rgb * pow(max(dot(h,normal), 0.0),shininess),1.0);",
			"	}",
			"	diffuseAmount.r = min(diffuseAmount.r, shadowColor.r);",
			"	diffuseAmount.g = min(diffuseAmount.g, shadowColor.g);",
			"	diffuseAmount.b = min(diffuseAmount.b, shadowColor.b);",
			"	return vec4(ambient.rgb * matDiffuse.rgb + attenuation * spotFactor * diffuseAmount,1.0);",
			"}",
			"else {",
			"	return vec4(ambient.rgb * matDiffuse.rgb,1.0);",
			"}"
		});
	}

	shaderSource().endFunction();
	return *this;
}

Lighting & Lighting::getShadowColor() {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	shaderSource().beginFunction(kFloat4, "getShadowColor", {
		Parameter(kFloat4, "vertexPosFromLight"),
		Parameter(kSampler2D, "shadowMap"),
		Parameter(kInt, "shadowType"),
		Parameter(kFloat, "shadowStrength"),
		Parameter(kFloat, "shadowBias"),
		Parameter(kFloat4, "shadowColor")
	});

	if(bg::Engine::IsOpenGL()) {
		shaderSource().functionBody({
			"float visibility = 1.0;",
			"vec3 depth = vertexPosFromLight.xyz / vertexPosFromLight.w;",
			"  const float kShadowBorderOffset = 1.0;",
			"float bias = shadowBias;",
			"vec4 shadow = vec4(1.0);",
			"float shadowDepth = texture(shadowMap, depth.xy).z;",
			"if(shadowType == 0 && shadowDepth>0.0) {	// hard",
			"	float shadowDepth = unpack(texture(shadowMap, depth.xy));",
			"	if(shadowDepth<depth.z - bias &&",
			"		(depth.x>0.0 && depth.x<1.0 && depth.y>0.0 && depth.y<1.0))",
			"	{",
			"		visibility = 1.0 - shadowStrength;",
			"	}",
			"	shadow = clamp(shadowColor + visibility, 0.0, 1.0);",
			"}",
#if BG2E_OPENGL_AVAILABLE==1
			"else if(shadowType == 1 && shadowDepth>0.0) {	// soft",
#else
			"else if((shadowType == 1 || shadowType == 2) && shadowDepth>0.0) {	// soft/stratified in OpenGL ES",
#endif
			"	vec2 poissonDisk[4];",
			"	poissonDisk[0] = vec2(-0.94201624, -0.39906216);",
			"	poissonDisk[1] = vec2(0.94558609, -0.76890725);",
			"	poissonDisk[2] = vec2(-0.094184101, -0.92938870);",
			"	poissonDisk[3] = vec2(0.34495938, 0.29387760);",

			"	for(int i = 0; i<4; ++i) {",
			"		float shadowDepth = unpack(texture(shadowMap, depth.xy + poissonDisk[i] / 1000.0));",

			"		if(shadowDepth<depth.z - bias",
			"			&& (depth.x>0.0 && depth.x<1.0 && depth.y>0.0 && depth.y<1.0)) {",
			"				visibility -= (shadowStrength) * 0.25;",
			"		}",
			"	}",
			"	shadow = clamp(shadowColor + visibility, 0.0, 1.0);",
			"}",
#if BG2E_OPENGL_AVAILABLE==1
			"else if (shadowType==2 && shadowDepth>0.0) { // stratified shadows",
			"		const vec3 poissonDisk[16] = vec3[](",
			"			vec3(-0.94201624, -0.39906216, 0.0),",
			"			vec3(0.94558609, -0.76890725, 0.0),",
			"			vec3(-0.094184101, -0.92938870, 0.0),",
			"			vec3(0.34495938, 0.29387760, 0.0),",
			"			vec3(-0.91588581, 0.45771432, 0.0),",
			"			vec3(-0.81544232, -0.87912464, 0.0),",
			"			vec3(-0.38277543, 0.27676845, 0.0),",
			"			vec3(0.97484398, 0.75648379, 0.0),",
			"			vec3(0.44323325, -0.97511554, 0.0),",
			"			vec3(0.53742981, -0.47373420, 0.0),",
			"			vec3(-0.26496911, -0.41893023, 0.0),",
			"			vec3(0.79197514, 0.19090188, 0.0),",
			"			vec3(-0.24188840, 0.99706507, 0.0),",
			"			vec3(-0.81409955, 0.91437590, 0.0),",
			"			vec3(0.19984126, 0.78641367, 0.0),",
			"			vec3(0.14383161, -0.14100790, 0.0)",
			"			);",
			"	for(int i = 0; i<4; ++i) {",
			"		int index = int(mod(float(16.0*random(gl_FragCoord.x * int(i))),16.0));",
			"		float shadowDepth = unpack(texture(shadowMap, (depth + poissonDisk[index] / 1000.0).xy));",
			
			"		if(shadowDepth<depth.z - bias",
			"			&& (depth.x>0.0 && depth.x<1.0 && depth.y>0.0 && depth.y<1.0)) {",
			"			visibility -= (shadowStrength) * 0.25;",
			"		}",
			"	}",
			"	shadow = clamp(shadowColor + visibility,0.0,1.0);",
			"}",
#endif
			"return shadow;"
		});
	}

	shaderSource().endFunction();
	return *this;
}

}
}
