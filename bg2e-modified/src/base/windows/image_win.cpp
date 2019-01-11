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

#ifndef WIN32
#define WIN32
#endif
#include <WinSock2.h>
#include <Windows.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

namespace bg {
namespace base {

class GdiPlusUtils {
public:
	enum SaveFormats {
		kFormatJpg = 1,
		kFormatPng = 2,
		kFormatTga = 3,
		kFormatBmp = 4
	};
	
	GdiPlusUtils() {
		GdiplusStartup(&_token, &_startupInput, nullptr);
	}
	
	~GdiPlusUtils() {
		Gdiplus::GdiplusShutdown(_token);
	}
	
	int getEncoder(const WCHAR * format, CLSID & pClsid) {
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes
		
		Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
		
		Gdiplus::GetImageEncodersSize(&num, &size);
		if (size == 0)
			return -1;  // Failure
		
		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == NULL)
			return -1;  // Failure
		
		GetImageEncoders(num, size, pImageCodecInfo);
		
		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
			{
				pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j;  // Success
			}
		}
		
		free(pImageCodecInfo);
		return -1;  // Failure
	}
	
protected:
	Gdiplus::GdiplusStartupInput _startupInput;
	ULONG_PTR _token;
};

Image * Image::load(const std::string &path) {
	unsigned char * imageBuffer = nullptr;
	destroy();
	GdiPlusUtils gdip;
	std::wstring wpath(path.begin(), path.end());
	
	Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(wpath.c_str(), false);
	if (bitmap && bitmap->GetLastStatus()==Gdiplus::Ok) {
		_width = bitmap->GetWidth();
		_height = bitmap->GetHeight();
		_bytesPerPixel = 4;
		_format = kFormatRGBA;
		Gdiplus::Rect rt(0, 0, _width, _height);
		Gdiplus::BitmapData data;
		imageBuffer = new unsigned char[_width * _height * _bytesPerPixel];
		bitmap->LockBits(&rt, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data);
		memcpy(imageBuffer, data.Scan0, sizeof(unsigned char)* _width * _height * _bytesPerPixel);
		
		bitmap->UnlockBits(&data);
		
		delete bitmap;
		bitmap = nullptr;
	}
	
	
	if (imageBuffer != nullptr) {
		// Flip vertical and convert BGRA to RGBA
		_buffer = new unsigned char[_width*_height*_bytesPerPixel];
		unsigned int lineWidth = _width * _bytesPerPixel;
		for (unsigned int i = 0; i<_height; ++i) {
			for (unsigned int j = 0; j < lineWidth; j += _bytesPerPixel) {
				unsigned char * src = &imageBuffer[i*_width*_bytesPerPixel + j];
				_buffer[(_height - i - 1)*_width*_bytesPerPixel + j] = src[2];
				_buffer[(_height - i - 1)*_width*_bytesPerPixel + j + 1] = src[1];
				_buffer[(_height - i - 1)*_width*_bytesPerPixel + j + 2] = src[0];
				_buffer[(_height - i - 1)*_width*_bytesPerPixel + j + 3] = src[3];
			}
		}
		_fileName = path;
	}
	_lastStatus = _buffer != NULL ? true : false;
	return this;
}

Image * Image::saveBMP(const std::string & path) {
	this->save(path, GdiPlusUtils::kFormatBmp);
	return this;
}

Image * Image::saveTGA(const std::string & path) {
	this->save(path, GdiPlusUtils::kFormatTga);
	return this;
}

Image * Image::saveJPG(const std::string & path) {
	this->save(path, GdiPlusUtils::kFormatJpg);
	return this;
}

Image * Image::savePNG(const std::string & path) {
	this->save(path, GdiPlusUtils::kFormatPng);
	return this;
}

void Image::save(const std::string & path, int format) {
	if (!valid()) {
		_lastStatus = false;
	}
	else {
		GdiPlusUtils gdiPlusUtils;
		Gdiplus::PixelFormat fmt = PixelFormat24bppRGB;
		unsigned char *saveBuffer = new unsigned char[_width * _height * 3];
		int stride = 3 * _width;
		
		// Save in rgb format and flip vertically the image
		unsigned int lineWidth = _width * _bytesPerPixel;
		for (unsigned int i = 0; i < _height; ++i) {
			unsigned int k = 0;
			for (unsigned int j = 0; j < lineWidth; j += _bytesPerPixel) {
				unsigned char * src = &_buffer[(_height - i - 1) * _width * _bytesPerPixel + j];
				unsigned char * dst = &saveBuffer[i * _width * 3 + k];
				dst[0] = src[2];
				dst[1] = src[1];
				dst[2] = src[0];
				k+=3;
			}
		}
		
		Gdiplus::Bitmap bitm(_width, _height, stride, fmt, saveBuffer);
		
		CLSID clsid;
		switch (format) {
			case GdiPlusUtils::kFormatBmp:
				gdiPlusUtils.getEncoder(L"image/bmp", clsid);
				break;
			case GdiPlusUtils::kFormatJpg:
				gdiPlusUtils.getEncoder(L"image/jpeg", clsid);
				break;
			case GdiPlusUtils::kFormatPng:
				gdiPlusUtils.getEncoder(L"image/png", clsid);
				break;
			case GdiPlusUtils::kFormatTga:
				gdiPlusUtils.getEncoder(L"image/tga", clsid);
				break;
		}
		
		std::wstring dstPath(path.begin(), path.end());
		Gdiplus::Status s = bitm.Save(dstPath.c_str(), &clsid);
		if (s==Gdiplus::Ok) {
			_lastStatus = true;
		}
		
		delete[] saveBuffer;
	}
	
}

}
}
