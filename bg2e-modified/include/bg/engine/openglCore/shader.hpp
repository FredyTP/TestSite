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


#ifndef _bg2e_engine_openglCore_shader_hpp_
#define _bg2e_engine_openglCore_shader_hpp_

#include <bg/base/context_object.hpp>
#include <bg/engine/ogl_includes.hpp>
#include <bg/base/exception.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/base/texture.hpp>
#include <bg/math/vector.hpp>
#include <bg/math/matrix.hpp>
#include <bg/math/quaternion.hpp>
#include <bg/engine/ogl_impl.hpp>

#include <bg/engine/openglCore/opengl_state.hpp>

#include <string>
#include <vector>
#include <unordered_map>

#ifndef GL_TESS_CONTROL_SHADER
#define GL_TESS_CONTROL_SHADER 1
#endif

#ifndef GL_TESS_EVALUATION_SHADER
#define GL_TESS_EVALUATION_SHADER 2
#endif

#ifndef GL_GEOMETRY_SHADER
#define GL_GEOMETRY_SHADER 3
#endif

#ifndef GL_DRAW_INDIRECT_BUFFER
#define GL_DRAW_INDIRECT_BUFFER 4
#endif

#ifndef GL_TEXTURE_BUFFER
#define GL_TEXTURE_BUFFER 5
#endif

#ifndef GL_DOUBLE
#define GL_DOUBLE 6
#endif

namespace bg  {
namespace engine {
namespace openglCore {

enum ShaderType {
	kVertexShader		= GL_VERTEX_SHADER,
	kTessControlShader	= GL_TESS_CONTROL_SHADER,
	kTessEvalShader		= GL_TESS_EVALUATION_SHADER,
	kGeometryShader		= GL_GEOMETRY_SHADER,
	kFragmentShader		= GL_FRAGMENT_SHADER
};

enum ShaderParamType {
	kShaderOutFragmentLocation	= 1,
	kShaderInAttribure,
	kShaderInUniform
};

enum BindBufferTarget {
	kBindArrayBuffer = GL_ARRAY_BUFFER,
	kBindElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
	kBindCopyReadBuffer = GL_COPY_READ_BUFFER,
	kBindCopyWriteBuffer = GL_COPY_WRITE_BUFFER,
	kBindDrawIndirectBuffer = GL_DRAW_INDIRECT_BUFFER,
	kBindPixelPackBuffer = GL_PIXEL_PACK_BUFFER,
	kBindPixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
	kBindTextureBuffer = GL_TEXTURE_BUFFER,
	kBindTransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER,
	kBindUniformBuffer = GL_UNIFORM_BUFFER
};

enum BufferPointerType {
	kTypeByte = GL_BYTE,
	kTypeUnsignedByte = GL_UNSIGNED_BYTE,
	kTypeShort = GL_SHORT,
	kTypeUnsignedShort = GL_UNSIGNED_SHORT,
	kTypeInt = GL_INT,
	kTypeUnsignedInt = GL_UNSIGNED_INT,
	kTypeFloat = GL_FLOAT,
	kTypeFixed = GL_FIXED,
	kTypeHalfFloat = GL_HALF_FLOAT,
	kTypeDouble = GL_DOUBLE,
	kTypeInt_2_10_10_10 = GL_INT_2_10_10_10_REV,
	kTypeUnsignedInt_2_10_10_10 = GL_UNSIGNED_INT_2_10_10_10_REV,
	kTypeUnsignedInt_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV
};
	
enum VertexBufferRole {
	kRoleVertex = 1,
	kRoleNormal,
	kRoleTex0,
	kRoleTex1,
	kRoleTex2,
	kRoleColor,
	kRoleTangent,
	kRoleIndex
};

typedef std::unordered_map<std::string,GLint> ShaderUniformMap;	// attrib name, attrib location
	
class BG2E_EXPORT Shader : public bg::base::ContextObject,
						   public bg::base::ReferencedPointer,
						   public bg::engine::OpenGLImpl {
public:
	static bool DebugMode() { return s_debugMode; }
	static void	SetDebugMode(bool d) { s_debugMode = d; }
	static void ClearActive();
	
	Shader(bg::base::Context *);
	
	void attachShader(ShaderType shaderType, const std::string & shaderSource);
	void link();
	inline bool isLinked() { return _linked; }
	void setOutputParameterName(ShaderParamType type, const std::string & name, int dataLocation = 0);
	
	void destroy();

	void setActive();
	
	void clearActive();
	
	void addVertexBufferInput(const std::string &, VertexBufferRole);
	void initUniforms(const std::vector<std::string> &);
	void bindPolyList(bg::base::PolyList *);
	void unbindPolyList();
	
	void setUniform(const std::string & name, float v0);
	void setUniform(const std::string & name, float v0, float v1);
	void setUniform(const std::string & name, float v0, float v1, float v2);
	void setUniform(const std::string & name, float v0, float v1, float v2, float v3);
	void setUniform(const std::string & name, int v0);
	void setUniform(const std::string & name, int v0, int v1);
	void setUniform(const std::string & name, int v0, int v1, int v2);
	void setUniform(const std::string & name, int v0, int v1, int v2, int v3);
	void setUniform(const std::string & name, unsigned int v0);
	void setUniform(const std::string & name, unsigned int v0, unsigned int v1);
	void setUniform(const std::string & name, unsigned int v0, unsigned int v1, unsigned int v2);
	void setUniform(const std::string & name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3);
	void setUniform(const std::string & name, bool v);
	void setUniform(const std::string & name, float * v, int stride, int size);
	void setUniform(const std::string & name, int * v, int stride, int size);
	
	void setUniform(const std::string & name, const bg::math::Vector2 & v);
	void setUniform(const std::string & name, const bg::math::Vector3 & v);
	void setUniform(const std::string & name, const bg::math::Vector4 & v);
	void setUniform(const std::string & name, const bg::math::Quaternion & q);
	void setUniform(const std::string & name, const bg::math::Matrix3 & m, bool transpose = false);
	void setUniform(const std::string & name, const bg::math::Matrix4 & m, bool transpose = false);

	void setUniform(const std::string & name, const std::vector<float> &);
	void setUniform(const std::string & name, const std::vector<int> &);
	void setUniform(const std::string & name, const std::vector<bg::math::Vector2> &);
	void setUniform(const std::string & name, const std::vector<bg::math::Vector3> &);
	void setUniform(const std::string & name, const std::vector<bg::math::Vector4> &);
	
	void setUniform(const std::string & name, bg::base::Texture * tex, int unit);
	
protected:
	virtual ~Shader();
	
	bool _linked;
	bool _requirePatches;
	GLuint _shaderProgram;
	GLint _vertexLocation;
	GLint _normalLocation;
	GLint _tex0Location;
	GLint _tex1Location;
	GLint _tex2Location;
	GLint _colorLocation;
	GLint _tangentLocation;
	ShaderUniformMap _uniforms;
	
	bg::ptr<bg::base::PolyList> _bindedPlist;
	
	static bool s_debugMode;
	
    void bindAttrib(GLint buffer, GLint attrib, int size);
	void unbindAttrib(GLint buffer, GLint attrib);

	virtual bg::engine::openglCore::OpenGLState * gl() { return dynamic_cast<bg::engine::openglCore::GLStateObject*>(context()) ? dynamic_cast<bg::engine::openglCore::GLStateObject*>(context())->glState() : nullptr; }
};

}
}
}

#endif
