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


#include <bg/fx/color_picker_fx.hpp>

#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/engine/openglCore/shader.hpp>
#include <bg/engine/openglEs/shader.hpp>

#include <bg/tools/shader_source.hpp>

#include <bg/shader/utils.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace fx {


ColorPickerEffect::ColorPickerEffect(bg::base::Context * ctx)
	:Effect(ctx)
{
}

ColorPickerEffect::~ColorPickerEffect() {
}

void ColorPickerEffect::build() {
	using namespace bg::engine;
	if (Engine::Get()->identifier() == Engine::Identifier<OpenGLCore>()) {
		buildGL(false);
	}
	else if (Engine::Get()->identifier() == Engine::Identifier<OpenGLES3>()) {
		buildGL(true);
	}
	else {
		throw bg::base::EngineException("fx::ColorPickerEffect effect is not compatible with the current rendering API.");
	}
}

void ColorPickerEffect::activate() {
	using namespace bg::engine;
	activateGL(Engine::Get()->identifier() == Engine::Identifier<OpenGLES3>());
}

void ColorPickerEffect::bindPolyList(bg::base::PolyList * plist) {
	using namespace bg::engine;
	bindPolyListGL(plist, Engine::Get()->identifier() == Engine::Identifier<OpenGLES3>());
}

void ColorPickerEffect::unbind() {
	using namespace bg::engine;
	unbindGL(Engine::Get()->identifier() == Engine::Identifier<OpenGLES3>());
}

void ColorPickerEffect::buildGL(bool mobile) {
	using namespace bg::engine;
	using namespace bg::tools;
	int version = mobile ? 300 : 410;
	ShaderSource vshader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore, version);
	ShaderSource fshader(mobile ? ShaderSource::kLangOpenGLES3 : ShaderSource::kLangOpenGLCore, version);
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

void ColorPickerEffect::activateGL(bool mobile) {
	_glShader->setActive();
	
	bg::math::Matrix4 & model = matrixState().modelMatrixStack().matrix();
	bg::math::Matrix4 & view = matrixState().viewMatrixStack().matrix();
	bg::math::Matrix4 & proj = matrixState().projectionMatrixStack().matrix();
	
	_glShader->setUniform("inModelMatrix", model);
	_glShader->setUniform("inViewMatrix", view);
	_glShader->setUniform("inProjectionMatrix", proj);
	
	_glShader->setUniform("inColorIdentifier", _colorPickId.colorValue());
	//_glShader->setUniform("inColorIdentifier", bg::math::Color::Yellow());
}

void ColorPickerEffect::bindPolyListGL(bg::base::PolyList * plist, bool) {
	_glShader->bindPolyList(plist);
}

void ColorPickerEffect::unbindGL(bool mobile) {
	_glShader->clearActive();
	_glShader->unbindPolyList();
}


void ColorPickerEffect::getVertexShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	
	src.precision(kHighp, kFloat);
	src.addShaderInput(Parameter(kFloat3, "inVertex", kRoleVertex));
	
	src.addUniform({
		Parameter(kMatrix4, "inModelMatrix"),
		Parameter(kMatrix4, "inViewMatrix"),
		Parameter(kMatrix4, "inProjectionMatrix")
	});
	
	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"gl_Position = inProjectionMatrix * inViewMatrix * inModelMatrix * vec4(inVertex, 1.0);"
	});
	src.endFunction();
}

void ColorPickerEffect::getFragmentShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	
	src.precision(kHighp, kFloat);
	
	src.addShaderOutput(Parameter(kFloat4, "fs_outColor", 0));
	
	src.addUniform({
		Parameter(kFloat4, "inColorIdentifier")
	});
	
	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"fs_outColor = inColorIdentifier;"
	});
	src.endFunction();
}

	
}
}
