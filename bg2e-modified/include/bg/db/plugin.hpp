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


#ifndef _bg2e_db_plugin_hpp_
#define _bg2e_db_plugin_hpp_

#include <bg/system/path.hpp>
#include <bg/base/referenced_pointer.hpp>
#include <bg/base/context.hpp>
#include <bg/audio/context.hpp>

namespace bg {
namespace db {

class Plugin : public bg::base::ReferencedPointer {
public:
	virtual bool supportFileType(const std::string & extension) = 0;
	inline bool supportFileType(const bg::system::Path & p) { return supportFileType(p.extension()); }
	
	template <class PluginClass>
	static PluginClass * FindPlugin(std::vector<bg::ptr<PluginClass> > & plugins, const std::string & fileType) {
		for (auto item : plugins) {
			if (item->supportFileType(fileType)) {
				return item.getPtr();
			}
		}
		return nullptr;
	}

	static void RegisterDependencies(Plugin * plugin) {
		plugin->registerDependencies();
	}

	virtual void registerDependencies() {}
};

template <class T>
class ReadPlugin : public Plugin {
public:
	virtual T * load(bg::base::Context *, const bg::system::Path &) = 0;
};

template <class T>
class FilePlugin : public Plugin {
public:
	virtual T * load(const bg::system::Path &) = 0;
};

template <class T>
class AudioPlugin : public Plugin {
public:
	virtual T * load(bg::audio::Context *, const bg::system::Path &) = 0;
};

template <class T>
class WritePlugin : public Plugin {
public:
	virtual void save(bg::base::Context *, const bg::system::Path &, T *) = 0;
};

}
}

#endif