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


#include <bg/fx/deferred_mix.hpp>

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

DeferredMix::DeferredMix(bg::base::Context * ctx)
	:TextureEffect(ctx)
{
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		buildGL(mobile);
	}
	else {
		throw bg::base::EngineException("fx::DeferredMix effect is not compatible with the current rendering API.");
	}
}

DeferredMix::~DeferredMix() {
}

void DeferredMix::activate(bg::base::TextureVector & texture) {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		activateGL(mobile, texture);
	}
}

void DeferredMix::bindPolyList(bg::base::PolyList * plist) {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		bindPolyListGL(plist, mobile);
	}
}

void DeferredMix::unbind() {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		unbindGL(mobile);
	}
}

void DeferredMix::buildGL(bool mobile) {
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

void DeferredMix::activateGL(bool, bg::base::TextureVector & textures) {
	using namespace bg::base;
	Texture * lighting = textures[0].getPtr();
	Texture * diffuse = textures[1].getPtr();
	Texture * ssao = textures[2].getPtr();
	Texture * reflection = textures[3].getPtr();
	Texture * material = textures[4].getPtr();
	Texture * position = textures[5].getPtr();
	Texture * specular = textures[6].getPtr();
	Texture * opaqueDepth = _opaqueDepth.valid() ? _opaqueDepth.getPtr() : position;
	Texture * shininess = textures[7].getPtr();

	_glShader->setActive();

	_glShader->setUniform("inLighting", lighting, 0);
	_glShader->setUniform("inDiffuse", diffuse, 1);
	_glShader->setUniform("inSSAO", ssao, 2);
	_glShader->setUniform("inReflection", reflection, 3);
	_glShader->setUniform("inMaterial", material, 4);
	_glShader->setUniform("inSpecularMap", specular, 5);
	_glShader->setUniform("inPositionMap", position, 6);
	_glShader->setUniform("inOpaqueDepth", opaqueDepth, 7);
	_glShader->setUniform("inShininess", shininess, 8);
	_glShader->setUniform("inSSAOBlur", _ssaoBlur);
	_glShader->setUniform("inSSRTScale", _ssrtScale);
	_glShader->setUniform("inViewSize", bg::math::Vector2(static_cast<float>(lighting->size().x()),
		static_cast<float>(lighting->size().y())));
}

void DeferredMix::bindPolyListGL(bg::base::PolyList * plist, bool) {
	_glShader->bindPolyList(plist);
}

void DeferredMix::unbindGL(bool) {
	_glShader->unbindPolyList();
}

void DeferredMix::getVertexShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;

	src.precision(kHighp,kFloat);
	src.addShaderInput(Parameter(kFloat3,"inPosition",kRoleVertex));
	src.addShaderInput(Parameter(kFloat2,"inTex0",kRoleTex0));
	src.addShaderOutput(Parameter(kFloat2,"fsTex0Coord",kRoleOutput));

	src.beginFunction(kVoid,"main",{});
	src.functionBody({
		"gl_Position = vec4(inPosition,1.0);",
		"fsTex0Coord = inTex0;"
	});
	src.endFunction();
}

void DeferredMix::getFragmentShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;

	src.precision(kHighp,kFloat);
	src.addShaderOutput(Parameter(kFloat4,"fs_outColor",0));
	src.addUniform({
		Parameter(kSampler2D,"inLighting"),
		Parameter(kSampler2D,"inDiffuse"),	// Used to compute the alpha value
		Parameter(kSampler2D,"inPositionMap"),	// Used to compute the layer depth test
		Parameter(kSampler2D,"inSSAO"),
		Parameter(kSampler2D,"inReflection"),
		Parameter(kSampler2D,"inMaterial"),
		Parameter(kSampler2D,"inSpecularMap"),	// The roughness parameter is stored in A component, inside the specular map
		Parameter(kSampler2D,"inOpaqueDepth"),
		Parameter(kSampler2D, "inShininess"),
		Parameter(kFloat2,"inViewSize"),
		Parameter(kInt, "inSSAOBlur"),
		Parameter(kFloat, "inSSRTScale")
	});
	src.addShaderInput(Parameter(kFloat2,"fsTex0Coord"));

	bg::shader::Utils(&src)
		.blur()
		.textureDownsample()
		.blurDownsample()
		.bloom();

	src.beginFunction(kVoid,"main",{});
	src.functionBody({
		"vec4 lighting = texture(inLighting,fsTex0Coord);",
		"vec4 diffuse = texture(inDiffuse,fsTex0Coord);",
		"vec4 pos = texture(inPositionMap,fsTex0Coord);",
		"vec4 ssao = blurDownsample(inSSAO,fsTex0Coord,inSSAOBlur,inViewSize,10);",
		"vec4 shin = texture(inShininess,fsTex0Coord);",
		//"vec4 reflect = texture(inReflection,fsTex0Coord);",
		"vec4 material = texture(inMaterial,fsTex0Coord);",

		"vec4 specular = texture(inSpecularMap,fsTex0Coord);",	// The roughness parameter is stored in A component, inside the specular map

		"float roughness = specular.a;",
		"float ssrtScale = inSSRTScale;",
		"roughness *= 250.0 * ssrtScale;",
		"vec4 reflect = blurDownsample(inReflection,fsTex0Coord,int(max(roughness,1.0)),inViewSize * ssrtScale, 15);",

		"vec4 opaqueDepth = texture(inOpaqueDepth,fsTex0Coord);",
		"if (pos.z<opaqueDepth.z && opaqueDepth.w<1.0) {",
		"	discard;",
		"}",
		"else {",
		"	float reflectionAmount = material.b;",
		"	vec3 finalColor = lighting.rgb * (1.0 - reflectionAmount);",
		"	finalColor += reflect.rgb * reflectionAmount * diffuse.rgb + shin.rgb;",
		"	finalColor *= ssao.rgb;",
		"	fs_outColor = vec4(finalColor,diffuse.a);",
		"}"
	});
	src.endFunction();
}

}
}
