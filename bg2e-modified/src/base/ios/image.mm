
#include <bg/base/image.hpp>

#import <UIKit/UIKit.h>

namespace bg {
namespace base {
	
Image * Image::load(const std::string &path) {
	destroy();
	NSString * sPath = [NSString stringWithUTF8String:path.c_str()];
	NSData * imgData = [NSData dataWithContentsOfFile:sPath];
	UIImage * image = [UIImage imageWithData:imgData];
	if (image) {
		_format = kFormatRGBA;
		_width = image.size.width;
		_height = image.size.height;
		_bytesPerPixel = 4;
		CGImageRef cgimage = [image CGImage];
		CFDataRef data = CGDataProviderCopyData(CGImageGetDataProvider(cgimage));
		const unsigned char * buffer =  CFDataGetBytePtr(data);
		_buffer = new unsigned char[_width*_height*_bytesPerPixel];
		for (unsigned int i=0; i<_height; ++i) {
			memcpy(&_buffer[(_height-i-1)*_width*_bytesPerPixel], &buffer[i*_width*_bytesPerPixel], _width*_bytesPerPixel);
		}
		_fileName = path;
		CFRelease(data);
		
	}
	_lastStatus = _buffer!=NULL ? true:false;
	return this;
}

Image * Image::saveBMP(const std::string & path) {
	// TODO: Implement saveBMP in iOS
	return this;
}

Image * Image::saveTGA(const std::string & path) {
	// TODO: Implement saveTGA in iOS
	return this;
}

Image * Image::saveJPG(const std::string & path) {
	// TODO: Implement saveTGA in iOS
	return this;
}

Image * Image::savePNG(const std::string & path) {
	// TODO: Implement saveTGA in iOS
	return this;
}

void Image::save(const std::string & path, int format) {
	// TODO: Implement save in iOS
	if (!valid()) {
		_lastStatus = false;
	}
	else {
		//	_lastStatus = SOIL_save_image(path.c_str(), format, _width, _height, _bytesPerPixel, _buffer)==0 ? false:true;
	}
}

}
}
