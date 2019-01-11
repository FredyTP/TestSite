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


#include <bg/fx/shadow_map.hpp>

#include <bg/engine.hpp>

#include <bg/engine/openglCore/shader.hpp>
#include <bg/engine/openglEs/shader.hpp>

#include <bg/engine/directx11.hpp>
#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>

#include <bg/shader/utils.hpp>

#include <bg/tools/texture_cache.hpp>

namespace bg {
namespace fx {

void shaderMap_VertexSource(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	src.addShaderInput({
		Parameter(kFloat3, "inVertex", kRoleVertex),
		Parameter(kFloat2, "inTex0", kRoleTex0)
	});

	src.addUniform({
		Parameter(kMatrix4, "inModelMatrix"),
		Parameter(kMatrix4, "inViewMatrix"),
		Parameter(kMatrix4, "inProjectionMatrix")
	});

	src.addShaderOutput({
		Parameter(kFloat2, "fsTexCoord")
	});

	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"gl_Position = inProjectionMatrix * inViewMatrix * inModelMatrix * vec4(inVertex, 1.0);",
		"fsTexCoord = inTex0;"
	});
	src.endFunction();
}

void shaderMap_FragmentSource(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	
	src.precision(kHighp, kFloat);
	src.addUniform({
		Parameter(kBool, "inCastShadows"),
		Parameter(kSampler2D, "inTexture"),
		Parameter(kFloat2, "inTextureOffset"),
		Parameter(kFloat2, "inTextureScale"),
		Parameter(kFloat, "inAlphaCutoff")
	});
	
	src.addShaderInput({
		Parameter(kFloat2, "fsTexCoord")
	});

	src.addShaderOutput({
		Parameter(kFloat4, "out_FragColor")
	});

	bg::shader::Utils(&src)
		.pack()
		.samplerColor();

	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"float alpha = samplerColor(inTexture, fsTexCoord, inTextureOffset, inTextureScale).a;",
		"if (inCastShadows && alpha>inAlphaCutoff) {",
		"	out_FragColor = pack(gl_FragCoord.z);",
		"}",
		"else {",
		"	discard;",
		"}"
	});
	src.endFunction();
}

ShadowMap::ShadowMap(bg::base::Context * ctx)
	:bg::base::Effect(ctx)
{
	_light = new bg::base::Light();
	if (bg::Engine::IsDX11()) {
		throw bg::base::EngineException("fx::ShadowMap effect is not compatible with DirectX 11 engine.");
		//buildDX11();
	}
	else if (bg::Engine::IsOpenGL()) {
		buildGL(bg::Engine::IsOpenGLES3());
	}
}

ShadowMap::~ShadowMap() {
}

void ShadowMap::activate() {
	if (bg::Engine::IsDX11()) {
		activateDX11();
	}
	else if (bg::Engine::IsOpenGL()) {
		activateGL(bg::Engine::IsOpenGLES3());
	}
}

void ShadowMap::bindPolyList(bg::base::PolyList * plist) {
	if (bg::Engine::IsDX11()) {
		bindPolyListDX11(plist);
	}
	else if (bg::Engine::IsOpenGL()) {
		bindPolyListGL(plist, bg::Engine::IsOpenGLES3());
	}
}

void ShadowMap::unbind() {
	if(bg::Engine::IsDX11()) {
		unbindDX11();
	}
	else if(bg::Engine::IsOpenGL()) {
		unbindGL(bg::Engine::IsOpenGLES3());
	}
}

void ShadowMap::buildDX11() {
}

void ShadowMap::buildGL(bool mobile) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	bg::tools::ShaderSource vshaderSrc(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	bg::tools::ShaderSource fshaderSrc(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	shaderMap_VertexSource(vshaderSrc);
	shaderMap_FragmentSource(fshaderSrc);

	_glShader = mobile ?	new bg::engine::openglEs::Shader(context()) :
							new bg::engine::openglCore::Shader(context());

	_glShader->attachShader(bg::engine::openglCore::kVertexShader, vshaderSrc.text());
	_glShader->attachShader(bg::engine::openglCore::kFragmentShader, fshaderSrc.text());
	_glShader->link();
	_glShader->setOutputParameterName(bg::engine::openglCore::ShaderParamType::kShaderOutFragmentLocation, "out_FragColor");

	vshaderSrc.initVertexBufferInputs(_glShader.getPtr());
	std::vector<std::string> uniformNames;
	vshaderSrc.uniformNames(uniformNames);
	fshaderSrc.uniformNames(uniformNames);
	_glShader->initUniforms(uniformNames);
}

void ShadowMap::activateDX11() {
}

void ShadowMap::activateGL(bool mobile) {
	_glShader->setActive();

	bg::math::Matrix4 & model = matrixState().modelMatrixStack().matrix();
	bg::math::Matrix4 & view = matrixState().viewMatrixStack().matrix();
	bg::math::Matrix4 & proj = matrixState().projectionMatrixStack().matrix();

	_glShader->setUniform("inModelMatrix", model);
	_glShader->setUniform("inViewMatrix", view);
	_glShader->setUniform("inProjectionMatrix", proj);

	bg::base::Texture * texture = material().texture() ? material().texture() : bg::tools::TextureCache::WhiteTexture(context());
	_glShader->setUniform("inCastShadows", material().castShadows());
	_glShader->setUniform("inTexture", texture, 0);
	_glShader->setUniform("inTextureOffset", material().textureOffset());
	_glShader->setUniform("inTextureScale", material().textureScale());
	_glShader->setUniform("inAlphaCutoff", material().alphaCutoff());

}

void ShadowMap::bindPolyListDX11(bg::base::PolyList * plist) {
}

void ShadowMap::bindPolyListGL(bg::base::PolyList * plist, bool mobile) {
	_glShader->bindPolyList(plist);
}

void ShadowMap::unbindDX11() {
}

void ShadowMap::unbindGL(bool mobile) {
	_glShader->clearActive();
	_glShader->unbindPolyList();
}

}
}
