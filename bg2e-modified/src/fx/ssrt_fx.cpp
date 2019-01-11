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


#include <bg/fx/ssrt.hpp>

#include <bg/scene/cubemap.hpp>

#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/engine/openglCore/shader.hpp>
#include <bg/engine/openglEs/shader.hpp>

#include <bg/tools/texture_cache.hpp>
#include <bg/tools/shader_source.hpp>

#include <bg/shader/utils.hpp>
#include <bg/shader/lighting.hpp>

#include <bg/scene/camera.hpp>


namespace bg {
namespace fx {

Ssrt::Ssrt(bg::base::Context * ctx)
	:TextureEffect(ctx)
	,_quality(bg::base::kQualityExtreme)
{
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		buildGL(mobile);
	}
	else {
		throw bg::base::EngineException("fx::DeferredMix effect is not compatible with the current rendering API.");
	}
}

Ssrt::~Ssrt() {
}



void Ssrt::activate(bg::base::TextureVector & textures) {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		activateGL(mobile, textures);
	}
}

void Ssrt::bindPolyList(bg::base::PolyList * plist) {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		bindPolyListGL(plist, mobile);
	}
}

void Ssrt::unbind() {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		unbindGL(mobile);
	}
}

void Ssrt::buildGL(bool mobile) {
	using namespace bg::engine;
	using namespace bg::tools;
	ShaderSource vshader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	ShaderSource fshader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	getVertexShader(vshader);
	getFragmentShader(fshader);

	_glShader = mobile ? new openglEs::Shader(context()) : new openglCore::Shader(context());
	_glShader->attachShader(openglCore::ShaderType::kVertexShader, vshader.text());
	_glShader->attachShader(openglCore::ShaderType::kFragmentShader, fshader.text());
	_glShader->link();
	_glShader->setOutputParameterName(openglCore::ShaderParamType::kShaderOutFragmentLocation, "fs_outColor");

	vshader.initVertexBufferInputs(_glShader.getPtr());
	std::vector<std::string> uniformNames;
	vshader.uniformNames(uniformNames);
	fshader.uniformNames(uniformNames);
	_glShader->initUniforms(uniformNames);
}

void Ssrt::activateGL(bool mobile, bg::base::TextureVector & textures) {
	using namespace bg::base;
	if (qualityChanged()) {
		_glShader->destroy();
		_glShader = nullptr;
		buildGL(mobile);
	}
	Texture * positionMap = textures[0].getPtr();
	Texture * normalMap = textures[1].getPtr();
	Texture * lightingMap = textures[2].getPtr();
	Texture * materialMap = textures[3].getPtr();
	Texture * specularMap = textures[4].getPtr();
	Texture * samplePositionMap = textures.size()==5 ? positionMap : textures[5].getPtr();

	_glShader->setActive();
	_glShader->setUniform("inPositionMap", positionMap, 0);
	_glShader->setUniform("inNormalMap", normalMap, 1);
	_glShader->setUniform("inLightingMap", lightingMap, 2);
	_glShader->setUniform("inMaterialMap", materialMap, 3);
	_glShader->setUniform("inSamplePosMap", samplePositionMap, 4);
	_glShader->setUniform("inProjectionMatrix", _projectionMatrix);
	_glShader->setUniform("inCameraPos", _cameraPos);
	_glShader->setUniform("inCubeMap", bg::scene::Cubemap::Current(context()), 5);
	_glShader->setUniform("inFrameIndex", static_cast<float>(_frameIndex));
	_frameIndex = (_frameIndex + 1) % 4;

	static Texture * randomTexture = nullptr;
	if (!randomTexture) {
		randomTexture = bg::base::Texture::RandomTexture(context(), bg::math::Size2Di(2048));
	}
	_glShader->setUniform("inSpecularMap", specularMap, 6);
	_glShader->setUniform("inRandomTexture", randomTexture, 7);
}

void Ssrt::bindPolyListGL(bg::base::PolyList * plist, bool) {
	_glShader->bindPolyList(plist);
}

void Ssrt::unbindGL(bool) {
	_glShader->unbindPolyList();
}

void Ssrt::getVertexShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;

	src.precision(kHighp, kFloat);
	src.addShaderInput(Parameter(kFloat3, "inPosition", kRoleVertex));
	src.addShaderInput(Parameter(kFloat2, "inTex0", kRoleTex0));

	src.addShaderOutput(Parameter(kFloat2, "fsTex0Coord", kRoleOutput));

	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"gl_Position = vec4(inPosition, 1.0);",
		"fsTex0Coord = inTex0;"
	});
	src.endFunction();
}

void Ssrt::getFragmentShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;

	src.precision(kHighp, kFloat);

	src.addShaderOutput(Parameter(kFloat4, "fs_outColor", 0));

	src.addUniform({
		Parameter(kSampler2D,"inPositionMap"),
		Parameter(kSampler2D,"inNormalMap"),
		Parameter(kSampler2D,"inLightingMap"),
		Parameter(kSampler2D,"inMaterialMap"),
		Parameter(kSampler2D,"inSamplePosMap"),
		Parameter(kMatrix4,"inProjectionMatrix"),
		Parameter(kFloat3,"inCameraPos"),
		Parameter(kSamplerCubemap,"inCubeMap"),
		Parameter(kFloat,"inFrameIndex"),
		Parameter(kSampler2D,"inSpecularMap"),
		Parameter(kSampler2D,"inRandomTexture")
	});

	src.addShaderInput({
		Parameter(kFloat2,"fsTex0Coord")
	});

	bg::shader::Utils(&src)
		.random();

	int maxSamples = 10;
	float increment = 0.0f;
	switch (quality()) {
	case bg::base::kQualityLow:
		maxSamples = 50;
		increment = 0.025f;
		break;
	case bg::base::kQualityMedium:
		maxSamples = 100;
		increment = 0.0125f;
		break;
	case bg::base::kQualityHigh:
		maxSamples = 200;
		increment = 0.0062f;
		break;
	case bg::base::kQualityExtreme:
		maxSamples = 300;
		increment = 0.0031f;
		break;
	}

	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"vec2 p = vec2(floor(gl_FragCoord.x), floor(gl_FragCoord.y));",
		"bool renderFrame = false;",
		"if (inFrameIndex==0.0 && mod(p.x,2.0)==0.0 && mod(p.y,2.0)==0.0) {",
		"	renderFrame = true;",
		"}",
		"else if (inFrameIndex==1.0 && mod(p.x,2.0)==0.0 && mod(p.y,2.0)!=0.0) {",
		"	renderFrame = true;",
		"}",
		"else if (inFrameIndex==2.0 && mod(p.x,2.0)!=0.0 && mod(p.y,2.0)==0.0) {",
		"	renderFrame = true;",
		"}",
		"else if (inFrameIndex==3.0 && mod(p.x,2.0)!=0.0 && mod(p.y,2.0)!=0.0) {",
		"	renderFrame = true;",
		"}",
		"if (renderFrame) {",
		"	vec3 normal = texture(inNormalMap,fsTex0Coord).xyz * 2.0 - 1.0;",
		"	vec4 material = texture(inMaterialMap,fsTex0Coord);",
		"	float roughness = texture(inSpecularMap,fsTex0Coord).a * 0.3;",
		"	vec3 r = texture(inRandomTexture,fsTex0Coord*200.0).xyz * 2.0 - 1.0;",
		"	vec3 roughnessFactor = normalize(r) * roughness;",
		"	normal = normal + roughnessFactor;",
		"	vec4 vertexPos = texture(inPositionMap,fsTex0Coord);",
		"	vec3 cameraVector = vertexPos.xyz - inCameraPos;",
		"	vec3 rayDirection = normalize(reflect(cameraVector,normal));",
		"	vec4 lighting = texture(inLightingMap,fsTex0Coord);",

		"	vec3 lookup = reflect(cameraVector,normal);",
		"	vec4 rayFailColor = texture(inCubeMap, lookup);"

		"	float increment = " + std::to_string(increment) + ";",
		"	vec4 result = rayFailColor;",
		"	float maxSamples = " + std::to_string(maxSamples) + ".0;",
		"	if (material.b>0.0) {",	// material[2] is reflectionAmount
		"		result = vec4(0.0, 0.0, 0.0, 1.0);",
		"		for (float i=0.0; i<maxSamples; ++i) {",
		"			if (i==maxSamples) {",
		"				break;",
		"			}",

		"			float radius = i * increment;",
		"			increment *= 1.025;",
		"			vec3 ray = vertexPos.xyz + rayDirection * radius;",

		"			vec4 offset = inProjectionMatrix * vec4(ray, 1.0);",	// -w, w
		"			offset.xyz /= offset.w;",	// -1, 1
		"			offset.xyz = offset.xyz * 0.5 + 0.5;",	// 0, 1

		"			vec4 rayActualPos = texture(inSamplePosMap, offset.xy);",
		"			float hitDistance = rayActualPos.z - ray.z;",
		"			if (rayActualPos.w<0.6) {",
		"				break;",
		"			}",
		"			if (offset.x>1.0 || offset.y>1.0) {",
		"				result = rayFailColor;",
		"				break;",
		"			}",
		"			else if (hitDistance>0.01 && hitDistance<0.4) {",
		"				result = texture(inLightingMap,offset.xy);",
		"				break;",
		"			}",
		"		}",
		"	}",
		"	if (result.a==0.0) {",
		"		fs_outColor = rayFailColor;"
		"	}",
		"	else {",
		"		fs_outColor = result;",
		"	}",
		"}",
		"else {",
		"	discard;",
		"}"
	});
	src.endFunction();
}

}
}
