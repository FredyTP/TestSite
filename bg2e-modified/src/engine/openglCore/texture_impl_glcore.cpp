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


#include <bg/engine/openglCore/texture_impl.hpp>
#include <bg/engine/ogl_includes.hpp>
#include <bg/base/exception.hpp>

#include <iostream>

namespace bg {
namespace engine {
namespace openglCore {

void setTextureWrap(bg::engine::openglCore::OpenGLState * gl, GLuint wrapTarget, base::Texture::WrapMode mode) {
	using namespace bg::engine::opengl;
	switch (mode) {
		case base::Texture::kWrapModeClamp:
			gl->texParameteri(kTexture2D, wrapTarget, kClampToEdge);
			break;
		case base::Texture::kWrapModeRepeat:
			gl->texParameteri(kTexture2D, wrapTarget, kRepeat);
			break;
	}
}

TextureImpl::TextureImpl(bg::base::Texture * tex)
	:bg::engine::TextureImpl(tex)
	,_textureName(0)
{
	
}

TextureImpl::~TextureImpl() {
	
}

void TextureImpl::create(const unsigned char * buffer, int width, int height, int bpp) {
	using namespace bg::engine::opengl;
	if (_textureName!=0) destroy();
	if (!buffer) {
		throw bg::base::NullParameterException("OpenGL create texture failed: buffer is null.");
	}
	if (width<=0 || height<=0 || bpp<=0) {
		throw bg::base::InvalidParameterException("OpenGL create texture: invalid width, height or bytes per pixel.");
	}

	GLint format = bpp==3 ? kRgb:(bpp==4 ? kRgba:(bpp==1 ? GL_RED:kRgba));

#if BG2E_OPENGL_ES_AVAILABLE==0
	gl(_texture->context())->bindBuffer(kPixelUnpackBuffer, 0);
#endif
	
	gl(_texture->context())->genTextures(1, &_textureName);
	gl(_texture->context())->bindTexture(kTexture2D, _textureName);
	setTextureWrap(gl(_texture->context()),kTextureWrapS,_texture->wrapModeU());
	setTextureWrap(gl(_texture->context()),kTextureWrapT,_texture->wrapModeV());
	setTextureWrap(gl(_texture->context()),kTextureWrapR,_texture->wrapModeW());
	
	if (bpp==1) {
#if BG2E_OPENGL_AVAILABLE==1
		GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_RED };
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
#endif
	}
	switch (_texture->resizeFilter()) {
	case bg::base::Texture::kFilterNearest:
		gl(_texture->context())->texParameteri(kTexture2D, kTextureMinFilter, kNearest);
		gl(_texture->context())->texParameteri(kTexture2D, kTextureMagFilter, kNearest);
		break;
	case bg::base::Texture::kFilterLinear:
		gl(_texture->context())->texParameteri(kTexture2D, kTextureMinFilter, kLinear);
		gl(_texture->context())->texParameteri(kTexture2D, kTextureMagFilter, kLinear);
		break;
	case bg::base::Texture::kFilterMipmap:
		gl(_texture->context())->texParameteri(kTexture2D, kTextureMinFilter, kLinearMipmapLinear);
		gl(_texture->context())->texParameteri(kTexture2D, kTextureMagFilter, kLinear);
		break;
	}
	
	gl(_texture->context())->texImage2D(kTexture2D, 0, kRgba, width, height, 0, format, kUnsignedByte, buffer);
	
	if (_texture->resizeFilter()==bg::base::Texture::kFilterMipmap) {
		gl(_texture->context())->generateMipmap(kTexture2D);
	}

	gl(_texture->context())->bindTexture(kTexture2D, 0);
}

void TextureImpl::createCubemap(const std::vector<const unsigned char *> data, int w, int h, int bpp) {
	using namespace bg::engine::opengl;
	if (data.size() < 6) {
		throw bg::base::InvalidParameterException("OpenGL create cubemap texture failed: unexpected number of texture buffers");
	}
	else if (w <= 0 || h <= 0 || bpp <= 0) {
		throw bg::base::InvalidParameterException("OpenGL create cubemap texture failed: invalid width, height or bytes per pixel");
	}

	// Force clamp and linear filter in cube maps
	_texture->setWrapModeU(bg::base::Texture::kWrapModeClamp);
	_texture->setWrapModeV(bg::base::Texture::kWrapModeClamp);
	_texture->setWrapModeW(bg::base::Texture::kWrapModeClamp);
	_texture->setResizeFilter(bg::base::Texture::kFilterLinear);

	GLint format = bpp == 3 ? kRgb : (bpp == 4 ? kRgba : (bpp == 1 ? GL_RED : kRgba));
#if BG2E_OPENGL_ES_AVAILABLE==0
	gl(_texture->context())->bindBuffer(kPixelUnpackBuffer, 0);
#endif

	gl(_texture->context())->genTextures(1, &_textureName);
	gl(_texture->context())->bindTexture(kTextureCubeMap, _textureName);
	
	gl(_texture->context())->texParameteri(kTextureCubeMap, kTextureMagFilter, kLinear);
	gl(_texture->context())->texParameteri(kTextureCubeMap, kTextureMinFilter, kLinear);
	gl(_texture->context())->texParameteri(kTextureCubeMap, kTextureWrapS, kClampToEdge);
	gl(_texture->context())->texParameteri(kTextureCubeMap, kTextureWrapT, kClampToEdge);
	gl(_texture->context())->texParameteri(kTextureCubeMap, kTextureWrapR, kClampToEdge);

	auto i = 0;
	for (auto buff : data) {
		if (!buff) {
			throw bg::base::NullParameterException("OpenGL create cubemap texture failed: null data pointer found");
		}
		gl(_texture->context())->texImage2D(kTextureCubeMapPositiveX + i, 0, format, w, h, 0, format, kUnsignedByte, buff);
		++i;
	}

	gl(_texture->context())->bindTexture(kTextureCubeMap, 0);
}

void TextureImpl::destroy() {
	if (_textureName!=0) {
		gl(_texture->context())->deleteTextures(1, &_textureName);
		_textureName = 0;
	}
}

void TextureImpl::read(unsigned char *& buffer, int width, int height, int bpp) {
	using namespace bg::engine::opengl;
	if (_textureName!=0) {
		gl(_texture->context())->bindTexture(kTexture2D,_textureName);
		GLint texSize = width * height * bpp;
		GLint internalFormat = 0;
		switch (bpp) {
		case 3:
			internalFormat = kRgb;
			break;
		case 4:
			internalFormat = kRgba;
			break;
		default:
			throw bg::base::CompatibilityException("Incompatible texture format in TextureImpl::read()");
		}

		if (texSize) {
			buffer = new unsigned char[texSize];
			gl(_texture->context())->getTexImage(kTexture2D, 0, internalFormat, kUnsignedByte, buffer);
		}
		else {
			buffer = nullptr;
		}
	}
}

void TextureImpl::ClearActive() {
	glActiveTexture(0);
}

void TextureImpl::setActive(int textureUnit) {
	gl(_texture->context())->activeTexture(bg::engine::opengl::kTexture0 + textureUnit);
}
	
void TextureImpl::bindTexture2D() {
	gl(_texture->context())->bindTexture(bg::engine::opengl::kTexture2D, _textureName);
}

void TextureImpl::invalidate() {
	gl(_texture->context())->invalidateTexImage(_textureName, 0);
}

}
}
}
