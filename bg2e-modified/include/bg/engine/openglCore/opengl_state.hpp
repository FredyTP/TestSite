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

#ifndef _bg2e_engine_openglCore_opengl_state_hpp_
#define _bg2e_engine_openglCore_opengl_state_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/base/context_object.hpp>

#include <bg/engine/ogl_includes.hpp>
#include <bg/engine/ogl_enums.hpp>

#include <bg/math/math.hpp>

#include <map>

namespace bg {
namespace engine {
namespace openglCore {

class OpenGLState;
class BG2E_EXPORT GLStateObject {
public:
	GLStateObject(bg::base::Context *);
	
	inline bg::engine::openglCore::OpenGLState * glState() { return _glState.getPtr(); }
	inline const bg::engine::openglCore::OpenGLState * glState() const { return _glState.getPtr(); }
	
protected:
	bg::ptr<bg::engine::openglCore::OpenGLState> _glState;
};


// OpenGLState wrapper class
// IMPORTANT: This wrapper will work fine only if any OpenGL call
// is made directly using the OpenGL API.
class BG2E_EXPORT OpenGLState : public bg::base::ReferencedPointer,
								public bg::base::ContextObject {
public:
	enum FunctionId {
		k_viewport = 1,
		k_clearColor,
		k_clear,
		k_blendFunction,
		k_blendEquation,
		k_getBlendFunctionSource,
		k_getBlendFunctionDest,
		k_getBlendEquation,
		k_enableDepthTest,
		k_disableDepthTest,
		k_setDepthTestEnabled,
		k_depthTestEnabled,
		k_enableBlend,
		k_disableBlend,
		k_setBlendEnabled,
		k_blendEnabled,
		k_enableCullFace,
		k_disableCullFace,
		k_setCullFaceEnabled,
		k_cullFace,
		k_genVertexArrays,
		k_bindVertexArray,
		k_deleteVertexArray,
		k_vertexArrayBound,
		k_currentVertexArray,
		k_genBuffers,
		k_bindBuffer,
		k_bindArrayBuffer,
		k_bindElementArrayBuffer,
		k_bindCopyReadBuffer,
		k_bindCopyWriteBuffer,
		k_bindDrawIndirectBuffer,
		k_bindPixelPackBuffer,
		k_bindPixelUnpackBuffer,
		k_bindTransformFeedbackBuffer,
		k_bindUniformBuffer,
		k_bufferData,
		k_arrayBufferData,
		k_elementArrayBufferData,
		k_copyReadBufferData,
		k_copyWriteBufferData,
		k_drawIndirectBufferData,
		k_pixelPackBufferData,
		k_transformFeedbackBufferData,
		k_uniformBufferData,
		k_deleteBuffers,
		k_isBuffer,
		k_arrayBufferBound,
		k_currentArrayBuffer,
		k_elementArrayBufferBound,
		k_elementArrayBuffer,
		k_copyReadBufferBound,
		k_copyReadBuffer,
		k_copyWriteBufferBound,
		k_copyWriteBuffer,
		k_drawIndirectBufferBound,
		k_drawIndirectBuffer,
		k_pixelPackBufferBound,
		k_pixelPackBuffer,
		k_pixelUnpackBufferBound,
		k_pixelUnpackBuffer,
		k_transformFeedbackBufferBound,
		k_transformFeedbackBuffer,
		k_uniformBufferBound,
		k_uniformBuffer,
		k_createBuffers,
		k_genRenderbuffers,
		k_genFramebuffers,
		k_bindRenderbuffer,
		k_bindFramebuffer,
		k_renderbufferStorage,
		k_framebufferRenderbuffer,
		k_framebufferTexture2D,
		k_deleteRenderbuffers,
		k_deleteFramebuffers,
		k_drawBuffers,
		k_checkFramebufferStatus,
		k_renderbufferBound,
		k_currentRenderbuffer,
		k_framebufferBound,
		k_currentFramebuffer,
		k_genTextures,
		k_bindTexture,
		k_deleteTextures,
		k_texture1DBound,
		k_currentTexture1D,
		k_texture2DBound,
		k_currentTexture2D,
		k_texture3DBound,
		k_currentTexture3D,
		k_getTexImage,
		k_activeTexture,
		k_texParameteri,
		k_texImage2D,
		k_generateMipmap,
		k_patchParameteri,
		k_drawElements,
		k_drawArrays,
		k_readPixelsSupported,
		k_readPixels,
		k_createShader,
		k_shaderSource,
		k_compileShader,
		k_createProgram,
		k_getAttachedShaders,
		k_attachShader,
		k_linkProgram,
		k_useProgram,
		k_deleteShader,
		k_deleteProgram,
		k_programBound,
		k_currentProgram,
		k_bindFragDataLocation,
		k_getAttribLocation,
		k_getUniformLocation,
		k_enableVertexAttribArray,
		k_disableVertexAttribArray,
		k_vertexAttribPointer,
		k_uniform1f,
		k_uniform2f,
		k_uniform3f,
		k_uniform4f,
		k_uniform1i,
		k_uniform2i,
		k_uniform3i,
		k_uniform4i,
		k_uniform1ui,
		k_uniform2ui,
		k_uniform3ui,
		k_uniform4ui,
		k_uniform1fv,
		k_uniform2fv,
		k_uniform3fv,
		k_uniform4fv,
		k_uniform1iv,
		k_uniform2iv,
		k_uniform3iv,
		k_uniform4iv,
		k_uniformMatrix2fv,
		k_uniformMatrix3fv,
		k_uniformMatrix4fv,
		k_getString,
		k_getBooleanv,
		k_getDoublev,
		k_getFloatv,
		k_getIntegerv,
		k_getStringi,
		k_getBooleani_v,
		k_getDoublei_v,
		k_getFloati_v,
		k_getIntegeri_v,
		k_getBoolean,
		k_getDouble,
		k_getFloat,
		k_getInteger,
		k_getShaderiv,
		k_getShaderi,
		k_getShaderInfoLog,
		k_getProgramiv,
		k_getProgrami,
		k_getProgramInfoLog,
		k_InvalidateFramebuffer,
		k_invalidateTexImage
	};

	OpenGLState(bg::base::Context *);

	inline uint32_t getError() const {
		return _lastError;
	}
	
	// Synchronize the underlying OpenGL state with the wrapper state
	//	OpenGL > wrapper
	// Use this function the less as possible
	void sync();
	
	// Check driver capabilities
	void getExtensions(std::vector<std::string> & result);
	int getMaxDrawBuffers();
	
	void viewport(const bg::math::Viewport &);
	inline const bg::math::Viewport & currentViewport() const { return _viewport; }
	void clearColor(const bg::math::Color &);
	inline const bg::math::Color & currentclearcolor() const { return _clearColor; }
	void clear(uint32_t);

	void blendFunction(uint32_t src, uint32_t dst);
	void blendEquation(uint32_t eq);
	inline uint32_t getBlendFunctionSource() const { return _blendFuncSource; }
	inline uint32_t getBlendFunctionDest() const { return _blendFuncDest; }
	inline uint32_t getBlendEquation() const { return _blendEquation; }
	
	// equivalent to glEnable/glDisable
	void enableDepthTest();
	void disableDepthTest();
	inline bool setDepthTestEnabled(bool e) { e ? enableDepthTest() : disableDepthTest(); return _depthTest; }
	inline bool depthTestEnabled() const { return _depthTest; }
	
	void enableBlend();
	void disableBlend();
	inline bool setBlendEnabled(bool e) { e ? enableBlend() : disableBlend(); return _blend; }
	inline bool blendEnabled() const { return _blend; }
	
	void enableCullFace();
	void disableCullFace();
	inline bool setCullFaceEnabled(bool e) { e ? enableCullFace() : disableCullFace(); return _cullFace; }
	inline bool cullFace() const{ return _cullFace; }


	// Vertex/array buffers
	void genVertexArrays(size_t numArrays, uint32_t * arrays);
	void bindVertexArray(uint32_t a);
	void deleteVertexArray(size_t numArrays, uint32_t * arrays);
	inline bool vertexArrayBound() const { return _vertexArrayBound; }
	inline uint32_t currentVertexArray() const { return _vertexArray; }

	void genBuffers(size_t numBuffers, uint32_t * buffers);
	void bindBuffer(int32_t target, uint32_t buffer);
	void bindArrayBuffer(uint32_t buffer);
	void bindElementArrayBuffer(uint32_t buffer);
	void bindCopyReadBuffer(uint32_t buffer);
	void bindCopyWriteBuffer(uint32_t buffer);
	void bindDrawIndirectBuffer(uint32_t buffer);
	void bindPixelPackBuffer(uint32_t buffer);
	void bindPixelUnpackBuffer(uint32_t buffer);
	void bindTransformFeedbackBuffer(uint32_t buffer);
	void bindUniformBuffer(uint32_t buffer);

	void bufferData(uint32_t target, size_t size, const void * data, uint32_t usage);
	void arrayBufferData(size_t size, const void * data, uint32_t usage);
	void elementArrayBufferData(size_t size, const void * data, uint32_t usage);
	void copyReadBufferData(size_t size, const void * data, uint32_t usage);
	void copyWriteBufferData(size_t size, const void * data, uint32_t usage);
	void drawIndirectBufferData(size_t size, const void * data, uint32_t usage);
	void pixelPackBufferData(size_t size, const void * data, uint32_t usage);
	void transformFeedbackBufferData(size_t size, const void * data, uint32_t usage);
	void uniformBufferData(size_t size, const void * data, uint32_t usage);

	void deleteBuffers(size_t numBuffers, uint32_t * buffers);

	inline bool isBuffer(uint32_t b) const { return glIsBuffer(b)==GL_TRUE; }

	inline bool arrayBufferBound() const { return _arrayBufferBound; }
	inline uint32_t currentArrayBuffer() const { return _arrayBuffer; }
	inline bool elementArrayBufferBound() const { return _elementArrayBufferBound; }
	inline uint32_t elementArrayBuffer() const { return _elementArrayBuffer; }
	inline bool copyReadBufferBound() const { return _copyReadBufferBound; }
	inline uint32_t copyReadBuffer() const { return _copyReadBuffer; }
	inline bool copyWriteBufferBound() const { return _copyWriteBufferBound; }
	inline uint32_t copyWriteBuffer() const { return _copyWriteBuffer; }
	inline bool drawIndirectBufferBound() const { return _drawIndirectBufferBound; }
	inline uint32_t drawIndirectBuffer() const { return  _drawIndirectBuffer; }
	inline bool pixelPackBufferBound() const { return _pixelPackBufferBound; }
	inline uint32_t pixelPackBuffer() const { return _pixelPackBuffer; }
	inline bool pixelUnpackBufferBound() const { return _pixelUnpackBufferBound; }
	inline uint32_t pixelUnpackBuffer() const { return _pixelUnpackBuffer; }
	inline bool transformFeedbackBufferBound() const { return _transformFeedbackBufferBound; }
	inline uint32_t transformFeedbackBuffer() const { return _transformFeedbackBuffer; }
	inline bool uniformBufferBound() const { return _uniformBufferBound; }
	inline uint32_t uniformBuffer() const { return _uniformBuffer; }

	// OpenGL > 4.0 buffer functions
	void createBuffers(size_t n, uint32_t * buffers);


	// Framebuffers
	void genRenderbuffers(size_t n, uint32_t * renderbuffers);
	void genFramebuffers(size_t n, uint32_t * framebuffers);
	void bindRenderbuffer(uint32_t target, uint32_t renderbuffer);
	void bindFramebuffer(uint32_t target, uint32_t framebuffer);
	void renderbufferStorage(uint32_t target, uint32_t internalFormat, size_t width, size_t height);
	void framebufferRenderbuffer(uint32_t target, uint32_t attachment, uint32_t renderbufferTarget, uint32_t renderbuffer);
	void framebufferTexture2D(uint32_t target, uint32_t attachment, uint32_t texTarget, uint32_t texture, int level);
	void deleteRenderbuffers(size_t n, const uint32_t * renderbuffers);
	void deleteFramebuffers(size_t n, uint32_t * framebuffers);
	void drawBuffers(size_t n, const uint32_t * bufs);
	uint32_t checkFramebufferStatus(uint32_t target);
	inline bool renderbufferBound() const { return _renderbufferBound; }
	inline uint32_t currentRenderbuffer() const { return _renderbuffer; }
	inline bool framebufferBound() const { return _framebufferBound; }
	inline uint32_t currentFramebuffer() const { return _framebuffer; }

	// Textures
	void genTextures(size_t n, uint32_t * textures);
	void bindTexture(uint32_t target, uint32_t texture);
	void deleteTextures(uint32_t target, uint32_t * textures);
	inline bool texture1DBound() const { return _texture1DBound; }
	inline uint32_t currentTexture1D() const { return _texture1D; }
	inline bool texture2DBound() const { return _texture2DBound; }
	inline uint32_t currentTexture2D() const { return _texture2D; }
	inline bool texture3DBound() const { return _texture3DBound; }
	inline uint32_t currentTexture3D() const { return _texture3D; }
	void getTexImage(uint32_t target, int32_t level, uint32_t format, uint32_t type, void * buffer);
	void activeTexture(uint32_t textureUnit);

	void texParameteri(uint32_t target, uint32_t pname, int32_t param);
	void texImage2D(uint32_t target, int level, uint32_t inFormat, size_t width, size_t height, int border, uint32_t format, uint32_t type, const void * data);
	void generateMipmap(uint32_t target);

	void patchParameteri(uint32_t pname, int32_t value);

	void drawElements(uint32_t mode, size_t count, uint32_t type, const void * indices);
	void drawArrays(uint32_t mode, int32_t first, size_t count);

	inline bool readPixelsSupported() const { return BG2E_OPENGL_AVAILABLE==1; }
	void readPixels(int x, int y, size_t w, size_t h, uint32_t format, uint32_t type, void * data);

	// Shaders
	uint32_t createShader(uint32_t shaderType);
	void shaderSource(uint32_t shader, const std::string & source);
	void shaderSource(uint32_t shader, const std::vector<std::string> & sources);
	void compileShader(uint32_t shader);
	uint32_t createProgram();
	void getAttachedShaders(uint32_t program, size_t maxCount, size_t * count, uint32_t * shaders);
	void attachShader(uint32_t program, uint32_t shader);
	void linkProgram(uint32_t program);
	void useProgram(uint32_t);
	void deleteShader(uint32_t shader);
	void deleteProgram(uint32_t program);
	inline bool programBound() const { return _programBound; }
	inline uint32_t currentProgram() const { return _program; }

	void bindFragDataLocation(uint32_t program, uint32_t colorNumber, const std::string & name);
	int32_t getAttribLocation(uint32_t program, const std::string & name);
	int32_t getUniformLocation(uint32_t program, const std::string & name);
	void enableVertexAttribArray(uint32_t index);
	void disableVertexAttribArray(uint32_t index);
	void vertexAttribPointer(uint32_t index, int32_t size, uint32_t type, bool normalized, size_t stride, const void * pointer);

	void uniform1f(int32_t location, float v0);
	void uniform2f(int32_t location, float v0, float v1);
	void uniform3f(int32_t location, float v0, float v1, float v2);
	void uniform4f(int32_t location, float v0, float v1, float v2, float v3);
	void uniform1i(int32_t location, int32_t v0);
	void uniform2i(int32_t location, int32_t v0, int32_t v1);
	void uniform3i(int32_t location, int32_t v0, int32_t v1, int32_t v2);
	void uniform4i(int32_t location, int32_t v0, int32_t v1, int32_t v2, int32_t v3);
	void uniform1ui(int32_t location, uint32_t v0);
	void uniform2ui(int32_t location, uint32_t v0, uint32_t v1);
	void uniform3ui(int32_t location, uint32_t v0, uint32_t v1, uint32_t v2);
	void uniform4ui(int32_t location, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3);
	void uniform1fv(int32_t location, size_t count, const float * value);
	void uniform2fv(int32_t location, size_t count, const float * value);
	void uniform3fv(int32_t location, size_t count, const float * value);
	void uniform4fv(int32_t location, size_t count, const float * value);
	void uniform1iv(int32_t location, size_t count, const int32_t * value);
	void uniform2iv(int32_t location, size_t count, const int32_t * value);
	void uniform3iv(int32_t location, size_t count, const int32_t * value);
	void uniform4iv(int32_t location, size_t count, const int32_t * value);
	void uniformMatrix2fv(int32_t location, size_t count, bool transpose, const float * value);
	void uniformMatrix3fv(int32_t location, size_t count, bool transpose, const float * value);
	void uniformMatrix4fv(int32_t location, size_t count, bool transpose, const float * value);
	
	// glGet:
	std::string getString(uint32_t);
	void getBooleanv(uint32_t,bool *);
	void getDoublev(uint32_t,double *);
	void getFloatv(uint32_t,float *);
	void getIntegerv(uint32_t,int32_t *);
	std::string getStringi(uint32_t,int);
	void getBooleani_v(uint32_t,int,bool *);
	void getDoublei_v(uint32_t,int,double *);
	void getFloati_v(uint32_t,int,float *);
	void getIntegeri_v(uint32_t,int,int32_t *);
	bool getBoolean(uint32_t);
	double getDouble(uint32_t);
	float getFloat(uint32_t);
	int32_t getInteger(uint32_t);
	void getShaderiv(uint32_t, uint32_t, int32_t *);
	int32_t getShaderi(uint32_t, uint32_t);
	std::string getShaderInfoLog(uint32_t);
	void getProgramiv(uint32_t, uint32_t, int32_t *);
	int32_t getProgrami(uint32_t, uint32_t);
	std::string getProgramInfoLog(uint32_t);
	
	// Invalidate buffers
	void invalidateFramebuffer(uint32_t target, size_t numAttachments, const int32_t * attachments);
	void invalidateTexImage(uint32_t texture, int32_t level);

protected:
	virtual ~OpenGLState();
		
	bg::math::Viewport _viewport;
	bg::math::Color _clearColor;

	uint32_t _lastError;

	bool _depthTest;
	bool _blend;
	bool _cullFace;
	uint32_t _blendFuncSource;
	uint32_t _blendFuncDest;
	uint32_t _blendEquation;

	// Vertex/array buffers
	bool _vertexArrayBound = false;
	uint32_t _vertexArray = 0;
	
	bool _arrayBufferBound = false;
	uint32_t _arrayBuffer = 0;
	bool _elementArrayBufferBound = false;
	uint32_t _elementArrayBuffer = 0;
	bool _copyReadBufferBound = false;
	uint32_t _copyReadBuffer = 0;
	bool _copyWriteBufferBound = false;
	uint32_t _copyWriteBuffer = 0;
	bool _drawIndirectBufferBound = false;
	uint32_t _drawIndirectBuffer = 0;
	bool _pixelPackBufferBound = false;
	uint32_t _pixelPackBuffer = 0;
	bool _pixelUnpackBufferBound = false;
	uint32_t _pixelUnpackBuffer = 0;
	bool _transformFeedbackBufferBound = false;
	uint32_t _transformFeedbackBuffer = 0;
	bool _uniformBufferBound = false;
	uint32_t _uniformBuffer = 0;

	// Renderbuffers
	bool _renderbufferBound = false;
	uint32_t _renderbuffer = 0;
	bool _framebufferBound = false;
	uint32_t _framebuffer = 0;

	bool _texture1DBound = false;
	uint32_t _texture1D = 0;
	bool _texture2DBound = false;
	uint32_t _texture2D = 0;
	bool _texture3DBound = false;
	uint32_t _texture3D = 0;

	// Shaders
	bool _programBound = false;
	uint32_t _program;

	bool checkError(FunctionId, bool logIfError = true);

	void syncArrayBuffers();
	void syncBuffers();
	void syncRenderbuffers();
	void syncTexture();
	void syncProgram();

	static std::map<FunctionId,std::string> s_functionNames;
};

}
}
}

#endif
