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

#ifndef _bg2e_base_image_hpp_
#define _bg2e_base_image_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/math/vector.hpp>
#include <bg/system/path.hpp>

#include <bg/log.hpp>

namespace bg {
namespace base {

class BG2E_EXPORT Image : public ReferencedPointer {
public:
	enum ImageFormat {
		kFormatNone,
		kFormatRGB,
		kFormatRGBA,
		kFormatGrayScale
	};
	Image();
	Image(const std::string & path);
	Image(const Image * clone);
	Image(unsigned char * buffer, const bg::math::Size2Di size, int bytesPerPixel, ImageFormat fmt)
		:_buffer(buffer), _format(fmt), _bytesPerPixel(bytesPerPixel), _width(size.width()), _height(size.height()), _lastStatus(true)
	{
	}
	
	Image * load(const std::string & path);
	inline Image * load(const bg::system::Path & path) { return load(path.text()); }
	
	Image * saveBMP(const std::string & path);
	Image * saveTGA(const std::string & path);
	Image * saveJPG(const std::string & path);
	Image * savePNG(const std::string & path);
	
	void setData(unsigned char * buffer, const bg::math::Size2Di size, int bytesPerPixel, ImageFormat fmt, bool manageBuffer = true);
	
	const unsigned char * buffer() const { return _buffer; }
	void setBuffer(unsigned char * buffer, bool managed = true) { if (_buffer) destroy(); _buffer = buffer; _manageBuffer = managed; _lastStatus = true; }
	ImageFormat imageFormat() const { return _format; }
	void setImageFormat(ImageFormat format) { _format = format; }
	unsigned int bytesPerPixel() const { return _bytesPerPixel; }
	void setBytesPerPixel(unsigned int bpp) { _bytesPerPixel = bpp; }
	unsigned int width() const { return _width; }
	void setWidth(unsigned int w) { _width = w; }
	unsigned int height() const { return _height; }
	void setHeight(unsigned int h) { _height = h; }
	void setManageBuffer(bool m) { _manageBuffer = m; }
	inline bool isBufferManaged() const { return _manageBuffer; }
	
	bool valid() const { return _buffer!=NULL; }
	bool status() const { return _lastStatus; }
	
	const std::string & fileName() const { return _fileName; }
	void setFileName(const std::string & name) { _fileName = name; }
	
	void flipVertically();
	
	inline unsigned char * pixel(const bg::math::Position2Di & pos, const bg::math::Vector2i & offset = bg::math::Vector2i(0)) { return pixel(pos.x(), pos.y(), offset); }
	inline unsigned char * pixel(unsigned int x, unsigned int y, const bg::math::Vector2i & offset = bg::math::Vector2i(0)) {
		x = (x + offset.x()) % _width;
		y = (y + offset.y()) % _height;
		return _buffer ? _buffer + (y * _width + x																					) * _bytesPerPixel : nullptr;
	}
	
	inline void color(const bg::math::Position2Di & pos, bg::math::Color & c) {
		color(pos.x(), pos.y(), c);
	}
	
	inline void color(unsigned int x, unsigned int y, bg::math::Color & color) {
		unsigned char * px = pixel(x, y);
		if (px) {
			float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;
			switch (_format) {
			case kFormatGrayScale:
				r = static_cast<float>(*px) / 255.0f;
				g = static_cast<float>(*px) / 255.0f;
				b = static_cast<float>(*px) / 255.0f;
				a = static_cast<float>(*px) / 255.0f;
				break;
			case kFormatRGBA:
				r = static_cast<float>(*px) / 255.0f;
				g = static_cast<float>(*(px + 1)) / 255.0f;
				b = static_cast<float>(*(px + 2)) / 255.0f;
				a = static_cast<float>(*(px + 3)) / 255.0f;
				break;
			case kFormatRGB:
				r = static_cast<float>(*px) / 255.0f;
				g = static_cast<float>(*(px + 1)) / 255.0f;
				b = static_cast<float>(*(px + 2)) / 255.0f;
				a = 1.0f;
				break;
			case kFormatNone:
				break;
			}
			
			color = bg::math::Color(r, g, b, a);
		}
		else {
			color = bg::math::Color::Transparent();
		}
	}
	
	inline void setPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
		if (_buffer && x < _width && y < _height) {
			unsigned char * bufferPtr = _buffer + (y * _width + x) * _bytesPerPixel;
			switch(_format) {
			case kFormatGrayScale:
			{
				float rf = static_cast<float>(r) / 255.0f;
				float gf = static_cast<float>(g) / 255.0f;
				float bf = static_cast<float>(b) / 255.0f;
				bufferPtr[0] = static_cast<unsigned char>(sqrt((rf * rf + gf * gf + bf * bf) / 3.0f) * 255.0f);
				break;
			}
			case kFormatRGBA:
				bufferPtr[0] = r;
				bufferPtr[1] = g;
				bufferPtr[2] = b;
				bufferPtr[3] = a;
				break;
			case kFormatRGB:
				bufferPtr[0] = r;
				bufferPtr[1] = g;
				bufferPtr[2] = b;
				break;
            case kFormatNone:
                bg::log(bg::log::kWarning) << "Warning: setting pixel without format" << bg::endl;
                break;
			}
		}
	}
	
	inline void setPixel(unsigned int x, unsigned int y, const bg::math::Color & color) {
		setPixel(x, y,	static_cast<unsigned char>(color.r() * 255.0f),
					static_cast<unsigned char>(color.g() * 255.0f),
					static_cast<unsigned char>(color.b() * 255.0f),
					static_cast<unsigned char>(color.a() * 255.0f));
	}
	
	static Image * BuildNormalMap(bg::base::Image * dispMap, float heightFactor, bool useSobel);
	static Image * GaussianBlur(bg::base::Image * img, int radius);
	
protected:
	virtual ~Image();
	
	unsigned char * _buffer;
	ImageFormat _format;
	unsigned int _bytesPerPixel;
	unsigned int _width;
	unsigned int _height;
	std::string _fileName;
	bool _manageBuffer = true;
	
	bool _lastStatus;
	
	void destroy();
	
	void save(const std::string & path, int format);
};

	
}
}

#endif
