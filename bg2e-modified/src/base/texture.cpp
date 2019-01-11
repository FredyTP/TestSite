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


#include <bg/base/texture.hpp>
#include <bg/engine.hpp>
#include <bg/base/exception.hpp>

#include <iostream>

namespace bg {
namespace base {

const std::string & Texture::FileName(Texture * t) {
	static std::string s_fileName = "";
	if (t) {
		return t->fileName();
	}
	else {
		return s_fileName;
	}
}
	
Texture::Texture(Context * ctx)
	:ContextObject(ctx)
	,_type(kTexture2D)
	,_wrapModeU(kWrapModeRepeat)
	,_wrapModeV(kWrapModeRepeat)
	,_wrapModeW(kWrapModeRepeat)
	,_resizeFilter(kFilterMipmap)
{
	_impl = bg::Engine::Get()->createTextureImpl(this);
}

Texture * Texture::TextureWithImage(Context * ctx, Image * img, WrapMode m, ResizeFilter rf) {
    bg::ptr<Texture> result;
    if (img) {
        result = new Texture(ctx);
        result->setWrapModeU(m);
        result->setWrapModeV(m);
        result->setWrapModeW(m);
        result->setResizeFilter(rf);
        result->createWithImage(img);
    }
    return result.release();
}
    
Texture::~Texture() {
	destroy();
}

void Texture::createWithImage(Image * image) {
	if (!image) {
		throw bg::base::NullParameterException("Could not create texture: invalid image.");
	}
	_fileName = image->fileName();
	createWithBuffer(image->buffer(), image->width(), image->height(), image->bytesPerPixel());
}

void Texture::createWithBuffer(const unsigned char * data, int width, int height, int bpp) {
	if (!data) {
		throw bg::base::NullParameterException("Could not create texture: inavlid buffer");
	}
	else if (width<=0 || height<=0 || bpp<=0) {
		throw bg::base::InvalidParameterException("Could not create texture: invalid image or buffer format");
	}
	_impl->create(data, width, height, bpp);
	_size.set(width, height);
	_bpp = bpp;
	_valid = true;
}

void Texture::createCubemap(
	Image * posX, Image * negX,
	Image * posY, Image * negY,
	Image * posZ, Image * negZ
) {
	if (!posX || !negX || !posY || !negY || !posZ || !negZ) {
		throw bg::base::NullParameterException("Could not create texture cubemap: invalid image.");
	}
	if (posX->width() != negX->width() || posX->height() != negX->height() || 
		posX->width() != posY->width() || posX->height() != posY->height() || 
		posX->width() != negY->width() || posX->height() != negY->height() ||
		posX->width() != posZ->width() || posX->height() != posZ->height() ||
		posX->width() != negZ->width() || posX->height() != negZ->height())
	{
		throw bg::base::InvalidParameterException("Invalid parameter creating cube map. Size mismach in some cubemap images.");
	}
	posX->flipVertically();
	negX->flipVertically();
	posY->flipVertically();
	negY->flipVertically();
	posZ->flipVertically();
	negZ->flipVertically();
	_impl->createCubemap(std::vector<const unsigned char*> {
		posX->buffer(), negX->buffer(),
		posY->buffer(), negY->buffer(),
		posZ->buffer(), negZ->buffer()
	}, posX->width(), posX->height(), posX->bytesPerPixel());
	_size.set(posX->width(), posX->height());
	_bpp = posX->bytesPerPixel();
	_valid = true;
}

Texture * Texture::ColorTexture(bg::base::Context * ctx, const bg::math::Color & c, const bg::math::Size2Di & size) {
	ptr<Texture> tex = new Texture(ctx);
	
	size_t dataSize = size.width() * size.height() * 4;
	unsigned char * textureData = new unsigned char[dataSize];
	for (auto i = 0; i<dataSize; i+=4) {
		textureData[i] = static_cast<unsigned char>(c.r() * 255.0f);
		textureData[i + 1] = static_cast<unsigned char>(c.g() * 255.0f);
		textureData[i + 2] = static_cast<unsigned char>(c.b() * 255.0f);
		textureData[i + 3] = static_cast<unsigned char>(c.a() * 255.0f);
	}
	
	tex->createWithBuffer(textureData, size.width(), size.height(), 4);
	delete [] textureData;
	return tex.release();
}

Texture * Texture::ColorCubemap(bg::base::Context * ctx, const bg::math::Color & c, const bg::math::Size2Di & size) {
ptr<Texture> tex = new Texture(ctx);
	
	size_t dataSize = size.width() * size.height() * 4;
	unsigned char * textureData = new unsigned char[dataSize];
	for (auto i = 0; i<dataSize; i+=4) {
		textureData[i] = static_cast<unsigned char>(c.r() * 255.0f);
		textureData[i + 1] = static_cast<unsigned char>(c.g() * 255.0f);
		textureData[i + 2] = static_cast<unsigned char>(c.b() * 255.0f);
		textureData[i + 3] = static_cast<unsigned char>(c.a() * 255.0f);
	}
	
	bg::ptr<Image> img = new Image();
	img->setData(textureData, size, 4, Image::kFormatRGBA);

	tex->createCubemap(
		img.getPtr(),
		img.getPtr(),
		img.getPtr(),
		img.getPtr(),
		img.getPtr(),
		img.getPtr()
	);
	
	return tex.release();
}

Texture * Texture::RandomTexture(bg::base::Context * ctx, const bg::math::Size2Di & size) {
	ptr<Texture> tex = new Texture(ctx);
	size_t dataSize = size.width() * size.height() * 4;
	unsigned char * textureData = new unsigned char[dataSize];
	for (auto i = 0; i<dataSize; i+=4) {
		bg::math::Vector3 randVector(bg::math::random() * 2.0f - 1.0f,
									 bg::math::random() * 2.0f - 1.0f,
									 0.0f);
		randVector.normalize();

		textureData[i]		= static_cast<unsigned char>(randVector.x() * 255.0);
		textureData[i + 1]	= static_cast<unsigned char>(randVector.y() * 255.0);
		textureData[i + 2]	= static_cast<unsigned char>(randVector.z() * 255.0);
		textureData[i + 3]	= 255;
	}

	tex->createWithBuffer(textureData, size.width(), size.height(), 4);
	delete [] textureData;
	return tex.release();
}

void Texture::readTexture(unsigned char *& buffer) {
	_impl->read(buffer, _size.width(), _size.height(), _bpp);
}

bool Texture::saveAsImage(const std::string & fileName) {
	bg::system::Path path(fileName);
	std::string ext = path.extension();
	if (this->valid()) {
		unsigned char * buffer = new unsigned char[_size.width() * _size.height() * 4];
		readTexture(buffer);
		bg::ptr<bg::base::Image> img = new bg::base::Image();
		img->setBuffer(buffer);
		img->setWidth(_size.width());
		img->setHeight(_size.height());
		img->setBytesPerPixel(4);
		img->setImageFormat(bg::base::Image::kFormatRGBA);

		if(ext == "jpg" || ext == "jpeg") {
			img->saveJPG(fileName);
		}
		else if(ext == "png") {
			img->saveJPG(fileName);
		}
		else if(ext == "bmp") {
			img->saveJPG(fileName);
		}
		else if(ext == "tga") {
			img->saveJPG(fileName);
		}
	}
	return false;
}


}
}
