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


#ifndef _bg2e_db_drawable_hpp_
#define _bg2e_db_drawable_hpp_

#include <bg/base/context.hpp>
#include <bg/scene/drawable.hpp>
#include <bg/db/plugin.hpp>

namespace bg {
namespace db {


typedef ReadPlugin<bg::scene::Drawable> ReadDrawablePlugin;
typedef WritePlugin<bg::scene::Drawable> WriteDrawablePlugin;

class BG2E_EXPORT DrawableLoader {
public:
	typedef std::vector<bg::ptr<ReadDrawablePlugin> > ReadPluginVector;
	
	static void RegisterPlugin(ReadDrawablePlugin *);

	DrawableLoader(bg::base::Context *);

	inline bg::scene::Drawable * load(const bg::system::Path & p) { return load(p.text()); }
	bg::scene::Drawable * load(const std::string &);
	
protected:
	bg::ptr<bg::base::Context> _ctx;
	
	static ReadPluginVector s_readPlugins;
	static bool s_destroyCallbackRegistered;
};

/*
 *	Quick loader functions: loads a drawable with default configuration
 */
extern BG2E_EXPORT bg::scene::Drawable * loadDrawable(bg::base::Context *, const bg::system::Path &);
extern BG2E_EXPORT bg::scene::Drawable * loadDrawable(bg::base::Context *, const std::string &);


class BG2E_EXPORT DrawableWriter {
public:
	typedef std::vector<bg::ptr<WriteDrawablePlugin> > WritePluginVector;
	static void RegisterPlugin(WriteDrawablePlugin *);

	DrawableWriter(bg::base::Context *);

	inline void write(const bg::system::Path & p, bg::scene::Drawable * drw) { write(p.text(), drw); }
	void write(const std::string &, bg::scene::Drawable *);

protected:
	bg::ptr<bg::base::Context> _ctx;

	static WritePluginVector s_writePlugins;
	static bool s_destroyCallbackRegistered;
};

extern BG2E_EXPORT void writeDrawable(bg::base::Context *, const bg::system::Path & p, bg::scene::Drawable * drw);
extern BG2E_EXPORT void writeDrawable(bg::base::Context *, const std::string & p, bg::scene::Drawable * drw);

}
}

#endif
