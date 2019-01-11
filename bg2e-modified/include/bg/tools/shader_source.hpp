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

#ifndef _bg2e_tools_shader_source_hpp_
#define _bg2e_tools_shader_source_hpp_

#include <bg/export.hpp>
#include <bg/engine/openglCore/shader.hpp>

#include <vector>
#include <string>
#include <sstream>

#include <functional>

namespace bg {
namespace tools {

/*
 *	ShaderSource: create a string buffer with a shader source code
 *	and provides utilities to create shader code libraries and
 *	debugging tools.
 *
 *	There are three types of functions in ShaderSource:
 *	- The specific definition functions only generates source code if the target language
 *	matchs the function language.
 *	- The automatic definitions generates code that automatically match the syntax of
 *	the target language
 *	- The source code functions simply adds source code, and the developer is responsible of
 *	choose the correct syntax
 *
 *	Also, some typed definition does not generate any output, depending on the target language,
 *	for example, a SamplerState variable only produces output in DirectX11
 *
 *	Currently, the ShaderSource class supports generation of code in HLSL 5 and GLSL. The version
 *	of the code can be specified using the constructor. If nothing is specified, the default
 *	values for the version are:
 *		- 5.0 for DirectX11
 *		- 330 for OpenGL Core Profile
 *		- 300 for OpenGL ES 3
 */

namespace shaderTypes {

enum Type {
	kVoid = 0,
	kFloat = 1,
	kFloat2,
	kFloat3,
	kFloat4,
	kMatrix3,
	kMatrix4,
	kInt,
	kBool,
	kSampler2D,
	kSamplerCubemap,
	kDXSamplerState,
	kFloatOut,
	kFloat2Out,
	kFloat3Out,
	kFloat4Out
	// TODO: Complete the list of types
};

enum Precision {
	kNone = 0,
	kLowp = 1,
	kMediump,
	kHighp
};

enum ParamRole {
	kRoleVertex = bg::engine::openglCore::VertexBufferRole::kRoleVertex,
	kRoleNormal = bg::engine::openglCore::VertexBufferRole::kRoleNormal,
	kRoleTex0 = bg::engine::openglCore::VertexBufferRole::kRoleTex0,
	kRoleTex1 = bg::engine::openglCore::VertexBufferRole::kRoleTex1,
	kRoleTex2 = bg::engine::openglCore::VertexBufferRole::kRoleTex2,
	kRoleColor = bg::engine::openglCore::VertexBufferRole::kRoleColor,
	kRoleTangent = bg::engine::openglCore::VertexBufferRole::kRoleTangent,
	kRoleUniform = 0x100000,
	kRoleOutput = 0x200000,
	kRoleInputOutput = 0x300000
};

struct Parameter {
	shaderTypes::Type type;
	std::string name;
	std::string semantics;
	int layout;
	Precision precision;
	ParamRole role;
	int size;

	Parameter(shaderTypes::Type t, const std::string & n) :type(t), name(n), layout(-1), precision(kNone), role(kRoleUniform), size(1) {}
	Parameter(shaderTypes::Type t, const std::string & n, ParamRole r) :type(t), name(n), layout(-1), precision(kNone), role(r), size(1) {}
	Parameter(shaderTypes::Type t, const std::string & n, const std::string & s) :type(t), name(n), semantics(s), layout(-1), precision(kNone), role(kRoleUniform), size(1) {}
	Parameter(shaderTypes::Type t, const std::string & n, int l) :type(t), name(n), layout(l), precision(kNone), role(kRoleUniform), size(1) {}
	Parameter(shaderTypes::Type t, const std::string & n, Precision p) :type(t), name(n), precision(p), role(kRoleUniform), size(1) {}
	Parameter(shaderTypes::Type t, int s, const std::string & n) :type(t), name(n), precision(kNone), role(kRoleUniform), size(s) {}
};

}

class BG2E_EXPORT ShaderSource {
public:
	enum TargetLanguage {
		kLangDirectX11		= 0x1,
		kLangOpenGLCore		= 0x1 << 1,
		kLangOpenGLES3		= 0x1 << 2
	};
	
	
	
	
	ShaderSource(TargetLanguage, int version = -1);
	
	inline void clear() { _buffer.clear(); }
	inline const std::string & text() { _string = _buffer.str(); return _string; }

	/*
	 *	DirectX specific functions:
	 */
	void addDXConstantBuffer(const std::string &, const std::vector<shaderTypes::Parameter> &);
	
	/*
	 *	OpenGL specific functions
	 */
	void requireExtension(const std::string &, unsigned int lang = kLangOpenGLCore);
	void addShaderInput(const shaderTypes::Parameter &);
	void addUniform(const shaderTypes::Parameter &);
	void addShaderOutput(const shaderTypes::Parameter &);
	void addShaderInput(const std::vector<shaderTypes::Parameter> &);
	void addUniform(const std::vector<shaderTypes::Parameter> &);
	void addShaderOutput(const std::vector<shaderTypes::Parameter> &);

	// OpenGL ES specific
	void precision(shaderTypes::Precision p, shaderTypes::Type t);
	
	/*
	 *	Automatic definitions
	 */
	void var(const shaderTypes::Parameter &, const std::string & end = ";");

	void addStruct(const std::string & name, const std::string & varName, const std::vector<shaderTypes::Parameter> &);

	void beginFunction(const std::string & returnType, const std::string & name, const std::vector<shaderTypes::Parameter> &);
	void beginFunction(shaderTypes::Type returnType, const std::string & name, const std::vector<shaderTypes::Parameter> &);
	void addFunctionBody(const std::string & src) { _buffer << src; }
	void functionBodyString(const std::string & src) { _buffer << src; }
	void functionBody(const std::vector<std::string> &);
	void endFunction();

	/*
	 *	Source code functions
	 */
	inline std::stringstream & buffer() { return _buffer; }

	inline const std::vector<shaderTypes::Parameter> & shaderInputs() const { return _shaderInputs; }
	inline const std::vector<shaderTypes::Parameter> & shaderUniforms() const { return _shaderInputs; }
	inline const std::vector<shaderTypes::Parameter> & shaderOutputs() const { return _shaderInputs; }

	inline void eachInput(std::function<void(const shaderTypes::Parameter & p)> closure) {
		for (auto p : _shaderInputs) {
			closure(p);
		}
	}

	inline void eachUniform(std::function<void(const shaderTypes::Parameter & p)> closure) {
		for(auto p : _shaderUniforms) {
			closure(p);
		}
	}

	inline void eachOutput(std::function<void(const shaderTypes::Parameter & p)> closure) {
		for(auto p : _shaderOutputs) {
			closure(p);
		}
	}

	inline void uniformNames(std::vector<std::string> & names) {
		eachUniform([&](const shaderTypes::Parameter & p) {
			names.push_back(p.name);
		});
	}

	inline void initVertexBufferInputs(bg::engine::openglCore::Shader * shader) {
		eachInput([&](const shaderTypes::Parameter & p) {
			if (p.role == shaderTypes::kRoleVertex ||
				p.role == shaderTypes::kRoleNormal ||
				p.role == shaderTypes::kRoleTex0 ||
				p.role == shaderTypes::kRoleTex1 ||
				p.role == shaderTypes::kRoleTex2 ||
				p.role == shaderTypes::kRoleColor ||
				p.role == shaderTypes::kRoleTangent)
			{
				shader->addVertexBufferInput(p.name, static_cast<bg::engine::openglCore::VertexBufferRole>(p.role));
			}
		});
	}

protected:
	TargetLanguage _language;
	std::stringstream _buffer;
	std::string _string;
	int _version;
	std::vector<shaderTypes::Parameter> _shaderInputs;
	std::vector<shaderTypes::Parameter> _shaderUniforms;
	std::vector<shaderTypes::Parameter> _shaderOutputs;

	// Returns true if the type can be applied to the current language
	bool type(shaderTypes::Type);
	bool insertDXType(shaderTypes::Type);
	bool insertGLType(shaderTypes::Type);
};

}
}

#endif
