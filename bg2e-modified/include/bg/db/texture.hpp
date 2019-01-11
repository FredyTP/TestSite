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


#ifndef _bg2e_db_texture_hpp_
#define _bg2e_db_texture_hpp_


#include <bg/base/texture.hpp>
#include <bg/system/path.hpp>

#include <string>

namespace bg {
namespace db {

class BG2E_EXPORT TextureLoader {
public:
	enum WrapMode {
		kWrapModeRepeat = bg::base::Texture::kWrapModeRepeat,
		kWrapModeClamp = bg::base::Texture::kWrapModeClamp
	};
	
	TextureLoader(bg::base::Context *);
	
	inline void setUseCache(bool c) { _useCache = c; }
	inline bool useCache() const { return _useCache; }
	inline void setWrapMode(WrapMode m) { _wrapModeU = m; _wrapModeV = m; _wrapModeW = m; }
	inline void setWrapModeU(WrapMode m) { _wrapModeU = m; }
	inline void setWrapModeV(WrapMode m) { _wrapModeV = m; }
	inline void setWrapModeW(WrapMode m) { _wrapModeW = m; }
	
	inline bg::base::Texture * load(const bg::system::Path & path) { return load(path.text()); }
	bg::base::Texture * load(const std::string & path);
	inline bg::base::Texture * loadFromCache(const bg::system::Path & path) { return loadFromCache(path.text()); }
	bg::base::Texture * loadFromCache(const std::string & path);
	
protected:
	bg::ptr<bg::base::Context> _ctx;
	
	bool _useCache;
	WrapMode _wrapModeU;
	WrapMode _wrapModeV;
	WrapMode _wrapModeW;
};

/*
 *	Quick loader functions: loads a texture, using cache, with the default options defined in TextureLoader
 */
extern BG2E_EXPORT bg::base::Texture * loadTexture(bg::base::Context *, const bg::system::Path & path);
extern BG2E_EXPORT bg::base::Texture * loadTexture(bg::base::Context *, const std::string & path);

}
}

#endif