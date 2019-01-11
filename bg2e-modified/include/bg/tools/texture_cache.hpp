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

#ifndef _bg2e_tools_texture_cache_hpp_
#define _bg2e_tools_texture_cache_hpp_

#include <bg/base/texture.hpp>
#include <bg/base/exception.hpp>

#include <unordered_map>

namespace bg {
namespace tools {

typedef std::unordered_map<std::string, bg::ptr<bg::base::Texture> > TextureMap;

class BG2E_EXPORT TextureCache : public bg::base::ContextObject, public bg::base::ReferencedPointer {
public:
	/*
	 *	Gets the texture cache object, associated with the context
	 *	Throws bg::base::NullParameterException if the context is null
	 */
	static TextureCache * Get(bg::base::Context *);
	
	/*
	 *	Register a texture using a name, in its associated context. Throws bg::base::NullParameterException
	 *	if the texture is null
	 */
	static void RegisterTexture(const std::string & name, bg::base::Texture * tex);
	
	/*
	 *	Register a texture using the texture file name, in its associated context.
	 *	Throws bg::base::InvalidParameterException if the texture have an empty file name
	 *	Throws bg::base::NullParameterException if the texture is null
	 */
	static void RegisterTexture(bg::base::Texture * tex);
	
	/*
	 *	Returns the texture associated with the key "text", or nullptr if there isn't a
	 *	texture registered with this key.
	 *	Throws bg::base::NullParameterException if the context is null
	 */
	static bg::base::Texture * GetTexture(bg::base::Context *, const std::string & text);
	
	/*
	 *	Remove a texture from the register. Returns fase if the texture is not found
	 *	Throws bg::base::NullParameterException if the context is null
	 */
	static bool UnregisterTexture(bg::base::Context *, const std::string & name);
	
	/*
	 *	Removes a texture from the register, using the texture file name. Returns false if
	 *	the texture is not found
	 *	Throws a bg::base::InvalidParameterException if the texture have an empty file name
	 *	Throws bg::base::NullParameterException if the texture is null
	 */
	static bool UnregisterTexture(bg::base::Texture * tex);
	
	static bg::base::Texture * WhiteTexture(bg::base::Context * ctx);
	static bg::base::Texture * BlackTexture(bg::base::Context * ctx);
	static bg::base::Texture * NormalTexture(bg::base::Context * ctx);
	static bg::base::Texture * RandomTexture(bg::base::Context * ctx);
	static bg::base::Texture * TransparentTexture(bg::base::Context * ctx);
	static bg::base::Texture * WhiteCubemap(bg::base::Context * ctx);
	static bg::base::Texture * BlackCubemap(bg::base::Context * ctx);
	

	// Non-static versions of the previous functions
	void registerTexture(const std::string & name, bg::base::Texture * tex);
	void registerTexture(bg::base::Texture * tex);
	bg::base::Texture * getTexture(const std::string & text);
	bool unregisterTexture(const std::string & name);
	bool unregisterTexture(bg::base::Texture * tex);
	
	bg::base::Texture * whiteTexture();
	bg::base::Texture * blackTexture();
	bg::base::Texture * normalTexture();
	bg::base::Texture * randomTexture();
	bg::base::Texture * transparentTexture();
	bg::base::Texture * whiteCubemap();
	bg::base::Texture * blackCubemap();
	
	
protected:
	TextureCache(bg::base::Context *);
	virtual ~TextureCache();
	
	static bg::base::ContextObjectRegistry<TextureCache> s_registry;
	static bool s_destroyCallbackRegistered;
	
	TextureMap _textureMap;
};

}
}

#endif
