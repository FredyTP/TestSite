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


#include <bg/tools/shader_source.hpp>
#include <bg/base/exception.hpp>

namespace bg {
namespace tools {
 
ShaderSource::ShaderSource(TargetLanguage lang, int version)
	:_language(lang)
{
	switch (lang) {
		case kLangDirectX11:
			if (version==-1) version = 50;
			break;
		case kLangOpenGLCore:
			if (version==-1) version = 330;
			_buffer << "#version " << version << std::endl;
			break;
		case kLangOpenGLES3:
			if (version==-1) version = 300;
			_buffer << "#version " << version << " es" << std::endl;
			break;
	}
	_version = version;
}

void ShaderSource::addDXConstantBuffer(const std::string & name, const std::vector<shaderTypes::Parameter> & param) {
	if (_language==kLangDirectX11) {
		_buffer << "cbuffer " << name << "{" << std::endl;

		for (auto p : param) {
			_buffer << "\t";
			var(p);
			_buffer << std::endl;
		}

		_buffer << "};" << std::endl;
	}
}

void ShaderSource::requireExtension(const std::string & ext, unsigned int lang) {
	if ((_language==kLangOpenGLCore || _language==kLangOpenGLES3) && _language & lang) {
		_buffer << "#extension " << ext << " : require" << std::endl;
	}
}

void ShaderSource::addShaderInput(const shaderTypes::Parameter & p) {
	if(_language == kLangOpenGLCore || _language == kLangOpenGLES3) {
		_buffer << "in ";
		var(p);
		_buffer << std::endl;
	}
	_shaderInputs.push_back(p);
}

void ShaderSource::addUniform(const shaderTypes::Parameter & p) {
	if(_language == kLangOpenGLCore || _language == kLangOpenGLES3) {
		_buffer << "uniform ";
		var(p);
		_buffer << std::endl;
	}
	_shaderUniforms.push_back(p);
}

void ShaderSource::addShaderOutput(const shaderTypes::Parameter & p) {
	if(_language == kLangOpenGLCore || _language == kLangOpenGLES3) {
		if(p.layout != -1) {// && _language==kLangOpenGLCore) {
			_buffer << "layout (location = " << p.layout << ") ";
		}
		_buffer << "out ";
		if (p.precision && _language == kLangOpenGLES3) {
			switch (p.precision) {
				case shaderTypes::kLowp:
					_buffer << "lowp ";
					break;
				case shaderTypes::kMediump:
					_buffer << "mediump ";
					break;
				case shaderTypes::kHighp:
					_buffer << "highp ";
					break;
				case shaderTypes::kNone:
					break;
			};
		}
		var(p);
		_buffer << std::endl;
	}
	_shaderOutputs.push_back(p);
}

void ShaderSource::addShaderInput(const std::vector<shaderTypes::Parameter> & params) {
	for (auto p : params) {
		addShaderInput(p);
	}
}

void ShaderSource::addUniform(const std::vector<shaderTypes::Parameter> & params) {
	for(auto p : params) {
		addUniform(p);
	}
}

void ShaderSource::addShaderOutput(const std::vector<shaderTypes::Parameter> & params) {
	for(auto p : params) {
		addShaderOutput(p);
	}
}

void ShaderSource::precision(shaderTypes::Precision p, shaderTypes::Type t) {
	if (_language==kLangOpenGLES3) {
		_buffer << "precision ";
		switch (p) {
			case shaderTypes::kLowp:
				_buffer << "lowp";
				break;
			case shaderTypes::kMediump:
				_buffer << "mediump";
				break;
			case shaderTypes::kHighp:
				_buffer << "highp";
				break;
			case shaderTypes::kNone:
				break;
		}
		_buffer << " ";
		type(t);
		_buffer << ";" << std::endl;
	}
}

void ShaderSource::var(const shaderTypes::Parameter & p, const std::string & end) {
	if((_language == kLangOpenGLCore || _language==kLangOpenGLES3) && type(p.type)) {
		_buffer << " " << p.name;
		if (p.size>1) {
			_buffer << "[" << p.size << "]";
		}
		_buffer << end;
	}
	else if (_language == kLangDirectX11 && type(p.type)) {
		_buffer << " " << p.name;
		if (!p.semantics.empty()) {
			_buffer << " : " << p.semantics;
		}
		_buffer << end;
	}
}

void ShaderSource::addStruct(const std::string & name, const std::string & varName, const std::vector<shaderTypes::Parameter> & params) {
	_buffer << "struct " << name << " {" << std::endl;

	for (auto p : params) {
		_buffer << "\t";
		var(p);
		_buffer << std::endl;
	}

	_buffer << "}";
	if((_language & (kLangOpenGLCore | kLangOpenGLES3)) !=0) {
		_buffer << " " << varName;
	}
	_buffer << ";" << std::endl;
}

void ShaderSource::beginFunction(const std::string & returnType, const std::string & name, const std::vector<shaderTypes::Parameter> & params) {
	if (!returnType.empty()) {
		_buffer << returnType;
	}
	_buffer << " " << name << "(";

	size_t i = 0;
	for (auto p : params) {
		if (p.role==shaderTypes::kRoleOutput) {
			_buffer << "out ";
		}
		else if (p.role==shaderTypes::kRoleInputOutput) {
			_buffer << "in out ";
		}
		if(i<params.size() - 1) {
			var(p, ", ");
		}
		else {
			var(p, "");
		}
		++i;
	}

	_buffer << ") {" << std::endl;
}

void ShaderSource::beginFunction(shaderTypes::Type returnType, const std::string & name, const std::vector<shaderTypes::Parameter> & params) {
	type(returnType);
	beginFunction("",name, params);
}

//void ShaderSource::functionBody(const std::string & body) {
//	std::stringstream str(body);
//	char line[2048];
//	while (!str.eof()) {
//		str.getline(line, 2048);
//		_buffer << "\t" << line << std::endl;
//	}
//}

void ShaderSource::functionBody(const std::vector<std::string> & lines) {
	for (auto & line : lines) {
		if (line.empty()) {
			_buffer << std::endl;
		}
		else {
			_buffer << "\t";
			_buffer << line << std::endl;
		}
	}
}

void ShaderSource::endFunction() {
	_buffer << "}" << std::endl << std::endl;;
}



bool ShaderSource::type(shaderTypes::Type t) {
	if (_language==kLangDirectX11) {
		return insertDXType(t);
	}
	else if (_language==kLangOpenGLCore || _language==kLangOpenGLES3) {
		return insertGLType(t);
	}
	return false;
}

bool ShaderSource::insertDXType(shaderTypes::Type type) {
	using namespace shaderTypes;
	switch (type) {
	case kVoid:
		_buffer << "void";
		break;
	case kFloat:
		_buffer << "float";
		break;
	case kFloat2:
		_buffer << "float2";
		break;
	case kFloat3:
		_buffer << "float3";
		break;
	case kFloat4:
		_buffer << "float4";
		break;
	case kMatrix3:
		_buffer << "matrix <float, 3, 3>";
		break;
	case kMatrix4:
		_buffer << "matrix";
		break;
	case kInt:
		_buffer << "int";
		break;
	case kBool:
		_buffer << "bool";
		break;
	case kSampler2D:
		_buffer << "Texture2D";
		break;
	case kDXSamplerState:
		_buffer << "SamplerState";
		break;
	case kFloatOut:
		_buffer << "out float";
		break;
	case kFloat2Out:
		_buffer << "out float2";
		break;
	case kFloat3Out:
		_buffer << "out float3";
		break;
	case kFloat4Out:
		_buffer << "out float4";
		break;
	default:
		return false;
	}
	return true;
}

bool ShaderSource::insertGLType(shaderTypes::Type type) {
	using namespace shaderTypes;
	switch(type) {
	case kVoid:
		_buffer << "void";
		break;
	case kFloat:
		_buffer << "float";
		break;
	case kFloat2:
		_buffer << "vec2";
		break;
	case kFloat3:
		_buffer << "vec3";
		break;
	case kFloat4:
		_buffer << "vec4";
		break;
	case kMatrix3:
		_buffer << "mat3";
		break;
	case kMatrix4:
		_buffer << "mat4";
		break;
	case kInt:
		_buffer << "int";
		break;
	case kBool:
		_buffer << "bool";
		break;
	case kSampler2D:
		_buffer << "sampler2D";
		break;
	case kSamplerCubemap:
		_buffer << "samplerCube";
		break;
	case kFloatOut:
		_buffer << "out float";
		break;
	case kFloat2Out:
		_buffer << "out vec2";
		break;
	case kFloat3Out:
		_buffer << "out vec3";
		break;
	case kFloat4Out:
		_buffer << "out vec4";
		break;
	default:
		return false;
	}
	return true;
}

}
}
