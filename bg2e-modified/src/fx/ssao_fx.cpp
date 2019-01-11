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


#include <bg/fx/ssao.hpp>

#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/engine/openglCore/shader.hpp>
#include <bg/engine/openglEs/shader.hpp>

#include <bg/tools/texture_cache.hpp>
#include <bg/tools/shader_source.hpp>

#include <bg/shader/utils.hpp>
#include <bg/shader/lighting.hpp>

namespace bg {
namespace fx {

Ssao::Ssao(bg::base::Context * ctx)
	:TextureEffect(ctx)
{
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		buildGL(mobile);
	}
	else {
		throw bg::base::EngineException("fx::Ssao effect is not compatible with the current rendering API.");
	}
}

Ssao::~Ssao() {
}


void Ssao::activate(bg::base::TextureVector & textures) {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		activateGL(mobile, textures);
	}
}

void Ssao::bindPolyList(bg::base::PolyList * plist) {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		bindPolyListGL(plist, mobile);
	}
}

void Ssao::unbind() {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		unbindGL(mobile);
	}
}

void Ssao::buildGL(bool mobile) {
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

void Ssao::activateGL(bool, bg::base::TextureVector & textures) {
	using namespace bg::base;
	Texture * positionMap = textures[0].getPtr();
	Texture * normalMap = textures[1].getPtr();
	Texture * diffuseMap = textures[2].getPtr();
	Texture * randomMap = bg::tools::TextureCache::RandomTexture(context());
	bg::math::Vector2 vpSize(static_cast<float>(positionMap->size().x()),
							 static_cast<float>(positionMap->size().x()));
	bg::math::Vector2 randMapSize(static_cast<float>(randomMap->size().x()),
								  static_cast<float>(randomMap->size().x()));

	if (_currentKernelSize!=_kernelSize) {
		for (auto i=0; i<_kernelSize*3;i+=3) {
			bg::math::Vector3 kernel(bg::math::random() * 2.0f - 1.0f,
									 bg::math::random() * 2.0f - 1.0f,
									 bg::math::random());
			kernel.normalize();

			float scale = (static_cast<float>(i) / 3.0f) / static_cast<float>(_kernelSize);
			scale = bg::math::lerp(0.1f, 1.0f, scale * scale);

			kernel.scale(scale);
			
			_kernelOffsets[i]	= kernel.x();
			_kernelOffsets[i+1]	= kernel.y();
			_kernelOffsets[i+2] = kernel.z();
		}
		_currentKernelSize = _kernelSize;
	}

	_glShader->setActive();
	_glShader->setUniform("inPositionMap",positionMap,0);
	_glShader->setUniform("inNormalMap", normalMap, 1);
	_glShader->setUniform("inDiffuseMap", diffuseMap, 2);
	_glShader->setUniform("inRandomMap", randomMap, 3);
	_glShader->setUniform("inViewportSize",vpSize);
	_glShader->setUniform("inProjectionMatrix",_projectionMatrix);
	_glShader->setUniform("inRandomMapSize",randMapSize);
	_glShader->setUniform("inSampleRadius",_sampleRadius);
	_glShader->setUniform("inKernelOffsets",_kernelOffsets, 3, _kernelSize);
	_glShader->setUniform("inKernelSize",_kernelSize);
	_glShader->setUniform("inSSAOColor",_ssaoColor);
	_glShader->setUniform("inEnabled",_enabled);
	_glShader->setUniform("inMaxDistance",_maxDistance);
}

void Ssao::bindPolyListGL(bg::base::PolyList * plist, bool) {
	_glShader->bindPolyList(plist);
}

void Ssao::unbindGL(bool) {
	_glShader->unbindPolyList();
}

void Ssao::getVertexShader(bg::tools::ShaderSource & src) {
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

void Ssao::getFragmentShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;

	src.precision(kHighp, kFloat);

	src.addShaderOutput(Parameter(kFloat4, "fs_outColor", 0));

	src.addUniform({
		Parameter(kSampler2D,"inPositionMap"),
		Parameter(kSampler2D,"inNormalMap"),
		Parameter(kSampler2D,"inDiffuseMap"),
		Parameter(kSampler2D,"inRandomMap"),
		Parameter(kFloat2,"inViewportSize"),
		Parameter(kMatrix4,"inProjectionMatrix"),
		Parameter(kFloat2,"inRandomMapSize"),
		Parameter(kFloat,"inSampleRadius"),
		Parameter(kFloat3,kMaxKernelOffsets,"inKernelOffsets"),
		Parameter(kInt,"inKernelSize"),
		Parameter(kFloat4,"inSSAOColor"),
		Parameter(kBool,"inEnabled"),
		Parameter(kFloat,"inMaxDistance")
	});

	src.addShaderInput({
		Parameter(kFloat2,"fsTex0Coord")
	});

	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"if (!inEnabled) discard;",
		"else {",
		"	vec4 normalTex = texture(inNormalMap,fsTex0Coord);"
		"	vec3 normal = normalTex.xyz * 2.0 - 1.0;",
		"	vec4 vertexPos = texture(inPositionMap,fsTex0Coord);",
		"	vec4 diffuse = texture(inDiffuseMap,fsTex0Coord);",
		"	if (diffuse.a==0.0 || distance(vertexPos.xyz,vec3(0.0))>inMaxDistance || vertexPos.w==1.0 || normalTex.a==0.0) {",
		"		discard;",
		"	}",
		"	else {",
		"		vec2 noiseScale = vec2(inViewportSize.x / inRandomMapSize.x, inViewportSize.y / inRandomMapSize.y);",
		"		vec3 randomVector = texture(inRandomMap, fsTex0Coord * noiseScale).xyz * 2.0 - 1.0;",
		"		vec3 tangent = normalize(randomVector - normal * dot(randomVector, normal));",
		"		vec3 bitangent = cross(normal,tangent);",
		"		mat3 tbn = mat3(tangent,bitangent,normal);",

		"		float occlusion = 0.0;",
		"		for (int i=0; i<" + std::to_string(kMaxKernelOffsets) + "; ++i) {",
		"			if (inKernelSize==i) break;",
		"			vec3 samplePos = tbn * inKernelOffsets[i];",
		"			samplePos = samplePos * inSampleRadius + vertexPos.xyz;",

		"			vec4 offset = inProjectionMatrix * vec4(samplePos, 1.0);",	// -w, w
		"			offset.xyz /= offset.w;",	// -1, 1
		"			offset.xyz = offset.xyz * 0.5 + 0.5;",

		"			vec4 sampleRealPos = texture(inPositionMap,offset.xy);",
		"			if (samplePos.z<sampleRealPos.z) {",
		"				float dist = distance(vertexPos.xyz, sampleRealPos.xyz);",
		"				occlusion += dist<inSampleRadius ? 1.0 : 0.0;",
		"			}",
		"		}",

		"		occlusion = 1.0 - (occlusion / float(inKernelSize));",
		"		fs_outColor = clamp(vec4(occlusion,occlusion,occlusion,1.0) + inSSAOColor, 0.0, 1.0);",
		"	}",
		"}"
	});
	src.endFunction();
}


}
}