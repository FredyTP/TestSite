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


#ifndef _bg2e_db_ui_skin_hpp_
#define _bg2e_db_ui_skin_hpp_


#include <bg/gui/skin.hpp>
#include <bg/system/path.hpp>
#include <bg/db/plugin.hpp>

#include <string>

namespace bg {
namespace db {

typedef ReadPlugin<bg::gui::Skin> ReadSkinPlugin;

class BG2E_EXPORT GuiSkinLoader {
public:
	typedef std::vector<bg::ptr<ReadSkinPlugin> > ReadPluginVector;

	static void RegisterPlugin(ReadSkinPlugin *);

	GuiSkinLoader(bg::base::Context *);

	inline bg::gui::Skin * load(const bg::system::Path & path) { return load(path.text()); }
	bg::gui::Skin * load(const std::string & path);

protected:
	bg::ptr<bg::base::Context> _ctx;

	static ReadPluginVector s_readPlugins;
	static bool s_destroyCallbackRegistered;
};

/*
*	Quick loader functions
*/
extern BG2E_EXPORT bg::gui::Skin * loadSkin(bg::base::Context *, const bg::system::Path & path);
extern BG2E_EXPORT bg::gui::Skin * loadSkin(bg::base::Context *, const std::string & path);

}
}

#endif