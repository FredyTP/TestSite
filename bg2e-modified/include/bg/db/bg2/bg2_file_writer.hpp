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


#ifndef _bg2e_db_bg2_bg2_file_writer_hpp_
#define _bg2e_db_bg2_bg2_file_writer_hpp_

#include <bg/base/vwglb-utils.hpp>
#include <bg/base/context_object.hpp>
#include <bg/system/path.hpp>
#include <bg/scene/drawable.hpp>
#include <bg/db/json/value.hpp>

namespace bg {
namespace db {
namespace bg2 {

class BG2E_EXPORT Bg2Writer : public base::ContextObject {
public:
	Bg2Writer() :base::ContextObject(nullptr) {}

protected:

	bool writeData(const std::string & path, bg::scene::Drawable * model);

	void writeHeader(bg::scene::Drawable * drawable);
	void ensurePolyListNames(bg::scene::Drawable * drawable);
	void writeMaterial(bg::scene::Drawable * drawable, bg::scene::Node * node = nullptr);
	void writeJoints(bg::scene::Node * node);
	void writeComponents(bg::scene::Node * node, const std::string & path);
	void writePolyList(bg::base::PolyList * plist);
	void writeTexture(bg::db::json::Value *, const std::string & key, bg::base::Texture * tex);
	void saveTextureFile(const system::Path & imagePath);

	bg::base::VwglbUtils _fileUtils;
	bg::system::Path _dstPath;
};


}
}
}

#endif
