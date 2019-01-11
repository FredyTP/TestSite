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


#ifndef _bg2e_db_font_hpp_
#define _bg2e_db_font_hpp_

#include <bg/db/plugin.hpp>
#include <bg/text/font.hpp>

namespace bg {
namespace db {


// 2D (texture) fonts
template <class T>
class ReadFontPluginGeneric : public Plugin {
public:
	virtual T * load(bg::base::Context *, const bg::system::Path &, float size) = 0;
};

typedef ReadFontPluginGeneric<bg::text::Font> ReadFontPlugin;
typedef WritePlugin<bg::text::Font> WriteFontPlugin;

class BG2E_EXPORT FontLoader {
public:
	typedef std::vector<bg::ptr<ReadFontPlugin> > ReadPluginVector;

	static void RegisterPlugin(ReadFontPlugin *);

	FontLoader(bg::base::Context *);

	inline bg::text::Font * load(const bg::system::Path & p, float size) { return load(p.text(), size); }
	bg::text::Font * load(const std::string &, float size);

protected:
	bg::ptr<bg::base::Context> _ctx;

	static ReadPluginVector s_readPlugins;
	static bool s_destroyCallbackRegistered;
};

/*
*	Quick loader functions: loads a drawable with default configuration
*/
extern BG2E_EXPORT bg::text::Font * loadFont(bg::base::Context *, const bg::system::Path &, float size);
extern BG2E_EXPORT bg::text::Font * loadFont(bg::base::Context *, const std::string &, float size);

// TODO: 3D fonts

}
}

#endif
