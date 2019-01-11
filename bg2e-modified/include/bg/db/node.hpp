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


#ifndef _bg2e_db_node_hpp_
#define _bg2e_db_node_hpp_

#include <bg/scene/node.hpp>
#include <bg/db/plugin.hpp>

namespace bg {
namespace db {
	
typedef ReadPlugin<bg::scene::Node> ReadNodePlugin;
typedef WritePlugin<bg::scene::Node> WriteNodePlugin;

class BG2E_EXPORT NodeLoader {
public:
	typedef std::vector<bg::ptr<ReadNodePlugin> > ReadPluginVector;

	static void RegisterPlugin(ReadNodePlugin *);
		
	NodeLoader(bg::base::Context *);
	
	inline bg::scene::Node * loadScene(const bg::system::Path & p) { return loadScene(p.text()); }
	bg::scene::Node * loadScene(const std::string &);
	
	inline bg::scene::Node * loadPrefab(const bg::system::Path & p) { return loadPrefab(p.text()); }
	bg::scene::Node * loadPrefab(const std::string &);
	
protected:
	bg::ptr<bg::base::Context> _ctx;
	
	static ReadPluginVector s_readPlugins;
	static bool s_destroyCallbackRegistered;
};

/*
 *	Quick loader functions: loads a prefabs and scenes with default configuration
 */
extern BG2E_EXPORT bg::scene::Node * loadScene(bg::base::Context *, const bg::system::Path &);
extern BG2E_EXPORT bg::scene::Node * loadScene(bg::base::Context *, const std::string &);
extern BG2E_EXPORT bg::scene::Node * loadPrefab(bg::base::Context *, const bg::system::Path &);
extern BG2E_EXPORT bg::scene::Node * loadPrefab(bg::base::Context *, const std::string &);

class BG2E_EXPORT NodeWriter {
public:
	typedef std::vector<bg::ptr<WriteNodePlugin> > WritePluginVector;

	static void RegisterPlugin(WriteNodePlugin *);

	NodeWriter(bg::base::Context *);

	inline void writeScene(bg::scene::Node * node, const std::string & path) { writeScene(node, bg::system::Path(path)); }
	void writeScene(bg::scene::Node *, const bg::system::Path &);
	
protected:
	bg::ptr<bg::base::Context> _ctx;
	
	static WritePluginVector s_writePlugins;
	static bool s_destroyCallbackRegistered;	
};

/*
 *	Quick save
 */
extern BG2E_EXPORT void writeScene(bg::base::Context *, const std::string & path, bg::scene::Node * node);
extern BG2E_EXPORT void writeScene(bg::base::Context *, const bg::system::Path & path, bg::scene::Node * node);

}
}

#endif
