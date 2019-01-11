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


#include <bg/db/texture.hpp>
#include <bg/tools/texture_cache.hpp>

#include <iostream>

namespace bg {
namespace db {

TextureLoader::TextureLoader(bg::base::Context * ctx)
	:_ctx(ctx)
	,_useCache(true)
	,_wrapModeU(kWrapModeRepeat)
	,_wrapModeV(kWrapModeRepeat)
	,_wrapModeW(kWrapModeRepeat)
{
	
}

bg::base::Texture * TextureLoader::load(const std::string & path) {
	using namespace bg::base;
	ptr<Texture> tex = _useCache ? loadFromCache(path):nullptr;
	ptr<Image> image = new Image();
	if (!tex.valid() && image->load(path)) {
		tex = new Texture(_ctx.getPtr());
		tex->setWrapModeU(static_cast<Texture::WrapMode>(_wrapModeU));
		tex->setWrapModeV(static_cast<Texture::WrapMode>(_wrapModeV));
		tex->setWrapModeW(static_cast<Texture::WrapMode>(_wrapModeW));
		try {
			tex->createWithImage(image.getPtr());
			
			if (_useCache && tex->valid()) {
				bg::tools::TextureCache::RegisterTexture(tex.getPtr());
			}
		}
		catch (NullParameterException & e) {
			std::cerr << "WARNING - TextureLoader::load() - null parameter exception: " << e.what() << std::endl;
		}
		catch (InvalidStateException & e) {
			std::cerr << "WARNING - TextureLoader::load() - invalid parameter exception: " << e.what() << std::endl;
		}
		
	}
	
	return tex.release();
}

bg::base::Texture * TextureLoader::loadFromCache(const std::string &path) {
	using namespace bg::tools;
	return TextureCache::Get(_ctx.getPtr())->getTexture(path);
}

bg::base::Texture * loadTexture(bg::base::Context * ctx, const bg::system::Path & path) {
	return TextureLoader(ctx).load(path);
}

bg::base::Texture * loadTexture(bg::base::Context * ctx, const std::string & path) {
	return TextureLoader(ctx).load(path);
}

}
}