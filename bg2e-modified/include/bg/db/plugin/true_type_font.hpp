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

#ifndef _bg2e_db_plugin_true_type_font_hpp_
#define _bg2e_db_plugin_true_type_font_hpp_

#include <bg/db/font.hpp>

#include <bg/system/utf.hpp>

namespace bg {
namespace db {
namespace plugin {


class BG2E_EXPORT ReadFontTrueType : public ReadFontPlugin {
public:
	ReadFontTrueType();

	virtual bool supportFileType(const std::string & extension);

	virtual bg::text::Font * load(bg::base::Context *, const bg::system::Path &, float);

protected:
	virtual ~ReadFontTrueType();

	bg::text::BitmapGlyph * getGlyph(bg::base::Context * context, void * font, float scale, const std::string & mbChar, bg::system::UTFCode codepoint);
};

}
}
}


#endif