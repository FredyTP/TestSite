
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

#include <bg/engine/openglCore/opengl_state.hpp>

#include <bg/log.hpp>

#include <sstream>
#include <iostream>

namespace bg {
namespace engine {
namespace openglCore {

std::map<OpenGLState::FunctionId,std::string> OpenGLState::s_functionNames {
	{ k_viewport, "k_viewport  =" },
	{ k_clearColor, "clearColor" },
	{ k_clear, "clear" },
	{ k_blendFunction, "blendFunction" },
	{ k_blendEquation, "blendEquation" },
	{ k_getBlendFunctionSource, "getBlendFunctionSource" },
	{ k_getBlendFunctionDest, "getBlendFunctionDest" },
	{ k_getBlendEquation, "getBlendEquation" },
	{ k_enableDepthTest, "enableDepthTest" },
	{ k_disableDepthTest, "disableDepthTest" },
	{ k_setDepthTestEnabled, "setDepthTestEnabled" },
	{ k_depthTestEnabled, "depthTestEnabled" },
	{ k_enableBlend, "enableBlend" },
	{ k_disableBlend, "disableBlend" },
	{ k_setBlendEnabled, "setBlendEnabled" },
	{ k_blendEnabled, "blendEnabled" },
	{ k_enableCullFace, "enableCullFace" },
	{ k_disableCullFace, "disableCullFace" },
	{ k_setCullFaceEnabled, "setCullFaceEnabled" },
	{ k_cullFace, "cullFace" },
	{ k_genVertexArrays, "genVertexArrays" },
	{ k_bindVertexArray, "bindVertexArray" },
	{ k_deleteVertexArray, "deleteVertexArray" },
	{ k_vertexArrayBound, "vertexArrayBound" },
	{ k_currentVertexArray, "currentVertexArray" },
	{ k_genBuffers, "genBuffers" },
	{ k_bindBuffer, "bindBuffer" },
	{ k_bindArrayBuffer, "bindArrayBuffer" },
	{ k_bindElementArrayBuffer, "bindElementArrayBuffer" },
	{ k_bindCopyReadBuffer, "bindCopyReadBuffer" },
	{ k_bindCopyWriteBuffer, "bindCopyWriteBuffer" },
	{ k_bindDrawIndirectBuffer, "bindDrawIndirectBuffer" },
	{ k_bindPixelPackBuffer, "bindPixelPackBuffer" },
	{ k_bindPixelUnpackBuffer, "bindPixelUnpackBuffer" },
	{ k_bindTransformFeedbackBuffer, "bindTransformFeedbackBuffer" },
	{ k_bindUniformBuffer, "bindUniformBuffer" },
	{ k_bufferData, "bufferData" },
	{ k_arrayBufferData, "arrayBufferData" },
	{ k_elementArrayBufferData, "elementArrayBufferData" },
	{ k_copyReadBufferData, "copyReadBufferData" },
	{ k_copyWriteBufferData, "copyWriteBufferData" },
	{ k_drawIndirectBufferData, "drawIndirectBufferData" },
	{ k_pixelPackBufferData, "pixelPackBufferData" },
	{ k_transformFeedbackBufferData, "transformFeedbackBufferData" },
	{ k_uniformBufferData, "uniformBufferData" },
	{ k_deleteBuffers, "deleteBuffers" },
	{ k_createBuffers, "createBuffers" },
	{ k_isBuffer, "isBuffer" },
	{ k_arrayBufferBound, "arrayBufferBound" },
	{ k_currentArrayBuffer, "currentArrayBuffer" },
	{ k_elementArrayBufferBound, "elementArrayBufferBound" },
	{ k_elementArrayBuffer, "elementArrayBuffer" },
	{ k_copyReadBufferBound, "copyReadBufferBound" },
	{ k_copyReadBuffer, "copyReadBuffer" },
	{ k_copyWriteBufferBound, "copyWriteBufferBound" },
	{ k_copyWriteBuffer, "copyWriteBuffer" },
	{ k_drawIndirectBufferBound, "drawIndirectBufferBound" },
	{ k_drawIndirectBuffer, "drawIndirectBuffer" },
	{ k_pixelPackBufferBound, "pixelPackBufferBound" },
	{ k_pixelPackBuffer, "pixelPackBuffer" },
	{ k_pixelUnpackBufferBound, "pixelUnpackBufferBound" },
	{ k_pixelUnpackBuffer, "pixelUnpackBuffer" },
	{ k_transformFeedbackBufferBound, "transformFeedbackBufferBound" },
	{ k_transformFeedbackBuffer, "transformFeedbackBuffer" },
	{ k_uniformBufferBound, "uniformBufferBound" },
	{ k_uniformBuffer, "uniformBuffer" },
	{ k_genRenderbuffers, "genRenderbuffers" },
	{ k_genFramebuffers, "genFramebuffers" },
	{ k_bindRenderbuffer, "bindRenderbuffer" },
	{ k_bindFramebuffer, "bindFramebuffer" },
	{ k_renderbufferStorage, "renderbufferStorage" },
	{ k_framebufferRenderbuffer, "framebufferRenderbuffer" },
	{ k_framebufferTexture2D, "framebufferTexture2D" },
	{ k_deleteRenderbuffers, "deleteRenderbuffers" },
	{ k_deleteFramebuffers, "deleteFramebuffers" },
	{ k_drawBuffers, "drawBuffers" },
	{ k_checkFramebufferStatus, "checkFramebufferStatus" },
	{ k_renderbufferBound, "renderbufferBound" },
	{ k_currentRenderbuffer, "currentRenderbuffer" },
	{ k_framebufferBound, "framebufferBound" },
	{ k_currentFramebuffer, "currentFramebuffer" },
	{ k_genTextures, "genTextures" },
	{ k_bindTexture, "bindTexture" },
	{ k_deleteTextures, "deleteTextures" },
	{ k_texture1DBound, "texture1DBound" },
	{ k_currentTexture1D, "currentTexture1D" },
	{ k_texture2DBound, "texture2DBound" },
	{ k_currentTexture2D, "currentTexture2D" },
	{ k_texture3DBound, "texture3DBound" },
	{ k_currentTexture3D, "currentTexture3D" },
	{ k_getTexImage, "getTexImage" },
	{ k_activeTexture, "activeTexture" },
	{ k_texParameteri, "texParameteri" },
	{ k_texImage2D, "texImage2D" },
	{ k_generateMipmap, "generateMipmap" },
	{ k_patchParameteri, "patchParameteri" },
	{ k_drawElements, "drawElements" },
	{ k_drawArrays, "drawArrays" },
	{ k_readPixelsSupported, "readPixelsSupported" },
	{ k_readPixels, "readPixels" },
	{ k_createShader, "createShader" },
	{ k_shaderSource, "shaderSource" },
	{ k_compileShader, "compileShader" },
	{ k_createProgram, "createProgram" },
	{ k_getAttachedShaders, "getAttachedShaders" },
	{ k_attachShader, "attachShader" },
	{ k_linkProgram, "linkProgram" },
	{ k_useProgram, "useProgram" },
	{ k_deleteShader, "deleteShader" },
	{ k_deleteProgram, "deleteProgram" },
	{ k_programBound, "programBound" },
	{ k_currentProgram, "currentProgram" },
	{ k_bindFragDataLocation, "bindFragDataLocation" },
	{ k_getAttribLocation, "getAttribLocation" },
	{ k_getUniformLocation, "getUniformLocation" },
	{ k_enableVertexAttribArray, "enableVertexAttribArray" },
	{ k_disableVertexAttribArray, "disableVertexAttribArray" },
	{ k_vertexAttribPointer, "vertexAttribPointer" },
	{ k_uniform1f, "uniform1f" },
	{ k_uniform2f, "uniform2f" },
	{ k_uniform3f, "uniform3f" },
	{ k_uniform4f, "uniform4f" },
	{ k_uniform1i, "uniform1i" },
	{ k_uniform2i, "uniform2i" },
	{ k_uniform3i, "uniform3i" },
	{ k_uniform4i, "uniform4i" },
	{ k_uniform1ui, "uniform1ui" },
	{ k_uniform2ui, "uniform2ui" },
	{ k_uniform3ui, "uniform3ui" },
	{ k_uniform4ui, "uniform4ui" },
	{ k_uniform1fv, "uniform1fv" },
	{ k_uniform2fv, "uniform2fv" },
	{ k_uniform3fv, "uniform3fv" },
	{ k_uniform4fv, "uniform4fv" },
	{ k_uniform1iv, "uniform1iv" },
	{ k_uniform2iv, "uniform2iv" },
	{ k_uniform3iv, "uniform3iv" },
	{ k_uniform4iv, "uniform4iv" },
	{ k_uniformMatrix2fv, "uniformMatrix2fv" },
	{ k_uniformMatrix3fv, "uniformMatrix3fv" },
	{ k_uniformMatrix4fv, "uniformMatrix4fv" },
	{ k_getString, "getString" },
	{ k_getBooleanv, "getBooleanv" },
	{ k_getDoublev, "getDoublev" },
	{ k_getFloatv, "getFloatv" },
	{ k_getIntegerv, "getIntegerv" },
	{ k_getStringi, "getStringi" },
	{ k_getBooleani_v, "getBooleani_v" },
	{ k_getDoublei_v, "getDoublei_v" },
	{ k_getFloati_v, "getFloati_v" },
	{ k_getIntegeri_v, "getIntegeri_v" },
	{ k_getBoolean, "getBoolean" },
	{ k_getDouble, "getDouble" },
	{ k_getFloat, "getFloat" },
	{ k_getInteger, "getInteger" },
	{ k_getShaderiv, "getShaderiv" },
	{ k_getShaderi, "getShaderi" },
	{ k_getShaderInfoLog, "getShaderInfoLog" },
	{ k_getProgramiv, "getProgramiv" },
	{ k_getProgrami, "getProgrami" },
	{ k_getProgramInfoLog, "getProgramInfoLog" },
	{ k_InvalidateFramebuffer, "invalidateFramebuffer" },
	{ k_invalidateTexImage, "invalidateTexImage" }
};
	
GLStateObject::GLStateObject(bg::base::Context * ctx) {
	_glState = new bg::engine::openglCore::OpenGLState(ctx);
}
	
OpenGLState::OpenGLState(bg::base::Context * ctx)
	:ContextObject(ctx)
{
}

OpenGLState::~OpenGLState() {
	
}

bool OpenGLState::checkError(FunctionId fid, bool logIfError) {
	using namespace bg::engine::opengl;

	bool status = (_lastError = static_cast<uint32_t>(glGetError())) == GL_NO_ERROR;

#ifdef BG2E_DEBUG
	if (!status && logIfError) {
		std::string error = "";
		std::string function = s_functionNames[fid];
		switch (_lastError) {
		case kNoError:
			break;
		case kInvalidEnum:
			error ="Invalid enum";
			break;
		case kInvalidValue:
			error = "Invalid value";
			break;
		case kInvalidOperation:
			error = "invalid operation";
			break;
		case kInvalidFramebufferOperation:
			error = "Invalid framebuffer operation";
			break;
		case kOutOfMemory:
			error = "Out of memory";
			break;
		case kStackUnderflow:
			error = "Stack underflow";
			break;
		case kStackOverflow:
			error = "Stack overflow";
			break;
		}
		bg::log(bg::log::kWarning) << "Warning: OpenGL error found calling  " 
			<< function << ": " << error << bg::endl;
	}
#endif
	return status;
}

void OpenGLState::sync() {
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);
	_viewport.set(vp[0], vp[1], vp[2], vp[3]);

	GLfloat c[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, c);
	_clearColor.set(c[0], c[1], c[2], c[3]);


	glGetBooleanv(GL_CULL_FACE, reinterpret_cast<GLboolean*>(&_cullFace));
	glGetBooleanv(GL_BLEND, reinterpret_cast<GLboolean*>(&_blend));
	glGetBooleanv(GL_DEPTH_TEST, reinterpret_cast<GLboolean*>(&_depthTest));

	glGetIntegerv(GL_BLEND_SRC_RGB, reinterpret_cast<GLint*>(&_blendFuncSource));
	glGetIntegerv(GL_BLEND_DST_RGB, reinterpret_cast<GLint*>(&_blendFuncDest));
	glGetIntegerv(GL_BLEND_EQUATION, reinterpret_cast<GLint*>(&_blendEquation));
	
	syncArrayBuffers();
	syncBuffers();
	syncRenderbuffers();

	syncTexture();

	syncProgram();
}

void OpenGLState::getExtensions(std::vector<std::string> & result) {
	using namespace bg::engine;
#if BG2E_OPENGL_AVAILABLE==1
	int numExtensions = getInteger(opengl::kNumExtensions);
	for (auto i=0; i<numExtensions; ++i) {
		result.push_back(getStringi(opengl::kExtensions, i));
	}
#elif BG2E_OPENGL_ES_AVAILABLE==1
	const GLubyte * extensions = glGetString(GL_EXTENSIONS);
	std::stringstream iss;
	std::string s;
	iss << extensions;
	while (std::getline(iss, s, ' ')) {
		result.push_back(s);
	}
#endif
}

int OpenGLState::getMaxDrawBuffers() {
	int result;
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &result);
	return result;
}

void OpenGLState::viewport(const bg::math::Viewport & vp) {
	if (_viewport!=vp) {
		glViewport(vp.x(), vp.y(), vp.width(), vp.height());
		if (checkError(k_viewport)) {
			_viewport = vp;
		}
	}
}

void OpenGLState::clearColor(const bg::math::Color & c) {
	if (c!=_clearColor) {
		glClearColor(c.r(), c.g(), c.b(), c.a());
		_clearColor = c;
	}
}

void OpenGLState::clear(uint32_t buffers) {
	glClear(buffers);
}

void OpenGLState::blendFunction(uint32_t src, uint32_t dst) {
	if (src!=_blendFuncSource && dst!=_blendFuncDest) {
		glBlendFuncSeparate(src, dst, src, dst);
		if(checkError(k_blendFunction)) {
			_blendFuncSource = src;
			_blendFuncDest = dst;
		}
	}
}

void OpenGLState::blendEquation(uint32_t eq) {
	if (eq!=_blendEquation) {
		glBlendEquation(eq);
		if(checkError(k_blendEquation)) {
			_blendEquation = eq;
		}
	}
}

void OpenGLState::enableDepthTest() {
	if (!_depthTest) {
		glEnable(GL_DEPTH_TEST);
		_depthTest = true;
	}
}

void OpenGLState::disableDepthTest() {
	if (_depthTest) {
		glDisable(GL_DEPTH_TEST);
		_depthTest = false;
	}
}

void OpenGLState::enableBlend() {
	if (!_blend) {
		glEnable(GL_BLEND);
		_blend = true;
	}
}

void OpenGLState::disableBlend() {
	if (_blend) {
		glDisable(GL_BLEND);
		_blend = false;
	}
}

void OpenGLState::enableCullFace() {
	if (!_cullFace) {
		glEnable(GL_CULL_FACE);
		_cullFace = true;
	}
}

void OpenGLState::disableCullFace() {
	if (_cullFace) {
		glDisable(GL_CULL_FACE);
		_cullFace = false;
	}
}

void OpenGLState::genVertexArrays(size_t numArrays, uint32_t * arrays) {
	glGenVertexArrays(static_cast<GLsizei>(numArrays), arrays);
	checkError(k_genVertexArrays);
}

void OpenGLState::bindVertexArray(uint32_t a) {
	if (_vertexArray!=a) {
		glBindVertexArray(a);
		if(checkError(k_bindVertexArray)) {
			_vertexArrayBound = a!=0;
			_vertexArray = a;
		}
	}
}

void OpenGLState::deleteVertexArray(size_t numArrays, uint32_t * arrays) {
	glDeleteVertexArrays(static_cast<GLsizei>(numArrays),arrays);
	syncArrayBuffers();
	checkError(k_deleteVertexArray);
}

void OpenGLState::genBuffers(size_t numBuffers, uint32_t * buffers) {
	glGenBuffers(static_cast<GLsizei>(numBuffers), buffers);
	checkError(k_genBuffers);
}

void OpenGLState::bindBuffer(int32_t target, uint32_t buffer) {
	using namespace bg::engine::opengl;
	switch (target) {
	case kArrayBuffer:
		bindArrayBuffer(buffer);
		break;
	case kElementArrayBuffer:
		bindElementArrayBuffer(buffer);
		break;
	case kCopyReadBuffer:
		bindCopyReadBuffer(buffer);
		break;
	case kCopyWriteBuffer:
		bindCopyWriteBuffer(buffer);
		break;
	case kDrawIndirectBuffer:
		bindDrawIndirectBuffer(buffer);
		break;
	case kPixelPackBuffer:
		bindPixelPackBuffer(buffer);
		break;
	case kPixelUnpackBuffer:
		bindPixelUnpackBuffer(buffer);
		break;
	case kTransformFeedbackBuffer:
		bindTransformFeedbackBuffer(buffer);
		break;
	case kUniformBuffer:
		bindUniformBuffer(buffer);
		break;
	}
}

void OpenGLState::bindArrayBuffer(uint32_t buffer) {
	if (_arrayBuffer!=buffer) {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		if (checkError(k_bindArrayBuffer)) {
			_arrayBufferBound = buffer!=0;
			_arrayBuffer = buffer;
		}
	}
}

void OpenGLState::bindElementArrayBuffer(uint32_t buffer) {
	if (_elementArrayBuffer!=buffer) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		if (checkError(k_bindElementArrayBuffer)) {
			_elementArrayBufferBound = buffer != 0;
			_elementArrayBuffer = buffer;
		}
	}
}

void OpenGLState::bindCopyReadBuffer(uint32_t buffer) {
	if (_copyReadBuffer!=buffer) {
		glBindBuffer(GL_COPY_READ_BUFFER, buffer);
		if (checkError(k_bindCopyReadBuffer)) {
			_copyReadBufferBound = buffer != 0;
			_copyReadBuffer = buffer;
		}
	}
}

void OpenGLState::bindCopyWriteBuffer(uint32_t buffer) {
	if (_copyWriteBuffer!=buffer) {
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		if (checkError(k_bindCopyWriteBuffer)) {
			_copyWriteBufferBound = buffer != 0;
			_copyWriteBuffer = buffer;
		}
	}
}

void OpenGLState::bindDrawIndirectBuffer(uint32_t buffer) {
#if BG2E_OPENGL_AVAILABLE==1
	if (_drawIndirectBuffer!=buffer) {
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer);
		if (checkError(k_bindDrawIndirectBuffer)) {
			_drawIndirectBufferBound = buffer != 0;
			_drawIndirectBuffer = buffer;
		}
	}
#endif
}

void OpenGLState::bindPixelPackBuffer(uint32_t buffer) {
	if (_pixelPackBuffer!=buffer) {
		glBindBuffer(GL_PIXEL_PACK_BUFFER, buffer);
		if (checkError(k_bindPixelPackBuffer)) {
			_pixelPackBufferBound = buffer != 0;
			_pixelPackBuffer = buffer;
		}
	}
}

void OpenGLState::bindPixelUnpackBuffer(uint32_t buffer) {
	if (_pixelUnpackBuffer!=buffer) {
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer);
		if(checkError(k_bindPixelUnpackBuffer)) {
			_pixelUnpackBufferBound = buffer != 0;
			_pixelUnpackBuffer = buffer;
		}
	}
}

void OpenGLState::bindTransformFeedbackBuffer(uint32_t buffer) {
	if (_transformFeedbackBuffer!=buffer) {
		glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffer);
		if(checkError(k_bindTransformFeedbackBuffer)) {
			_transformFeedbackBufferBound = buffer != 0;
			_transformFeedbackBuffer = buffer;
		}
	}
}

void OpenGLState::bindUniformBuffer(uint32_t buffer) {
	if (_uniformBuffer!=buffer) {
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		if(checkError(k_bindUniformBuffer)) {
			_uniformBufferBound = buffer != 0;
			_uniformBuffer = buffer;
		}
	}
}

void OpenGLState::bufferData(uint32_t target, size_t size, const void * data, uint32_t usage) {
	glBufferData(target, size, data, usage);
	checkError(k_bufferData);
}

void OpenGLState::arrayBufferData(size_t size, const void * data, uint32_t usage) {
	using namespace bg::engine::opengl;
	glBufferData(kArrayBuffer, size, data, usage);
	checkError(k_arrayBufferData);
}

void OpenGLState::elementArrayBufferData(size_t size, const void * data, uint32_t usage) {
	using namespace bg::engine::opengl;
	glBufferData(kElementArrayBuffer, size, data, usage);
	checkError(k_elementArrayBufferData);
}

void OpenGLState::copyReadBufferData(size_t size, const void * data, uint32_t usage) {
	using namespace bg::engine::opengl;
	glBufferData(kCopyReadBuffer, size, data, usage);
	checkError(k_copyReadBufferData);
}

void OpenGLState::copyWriteBufferData(size_t size, const void * data, uint32_t usage) {
	using namespace bg::engine::opengl;
	glBufferData(kCopyWriteBuffer, size, data, usage);
	checkError(k_copyWriteBufferData);
}

void OpenGLState::drawIndirectBufferData(size_t size, const void * data, uint32_t usage) {
	using namespace bg::engine::opengl;
	glBufferData(kDrawIndirectBuffer, size, data, usage);
	checkError(k_drawIndirectBufferData);
}

void OpenGLState::pixelPackBufferData(size_t size, const void * data, uint32_t usage) {
	using namespace bg::engine::opengl;
	glBufferData(kPixelPackBuffer, size, data, usage);
	checkError(k_pixelPackBufferData);
}

void OpenGLState::transformFeedbackBufferData(size_t size, const void * data, uint32_t usage) {
	using namespace bg::engine::opengl;
	glBufferData(kTransformFeedbackBuffer, size, data, usage);
	checkError(k_transformFeedbackBufferData);
}

void OpenGLState::uniformBufferData(size_t size, const void * data, uint32_t usage) {
	using namespace bg::engine::opengl;
	glBufferData(kUniformBuffer, size, data, usage);
	checkError(k_uniformBufferData);
}

void OpenGLState::createBuffers(size_t n, uint32_t * buffers) {
#ifdef glCreateBuffers
	glCreateBuffers(static_cast<GLsizei>(n), buffers);
	checkError(k_createBuffers);
#else
	throw bg::base::CompatibilityException("glCreateBuffers not available in this platform.");
#endif
}

void OpenGLState::deleteBuffers(size_t numBuffers, uint32_t * buffers) {
	glDeleteBuffers(static_cast<GLsizei>(numBuffers), buffers);
	syncBuffers();
	checkError(k_deleteBuffers);
}


void OpenGLState::genRenderbuffers(size_t n, uint32_t * renderbuffers) {
	glGenRenderbuffers(static_cast<GLsizei>(n), renderbuffers);
	checkError(k_genRenderbuffers);
}

void OpenGLState::genFramebuffers(size_t n, uint32_t * framebuffers) {
	glGenFramebuffers(static_cast<GLsizei>(n), framebuffers);
	checkError(k_genFramebuffers);
}

void OpenGLState::bindRenderbuffer(uint32_t target, uint32_t renderbuffer) {
	if (_renderbuffer!=renderbuffer) {
		glBindRenderbuffer(target, renderbuffer);
		if (checkError(k_bindRenderbuffer)) {
			_renderbufferBound = renderbuffer!=0;
			_renderbuffer = renderbuffer;
		}
	}
}

void OpenGLState::bindFramebuffer(uint32_t target, uint32_t framebuffer) {
	if (_framebuffer!=framebuffer) {
		glBindFramebuffer(target, framebuffer);
		if (checkError(k_bindFramebuffer)) {
			_framebufferBound = framebuffer!=0;
			_framebuffer = framebuffer;
		}
	}
}

void OpenGLState::renderbufferStorage(uint32_t target, uint32_t internalFormat, size_t width, size_t height) {
	glRenderbufferStorage(target, internalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	checkError(k_renderbufferStorage);
}

void OpenGLState::framebufferRenderbuffer(uint32_t target, uint32_t attachment, uint32_t renderbufferTarget, uint32_t renderbuffer) {
	glFramebufferRenderbuffer(target, attachment, renderbufferTarget, renderbuffer);
	checkError(k_framebufferRenderbuffer);
}

void OpenGLState::framebufferTexture2D(uint32_t target, uint32_t attachment, uint32_t texTarget, uint32_t texture, int level) {
	glFramebufferTexture2D(target, attachment, texTarget, texture, level);
	checkError(k_framebufferTexture2D);
}

void OpenGLState::deleteRenderbuffers(size_t n, const uint32_t * renderbuffers) {
	glDeleteRenderbuffers(static_cast<GLsizei>(n),renderbuffers);
	syncBuffers();
	checkError(k_deleteRenderbuffers);
}

void OpenGLState::deleteFramebuffers(size_t n, uint32_t * framebuffers) {
	glDeleteRenderbuffers(static_cast<GLsizei>(n), framebuffers);
	if (checkError(k_deleteFramebuffers)) {
		for(auto i = 0; i<n; ++i) {
			if(_framebuffer == framebuffers[n]) {
				_framebuffer = 0;
				_framebufferBound = false;
				break;
			}
		}
	}
}

void OpenGLState::drawBuffers(size_t n, const uint32_t * bufs) {
	glDrawBuffers(static_cast<GLsizei>(n), bufs);
	checkError(k_drawBuffers);
}

uint32_t OpenGLState::checkFramebufferStatus(uint32_t target) {
	return glCheckFramebufferStatus(bg::engine::opengl::kFramebuffer);
}

void OpenGLState::genTextures(size_t n, uint32_t * textures) {
	glGenTextures(static_cast<GLsizei>(n), textures);
	checkError(k_genTextures);
}

void OpenGLState::bindTexture(uint32_t target, uint32_t texture) {
	using namespace bg::engine::opengl;
	switch (target) {
	case kTexture1D:
		glBindTexture(target, texture);
		if (checkError(k_bindTexture)) {
			_texture1DBound = texture!=0;
			_texture1D = texture;
		}
		break;
	case kTexture2D:
		glBindTexture(target, texture);
		if(checkError(k_bindTexture)) {
			_texture2DBound = texture != 0;
			_texture2D = texture;
		}
		break;
	case kTexture3D:
		glBindTexture(target, texture);
		if(checkError(k_bindTexture)) {
			_texture3DBound = texture != 0;
			_texture3D = texture;
		}
		break;
	}
}

void OpenGLState::deleteTextures(uint32_t target, uint32_t * textures) {
	glDeleteTextures(target, textures);
	checkError(k_deleteTextures);
}

void OpenGLState::getTexImage(uint32_t target, int32_t level, uint32_t format, uint32_t type, void * buffer) {
#if BG2E_OPENGL_AVAILABLE==1
	glGetTexImage(target, level, format, type, buffer);
	checkError(k_getTexImage);
#else
	buffer = nullptr;
#endif
}

void OpenGLState::activeTexture(uint32_t textureUnit) {
	glActiveTexture(textureUnit);
	checkError(k_activeTexture);
}

void OpenGLState::texParameteri(uint32_t target, uint32_t pname, int32_t param) {
	glTexParameteri(target, pname, param);
	checkError(k_texParameteri);
}

void OpenGLState::texImage2D(uint32_t target, int level, uint32_t inFormat, size_t width, size_t height, int border, uint32_t format, uint32_t type, const void * data) {
	glTexImage2D(target, level, inFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), border, format, type, data);
	checkError(k_texImage2D);
}

void OpenGLState::generateMipmap(uint32_t target) {
	glGenerateMipmap(target);
	checkError(k_generateMipmap);
}

void OpenGLState::patchParameteri(uint32_t pname, int32_t value) {
#if BG2E_OPENGL_AVAILABLE==1
	glPatchParameteri(pname, value);
	checkError(k_patchParameteri);
#endif
}

void OpenGLState::drawElements(uint32_t mode, size_t count, uint32_t type, const void * indices) {
	glDrawElements(mode, static_cast<GLsizei>(count), type, indices);
	checkError(k_drawElements);
}

void OpenGLState::drawArrays(uint32_t mode, int32_t first, size_t count) {
	glDrawArrays(mode, first, static_cast<GLsizei>(count));
	checkError(k_drawArrays);
}

void OpenGLState::readPixels(int x, int y, size_t w, size_t h, uint32_t format, uint32_t type, void * data) {
#if BG2E_OPENGL_AVAILABLE==1
	glReadPixels(x, y, static_cast<GLsizei>(w), static_cast<GLsizei>(h), format, type, data);
	checkError(k_readPixels);
#endif
}

uint32_t OpenGLState::createShader(uint32_t shaderType) {
	uint32_t res = glCreateShader(shaderType);
	checkError(k_createShader);
	return res;
}

void OpenGLState::shaderSource(uint32_t shader, const std::string & source) {
	const char * src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	checkError(k_shaderSource);
}

void OpenGLState::shaderSource(uint32_t shader, const std::vector<std::string> & sources) {
	for (auto src : sources) {
		shaderSource(shader, src);
	}
}

void OpenGLState::compileShader(uint32_t shader) {
	glCompileShader(shader);
	checkError(k_compileShader);
}

uint32_t OpenGLState::createProgram() {
	return glCreateProgram();
}

void OpenGLState::getAttachedShaders(uint32_t program, size_t maxCount, size_t * count, uint32_t * shaders) {
	GLsizei glCount = 0;
	glGetAttachedShaders(program, static_cast<GLsizei>(maxCount), &glCount, shaders);
	*count = static_cast<size_t>(glCount);
	checkError(k_getAttachedShaders);
}

void OpenGLState::attachShader(uint32_t program, uint32_t shader) {
	glAttachShader(program, shader);
	checkError(k_attachShader);
}

void OpenGLState::linkProgram(uint32_t program) {
	glLinkProgram(program);
	checkError(k_linkProgram);
}

void OpenGLState::useProgram(uint32_t program) {
	if (_program!=program) {
		glUseProgram(program);
		if (checkError(k_useProgram)) {
			_program = program;
		}
	}
}

void OpenGLState::deleteShader(uint32_t shader) {
	glDeleteShader(shader);
	checkError(k_deleteShader);
}

void OpenGLState::deleteProgram(uint32_t program) {
	glDeleteProgram(program);
	checkError(k_deleteProgram);
	glGetIntegerv(opengl::kCurrentProgram, reinterpret_cast<GLint*>(&_program));
}

void OpenGLState::bindFragDataLocation(uint32_t program, uint32_t colorNumber, const std::string & name) {
#if BG2E_OPENGL_AVAILABLE==1
	const char * n = name.c_str();

	glBindFragDataLocation(program, colorNumber, n);
	checkError(k_bindFragDataLocation);
#endif
}

int32_t OpenGLState::getAttribLocation(uint32_t program, const std::string & name) {
	const char * n = name.c_str();
	uint32_t res = glGetAttribLocation(program,n);
	checkError(k_getAttribLocation);
	return res;
}

int32_t OpenGLState::getUniformLocation(uint32_t program, const std::string & name) {
	const char * n = name.c_str();
	uint32_t res = glGetUniformLocation(program, n);
	checkError(k_getUniformLocation);
	return res;
}

void OpenGLState::enableVertexAttribArray(uint32_t index) {
	glEnableVertexAttribArray(index);
	checkError(k_enableVertexAttribArray);
}

void OpenGLState::disableVertexAttribArray(uint32_t index) {
	glDisableVertexAttribArray(index);
	checkError(k_disableVertexAttribArray, false);
}

void OpenGLState::vertexAttribPointer(uint32_t index, int32_t size, uint32_t type, bool normalized, size_t stride, const void * pointer) {
	glVertexAttribPointer(index, size, type, normalized ? GL_TRUE : GL_FALSE, static_cast<GLsizei>(stride), pointer);
	checkError(k_vertexAttribPointer);
}

void OpenGLState::uniform1f(int32_t location, float v0) {
	glUniform1f(location, v0);
	checkError(k_uniform1f);
}

void OpenGLState::uniform2f(int32_t location, float v0, float v1) {
	glUniform2f(location, v0, v1);
	checkError(k_uniform2f);
}

void OpenGLState::uniform3f(int32_t location, float v0, float v1, float v2) {
	glUniform3f(location, v0, v1, v2);
	checkError(k_uniform3f);
}

void OpenGLState::uniform4f(int32_t location, float v0, float v1, float v2, float v3) {
	glUniform4f(location, v0, v1, v2, v3);
	checkError(k_uniform4f);
}

void OpenGLState::uniform1i(int32_t location, int32_t v0) {
	glUniform1i(location, v0);
	checkError(k_uniform1i);
}

void OpenGLState::uniform2i(int32_t location, int32_t v0, int32_t v1) {
	glUniform2i(location, v0, v1);
	checkError(k_uniform2i);
}

void OpenGLState::uniform3i(int32_t location, int32_t v0, int32_t v1, int32_t v2) {
	glUniform3i(location, v0, v1, v2);
	checkError(k_uniform3i);
}

void OpenGLState::uniform4i(int32_t location, int32_t v0, int32_t v1, int32_t v2, int32_t v3) {
	glUniform4i(location, v0, v1, v2, v3);
	checkError(k_uniform4i);
}

void OpenGLState::uniform1ui(int32_t location, uint32_t v0) {
	glUniform1ui(location, v0);
	checkError(k_uniform1ui);
}

void OpenGLState::uniform2ui(int32_t location, uint32_t v0, uint32_t v1) {
	glUniform2ui(location, v0, v1);
	checkError(k_uniform2ui);
}

void OpenGLState::uniform3ui(int32_t location, uint32_t v0, uint32_t v1, uint32_t v2) {
	glUniform3ui(location, v0, v1, v2);
	checkError(k_uniform3ui);
}

void OpenGLState::uniform4ui(int32_t location, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) {
	glUniform4ui(location, v0, v1, v2, v3);
	checkError(k_uniform4ui);
}

void OpenGLState::uniform1fv(int32_t location, size_t count, const float * value) {
	glUniform1fv(location, static_cast<GLsizei>(count), value);
	checkError(k_uniform1fv);
}

void OpenGLState::uniform2fv(int32_t location, size_t count, const float * value) {
	glUniform2fv(location, static_cast<GLsizei>(count), value);
	checkError(k_uniform2fv);
}

void OpenGLState::uniform3fv(int32_t location, size_t count, const float * value) {
	glUniform3fv(location, static_cast<GLsizei>(count), value);
	checkError(k_uniform3fv);
}

void OpenGLState::uniform4fv(int32_t location, size_t count, const float * value) {
	glUniform4fv(location, static_cast<GLsizei>(count), value);
	checkError(k_uniform4fv);
}

void OpenGLState::uniform1iv(int32_t location, size_t count, const int32_t * value) {
	glUniform1iv(location, static_cast<GLsizei>(count), value);
	checkError(k_uniform1iv);
}

void OpenGLState::uniform2iv(int32_t location, size_t count, const int32_t * value) {
	glUniform2iv(location, static_cast<GLsizei>(count), value);
	checkError(k_uniform2iv);
}

void OpenGLState::uniform3iv(int32_t location, size_t count, const int32_t * value) {
	glUniform3iv(location, static_cast<GLsizei>(count), value);
	checkError(k_uniform3iv);
}

void OpenGLState::uniform4iv(int32_t location, size_t count, const int32_t * value) {
	glUniform4iv(location, static_cast<GLsizei>(count), value);
	checkError(k_uniform4iv);
}

void OpenGLState::uniformMatrix2fv(int32_t location, size_t count, bool transpose, const float * value) {
	glUniformMatrix2fv(location, static_cast<GLsizei>(count), transpose ? GL_TRUE : GL_FALSE, value);
	checkError(k_uniformMatrix2fv);
}

void OpenGLState::uniformMatrix3fv(int32_t location, size_t count, bool transpose, const float * value) {
	glUniformMatrix3fv(location, static_cast<GLsizei>(count), transpose ? GL_TRUE : GL_FALSE, value);
	checkError(k_uniformMatrix3fv);
}

void OpenGLState::uniformMatrix4fv(int32_t location, size_t count, bool transpose, const float * value) {
	glUniformMatrix4fv(location, static_cast<GLsizei>(count), transpose ? GL_TRUE : GL_FALSE, value);
	checkError(k_uniformMatrix4fv);
}

//glGet

std::string OpenGLState::getString(uint32_t req) {
	auto res = std::string(reinterpret_cast<const char*>(glGetString(req)));
	checkError(k_getString);
	return res;
}

void OpenGLState::getBooleanv(uint32_t req,bool * res) {
	glGetBooleanv(req,reinterpret_cast<GLboolean*>(res));
	checkError(k_getBoolean);
}

void OpenGLState::getDoublev(uint32_t req,double * res) {
#if BG2E_OPENGL_AVAILABLE==1
	glGetDoublev(req, reinterpret_cast<GLdouble*>(res));
#elif BG2E_OPENGL_ES_AVAILABLE==1
	GLfloat v;
	glGetFloatv(req, &v);
	*res = static_cast<double>(v);
	std::cerr << "WARNING: getDoublev is not available in OpenGL ES. The query will be resolved using getFloatv." << std::endl;
#endif
	checkError(k_getDoublev);
}

void OpenGLState::getFloatv(uint32_t req,float * res) {
	glGetFloatv(req,reinterpret_cast<GLfloat*>(res));
	checkError(k_getFloatv);
}

void OpenGLState::getIntegerv(uint32_t req,int32_t * res) {
	glGetIntegerv(req,reinterpret_cast<GLint*>(res));
	checkError(k_getIntegerv);
}

std::string OpenGLState::getStringi(uint32_t req,int index) {
	auto res = std::string(reinterpret_cast<const char *>(glGetStringi(req, index)));
	checkError(k_getStringi);
	return res;
}

void OpenGLState::getBooleani_v(uint32_t req,int index,bool * res) {
#if BG2E_OPENGL_AVAILABLE==1
	glGetBooleani_v(req,index,reinterpret_cast<GLboolean*>(res));
#elif BG2E_OPENGL_ES_AVAILABLE==1
	getBooleanv(req,res);
	std::cerr << "WARNING: getBooleani_v is not available in OpenGL ES. The query will be resolved using getBooleanv with index 0." << std::endl;
#endif
	checkError(k_getBooleani_v);
}

void OpenGLState::getDoublei_v(uint32_t req,int index,double * res) {
#if BG2E_OPENGL_AVAILABLE==1
	glGetDoublei_v(req, index, reinterpret_cast<GLdouble*>(res));
#elif BG2E_OPENGL_ES_AVAILABLE==1
	getDoublev(req, res);
	std::cerr << "WARNING: getDoublei_v is not available in OpenGL ES. The query will be resolved using getFloatv with index 0." << std::endl;
#endif
	checkError(k_getDoublei_v);
}

void OpenGLState::getFloati_v(uint32_t req,int index,float * res) {
#if BG2E_OPENGL_AVAILABLE==1
	glGetFloati_v(req,index,reinterpret_cast<GLfloat*>(res));
#elif BG2E_OPENGL_ES_AVAILABLE==1
	getFloatv(req,res);
	std::cerr << "WARNING: getFloati_v is not available in OpenGL ES. The query will be resolved using getFloatv with index 0." << std::endl;
#endif
	checkError(k_getFloati_v);
}

void OpenGLState::getIntegeri_v(uint32_t req,int index,int32_t * res) {
	glGetIntegeri_v(req, index, reinterpret_cast<GLint*>(res));
	checkError(k_getIntegeri_v);
}

bool OpenGLState::getBoolean(uint32_t req) {
	bool res;
	getBooleanv(req,&res);
	return res;
}

double OpenGLState::getDouble(uint32_t req) {
	double res;
	getDoublev(req, &res);
	return res;
}

float OpenGLState::getFloat(uint32_t req) {
	float res;
	getFloatv(req,&res);
	return res;
}

int32_t OpenGLState::getInteger(uint32_t req) {
	int32_t res;
	getIntegerv(req,&res);
	return res;
}

void OpenGLState::getShaderiv(uint32_t shader, uint32_t pname, int32_t * res) {
	glGetShaderiv(shader, pname, reinterpret_cast<int32_t*>(res));
	checkError(k_getShaderiv);
}

int32_t OpenGLState::getShaderi(uint32_t shader, uint32_t pname) {
	int32_t res;
	getShaderiv(shader,pname,&res);
	return res;
}

std::string OpenGLState::getShaderInfoLog(uint32_t shader) {
	using namespace bg::engine;
	std::string result;
	int32_t length = getShaderi(shader,opengl::kInfoLogLength);
	checkError(k_getShaderInfoLog);
	if (length>0) {
		GLsizei written;
		char * log = new char[length];
		glGetShaderInfoLog(shader, opengl::kInfoLogLength, &written, log);
		checkError(k_getShaderInfoLog);
		result = log;
		delete [] log;
	}
	return result;
}

void OpenGLState::getProgramiv(uint32_t program, uint32_t pname, int32_t * res) {
	glGetProgramiv(program, pname, reinterpret_cast<int32_t*>(res));
	checkError(k_getProgramiv);
}

int32_t OpenGLState::getProgrami(uint32_t program, uint32_t pname) {
	int32_t res;
	getProgramiv(program, pname, &res);
	return res;
}

std::string OpenGLState::getProgramInfoLog(uint32_t program) {
	using namespace bg::engine;
	std::string result;
	int32_t length = getProgrami(program,opengl::kInfoLogLength);
	checkError(k_getProgramInfoLog);
	if (length>0) {
		GLsizei written;
		char * log = new char[length];
		glGetProgramInfoLog(program, opengl::kInfoLogLength, &written, log);
		checkError(k_getProgramInfoLog);
		result = log;
		delete [] log;
	}
	return result;
}


void OpenGLState::syncArrayBuffers() {
    glGetIntegerv(bg::engine::opengl::kVertexArrayBinding, reinterpret_cast<GLint*>(&_vertexArray));
	_vertexArrayBound = _vertexArray != 0;
}

void OpenGLState::syncBuffers() {
    using namespace bg::engine::opengl;
	glGetIntegerv(kArrayBufferBinding, reinterpret_cast<GLint*>(&_arrayBuffer));
	_arrayBufferBound = (_arrayBufferBound != 0);
	glGetIntegerv(kElementArrayBufferBinding, reinterpret_cast<GLint*>(&_elementArrayBuffer));
	_elementArrayBufferBound = (_elementArrayBuffer != 0);
	glGetIntegerv(kCopyReadBufferBinding, reinterpret_cast<GLint*>(&_copyReadBuffer));
	_copyReadBufferBound = (_copyReadBuffer != 0);
	glGetIntegerv(kCopyWriteBufferBinding, reinterpret_cast<GLint*>(&_copyWriteBuffer));
	_copyWriteBufferBound = (_copyWriteBuffer != 0);
	glGetIntegerv(kDrawIndirectBufferBinding, reinterpret_cast<GLint*>(&_drawIndirectBuffer));
	_drawIndirectBufferBound = (_drawIndirectBuffer != 0);
	glGetIntegerv(kPixelPackBufferBinding, reinterpret_cast<GLint*>(&_pixelPackBuffer));
	_pixelPackBufferBound = (_pixelPackBuffer != 0);
	glGetIntegerv(kPixelUnpackBufferBinding, reinterpret_cast<GLint*>(&_pixelUnpackBuffer));
	_pixelUnpackBufferBound = (_pixelUnpackBuffer != 0);
	glGetIntegerv(kTransformFeedbackBufferBinding, reinterpret_cast<GLint*>(&_transformFeedbackBuffer));
	_transformFeedbackBufferBound = (_transformFeedbackBuffer != 0);
	glGetIntegerv(kUniformBufferBinding, reinterpret_cast<GLint*>(&_uniformBuffer));
	_uniformBufferBound = (_uniformBuffer != 0);
}

void OpenGLState::syncRenderbuffers() {
    glGetIntegerv(bg::engine::opengl::kRenderbufferBinding, reinterpret_cast<GLint*>(&_renderbuffer));
	_renderbufferBound = (_renderbuffer != 0);
}

void OpenGLState::syncTexture() {
    using namespace bg::engine::opengl;
	glGetIntegerv(kTextureBinding1D, reinterpret_cast<GLint*>(&_texture1D));
	glGetIntegerv(kTextureBinding2D, reinterpret_cast<GLint*>(&_texture2D));
	glGetIntegerv(kTextureBinding3D, reinterpret_cast<GLint*>(&_texture3D));
	_texture1DBound = _texture1D!=0;
	_texture2DBound = _texture2D!=0;
	_texture3DBound = _texture3D!=0;
}

void OpenGLState::syncProgram() {
	using namespace bg::engine::opengl;
	glGetIntegerv(kCurrentProgram, reinterpret_cast<GLint*>(&_program));
	_programBound = _program!=0;
}

void OpenGLState::invalidateFramebuffer(uint32_t target, size_t numAttachments, const int32_t * attachments) {
#if BG2E_MAC==0		// glInvalidateFramebuffer is not available in macOS
	glInvalidateFramebuffer(target, static_cast<GLsizei>(numAttachments), reinterpret_cast<const GLenum*>(attachments));
	checkError(k_InvalidateFramebuffer);
#endif
}

void OpenGLState::invalidateTexImage(uint32_t texture, int32_t level) {
#if BG2E_WINDOWS==1		// glInvalidateTexImage is not available in macOS
	glInvalidateTexImage(texture, level);
	checkError(k_InvalidateFramebuffer);
#endif
}

}
}
}
