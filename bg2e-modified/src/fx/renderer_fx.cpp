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

 #include <bg/fx/renderer.hpp>

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


Renderer::Renderer(bg::base::Context * ctx)
	:bg::base::TextureEffect(ctx)
{
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		buildGL(mobile);
	}
	else {
		throw bg::base::EngineException("fx::Lighting effect is not compatible with the current rendering API.");
	}
}

Renderer::~Renderer() {
	
}
	
void Renderer::activate(bg::base::TextureVector & texture) {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		activateGL(mobile, texture);
	}
}

void Renderer::bindPolyList(bg::base::PolyList * plist) {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		bindPolyListGL(plist, mobile);
	}

}

void Renderer::unbind() {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if(bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		unbindGL(mobile);
	}

}
	
void Renderer::buildGL(bool mobile) {
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

void Renderer::activateGL(bool, bg::base::TextureVector & textures) {
	bg::base::Texture * opaque = textures[0].getPtr();
	bg::base::Texture * transparent= textures.size()>1 ? textures[1].getPtr() : bg::tools::TextureCache::TransparentTexture(context());
	bg::base::Texture * transNormal = textures.size()>2 ? textures[2].getPtr() : bg::tools::TextureCache::NormalTexture(context());
	
	_glShader->setActive();
	
	_glShader->setUniform("inOpaque", opaque, 0);
	_glShader->setUniform("inTransparent", transparent, 1);
	_glShader->setUniform("inTransparentNormal", transNormal, 2);
	
	_glShader->setUniform("inRefractionAmount", 0.035f);
}

void Renderer::bindPolyListGL(bg::base::PolyList * plist, bool) {
	_glShader->bindPolyList(plist);
}

void Renderer::unbindGL(bool) {
	_glShader->unbindPolyList();
}
	
void Renderer::getVertexShader(bg::tools::ShaderSource & src) {
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

void Renderer::getFragmentShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	
	src.precision(kHighp, kFloat);
	
	src.addShaderOutput(Parameter(kFloat4, "fs_outColor", 0));
	
	src.addUniform({
		Parameter(kSampler2D, "inOpaque"),
		Parameter(kSampler2D, "inTransparent"),
		Parameter(kSampler2D, "inTransparentNormal"),
		Parameter(kFloat, "inRefractionAmount")
	});
	src.addShaderInput(Parameter(kFloat2, "fsTex0Coord"));
	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"vec4 opaque = texture(inOpaque,fsTex0Coord);",
		"vec4 transparent = texture(inTransparent,fsTex0Coord);",
		"vec3 normal = texture(inTransparentNormal, fsTex0Coord).rgb * 2.0 - 1.0;",
		"if (transparent.a>0.0) {",
		"	vec2 offset = fsTex0Coord - normal.xy * inRefractionAmount;",
		"	opaque = texture(inOpaque,offset);",
		"}",
		"vec3 color = opaque.rgb * (1.0 - transparent.a) + transparent.rgb * transparent.a;",
		"fs_outColor = vec4(color, 1.0);"
	});
	src.endFunction();
}
	
}
}
