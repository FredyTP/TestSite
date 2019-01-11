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

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

namespace bg {
namespace base {



Image * Image::load(const std::string &path) {
	using namespace bg::base;
	int w,h,bpp;
	unsigned char *data = stbi_load(path.c_str(), &w, &h, &bpp, 4);
	if (data) {
		_buffer = data;
		_width = w;
		_height = h;
		_format = kFormatRGBA;
		_bytesPerPixel = 4;
		_fileName = path;
        flipVertically();
	}
	return this;
}

Image * Image::saveBMP(const std::string & path) {
	return this;
}

Image * Image::saveTGA(const std::string & path) {
	return this;
}

Image * Image::saveJPG(const std::string & path) {
	return this;
}

Image * Image::savePNG(const std::string & path) {
	return this;
}

void Image::save(const std::string & path, int format) {
}

}
}
