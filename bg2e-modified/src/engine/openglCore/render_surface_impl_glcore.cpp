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


#include <bg/engine/openglCore/render_surface_impl.hpp>
#include <bg/engine/ogl_includes.hpp>
#include <bg/engine/opengl_core.hpp>
#include <bg/base/exception.hpp>
#include <bg/engine/openglCore/texture_impl.hpp>

namespace bg {
namespace engine {
namespace openglCore {


bool checkValid(base::RenderSurface::ColorPlane & colorPlane) {
	using namespace bg::base;
	switch (colorPlane.type) {
	case RenderSurface::kTypeColor:
		return	colorPlane.format==RenderSurface::kFormatUnsignedByte ||
				colorPlane.format==RenderSurface::kFormatFloat;
	case RenderSurface::kTypeDepth:
		return  colorPlane.format==RenderSurface::kFormatUnsignedShort ||
				colorPlane.format==RenderSurface::kFormatScreen;
	case RenderSurface::kTypeUndefined:
	default:
		return false;
	}
}

void assertCompatibility(bg::engine::openglCore::OpenGLState * state, base::RenderSurface::ColorPlaneSetup & colorPlaneSetup) {
	using namespace bg::base;
	using namespace bg::engine::opengl;
	int colorAttachments = 0;
	bg::Engine * engine = bg::Engine::Get();
	int maxDrawBuffers = state->getMaxDrawBuffers();
	for (auto colorPlane : colorPlaneSetup) {
		std::string typeString, formatString;
		RenderSurface::GetTypeAndFormatString(colorPlane, typeString, formatString);
		if (!checkValid(colorPlane)) {
			throw bg::base::InvalidParameterException("Invalid combination of color plane type and format: " + typeString + " and " + formatString);
		}
		if (colorPlane.type==RenderSurface::kTypeDepth && colorPlane!=colorPlaneSetup.back()) {
			throw bg::base::InvalidParameterException("Invalid color plane setup: depth color plane must be specified as the last element.");
		}
		
		if (colorPlane.type==RenderSurface::kTypeColor) {
			++colorAttachments;
		}
		
		if (colorPlane.format==RenderSurface::kFormatFloat && !engine->supportsFeature(state->context(), kFloatTextures)) {
			throw bg::base::InvalidParameterException("Invalid color plane format: floating point textures are not supported in this system.");
		}
		if (colorPlane.type==RenderSurface::kTypeDepth &&
			colorPlane.format!=RenderSurface::kFormatScreen &&
			!engine->supportsFeature(state->context(), kDepthTextures) )
		{
			throw bg::base::InvalidParameterException("Invalid color plane setup: depth textures are nos supported in this system.");
		}
		if (colorAttachments>maxDrawBuffers) {
			throw bg::base::InvalidParameterException("Incalid color plane setup: the requested setup requires more color planes than the system supports.");
		}
	}
}

GLint glFormat(bg::engine::openglCore::OpenGLState * state, base::RenderSurface::ColorPlane & colorPlane) {
	using namespace bg::base;
	using namespace bg::engine::opengl;
	bg::Engine * engine = bg::Engine::Get();
	switch (colorPlane.format) {
	case RenderSurface::kFormatUndefined:
		return 0;
	case RenderSurface::kFormatUnsignedShort:
		return kUnsignedShort;
	case RenderSurface::kFormatFloat:
		if (engine->supportsFeature(state->context(), kFloatTexture32)) {
			return kFloat;
		}
		else if (engine->supportsFeature(state->context(), kFloatTexture16)) {
			return kHalfFloat;
		}
	case RenderSurface::kFormatUnsignedByte:
		return kUnsignedByte;
	case RenderSurface::kFormatScreen:
		return kRenderbuffer;
	default:
		return 0;
	}
}

GLint glType(bg::engine::openglCore::OpenGLState * state, base::RenderSurface::ColorPlane & colorPlane) {
	using namespace bg::base;
	using namespace bg::engine::opengl;
	switch (colorPlane.type) {
	case RenderSurface::kTypeUndefined:
		return 0;
	case RenderSurface::kTypeColor:
		return kRgba;
	case RenderSurface::kTypeDepth:
		return kDepthComponent;
	default:
		return 0;
	}
}

GLint glInternalType(bg::engine::openglCore::OpenGLState * state, base::RenderSurface::ColorPlane & colorPlane) {
	using namespace bg::base;
	using namespace bg::engine::opengl;
	bg::Engine * engine = bg::Engine::Get();
	if (colorPlane.type==RenderSurface::kTypeDepth && colorPlane.format!=RenderSurface::kFormatScreen) {
		return kDepthComponent;
	}

	switch(colorPlane.format) {
	case RenderSurface::kFormatUndefined:
		return 0;
	case RenderSurface::kFormatUnsignedShort:
		return kRgba;
	case RenderSurface::kFormatFloat:
		if (engine->supportsFeature(state->context(), kFloatTexture32)) {
			return kRgba32f;
		}
		else if (engine->supportsFeature(state->context(), kFloatTexture16)) {
			return kRgba16f;
		}
	case RenderSurface::kFormatUnsignedByte:
		return kRgba;
	case RenderSurface::kFormatScreen:
		return kRgba;
	default:
		return 0;
	}
}

int getBytesPerPixel(GLint type) {
	using namespace bg::engine::opengl;
	switch (type) {
	case kRgb:
		return 3;
	case kRgba:
		return 4;
	case kRgba32f:
		return 32;
	case kRgba16f:
		return 16;
	default:
		return 0;
	}
}

void addAttachment(bg::engine::openglCore::OpenGLState * state, RenderSurfaceImpl::Attachment & att, int attachmentIndex, GLint fbo, const math::Size2Di & size, base::RenderSurface::ColorPlane & colorPlane) {
	using namespace bg::engine::opengl;
	att.glType = glType(state,colorPlane);
	att.glFormat = glFormat(state,colorPlane);
	att.glInternalType = glInternalType(state,colorPlane);
	if (colorPlane.format==base::RenderSurface::kFormatScreen) {
		GLuint renderbuffer;
		state->genRenderbuffers(1, &renderbuffer);
		state->bindRenderbuffer(kRenderbuffer, renderbuffer);
		//	Why kDepthComponent16????
		// state->renderbufferStorage(kRenderbuffer, kDepthComponent16, size.width(), size.height());
		state->renderbufferStorage(kRenderbuffer, kDepthComponent, size.width(), size.height());
		state->framebufferRenderbuffer(kFramebuffer, kDepthAttachment, kRenderbuffer, fbo);
		state->bindRenderbuffer(kRenderbuffer, 0);
		
		att.texture = nullptr;
		att.renderbuffer = renderbuffer;
	}
	else {
		ptr<base::Texture> result = new base::Texture(state->context());
		
		openglCore::TextureImpl * tex = result->impl<openglCore::TextureImpl>();
		GLuint texName;
		state->genTextures(1, &texName);
		
		tex->setTextureName(texName);
		tex->bindTexture2D();
		state->texParameteri(kTexture2D, kTextureMinFilter, kLinear);
		state->texParameteri(kTexture2D, kTextureMagFilter, kLinear);
		state->texParameteri(kTexture2D, kTextureWrapS, kClampToEdge);
		state->texParameteri(kTexture2D, kTextureWrapT, kClampToEdge);
		result->setWrapModeU(base::Texture::kWrapModeClamp);
		result->setWrapModeV(base::Texture::kWrapModeClamp);
		
		
		if (att.glInternalType==kDepthComponent) {
			state->texImage2D(kTexture2D, 0, GL_DEPTH_COMPONENT, size.width(), size.height(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
			state->framebufferTexture2D(kFramebuffer, GL_DEPTH_ATTACHMENT, kTexture2D, texName, 0);
		}
		else {
			state->texImage2D(kTexture2D, 0, att.glInternalType, size.width(), size.height(), 0, att.glType, att.glFormat, 0);
			state->framebufferTexture2D(kFramebuffer, kColorAttachment0 + attachmentIndex, kTexture2D, texName, 0);
		}
		
		state->bindTexture(kTexture2D, 0);
		result->setSize(size);
		result->setBytesPerPixel(getBytesPerPixel(att.glInternalType));
		
		att.texture = result.getPtr();
		att.renderbuffer = 0;
	}
}

void resizeAttachment(bg::engine::openglCore::OpenGLState * state, const math::Size2Di & size , RenderSurfaceImpl::Attachment & att, int attachmentIndex) {
	using namespace bg::engine::opengl;
	if (att.texture.valid()) {
		att.texture->setSize(size);
		openglCore::TextureImpl * impl = att.texture->impl<openglCore::TextureImpl>();
		impl->bindTexture2D();
		
		if (att.glInternalType==kDepthComponent) {
			state->texImage2D(kTexture2D, 0, GL_DEPTH_COMPONENT, size.width(), size.height(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		}
		else {
			state->texImage2D(kTexture2D, 0, att.glInternalType, size.width(), size.height(), 0, att.glType, att.glFormat, 0L);
		}
        
		state->bindTexture(kTexture2D, 0);
	}
	else if (att.renderbuffer) {
		state->bindRenderbuffer(kRenderbuffer, att.renderbuffer);
		//	Why kDepthComponent16????
		// state->renderbufferStorage(kRenderbuffer, kDepthComponent16, size.width(), size.height());
		state->renderbufferStorage(kRenderbuffer, kDepthComponent, size.width(), size.height());
		state->bindRenderbuffer(kRenderbuffer, 0);
	}
	else {
		throw bg::base::NullParameterException("Could not resize attachment: the attachment does not contains neither texture nor renderbuffer.");
	}
}

RenderSurfaceImpl::RenderSurfaceImpl(bg::base::RenderSurface * surface, RenderSurfaceTarget target)
	:bg::engine::RenderSurfaceImpl(surface,target)
	,_fbo(0)
{
}
	
RenderSurfaceImpl::~RenderSurfaceImpl() {
	
}

bool RenderSurfaceImpl::supportFormat(int fmt) {
	switch (fmt) {
	case bg::base::RenderSurface::kFormatFloat:
		return bg::Engine::Get()->supportsFeature(_renderSurface->context(), bg::kFloatTextures);
		break;
	case bg::base::RenderSurface::kFormatUnsignedByte:
		return true;
		break;
	case bg::base::RenderSurface::kFormatUnsignedShort:
		return true;
		break;
	case bg::base::RenderSurface::kFormatUndefined:
		return false;
	default:
		return false;
	}
}

bool RenderSurfaceImpl::supportType(int type) {
	if (type==bg::base::RenderSurface::kTypeColor) {
		return true;
	}
	else if (type==bg::base::RenderSurface::kTypeDepth) {
		return bg::Engine::Get()->supportsFeature(_renderSurface->context(), bg::kDepthTextures);
	}
	return false;
}

int RenderSurfaceImpl::maxOutpuColorPlanes() {
	return gl()->getMaxDrawBuffers();
}

void RenderSurfaceImpl::invalidateSurface() {
	if (_fbo) {
		using namespace bg::engine::opengl;
		std::vector<int32_t> attachments;
		for (auto att : _attachments) {
			if (att.texture.valid()) {
				attachments.push_back(kColorAttachment0 + static_cast<int>(attachments.size()));
			}
			else if (att.renderbuffer) {
				attachments.push_back(kDepthAttachment);
			}
		}
		gl()->bindFramebuffer(kFramebuffer,_fbo);
		gl()->invalidateFramebuffer(kFramebuffer,attachments.size(),&attachments[0]);
		gl()->bindFramebuffer(kFramebuffer, 0);
	}
}

void RenderSurfaceImpl::invalidateTextures() {
	if(_fbo) {
		using namespace bg::engine::opengl;
		for(auto att : _attachments) {
			if(att.texture.valid()) {
				att.texture->invalidate();
			}
		}
	}
}

void RenderSurfaceImpl::setSize(const bg::math::Size2Di & size) {
	if (_size!=size) {
		_size = size;
		if(_fbo) {
			int index = 0;
			for(auto att : _attachments) {
				resizeAttachment(gl(), _size, att, index++);
			}
		}
	}
}

void RenderSurfaceImpl::addOutputColorPlane(int type, int format) {
	_colorPlaneSetup.push_back(base::RenderSurface::ColorPlane(static_cast<base::RenderSurface::RenderSurfaceType>(type),
															   static_cast<base::RenderSurface::RenderSurfaceFormat>(format)));
}

void RenderSurfaceImpl::create() {
	using namespace bg::engine::opengl;
	assertCompatibility(gl(), _colorPlaneSetup);
	gl()->genFramebuffers(1, &_fbo);
	createTextures();
}

void RenderSurfaceImpl::setActive() {
	using namespace bg::engine::opengl;
	switch (_target) {
	case RenderSurfaceImpl::kTargetScreen:
		gl()->bindFramebuffer(kFramebuffer, 0);
		break;
	case RenderSurfaceImpl::kTargetTexture:
		gl()->bindFramebuffer(kFramebuffer, _fbo);
		break;
	}
}

unsigned char * RenderSurfaceImpl::readPixels(const bg::math::Viewport & vp) {
	using namespace bg::engine::opengl;
	unsigned char * pixels = new unsigned char[vp.width() * vp.height() * 4];
	gl()->readPixels(vp.x(), vp.y(), vp.width(), vp.height(), kRgba, kUnsignedByte, pixels);
	return pixels;
}

void RenderSurfaceImpl::destroy() {
	using namespace bg::engine::opengl;
	if (_fbo) {
		gl()->bindFramebuffer(kFramebuffer, 0);
		gl()->deleteBuffers(1, &_fbo);
		_fbo = 0;
		destroyTextures();
	}
}

void RenderSurfaceImpl::createTextures() {
	using namespace bg::engine::opengl;
	gl()->bindFramebuffer(kFramebuffer, _fbo);
	
	int attachment = 0;
	std::vector<uint32_t> attVector;
	for (auto colorPlane : _colorPlaneSetup) {
		_attachments.push_back(Attachment());
		addAttachment(gl(), _attachments.back(), attachment++, _fbo, _size, colorPlane);
		if (_attachments.back().texture.valid()) {
			attVector.push_back(kColorAttachment0 + static_cast<uint32_t>(attVector.size()));
		}
	}
	gl()->drawBuffers(static_cast<GLsizei>(attVector.size()), &attVector[0]);
	GLenum status = gl()->checkFramebufferStatus(kFramebuffer);
	switch (status) {
		case kFramebufferComplete: // Ok
			break;
		case kFramebufferIncompleteAttachment:
			throw bg::base::FramebufferCreationException("Error creating framebuffer: invalid attachment. Check compatibility of the specified color planes.");
		case kFramebufferIncompleteMissingAttachment:
			throw bg::base::FramebufferCreationException("Error creating framebuffer: missing attachment. Any color planes is specified.");
		case kFramebufferIncompleteDimensions:
			throw bg::base::FramebufferCreationException("Error creating framebuffer: missing attachment. Any color planes is specified.");
		case kFramebufferUnsupported:
			throw bg::base::FramebufferCreationException("Error creating framebuffer: unsupported. Invalid combination of color planes formats.");
		default:
			throw bg::base::FramebufferCreationException();
		}
	gl()->bindFramebuffer(kFramebuffer, 0);
}

void RenderSurfaceImpl::destroyTextures() {
	for (auto att : _attachments) {
		if (att.texture.valid()) {
			att.texture->destroy();
		}
		else if (att.renderbuffer) {
			gl()->deleteRenderbuffers(1, &att.renderbuffer);
		}
	}
	_attachments.clear();
}

bg::base::Texture * RenderSurfaceImpl::texture(int colorPlane) {
	if (_target==kTargetScreen) {
		throw bg::base::EngineException("Could not get texture from a screen render surface.");
	}
	if (colorPlane>=_attachments.size()) {
		std::stringstream msg;
		msg << "Could not get color plane texture: color plane not found with index " << colorPlane;
		throw bg::base::InvalidParameterException(msg.str());
	}
	else if (!_attachments[colorPlane].texture.valid()) {
		throw bg::base::InvalidParameterException("Could not get color plane texture: the specified index does not contains texture data.");
	}
	else {
		return _attachments[colorPlane].texture.getPtr();
	}
}

}
}
}
