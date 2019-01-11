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

#ifndef _bg2e_db_plugin_bg2_node_hpp_
#define _bg2e_db_plugin_bg2_node_hpp_

#include <bg/db/node.hpp>

namespace bg {
namespace db {
namespace plugin {

class BG2E_EXPORT ReadScene : public ReadNodePlugin {
public:
	ReadScene();

	bool supportFileType(const std::string & extension);
	bg::scene::Node * load(bg::base::Context *, const bg::system::Path &);
	
	virtual void registerDependencies();

protected:
	virtual ~ReadScene();
};

class BG2E_EXPORT WriteScene : public WriteNodePlugin {
public:
	WriteScene();
	
	bool supportFileType(const std::string & extension);
	void save(bg::base::Context *, const bg::system::Path &, bg::scene::Node *);
	
	virtual void registerDependencies();

protected:
	virtual ~WriteScene();
};

}
}
}

#endif
