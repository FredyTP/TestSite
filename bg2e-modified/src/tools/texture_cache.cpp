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


#include <bg/tools/texture_cache.hpp>
#include <bg/base/exception.hpp>

#include <bg/engine.hpp>
#include <bg/engine/openglCore/texture_impl.hpp>

namespace bg {
namespace tools {
	
bg::base::ContextObjectRegistry<TextureCache> TextureCache::s_registry;
bool TextureCache::s_destroyCallbackRegistered = false;
	
TextureCache * TextureCache::Get(bg::base::Context * ctx) {
	if (ctx==nullptr) throw bg::base::NullParameterException("Invalid parameter: trying to get TextureCache of null context.");
	if (!s_registry.find(ctx)) {
		s_registry.set(new TextureCache(ctx));
	}
	if (!s_destroyCallbackRegistered) {
		s_destroyCallbackRegistered = true;
		bg::Engine::AddDestroyCallback([&]() {
			s_registry.clear();
		});
	}
	return s_registry.get(ctx);
}

void TextureCache::RegisterTexture(const std::string & name, bg::base::Texture * tex) {
	Get(bg::base::ContextObject::GetContext(tex))->registerTexture(name,tex);
}
	
void TextureCache::RegisterTexture(bg::base::Texture * tex) {
	Get(bg::base::ContextObject::GetContext(tex))->registerTexture(tex);
}
	
bg::base::Texture * TextureCache::GetTexture(bg::base::Context * ctx, const std::string & text) {
    return Get(ctx)->getTexture(text);
}
	
bool TextureCache::UnregisterTexture(bg::base::Context * ctx, const std::string & name) {
	return Get(ctx)->unregisterTexture(name);
}
	
bool TextureCache::UnregisterTexture(bg::base::Texture * tex) {
	if (!tex) throw bg::base::NullParameterException("Invalid parameter: trying to unregister a null texture.");
	return Get(tex->context())->unregisterTexture(tex);
}

bg::base::Texture * TextureCache::WhiteTexture(bg::base::Context * ctx) {
	return TextureCache::Get(ctx)->whiteTexture();
}

bg::base::Texture * TextureCache::BlackTexture(bg::base::Context * ctx) {
	return TextureCache::Get(ctx)->blackTexture();
}

bg::base::Texture * TextureCache::NormalTexture(bg::base::Context * ctx) {
	return TextureCache::Get(ctx)->normalTexture();
}

bg::base::Texture * TextureCache::RandomTexture(bg::base::Context * ctx) {
	return TextureCache::Get(ctx)->randomTexture();
}

bg::base::Texture * TextureCache::TransparentTexture(bg::base::Context * ctx) {
	return TextureCache::Get(ctx)->transparentTexture();
}

bg::base::Texture * TextureCache::WhiteCubemap(bg::base::Context * ctx) {
	return TextureCache::Get(ctx)->whiteCubemap();
}

bg::base::Texture * TextureCache::BlackCubemap(bg::base::Context * ctx) {
	return TextureCache::Get(ctx)->blackCubemap();
}

TextureCache::TextureCache(bg::base::Context * ctx)
	:bg::base::ContextObject(ctx)
{
	
}

TextureCache::~TextureCache() {
	
}

void TextureCache::registerTexture(const std::string & name, bg::base::Texture * tex) {
	if (name.empty()) {
		throw bg::base::InvalidParameterException("Invalid parameter: trying to register a texture using an empty key.");
	}
	else if (!tex) {
		throw bg::base::InvalidParameterException("Invalid parameter: trying to register a null texture.");
	}
	
	_textureMap[name] = tex;
}

void TextureCache::registerTexture(bg::base::Texture * tex) {
	std::string key = bg::base::Texture::FileName(tex);
	if (key.empty()) {
		throw bg::base::InvalidParameterException("Invalid parameter: trying to register a texture using an empty file name as key.");
	}
	else if (!tex) {
		throw bg::base::InvalidParameterException("Invalid parameter: trying to register a null texture.");
	}
	
	_textureMap[key] = tex;
}

bg::base::Texture * TextureCache::getTexture(const std::string & text) {
    if (_textureMap.find(text)==_textureMap.end()) {
        return nullptr;
    }
    else {
        return _textureMap[text].getPtr();
    }
}

bool TextureCache::unregisterTexture(const std::string & name) {
	if (name.empty()) throw bg::base::NullParameterException("Invalid parameter: trying to unregister a texture with an empty key");
	TextureMap::iterator it = _textureMap.find(name);
	if (it!=_textureMap.end()) {
		_textureMap.erase(it);
		return true;
	}
	return false;
}

bool TextureCache::unregisterTexture(bg::base::Texture * tex) {
	if (!tex) throw bg::base::NullParameterException("Invalid parameter: trying to unregister a null texture.");
	else if (tex->fileName().empty()) throw bg::base::NullParameterException("Invalid parameter: trying to unregister a texture using its empty file name.");
	return unregisterTexture(tex);
}

bg::base::Texture * TextureCache::whiteTexture() {
	const char * texture_name = "_white_texture_";
	bg::ptr<bg::base::Texture> result = getTexture(texture_name);
	if (!result.valid()) {
		result = bg::base::Texture::ColorTexture(context(), bg::math::Color::White(), bg::math::Size2Di(8));
		registerTexture(texture_name, result.getPtr());
	}
	return result.release();
}

bg::base::Texture * TextureCache::blackTexture() {
	const char * texture_name = "_black_texture";
	bg::ptr<bg::base::Texture> result = getTexture(texture_name);
	if (!result.valid()) {
		result = bg::base::Texture::ColorTexture(context(), bg::math::Color::Black(), bg::math::Size2Di(8));
		registerTexture(texture_name, result.getPtr());
	}
	return result.release();
}

bg::base::Texture * TextureCache::normalTexture() {
	const char * texture_name = "_normal_texture_";
	bg::ptr<bg::base::Texture> result = getTexture(texture_name);
	if (!result.valid()) {
		result = bg::base::Texture::ColorTexture(context(), bg::math::Color(0.5f, 0.5f, 1.0f, 1.0f), bg::math::Size2Di(8));
		registerTexture(texture_name, result.getPtr());
	}
	return result.release();
}

bg::base::Texture * TextureCache::randomTexture() {
	const char * texture_name = "_random_texture_";
	bg::ptr<bg::base::Texture> result = getTexture(texture_name);
	if (!result.valid()) {
		result = bg::base::Texture::RandomTexture(context(), bg::math::Size2Di(1024));
		registerTexture(texture_name, result.getPtr());
	}
	return result.release();
}

bg::base::Texture * TextureCache::transparentTexture() {
	const char * texture_name = "_transparent_texture";
	bg::ptr<bg::base::Texture> result = getTexture(texture_name);
	if (!result.valid()) {
		result = bg::base::Texture::ColorTexture(context(), bg::math::Color::Transparent(), bg::math::Size2Di(8));
		registerTexture(texture_name, result.getPtr());
	}
	return result.release();
}

bg::base::Texture * TextureCache::whiteCubemap() {
	const char * texture_name = "_white_cubemap";
	bg::ptr<bg::base::Texture> result = getTexture(texture_name);
	if (result.isNull()) {
		result = bg::base::Texture::ColorCubemap(context(), bg::math::Color::White(), bg::math::Size2Di(8));
		registerTexture(texture_name, result.getPtr());
	}
	return result.release();
}

bg::base::Texture * TextureCache::blackCubemap() {
	const char * texture_name = "_black_cubemap";
	bg::ptr<bg::base::Texture> result = getTexture(texture_name);
	if (result.isNull()) {
		result = bg::base::Texture::ColorCubemap(context(), bg::math::Color::Black(), bg::math::Size2Di(8));
		registerTexture(texture_name, result.getPtr());
	}
	return result.release();
}

}
}
