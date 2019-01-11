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


#include <bg/engine/openglCore/shader.hpp>
#include <bg/engine/openglCore/poly_list_impl.hpp>
#include <bg/engine/openglCore/texture_impl.hpp>

#include <bg/log.hpp>

#include <iostream>
#include <string>

#include <bg/android/cplusplus_support.hpp>

namespace bg {
namespace engine {
namespace openglCore {

bool Shader::s_debugMode = true;

void Shader::ClearActive() {
	glUseProgram(0);
	glGetError();
	glBindTexture(GL_TEXTURE_2D, 0);
}

Shader::Shader(bg::base::Context * ctx)
	:bg::base::ContextObject(ctx)
	,_linked(false)
	,_requirePatches(false)
	,_shaderProgram(0)
	,_vertexLocation(-1)
	,_normalLocation(-1)
	,_tex0Location(-1)
	,_tex1Location(-1)
	,_tex2Location(-1)
	,_colorLocation(-1)
	,_tangentLocation(-1)
{
	_shaderProgram = gl()->createProgram();
}

Shader::~Shader()
{
	destroy();
}


void Shader::attachShader(ShaderType shaderType, const std::string & shaderSource) {
#if BG2E_OPENGL_ES_AVAILABLE==0
	_requirePatches = _requirePatches ||
						shaderType==kTessControlShader ||
						shaderType==kTessEvalShader ||
						shaderType==kGeometryShader;
#else
	_requirePatches = false;
#endif
	if (!_linked) {
		using namespace bg::engine::opengl;
		if (_shaderProgram==-1) {
			ErrorInfo::ClearError();
			throw bg::base::ShaderException("Could not attach shader: shaderProgram is null");
		}
		
		GLint shader = gl()->createShader(shaderType);
		gl()->shaderSource(shader, shaderSource);
		gl()->compileShader(shader);
		
		if (!ErrorInfo::CompileShaderStatus(shader)) {
			std::string error = ErrorInfo::CompileErrorString(shader);
			ErrorInfo::ClearError();
            bg::log(bg::log::kError) << "Shader compile error: " << error << bg::endl;
            std::stringstream strstream(shaderSource);
            std::string line;
            int lineNumber = 1;
            while (std::getline(strstream,line)) {
                std::string spaces = "";
                if (lineNumber<10) {
                    spaces = "  ";
                }
                else if (lineNumber<100) {
                    spaces = " ";
                }
                bg::log(bg::log::kError) << spaces << lineNumber++ << ": " << line << bg::endl;
            }
			throw bg::base::ShaderCompileException(error);
		}
		
		ErrorInfo::Error();
		if (!glIsProgram(_shaderProgram)) {
			ErrorInfo::ClearError();
			throw bg::base::ShaderException("Error: shader program is not a program object");
		}
		if (!glIsShader(shader)) {
			ErrorInfo::ClearError();
			throw bg::base::ShaderException("Error: shader is not a valid shader");
		}
		GLuint shaders[100];
		size_t count;
		size_t newCount;
		gl()->getAttachedShaders(_shaderProgram, 100, &count, shaders);
		gl()->attachShader(_shaderProgram, shader);
		ErrorInfo::ClearError();
		gl()->getAttachedShaders(_shaderProgram, 100, &newCount, shaders);
		if (newCount!=count+1) {
			std::string error = ErrorInfo::CompileErrorString(shader);
			std::string link = ErrorInfo::LinkErrorString(_shaderProgram);
			std::string compileError = ErrorInfo::ErrorString();
			ErrorInfo::ClearError();
			throw bg::base::ShaderException(compileError);
		}
		
		gl()->deleteShader(shader);
	}
}
	
void Shader::link() {
	using namespace bg::engine::opengl;
	if (!_linked) {
		if (_shaderProgram==-1) {
			throw bg::base::ShaderLinkException(": Could not link shader: shaderProgram is null");
		}
		gl()->linkProgram(_shaderProgram);
		if (ErrorInfo::LinkProgramStatus(_shaderProgram)==GL_FALSE) {
			std::string linkError = ErrorInfo::LinkErrorString(_shaderProgram);
			ErrorInfo::ClearError();
			throw bg::base::ShaderLinkException(linkError);
		}
		_linked = true;
	}
}

void Shader::setOutputParameterName(ShaderParamType type, const std::string & name, int dataLocation) {
#if BG2E_OPENGL_ES_AVAILABLE==0
	gl()->bindFragDataLocation(_shaderProgram, dataLocation, name);
#endif
}

void Shader::destroy() {
	if(_shaderProgram && gl()) {
		gl()->deleteProgram(_shaderProgram);
	}
	else if(_shaderProgram) {	// In some cases, the context may be deleted before reach shader destructor
		glDeleteProgram(_shaderProgram);
	}
}
	
void Shader::setActive() {
	gl()->useProgram(_shaderProgram);
}

void Shader::clearActive() {
	gl()->useProgram(0);
}

void Shader::addVertexBufferInput(const std::string & name, VertexBufferRole role) {
	GLint location = gl()->getAttribLocation(_shaderProgram, name);
	if (location<0) {
		if (Shader::DebugMode()) {
			bg::log(bg::log::kWarning) << "Input attribute not found in shader: " << name << bg::endl;
		}
		else {
			throw bg::base::ShaderException("Input attribute not found in shader: " + name);
		}
	}
	switch (role) {
	case kRoleVertex:
		_vertexLocation = location;
		break;
	case kRoleNormal:
		_normalLocation = location;
		break;
	case kRoleTex0:
		_tex0Location = location;
		break;
	case kRoleTex1:
		_tex1Location = location;
		break;
	case kRoleTex2:
		_tex2Location = location;
		break;
	case kRoleTangent:
		_tangentLocation = location;
		break;
	case kRoleColor:
		_colorLocation = location;
		break;
	default:
		throw bg::base::ShaderException("Invalid vertex buffer role specified");
		break;
	}
}

void Shader::initUniforms(const std::vector<std::string> & uniforms) {
	for (auto uniformName : uniforms) {
		GLint loc = gl()->getUniformLocation(_shaderProgram, uniformName);
		if (loc<0) {
			if (Shader::DebugMode()) {
				bg::log(bg::log::kWarning) << "SHADER DEBUG: Uniform not found in shader: " << uniformName << bg::endl;
			}
			else {
				throw bg::base::ShaderException("Uniform not found in shader: " + uniformName);
			}
		}
		else {
			_uniforms[uniformName] = loc;
		}
	}
}
    
void Shader::bindAttrib(GLint buffer, GLint attrib, int size) {
	using namespace bg::engine::opengl;
#if BG2E_OPENGL_AVAILABLE==1 || BG2E_OPENGL_ES_AVAILABLE==1
    if (buffer!=0 && attrib!=-1) {
        gl()->bindBuffer(kArrayBuffer, buffer);
        gl()->enableVertexAttribArray(attrib);
        gl()->vertexAttribPointer(attrib, size, kFloat, false, 0, 0);
    }
#endif
}

void Shader::bindPolyList(bg::base::PolyList * plist) {
	openglCore::PolyListImpl * impl = plist->impl<openglCore::PolyListImpl>();
	if (impl) {
		if (impl->vertexArrayObject()!=-1) {
			gl()->bindVertexArray(impl->vertexArrayObject());
		}
		bindAttrib(impl->vertexBuffer(), _vertexLocation, 3);
		bindAttrib(impl->normalBuffer(), _normalLocation, 3);
		bindAttrib(impl->texCoord0Buffer(), _tex0Location, 2);
		bindAttrib(impl->texCoord1Buffer(), _tex1Location, 2);
		bindAttrib(impl->texCoord2Buffer(), _tex2Location, 2);
		bindAttrib(impl->colorBuffer(), _colorLocation, 4);
		bindAttrib(impl->tangentBuffer(), _tangentLocation, 3);
		_bindedPlist = plist;
	}
}

void Shader::unbindAttrib(GLint buffer, GLint attrib) {
	using namespace bg::engine::opengl;
	if (buffer!=0 && attrib>0) {
        gl()->disableVertexAttribArray(attrib);
	}
}

void Shader::unbindPolyList() {
	if (!_bindedPlist.valid()) return;
	setActive();
	openglCore::PolyListImpl * impl = _bindedPlist->impl<openglCore::PolyListImpl>();
	if (impl) {
		
		unbindAttrib(impl->vertexBuffer(), _vertexLocation);
		unbindAttrib(impl->normalBuffer(), _normalLocation);
		unbindAttrib(impl->texCoord0Buffer(), _tex0Location);
		unbindAttrib(impl->texCoord1Buffer(), _tex1Location);
		unbindAttrib(impl->texCoord2Buffer(), _tex2Location);
		unbindAttrib(impl->tangentBuffer(), _tangentLocation);
		unbindAttrib(impl->colorBuffer(), _colorLocation);
		if (impl->vertexArrayObject()>0) {
            gl()->bindVertexArray(0);
        }
	}
}

void Shader::setUniform(const std::string & name, float v0) {
	gl()->uniform1f(_uniforms[name], v0);
}

void Shader::setUniform(const std::string & name, float v0, float v1) {
	gl()->uniform2f(_uniforms[name], v0, v1);
}

void Shader::setUniform(const std::string & name, float v0, float v1, float v2) {
	gl()->uniform3f(_uniforms[name], v0, v1, v2);
}

void Shader::setUniform(const std::string & name, float v0, float v1, float v2, float v3) {
	gl()->uniform4f(_uniforms[name], v0, v1, v2, v3);
}

void Shader::setUniform(const std::string & name, int v0) {
	gl()->uniform1i(_uniforms[name], v0);
}

void Shader::setUniform(const std::string & name, int v0, int v1) {
	gl()->uniform2i(_uniforms[name], v0, v1);
}

void Shader::setUniform(const std::string & name, int v0, int v1, int v2) {
	gl()->uniform3i(_uniforms[name], v0, v1, v2);
}

void Shader::setUniform(const std::string & name, int v0, int v1, int v2, int v3) {
	gl()->uniform4i(_uniforms[name], v0, v1, v2, v3);
}

void Shader::setUniform(const std::string & name, unsigned int v0) {
	gl()->uniform1i(_uniforms[name], v0);
}

void Shader::setUniform(const std::string & name, unsigned int v0, unsigned int v1) {
	gl()->uniform2i(_uniforms[name], v0, v1);
}

void Shader::setUniform(const std::string & name, unsigned int v0, unsigned int v1, unsigned int v2) {
	gl()->uniform3i(_uniforms[name], v0, v1, v2);
}

void Shader::setUniform(const std::string & name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) {
	gl()->uniform4i(_uniforms[name], v0, v1, v2, v3);
}

void Shader::setUniform(const std::string & name, bool v) {
	gl()->uniform1i(_uniforms[name], v);
}

void Shader::setUniform(const std::string & name, float * v, int stride, int size) {
	switch (stride) {
	case 1:
		gl()->uniform1fv(_uniforms[name], size, v);
		break;
	case 2:
		gl()->uniform2fv(_uniforms[name], size, v);
		break;
	case 3:
		gl()->uniform3fv(_uniforms[name], size, v);
		break;
	case 4:
		gl()->uniform4fv(_uniforms[name], size, v);
		break;
	default:
		throw bg::base::InvalidParameterException("Invalid stride specified as shader parameter. Valid values: 1, 2, 3, 4. specified value: " + std::to_string(stride));
	}
}

void Shader::setUniform(const std::string & name, int * v, int stride, int size) {
	switch(stride) {
	case 1:
		gl()->uniform1iv(_uniforms[name], size, v);
		break;
	case 2:
		gl()->uniform2iv(_uniforms[name], size, v);
		break;
	case 3:
		gl()->uniform3iv(_uniforms[name], size, v);
		break;
	case 4:
		gl()->uniform4iv(_uniforms[name], size, v);
		break;
	default:
		throw bg::base::InvalidParameterException("Invalid stride specified as shader parameter. Valid values: 1, 2, 3, 4. specified value: " + std::to_string(stride));
	}
}

void Shader::setUniform(const std::string & name, const bg::math::Vector2 & v) {
	gl()->uniform2fv(_uniforms[name], 1, v.raw());
}

void Shader::setUniform(const std::string & name, const bg::math::Vector3 & v) {
	gl()->uniform3fv(_uniforms[name], 1, v.raw());
}

void Shader::setUniform(const std::string & name, const bg::math::Vector4 & v) {
	gl()->uniform4fv(_uniforms[name], 1, v.raw());
}

void Shader::setUniform(const std::string & name, const bg::math::Quaternion & q) {
	gl()->uniform4fv(_uniforms[name], 1, q.raw());
}

void Shader::setUniform(const std::string & name, const bg::math::Matrix3 & m, bool transpose) {
	gl()->uniformMatrix3fv(_uniforms[name], 1, transpose, m.raw());
}

void Shader::setUniform(const std::string & name, const bg::math::Matrix4 & m, bool transpose) {
	gl()->uniformMatrix4fv(_uniforms[name], 1, transpose, m.raw());
}

void Shader::setUniform(const std::string & name, const std::vector<float> & vec) {
	gl()->uniform1fv(_uniforms[name], vec.size(), &vec[0]);
}

void Shader::setUniform(const std::string & name, const std::vector<int> & vec) {
	gl()->uniform1iv(_uniforms[name], vec.size(), &vec[0]);
}

void Shader::setUniform(const std::string & name, const std::vector<bg::math::Vector2> & vec) {
	std::vector<float> v;
	for (auto n : vec) {
		v.push_back(n[0]);
		v.push_back(n[1]);
	}
	gl()->uniform2fv(_uniforms[name], vec.size(), &v[0]);
}

void Shader::setUniform(const std::string & name, const std::vector<bg::math::Vector3> & vec) {
	std::vector<float> v;
	for(auto n : vec) {
		v.push_back(n[0]);
		v.push_back(n[1]);
		v.push_back(n[2]);
	}
	gl()->uniform3fv(_uniforms[name], vec.size(), &v[0]);
}

void Shader::setUniform(const std::string & name, const std::vector<bg::math::Vector4> & vec) {
	std::vector<float> v;
	for(auto n : vec) {
		v.push_back(n[0]);
		v.push_back(n[1]);
		v.push_back(n[2]);
		v.push_back(n[3]);
	}
	gl()->uniform4fv(_uniforms[name], vec.size(), &v[0]);
}

void Shader::setUniform(const std::string & name, bg::base::Texture * tex, int unit) {
	if (tex && _uniforms.find(name)!=_uniforms.end()) {
		tex->impl<openglCore::TextureImpl>()->setActive(unit);
		tex->impl<openglCore::TextureImpl>()->bindTexture2D();
		gl()->uniform1i(_uniforms[name],unit);
	}
}


}
}
}
