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


#include <bg/fx/shadow.hpp>

#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/engine/openglCore/shader.hpp>
#include <bg/engine/openglEs/shader.hpp>

#include <bg/tools/texture_cache.hpp>
#include <bg/tools/shader_source.hpp>

#include <bg/shader/utils.hpp>
#include <bg/shader/lighting.hpp>
#include <bg/shader/library.hpp>

namespace bg {
namespace fx {

Shadow::Shadow(bg::base::Context * ctx)
	:Effect(ctx)
{
	using namespace bg::engine;
	bool mobile = Engine::Get()->identifier() == Engine::Identifier<OpenGLES3>();
	if(Engine::Get()->identifier() == Engine::Identifier<OpenGLCore>() || mobile) {
		buildGL(mobile);
	}
	else {
		throw bg::base::EngineException("fx::Shadow effect is not compatible with the current rendering API.");
	}
}

Shadow::~Shadow() {
}

void Shadow::activate() {
	using namespace bg::engine;
	activateGL(Engine::Get()->identifier() == Engine::Identifier<OpenGLES3>());
}

void Shadow::bindPolyList(bg::base::PolyList * plist) {
	_glShader->bindPolyList(plist);
}

void Shadow::unbind() {
	_glShader->clearActive();
	_glShader->unbindPolyList();
}

void Shadow::buildGL(bool mobile) {
	using namespace bg::engine;
	using namespace bg::tools;
	ShaderSource vshader(mobile ? ShaderSource::kLangOpenGLES3 :ShaderSource::kLangOpenGLCore);
	ShaderSource fshader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore);
	getVertexShader(vshader);
	getFragmentShader(fshader);
	_glShader = mobile ?	new openglEs::Shader(context()) :
							new openglCore::Shader(context());
	_glShader->attachShader(openglCore::kVertexShader, vshader.text());
	_glShader->attachShader(openglCore::kFragmentShader, fshader.text());
	_glShader->link();
	_glShader->setOutputParameterName(openglCore::ShaderParamType::kShaderOutFragmentLocation, "fs_outColor");

	vshader.initVertexBufferInputs(_glShader.getPtr());
	std::vector<std::string> uniformNames;
	vshader.uniformNames(uniformNames);
	fshader.uniformNames(uniformNames);
	_glShader->initUniforms(uniformNames);
}

void Shadow::activateGL(bool) {
	_glShader->setActive();

	bg::math::Matrix4 & model = matrixState().modelMatrixStack().matrix();
	bg::math::Matrix4 & view = matrixState().viewMatrixStack().matrix();
	bg::math::Matrix4 & proj = matrixState().projectionMatrixStack().matrix();
	_glShader->setUniform("inModelMatrix", model);
	_glShader->setUniform("inViewMatrix", view);
	_glShader->setUniform("inProjectionMatrix", proj);

	_glShader->setUniform("inLightProjectionMatrix", _light->projection());
	_glShader->setUniform("inLightViewMatrix", _shadowMap.valid() ? _shadowMap->tunedViewMatrix() : _lightTransform);

	bg::base::Texture * whiteTexture = bg::tools::TextureCache::WhiteTexture(context());
	bg::base::Texture * tex = _shadowMap.valid() ? _shadowMap->texture() : whiteTexture;
	_glShader->setUniform("inShadowMap", tex, 0);

	_glShader->setUniform("inShadowStrength", _light->shadowStrength());
	_glShader->setUniform("inShadowColor", _shadowMap.valid() ? _shadowMap->shadowColor() : bg::math::Color::Black());
	_glShader->setUniform("inShadowBias", _light->shadowBias());
	_glShader->setUniform("inShadowType", _shadowMap.valid() ? _shadowMap->shadowType() : 0);	// 0:hard, 1:soft, 2:stratified

	_glShader->setUniform("inTexture", _material->texture() ? _material->texture() : whiteTexture, 1);
	_glShader->setUniform("inTextureOffset", _material->textureOffset());
	_glShader->setUniform("inTextureScale", _material->textureScale());
	_glShader->setUniform("inAlphaCutoff", _material->alphaCutoff());

	_glShader->setUniform("inReceiveShadows", _material->receiveShadows());
}

void Shadow::getVertexShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;

	src.precision(kHighp, kFloat);
	src.addShaderInput(Parameter(kFloat3, "inVertex", kRoleVertex));
	src.addShaderInput(Parameter(kFloat2, "inTex0", kRoleTex0));

	src.addUniform({
		Parameter(kMatrix4, "inModelMatrix"),
		Parameter(kMatrix4, "inViewMatrix"),
		Parameter(kMatrix4, "inProjectionMatrix"),
		Parameter(kMatrix4, "inLightProjectionMatrix"),
		Parameter(kMatrix4, "inLightViewMatrix")
	});

	src.addShaderOutput({
		Parameter(kFloat2, "fsTexCoord"),
		Parameter(kFloat4, "fsVertexPosFromLight")
	});

	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"mat4 ScaleMatrix = mat4(0.5, 0.0, 0.0, 0.0,"
								"0.0, 0.5, 0.0, 0.0,"
								"0.0, 0.0, 0.5, 0.0,"
								"0.5, 0.5, 0.5, 1.0);",
		"fsVertexPosFromLight = ScaleMatrix * inLightProjectionMatrix * inLightViewMatrix * inModelMatrix * vec4(inVertex,1.0);",
		"fsTexCoord = inTex0;",
		"gl_Position = inProjectionMatrix * inViewMatrix * inModelMatrix * vec4(inVertex, 1.0);"
	});
	src.endFunction();
}

void Shadow::getFragmentShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;

	src.precision(kHighp, kFloat);

	src.addShaderOutput(Parameter(kFloat4, "fs_outColor", 0));

	src.addUniform({
		Parameter(kSampler2D, "inShadowMap"),
		Parameter(kFloat, "inShadowStrength"),
		Parameter(kFloat4, "inShadowColor"),
		Parameter(kFloat, "inShadowBias"),
		Parameter(kInt, "inShadowType"),
		Parameter(kSampler2D, "inTexture"),
		Parameter(kFloat2, "inTextureOffset"),
		Parameter(kFloat2, "inTextureScale"),
		Parameter(kFloat, "inAlphaCutoff"),
		Parameter(kBool, "inReceiveShadows")
	});

	src.addShaderInput({
		Parameter(kFloat2, "fsTexCoord"),
		Parameter(kFloat4, "fsVertexPosFromLight")
	});

	bg::shader::Utils(&src)
		.samplerColor()
		.random()
		.unpack();

	bg::shader::Lighting(&src)
		.getShadowColor();

	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"float alpha = samplerColor(inTexture,fsTexCoord,inTextureOffset,inTextureScale).a;",
		"if (alpha>inAlphaCutoff) {",
		"	vec4 shadowColor = vec4(1.0);",
		"	float shadowBias = inShadowBias;",
		"	if (inReceiveShadows) {",
		"		shadowColor = getShadowColor(fsVertexPosFromLight,inShadowMap,inShadowType,"
											"inShadowStrength,shadowBias,inShadowColor);",
		"	}",
		"	fs_outColor = shadowColor;",
		"}",
		"else {",
		"	discard;",
		"}"
	});
	src.endFunction();
}

}
}
