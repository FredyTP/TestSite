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

#ifndef _bg2e_db_bg2_skin_plugin_hpp_
#define _bg2e_db_bg2_skin_plugin_hpp_


#include <bg/db/ui_skin.hpp>
#include <bg/db/json/value.hpp>

namespace bg {
namespace db {
namespace plugin {

class BG2E_EXPORT ReadGuiSkinBg2 : public ReadSkinPlugin {
public:
	ReadGuiSkinBg2();

	virtual bool supportFileType(const std::string & extension);

	virtual bg::gui::Skin * load(bg::base::Context *, const bg::system::Path &);

	virtual void registerDependencies();

protected:
	virtual ~ReadGuiSkinBg2();

	void parseWidgetProperties(bg::base::Context *, bg::gui::Skin *, bg::db::json::Value * doc, bg::gui::Skin::WidgetType, const bg::system::Path &);
	void parseStatusProperties(bg::base::Context *, bg::gui::Skin *, bg::db::json::Value * doc, bg::gui::Skin::WidgetType widgetType, bg::gui::Status status, const bg::system::Path &);
};

}
}
}

#endif
