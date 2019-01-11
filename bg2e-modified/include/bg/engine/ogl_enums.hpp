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

#include <bg/engine/ogl_includes.hpp>

namespace bg {
namespace engine {
namespace opengl {
	
enum OpenGLEnums {
	kTrue							= GL_TRUE,
	kFalse							= GL_FALSE,
	
	// Checking
	kMaxDrawBuffers					= GL_MAX_DRAW_BUFFERS,
	
	// Errors
	kNoError						= GL_NO_ERROR,
	kInvalidEnum					= GL_INVALID_ENUM,
	kInvalidValue					= GL_INVALID_VALUE,
	kInvalidOperation				= GL_INVALID_OPERATION,
	kInvalidFramebufferOperation	= GL_INVALID_FRAMEBUFFER_OPERATION,
	kOutOfMemory					= GL_OUT_OF_MEMORY,
#ifndef GL_STACK_OVERFLOW
	kStackUnderflow					= 0x0504,
#else
	kStackUnderflow					= GL_STACK_UNDERFLOW,
#endif
#ifdef GL_STACK_UNDERFLOW
	kStackOverflow					= GL_STACK_OVERFLOW,
#else
	kStackOverflow					= 0x0503,
#endif
	
	// Shaders
	kVertexShader					= GL_VERTEX_SHADER,
#ifndef GL_TESS_CONTROL_SHADER
	kTessControlShader 				= 0x00008e88,
#else
	kTessControlShader				= GL_TESS_CONTROL_SHADER,
#endif
#ifndef GL_TESS_EVALUATION_SHADER
	kTessEvaluationShader			= 0x00008e87,
#else
	kTessEvaluationShader			= GL_TESS_EVALUATION_SHADER,
#endif
#ifndef GL_GEOMETRY_SHADER
	kGeometryShader					= 0x8DD9,
#else
	kGeometryShader					= GL_GEOMETRY_SHADER,
#endif
	kFragmentShader					= GL_FRAGMENT_SHADER,
	
	kArrayBuffer				= GL_ARRAY_BUFFER,
	kElementArrayBuffer			= GL_ELEMENT_ARRAY_BUFFER,
	kCopyReadBuffer				= GL_COPY_READ_BUFFER,
	kCopyWriteBuffer			= GL_COPY_WRITE_BUFFER,
#ifndef GL_DRAW_INDIRECT_BUFFER
	kDrawIndirectBuffer				= 0x8F3F,
#else
	kDrawIndirectBuffer				= GL_DRAW_INDIRECT_BUFFER,
#endif
	kPixelPackBuffer			= GL_PIXEL_PACK_BUFFER,
	kPixelUnpackBuffer			= GL_PIXEL_UNPACK_BUFFER,
	kTransformFeedbackBuffer	= GL_TRANSFORM_FEEDBACK_BUFFER,
	kUniformBuffer				= GL_UNIFORM_BUFFER,
	
	
	// Draw mode
	kLines						= GL_LINES,
	kTriangles					= GL_TRIANGLES,
#ifndef GL_PATCHES
	kPatches					= 0x0000000e,
#else
	kPatches					= GL_PATCHES,
#endif
#ifndef GL_PATCH_VERTICES
	kPatchVertices				= 0x8E72,
#else
	kPatchVertices				= GL_PATCH_VERTICES,
#endif
	
	// Draw buffers
	kColorBufferBit				= GL_COLOR_BUFFER_BIT,
	kDepthBufferBit				= GL_DEPTH_BUFFER_BIT,
	
	//glBlendFunc enums
	kZero 					= GL_ZERO,
	kOne 					= GL_ONE,
	kSrcColor 				= GL_SRC_COLOR,
	kOneMinusSrcColor 		= GL_ONE_MINUS_SRC_COLOR,
	kDstColor 				= GL_DST_COLOR,
	kOneMinusDstColor 		= GL_ONE_MINUS_DST_COLOR,
	kSrcAlpha 				= GL_SRC_ALPHA,
	kOneMinusSrcAlpha		= GL_ONE_MINUS_SRC_ALPHA,
	kDstAlpha				= GL_DST_ALPHA,
	kOneMinusDstAlpha		= GL_ONE_MINUS_DST_ALPHA,
	kConstantColor			= GL_CONSTANT_COLOR,
	kOneMinusConstantColor	= GL_ONE_MINUS_CONSTANT_COLOR,
	kConstantAlpha			= GL_CONSTANT_ALPHA,
	kOneMinusConstantAlpha	= GL_ONE_MINUS_CONSTANT_ALPHA,
	
	kFuncAdd				= GL_FUNC_ADD,
	kFuncSubtract			= GL_FUNC_SUBTRACT,
	
	// glEnable / glDisable enums
	kDepthTest				= GL_DEPTH_TEST,
	kBlend					= GL_BLEND,
	kCullFace				= GL_CULL_FACE,
	
	
	
	// Types
#ifndef GL_RGBA32F
#ifdef GL_RGBA32F_ARB
	kRgba32f					= GL_RGBA32F_ARB,
#else
	kRgba32f					= GL_RGBA,
#endif
#else
	kRgba32f					= GL_RGBA32F,
#endif
#ifndef GL_RGBA16F
#ifdef GL_RGBA16F_ARB
	kRgba16f					= GL_RGBA16F_ARB,
#else
	kRgba16f					= GL_RGBA,
#endif
#else
	kRgba16f					= GL_RGBA16F,
#endif
	kRgba						= GL_RGBA,
	kRgb						= GL_RGB,
	kDepthComponent				= GL_DEPTH_COMPONENT,
	kDepthComponent16			= GL_DEPTH_COMPONENT16,
	kFloat						= GL_FLOAT,
	kInt						= GL_INT,
	kShort						= GL_SHORT,
	kByte						= GL_BYTE,
	kUnsignedInt				= GL_UNSIGNED_INT,
	kUnsignedShort				= GL_UNSIGNED_SHORT,
	kUnsignedByte				= GL_UNSIGNED_BYTE,
	kHalfFloat					= GL_HALF_FLOAT,
	kFixed						= GL_FIXED,
    kInt_2_10_10_10				= GL_INT_2_10_10_10_REV,
	kUnsignedInt_2_10_10_10		= GL_UNSIGNED_INT_2_10_10_10_REV,
   	kUnsignedInt_10f_11f_11f	= GL_UNSIGNED_INT_10F_11F_11F_REV,
#ifndef GL_DOUBLE
	kDouble					= 0x140A,
#else
	kDouble					= GL_DOUBLE,
#endif

	// Renderbuffers & framebuffers
	kRenderbuffer			= GL_RENDERBUFFER,
	kFramebuffer			= GL_FRAMEBUFFER,
	kFramebufferComplete					= GL_FRAMEBUFFER_COMPLETE,
	kFramebufferIncompleteAttachment		= GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
	kFramebufferIncompleteMissingAttachment	= GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
#ifndef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
	kFramebufferIncompleteDimensions		= 0x8CD9,
#else
	kFramebufferIncompleteDimensions		= GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS,
#endif
	kFramebufferUnsupported					= GL_FRAMEBUFFER_UNSUPPORTED,
		
	// Textures
#ifndef GL_TEXTURE_BUFFER
	kTextureBuffer			= 0x8C2A,
#else
	kTextureBuffer			= GL_TEXTURE_BUFFER,
#endif
    
#ifndef GL_TEXTURE_1D
    kTexture1D              = 0x0DE0,
#else
	kTexture1D				= GL_TEXTURE_1D,
#endif
	kTexture2D				= GL_TEXTURE_2D,
#ifndef GL_TEXTURE_3D
	kTexture3D				= 0x806F,
#else
    kTexture3D              = GL_TEXTURE_3D,
#endif

	// Cube maps
	kTextureCubeMap				= GL_TEXTURE_CUBE_MAP,
	kTextureCubeMapPositiveX	= GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	kTextureCubeMapNegativeX	= GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	kTextureCubeMapPositiveY	= GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	kTextureCubeMapNegativeY	= GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	kTextureCubeMapPositiveZ	= GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	kTextureCubeMapNegativeZ	= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,

	kClampToEdge			= GL_CLAMP_TO_EDGE,
	kRepeat					= GL_REPEAT,
	kTextureWrapS			= GL_TEXTURE_WRAP_S,
	kTextureWrapT			= GL_TEXTURE_WRAP_T,
	kTextureWrapR			= GL_TEXTURE_WRAP_R,
	kTextureMinFilter		= GL_TEXTURE_MIN_FILTER,
	kTextureMagFilter		= GL_TEXTURE_MAG_FILTER,
	kLinear					= GL_LINEAR,
	kNearest				= GL_NEAREST,
	kNearestMipmapNeares	= GL_NEAREST_MIPMAP_NEAREST,
	kLinearMipmapNearest	= GL_LINEAR_MIPMAP_NEAREST,
	kNearestMipmapLinear	= GL_NEAREST_MIPMAP_LINEAR,
	kLinearMipmapLinear		= GL_LINEAR_MIPMAP_LINEAR,
	kTexture0				= GL_TEXTURE0,
	kTexture1,
	kTexture2,
	kTexture3,
	kTexture4,
	kTexture5,
	kTexture6,
	kTexture7,
	kTexture8,
	kTexture9,
	kTexture10,
	kTexture11,
	kTexture12,
	kTexture13,
	kTexture14,
	kTexture15,
	kTexture16,
	kTexture17,
	kTexture18,
	kTexture19,
	kTexture20,
	kTexture21,
	kTexture22,
	kTexture23,
	kTexture24,
	kTexture25,
	kTexture26,
	kTexture27,
	kTexture28,
	kTexture29,
	kTexture30,
	kTexture31,

	// Buffer usage
	kStreamDraw			= GL_STREAM_DRAW,
	kStreamRead			= GL_STREAM_READ,
	kStreamCopy			= GL_STREAM_COPY,
	kStaticDraw			= GL_STATIC_DRAW,
	kStaticRead			= GL_STATIC_READ,
	kStaticCopy			= GL_STATIC_COPY,
	kDynamicDraw		= GL_DYNAMIC_DRAW,
	kDynamicRead		= GL_DYNAMIC_READ,
	kDynamicCopy		= GL_DYNAMIC_COPY,

	kNone				= GL_NONE,
    
#ifndef GL_FRONT_LEFT
	kFrontLeft			= 0x0400,
#else
    kFrontLeft			= GL_FRONT_LEFT,
#endif
#ifndef GL_FRONT_RIGHT
    kFrontRight			= 0x0401,
#else
	kFrontRight			= GL_FRONT_RIGHT,
#endif
#ifndef GL_BACK_LEFT
    kBackLeft			= 0x0402,
#else
	kBackLeft			= GL_BACK_LEFT,
#endif
#ifndef GL_BACK_RIGHT
	kBackRight			= 0x0403,
#else
    kBackRight			= GL_BACK_RIGHT,
#endif
	kDepthAttachment = GL_DEPTH_ATTACHMENT,
	kColorAttachment0	= GL_COLOR_ATTACHMENT0,
	kColorAttachment1 = GL_COLOR_ATTACHMENT1,
	kColorAttachment2 = GL_COLOR_ATTACHMENT2,
	kColorAttachment3 = GL_COLOR_ATTACHMENT3,
	kColorAttachment4 = GL_COLOR_ATTACHMENT4,
	kColorAttachment5 = GL_COLOR_ATTACHMENT5,
	kColorAttachment6 = GL_COLOR_ATTACHMENT6,
	kColorAttachment7 = GL_COLOR_ATTACHMENT7,
	kColorAttachment8 = GL_COLOR_ATTACHMENT8,
	kColorAttachment9 = GL_COLOR_ATTACHMENT9,
	kColorAttachment10 = GL_COLOR_ATTACHMENT10,
	kColorAttachment11 = GL_COLOR_ATTACHMENT11,
	kColorAttachment12 = GL_COLOR_ATTACHMENT12,
	kColorAttachment13 = GL_COLOR_ATTACHMENT13,
	kColorAttachment14 = GL_COLOR_ATTACHMENT14,
	kColorAttachment15 = GL_COLOR_ATTACHMENT15,
    
    
#ifndef GL_ARRAY_BUFFER_BINDING
    kArrayBufferBinding             = 0x8894,
#else
    kArrayBufferBinding             = GL_ARRAY_BUFFER_BINDING,
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER_BINDING
    kElementArrayBufferBinding      = 0x8895,
#else
    kElementArrayBufferBinding      = GL_ELEMENT_ARRAY_BUFFER_BINDING,
#endif
#ifndef GL_COPY_READ_BUFFER_BINDING
    kCopyReadBufferBinding          = 0x8F36,
#else
    kCopyReadBufferBinding          = GL_COPY_READ_BUFFER_BINDING,
#endif
#ifndef GL_COPY_WRITE_BUFFER_BINDING
    kCopyWriteBufferBinding         = 0x8F37,
#else
    kCopyWriteBufferBinding         = GL_COPY_WRITE_BUFFER_BINDING,
#endif
#ifndef GL_DRAW_INDIRECT_BUFFER_BINDING
    kDrawIndirectBufferBinding		= 0x8F43,
#else
    kDrawIndirectBufferBinding		= GL_DRAW_INDIRECT_BUFFER_BINDING,
#endif
#ifndef GL_PIXEL_PACK_BUFFER_BINDING
    kPixelPackBufferBinding         = 0x88ED,
#else
    kPixelPackBufferBinding         = GL_PIXEL_PACK_BUFFER_BINDING,
#endif
#ifndef GL_PIXEL_UNPACK_BUFFER_BINDING
    kPixelUnpackBufferBinding       = 0x88EF,
#else
    kPixelUnpackBufferBinding       = GL_PIXEL_UNPACK_BUFFER_BINDING,
#endif
#ifndef GL_TRANSFORM_FEEDBACK_BUFFER_BINDING
    kTransformFeedbackBufferBinding = 0x8C8F,
#else
    kTransformFeedbackBufferBinding = GL_TRANSFORM_FEEDBACK_BUFFER_BINDING,
#endif
#ifndef GL_UNIFORM_BUFFER_BINDING
    kUniformBufferBinding           = 0x8A28,
#else
    kUniformBufferBinding           = GL_UNIFORM_BUFFER_BINDING,
#endif
    
#ifndef GL_TEXTURE_BINDING_1D
    kTextureBinding1D               = 0x8068,
#else
    kTextureBinding1D               = GL_TEXTURE_BINDING_1D,
#endif
    kTextureBinding2D               = GL_TEXTURE_BINDING_2D,
#ifndef GL_TEXTURE_BINDING_3D
    kTextureBinding3D               = 0x806A,
#else
    kTextureBinding3D               = GL_TEXTURE_BINDING_3D,
#endif
    
    kRenderbufferBinding            = GL_RENDERBUFFER_BINDING,
    kVertexArrayBinding             = GL_VERTEX_ARRAY_BINDING,

	kCurrentProgram					= GL_CURRENT_PROGRAM,

	// glGet vendor and driver support enums
	kRenderer						= GL_RENDERER,
	kVendor							= GL_VENDOR,
	kVersion						= GL_VERSION,
	kShadingLanguageVersion			= GL_SHADING_LANGUAGE_VERSION,
	kMajorVersion					= GL_MAJOR_VERSION,
	kMinorVersion					= GL_MINOR_VERSION,
	kExtensions						= GL_EXTENSIONS,
	kNumExtensions					= GL_NUM_EXTENSIONS,
	kCompileStatus					= GL_COMPILE_STATUS,
	kInfoLogLength					= GL_INFO_LOG_LENGTH,
	kLinkStatus						= GL_LINK_STATUS,

};


}
}
}
