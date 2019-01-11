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


#include <bg/base/image.hpp>
#include <bg/system/system.hpp>
#include <cstring>

namespace bg {
namespace base {


class NormalMapUtils {
public:
	static void GenerateNormal(Image * img, unsigned int x, unsigned int y, bg::base::Image * dispMap, float heightFactor, bool sobelFilter) {
		float topLeft = height(dispMap->pixel(x, y, bg::math::Vector2i(-1, -1)), heightFactor, sobelFilter);
		float top = height(dispMap->pixel(x, y, bg::math::Vector2i(0, -1)), heightFactor, sobelFilter);
		float topRight = height(dispMap->pixel(x, y, bg::math::Vector2i(1, -1)), heightFactor, sobelFilter);
		float left = height(dispMap->pixel(x, y, bg::math::Vector2i(-1, 0)), heightFactor, sobelFilter);
		float right = height(dispMap->pixel(x, y, bg::math::Vector2i(1, 0)), heightFactor, sobelFilter);
		float bottomLeft = height(dispMap->pixel(x, y, bg::math::Vector2i(-1, 1)), heightFactor, sobelFilter);
		float bottom = height(dispMap->pixel(x, y, bg::math::Vector2i(0, 1)), heightFactor, sobelFilter);
		float bottomRight = height(dispMap->pixel(x, y, bg::math::Vector2i(1, 1)), heightFactor, sobelFilter);
		bg::math::Vector3 norm;
		
		if (sobelFilter) {
			float Gx = topLeft - bottomRight + 2.0f * left - 2.0f * right + bottomLeft - bottomRight;
			float Gy = topLeft + 2.0f * top + topRight - bottomLeft - 2.0f * bottom - bottomRight;
			float Gz = 0.5f * sqrt(1.0f - Gx * Gx - Gy * Gy);
			
			norm.set(2.0f * Gy, 2.0f * Gx, Gz);
			norm.normalize();
		}
		else {
			bg::math::Vector3 p0(0.0f, 0.0f, height(dispMap->pixel(x, y), heightFactor, sobelFilter));
			bg::math::Vector3 p1(1.0f, 0.0f, right);
			bg::math::Vector3 p2(0.0f, -1.0f, bottom);
			
			bg::math::Vector3 v1 = p0 - p1;
			bg::math::Vector3 v2 = p0 - p2;
			v1.normalize();
			v2.normalize();
			
			
			norm = v1.cross(v2);
			norm.x(norm.x() * -1.0f);
			norm.y(norm.y() * 1.0f);
			norm.z(norm.z() * -1.0f);
			norm.normalize();
		}
		img->setPixel(x, y, map_component(norm.x()), map_component(norm.y()), map_component(norm.z()));
	}
	
	static inline float height(const unsigned char * pixel, float heightFactor, bool useSobel) {
		return static_cast<float>(pixel[0]) / 255.0f * heightFactor * (useSobel ? 0.03f : 1.0f);
	}
	
	static inline const unsigned char map_component(float pX)
	{
		return static_cast<unsigned char>((pX + 1.0f) * (255.0f / 2.0f));
	}
};


Image * Image::BuildNormalMap(bg::base::Image * dispMap, float heightFactor, bool useSobel) {
	ptr<Image> nmap;
	
	if (dispMap && dispMap->valid()) {
		heightFactor = -heightFactor;
		nmap = new Image();
		
		nmap->_width = dispMap->width();
		nmap->_height = dispMap->height();
		nmap->_bytesPerPixel = 3;
		nmap->_format = kFormatRGB;
		nmap->_buffer = new unsigned char[nmap->_width * nmap->_height * nmap->_bytesPerPixel];
		
		for (unsigned int i = 0; i < nmap->_width; ++i) {
			for (unsigned int j = 0; j < nmap->_height; ++j) {
				NormalMapUtils::GenerateNormal(nmap.getPtr(), i, j, dispMap, heightFactor, useSobel);
			}
		}
	}
	
	return nmap.release();
}

Image * Image::GaussianBlur(Image * img, int radius) {
	ptr<Image> result;
	if (img && img->valid()) {
		result = new Image();
		int w = static_cast<int>(img->width());
		int h = static_cast<int>(img->height());
		auto bpp = img->bytesPerPixel();
		Image::ImageFormat fmt = img->imageFormat();
		unsigned char * buffer = new unsigned char[w * h * bpp];
		result->setBuffer(buffer);
		result->setWidth(w);
		result->setHeight(h);
		result->setBytesPerPixel(bpp);
		result->setImageFormat(fmt);
		
		int rs = static_cast<int>(ceil(static_cast<float>(radius)* 2.57f));
		for (auto i = 0; i < h; ++i) {
			for (auto j = 0; j < w; ++j) {
				bg::math::Color val;
				float wsum = 0;
				for (auto iy = i - rs; iy < i + rs + 1; ++iy) {
					for (auto ix = j - rs; ix < j + rs + 1; ++ix) {
						bg::math::Color srcPx;
						img->color(ix, iy, srcPx);
						int dsq = (ix - j)*(ix - j) + (iy - i)*(iy - i);
						float weight = exp(static_cast<float>(-dsq / (2 * radius * radius))) / math::kPi * 2.0f * static_cast<float>(radius * radius);
						val = val + srcPx.scale(weight);
						wsum += weight;
					}
				}
				val.scale(1.0f / wsum);
				val.a(1.0f);
				result->setPixel(j, i, val);
			}
		}
		
	}
	return result.release();
}

Image::Image()
	:_buffer(0L),
	_format(kFormatNone),
	_bytesPerPixel(0),
	_width(0),
	_height(0),
	_lastStatus(true)
{
	
}

Image::Image(const std::string & path)
	:_buffer(0L),
	_format(kFormatNone),
	_bytesPerPixel(0),
	_width(0),
	_height(0),
	_lastStatus(true)
{
	load(path);
}

Image::Image(const Image * clone)
	:_buffer(0L),
	_format(kFormatNone),
	_bytesPerPixel(0),
	_width(0),
	_height(0),
	_lastStatus(true)
{
	if (clone) {
		std::memcpy(_buffer, clone->_buffer, clone->_width * clone->_height * clone->_bytesPerPixel);
		_format = clone->_format;
		_bytesPerPixel = clone->_bytesPerPixel;
		_width = clone->_width;
		_height = clone->_height;
	}
	else {
		_lastStatus = false;
	}
}

Image::~Image() {
	destroy();
}

void Image::setData(unsigned char * buffer, const bg::math::Size2Di size, int bytesPerPixel, ImageFormat fmt, bool manageBuffer) {
	setBuffer(buffer, manageBuffer);
	_width = size.width();
	_height = size.height();
	_bytesPerPixel = bytesPerPixel;
	_format = fmt;
}

void Image::flipVertically() {
	if (_buffer) {
		unsigned int lineSize = _width * _bytesPerPixel;
		unsigned int size = lineSize * _height;
		unsigned char * newBuffer = new unsigned char[size];
		for (unsigned int i=0; i<_height; ++i) {
			unsigned int dstLine = _height - (i + 1);
			unsigned char * src = _buffer + lineSize * i;
			unsigned char * dst = newBuffer + lineSize * dstLine;
			memcpy(dst, src, lineSize);
		}
		delete [] _buffer;
		_buffer = newBuffer;
	}
}

void Image::destroy() {
	if (valid()) {
		if (_manageBuffer) delete [] _buffer;
		_buffer = 0L;
		_format = kFormatNone;
		_bytesPerPixel = 0;
		_width = 0;
		_height = 0;
	}
}
	
}
}
