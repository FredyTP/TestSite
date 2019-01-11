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


#ifndef _bg2e_base_texture_hpp_
#define _bg2e_base_texture_hpp_

#include <bg/base/context_object.hpp>
#include <bg/engine/texture_impl.hpp>
#include <bg/base/image.hpp>

#include <string>

namespace bg {

namespace base {

class BG2E_EXPORT Texture : public ContextObject, public ReferencedPointer {
public:
	enum CubemapFace {
		kFacePositiveX = 0,
		kFaceNegativeX,
		kFacePositiveY,
		kFaceNegativeY,
		kFacePositiveZ,
		kFaceNegativeZ
	};

	enum WrapMode {
		kWrapModeRepeat = 1,
		kWrapModeClamp
	};

	enum ResizeFilter {
		kFilterNearest = 1,
		kFilterLinear,
		kFilterMipmap
	};

	enum Type {
		kTexture2D,
		kCubemap
	};
	
	static const std::string & FileName(Texture * t);
	static const std::string & FileName(ptr<Texture> & t) { return FileName(t.getPtr()); }
    
    static Texture * TextureWithImage(Context * ctx, Image *, WrapMode m = kWrapModeRepeat, ResizeFilter rf = kFilterMipmap);
	
	Texture(Context * ctx);

	inline void setWrapModeU(WrapMode m) { _wrapModeU = m; }
	inline WrapMode wrapModeU() const { return _wrapModeU; }
	inline void setWrapModeV(WrapMode m) { _wrapModeV = m; }
	inline WrapMode wrapModeV() const { return _wrapModeV; }
	inline void setWrapModeW(WrapMode m) { _wrapModeW = m; }
	inline WrapMode wrapModeW() const { return _wrapModeW; }

	inline void setResizeFilter(ResizeFilter rf) { _resizeFilter = rf; }
	inline ResizeFilter resizeFilter() const { return _resizeFilter; }
	
	void createWithImage(Image *);
	void createWithBuffer(const unsigned char * data, int width, int height, int bpp);

	void createCubemap(
		Image * posX, Image * negX,
		Image * posY, Image * negY,
		Image * posZ, Image * negZ
	);
	
	inline void destroy() { _impl->destroy(); _size.set(0); }
	
	inline const bg::math::Size2Di & size() const { return _size; }
	inline int bytesPerPixel() const { return _bpp; }
	

	template <class T>
	T * impl() { return dynamic_cast<T*>(_impl.getPtr()); }

	inline void setFileName(const std::string & fileName) { _fileName = fileName; }
	inline const std::string & fileName() const { return _fileName; }
	
	inline bool valid() const { return _valid; }
	
	static Texture * ColorTexture(bg::base::Context * ctx, const bg::math::Color & c, const bg::math::Size2Di & size);
	static Texture * RandomTexture(bg::base::Context * ctx, const bg::math::Size2Di & size);
	static Texture * ColorCubemap(bg::base::Context * ctx, const bg::math::Color & c, const bg::math::Size2Di & size);
	
	// Use this functions only if you need to create and manage the texture manually:
	inline void setSize(const bg::math::Size2Di & size) { _size = size; }
	inline void setBytesPerPixel(int bpp) { _bpp = bpp; }
	

	void readTexture(unsigned char *& buffer);

	bool saveAsImage(const std::string &);

	void invalidate() { _impl->invalidate(); }
	
protected:
	virtual ~Texture();
	
	ptr<engine::TextureImpl> _impl;
	Type _type;
	std::string _fileName;
	std::vector<std::string> _cubemapFiles{ "", "", "", "", "", "" };
	WrapMode _wrapModeU;
	WrapMode _wrapModeV;
	WrapMode _wrapModeW;
	ResizeFilter _resizeFilter;
	bg::math::Size2Di _size;
	int _bpp = 0;
	
	bool _valid;
	
};

typedef std::vector<bg::ptr<Texture>> TextureVector;

}
}

#endif
