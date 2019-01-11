

#import <Cocoa/Cocoa.h>

#include <bg/base/image.hpp>
#include <bg/math/vector.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

namespace bg {
namespace base {


enum ImageFormats {
	kFormatBMP = 1,
	kFormatTGA,
	kFormatJPG,
	kFormatPNG
};

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
	this->save(path, ImageFormats::kFormatBMP);
	return this;
}

Image * Image::saveTGA(const std::string & path) {
	this->save(path, ImageFormats::kFormatTGA);
	return this;
}

Image * Image::saveJPG(const std::string & path) {
	this->save(path, ImageFormats::kFormatJPG);
	return this;
}

Image * Image::savePNG(const std::string & path) {
	this->save(path, ImageFormats::kFormatPNG);
	return this;
}

void Image::save(const std::string & path, int format) {
	if (!valid()) {
		_lastStatus = false;
	}
	else {
		
		NSBitmapImageRep * imageRep = nullptr;
		unsigned char *saveBuffer = new unsigned char[_width * _height * 3];
		
		// Save in rgb format and flip vertically the image
		unsigned int lineWidth = _width * _bytesPerPixel;
		for (unsigned int i = 0; i < _height; ++i) {
			unsigned int k = 0;
			for (unsigned int j = 0; j < lineWidth; j += _bytesPerPixel) {
				unsigned char * src = &_buffer[(_height - i - 1) * _width * _bytesPerPixel + j];
				unsigned char * dst = &saveBuffer[i * _width * 3 + k];
				dst[0] = src[0];
				dst[1] = src[1];
				dst[2] = src[2];
				k+=3;
			}
		}
		
		imageRep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:&saveBuffer
														   pixelsWide:_width
														   pixelsHigh:_height
														bitsPerSample:8
													  samplesPerPixel:3
															 hasAlpha:NO
															 isPlanar:NO
													   colorSpaceName:NSDeviceRGBColorSpace
														 bitmapFormat:NSAlphaFirstBitmapFormat
														  bytesPerRow:_width * 3
														 bitsPerPixel:24];
		
		if (imageRep) {
			NSData * imgData = nullptr;
			switch (format) {
				case ImageFormats::kFormatBMP:
					imgData = [imageRep representationUsingType:NSBMPFileType properties:@{}];
					break;
				case ImageFormats::kFormatTGA:
					imgData = [imageRep representationUsingType:NSTIFFFileType properties:@{}];
					break;
					
				case ImageFormats::kFormatJPG:
					imgData = [imageRep representationUsingType:NSJPEGFileType properties:@{}];
					break;
				case ImageFormats::kFormatPNG:
					imgData = [imageRep representationUsingType:NSPNGFileType properties:@{}];
					break;
				default:
					break;
			}
			[imgData writeToFile:[NSString stringWithUTF8String:path.c_str()] atomically:YES];
			delete [] saveBuffer;
		}
	}
}


}
}
