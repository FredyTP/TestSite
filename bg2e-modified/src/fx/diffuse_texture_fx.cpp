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

#include <bg/fx/diffuse_texture.hpp>

#include <bg/engine.hpp>
#include <bg/engine/directx11.hpp>
#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/engine/openglEs/shader.hpp>

#include <bg/tools/texture_cache.hpp>

#include <bg/shader/utils.hpp>

namespace bg {
namespace fx {
	
DiffuseTexture::DiffuseTexture(bg::base::Context * ctx)
	:Effect(ctx)
	,_useAlpha(true)
{
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::DirectX11>()) {
		// TODO: Direct3D support
		throw bg::base::EngineException("fx::Forward effect is not compatible with DirectX 11 engine.");
		//buildDX11();
	}
	else if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		buildGL(mobile);
	}

}
	
DiffuseTexture::~DiffuseTexture() {
	
}
	
void DiffuseTexture::activate() {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::DirectX11>()) {
		//activateDX11();
	}
	else if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		activateGL(mobile);
	}
}

void DiffuseTexture::bindPolyList(bg::base::PolyList * plist) {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::DirectX11>()) {
		//bindPolyListDX11(plist);
	}
	else if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		bindPolyListGL(plist, mobile);
	}
}

void DiffuseTexture::unbind() {
	bool mobile = bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLES3>();
	if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::DirectX11>()) {
		//unbindDX11();
	}
	else if (bg::Engine::Get()->identifier() == bg::Engine::Identifier<bg::engine::OpenGLCore>() || mobile) {
		unbindGL(mobile);
	}
}
	
	
void DiffuseTexture::buildGL(bool mobile) {
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

void DiffuseTexture::activateGL(bool mobile) {
	using namespace bg::math;
	_glShader->setActive();
	
	_glShader->setUniform("inModelMatrix", matrixState().modelMatrixStack().matrix());
	_glShader->setUniform("inViewMatrix", matrixState().viewMatrixStack().matrix());
	_glShader->setUniform("inProjectionMatrix", matrixState().projectionMatrixStack().matrix());
	_glShader->setUniform("inColor", _material->diffuse());
	_glShader->setUniform("inTexture", _material->texture() ? _material->texture() : bg::tools::TextureCache::WhiteTexture(context()), 0);
	_glShader->setUniform("inTextureOffset", _material->textureOffset());
	_glShader->setUniform("inTextureScale", _material->textureScale());
	_glShader->setUniform("inAlphaCutoff", _material->alphaCutoff());
	_glShader->setUniform("inAlphaAdd", _useAlpha ? 0.0f : 1.0f );
}

void DiffuseTexture::bindPolyListGL(bg::base::PolyList * plist, bool mobile) {
	_glShader->bindPolyList(plist);
}

void DiffuseTexture::unbindGL(bool mobile) {
	_glShader->clearActive();
	_glShader->unbindPolyList();
}
	
void DiffuseTexture::getVertexShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	
	src.precision(kHighp, kFloat);
	src.addShaderInput(Parameter(kFloat3, "inVertex", kRoleVertex));
	src.addShaderInput(Parameter(kFloat2, "inTex0", kRoleTex0));
	
	src.addUniform({
		Parameter(kMatrix4, "inModelMatrix"),
		Parameter(kMatrix4, "inViewMatrix"),
		Parameter(kMatrix4, "inProjectionMatrix"),
	});
	
	src.addShaderOutput({
		Parameter(kFloat2, "fsTexCoord")
	});
	
	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"gl_Position = inProjectionMatrix * inViewMatrix * inModelMatrix * vec4(inVertex,1.0);",
		"fsTexCoord = inTex0;"
	});
	src.endFunction();

}

void DiffuseTexture::getFragmentShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	
	src.precision(kHighp, kFloat);
	
	src.addShaderOutput(Parameter(kFloat4, "fs_outColor", 0));
	
	src.addUniform({
		Parameter(kSampler2D, "inTexture"),
		Parameter(kFloat2, "inTextureOffset"),
		Parameter(kFloat2, "inTextureScale"),
		Parameter(kFloat, "inAlphaCutoff"),
		Parameter(kFloat, "inAlphaAdd"),
		Parameter(kFloat4, "inColor")
	});
	
	src.addShaderInput({
		Parameter(kFloat2, "fsTexCoord")
	});
	
	bg::shader::Utils(&src)
		.samplerColor();

	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"vec4 diffuseColor = samplerColor(inTexture, fsTexCoord, inTextureOffset, inTextureScale) * inColor;",
		"if(diffuseColor.a >= inAlphaCutoff) {",
		"	fs_outColor = vec4(diffuseColor.rgb,clamp(inAlphaAdd + diffuseColor.a,0.0,1.0));",
		"}",
		"else {",
		"	discard;",
		"}"
	});
	src.endFunction();
}

}
}
