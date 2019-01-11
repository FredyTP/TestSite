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

#ifndef _bg2e_db_plugin_bg2_drawable_hpp_
#define _bg2e_db_plugin_bg2_drawable_hpp_

#include <bg/db/drawable.hpp>
#include <bg/db/bg2/bg2_file_reader.hpp>
#include <bg/db/bg2/bg2_file_writer.hpp>

namespace bg {
namespace db {
namespace plugin {

class Bg2ReaderDelegate : public bg::db::bg2::Bg2FileReaderDelegate {
public:
	Bg2ReaderDelegate(bg::base::Context *, const bg::system::Path &);
	
	inline bg::scene::Drawable * drawable() { return _drw.getPtr(); }
	inline bg::scene::Node * node() { return _node.getPtr(); }
	
	virtual void onError(const std::string &);
	virtual void onWarning(const std::string &);
	virtual void fileVersion(const FileVersion &);
	virtual void metadata(FileMetadata, int);
	virtual void metadata(FileMetadata, const std::string &);
	virtual void metadata(FileMetadata, bool);
	virtual void metadata(FileMetadata, float);
	virtual void polyList(base::PolyList *);
	virtual void materials(const std::string &);
	virtual void joint(JointType, const bg::math::Vector3 &, float, float, float);
	
protected:
	bg::system::Path _file;
	bg::ptr<bg::scene::Drawable> _drw;
	bg::ptr<bg::scene::Node> _node;
	bg::ptr<bg::base::Context> _ctx;
};

class BG2E_EXPORT ReadDrawableBg2 : public ReadDrawablePlugin {
public:
	ReadDrawableBg2();
	
	bool supportFileType(const std::string & extension);
	bg::scene::Drawable * load(bg::base::Context *, const bg::system::Path &);
	
protected:
	virtual ~ReadDrawableBg2();
};

class BG2E_EXPORT WriteDrawableBg2 : public WriteDrawablePlugin, public bg::db::bg2::Bg2Writer {
public:
	WriteDrawableBg2();
	
	bool supportFileType(const std::string & extension);
	void save(bg::base::Context *, const bg::system::Path &, bg::scene::Drawable *);
	
protected:
	virtual ~WriteDrawableBg2();
};

}
}
}

#endif
