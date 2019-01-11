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


#include <bg/db/font.hpp>
#include <bg/engine.hpp>

#include <iostream>

namespace bg {
namespace db {

FontLoader::ReadPluginVector FontLoader::s_readPlugins;
bool FontLoader::s_destroyCallbackRegistered = false;

void FontLoader::RegisterPlugin(ReadFontPlugin * p) {
	auto it = std::find(s_readPlugins.begin(), s_readPlugins.end(), p);
	if(it == s_readPlugins.end()) {
		s_readPlugins.push_back(p);
		bg::db::Plugin::RegisterDependencies(p);
	}
	if(!s_destroyCallbackRegistered) {
		s_destroyCallbackRegistered = true;
		bg::Engine::AddDestroyCallback([&]() {
			s_readPlugins.clear();
		});
	}
}

FontLoader::FontLoader(bg::base::Context * ctx)
	:_ctx(ctx)
{

}

bg::text::Font * FontLoader::load(const std::string & path, float size) {
	bg::system::Path p(path);
	ReadFontPlugin * plugin = Plugin::FindPlugin<ReadFontPlugin>(s_readPlugins, p.extension());
	if(plugin) {
		bg::ptr<bg::text::Font> buffer = plugin->load(_ctx.getPtr(), p, size);
		return buffer.release();
	}
	return nullptr;
}

bg::text::Font * loadFont(bg::base::Context * ctx, const bg::system::Path & path, float size) {
	return FontLoader(ctx).load(path, size);
}

bg::text::Font * loadFont(bg::base::Context * ctx, const std::string & path, float size) {
	return FontLoader(ctx).load(path, size);
}

}
}
