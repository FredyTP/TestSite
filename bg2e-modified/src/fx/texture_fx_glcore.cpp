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


#include <bg/fx/texture.hpp>

#include <bg/engine/openglCore/shader.hpp>
#include <bg/engine/openglEs/shader.hpp>

namespace bg {
namespace fx {

namespace textureOpenGL {

void getVertexShader(bool mobile, std::string & source) {
	if (mobile) {
		source = "#version 300 es\n";
	}
	else {
		source = "#version 330\n";
	}
	source += ""
		"in vec3 inPosition;\n"
		"in vec2 inTex0;\n"
	
		"out vec2 fsTex0Coord;\n"
	
		"void main() {\n"
		"	gl_Position = vec4(inPosition,1.0);\n"
		"	fsTex0Coord = inTex0;\n"
		"}";
}

void getFragmentShader(bool mobile, std::string & source) {
	if (mobile) {
		source = "#version 300 es\n"
				"precision highp float;\n"
				"out mediump vec4 fs_outColor;\n";
	}
	else {
		source = "#version 330\n"
				"layout (location = 0) out vec4 fs_outColor;\n";
	}
	source +=
		"uniform sampler2D inTexture;\n"

		"in vec2 fsTex0Coord;\n"

		"void main() {\n"
		"	fs_outColor = texture(inTexture, fsTex0Coord);\n"
		"}\n";
}

}

void Texture::buildGL(bool mobile) {
	std::string vshader;
	std::string fshader;
	textureOpenGL::getVertexShader(mobile, vshader);
	textureOpenGL::getFragmentShader(mobile, fshader);
	using namespace bg::engine;

	_glShader = mobile ? new openglEs::Shader(context()) :
						 new openglCore::Shader(context());
	
	_glShader->attachShader(openglCore::ShaderType::kVertexShader, vshader);
	_glShader->attachShader(openglCore::ShaderType::kFragmentShader, fshader);
	_glShader->link();
	_glShader->setOutputParameterName(openglCore::ShaderParamType::kShaderOutFragmentLocation, "fs_outColor");
	
	_glShader->addVertexBufferInput("inPosition", openglCore::VertexBufferRole::kRoleVertex);
	_glShader->addVertexBufferInput("inTex0", openglCore::VertexBufferRole::kRoleTex0);
	_glShader->initUniforms({ "inTexture"});
}

void Texture::activateGL(bool mobile, bg::base::Texture * tex) {
	_glShader->setActive();
	_glShader->setUniform("inTexture", tex, 0);
}


}
}
