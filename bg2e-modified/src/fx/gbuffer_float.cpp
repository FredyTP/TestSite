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


#include <bg/fx/gbuffer.hpp>

#include <bg/engine/opengl_core.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/engine/openglCore/shader.hpp>
#include <bg/engine/openglEs/shader.hpp>

#include <bg/tools/texture_cache.hpp>
#include <bg/tools/shader_source.hpp>

#include <bg/shader/utils.hpp>


namespace bg {
namespace fx {

GBufferFloat::GBufferFloat(bg::base::Context * ctx)
	:GBuffer(ctx, "out_Position")
{
}

GBufferFloat::~GBufferFloat() {
}

void GBufferFloat::activateGL(bool mobile) {
	_glShader->setActive();

	bg::math::Matrix4 & model = matrixState().modelMatrixStack().matrix();
	bg::math::Matrix4 & view = matrixState().viewMatrixStack().matrix();
	bg::math::Matrix4 & proj = matrixState().projectionMatrixStack().matrix();

	_glShader->setUniform("inModelMatrix", model);
	_glShader->setUniform("inViewMatrix", view);
	_glShader->setUniform("inProjectionMatrix", proj);

	int textureUnit = 0;
	bg::base::Texture * whiteTexture = tools::TextureCache::WhiteTexture(context());
	_glShader->setUniform("inTexture", _material->texture() ? _material->texture() : whiteTexture, textureUnit++);
	_glShader->setUniform("inTextureOffset", _material->textureOffset());
	_glShader->setUniform("inTextureScale", _material->textureScale());
	_glShader->setUniform("inAlphaCutoff", _material->alphaCutoff());
}

void GBufferFloat::getVertexShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	
	src.precision(kHighp, kFloat);
	src.addShaderInput(Parameter(kFloat3, "inVertex", kRoleVertex));
	src.addShaderInput(Parameter(kFloat2, "inTex0", kRoleTex0));
	
	src.addUniform({
		Parameter(kMatrix4, "inModelMatrix"),
		Parameter(kMatrix4, "inViewMatrix"),
		Parameter(kMatrix4, "inProjectionMatrix"),
		Parameter(kMatrix4, "inNormalMatrix")
	});
	
	src.addShaderOutput({
		Parameter(kFloat3, "fsPosition"),
		Parameter(kFloat2, "fsTex0Coord"),
	});
	
	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		
		// TODO: 	Multiply view matrix in tesselation evaluation shader, when tessellation is implemented
		//"vec4 viewPos = inModelMatrix * vec4(inVertex, 1.0);",
		//"gl_Position = inProjectionMatrix * inViewMatrix * viewPos;"
		"vec4 viewPos = inViewMatrix * inModelMatrix * vec4(inVertex, 1.0);",
		"gl_Position = inProjectionMatrix * viewPos;",
		
		"fsTex0Coord = inTex0;",
		"fsPosition = viewPos.xyz;"
	});
	src.endFunction();
}

void GBufferFloat::getFragmentShader(bg::tools::ShaderSource & src) {
	using namespace bg::tools;
	using namespace bg::tools::shaderTypes;
	
	src.precision(kHighp, kFloat);
	
	//src.requireExtension("GL_EXT_draw_buffers");
	
	src.addShaderOutput(Parameter(kFloat4, "out_Position", 0));
	
	src.addUniform({
		Parameter(kSampler2D, "inTexture"),
		Parameter(kFloat2, "inTextureOffset"),
		Parameter(kFloat2, "inTextureScale"),
		Parameter(kFloat, "inAlphaCutoff")
	});
	
	src.addShaderInput({
		Parameter(kFloat3, "fsPosition"),
		Parameter(kFloat2, "fsTex0Coord")
	});
	
	bg::shader::Utils(&src)
	.samplerColor()
	.samplerNormal()
	.combineNormalWithMap()
	.applyTextureMask()
	.specularColor();
	
	src.beginFunction(kVoid, "main", {});
	src.functionBody({
		"vec4 diffuse = samplerColor(inTexture,fsTex0Coord,inTextureOffset,inTextureScale);",
		"if (diffuse.a >= inAlphaCutoff) {",
		"	out_Position = vec4(fsPosition,gl_FragCoord.z);",
		"}",
		"else {",
		"	discard;",
		"}"
	});
	src.endFunction();
}


}
}
