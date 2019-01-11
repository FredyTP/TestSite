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


#include <bg/db/node.hpp>

#include <bg/scene/camera.hpp>

#include <bg/engine.hpp>

namespace bg {
namespace db {

NodeLoader::ReadPluginVector NodeLoader::s_readPlugins;
bool NodeLoader::s_destroyCallbackRegistered = false;
	
void NodeLoader::RegisterPlugin(ReadNodePlugin * p) {
	auto it = std::find(s_readPlugins.begin(), s_readPlugins.end(), p);
	if (it==s_readPlugins.end()) {
		s_readPlugins.push_back(p);
		bg::db::Plugin::RegisterDependencies(p);
	}
	if (!s_destroyCallbackRegistered) {
		s_destroyCallbackRegistered = true;
		bg::Engine::AddDestroyCallback([&]() {
			s_readPlugins.clear();
		});
	}
}


	
NodeLoader::NodeLoader(bg::base::Context * ctx)
	:_ctx(ctx)
{
	
}
	
bg::scene::Node * NodeLoader::loadScene(const std::string & path) {
	bg::system::Path p(path);
	ReadNodePlugin * plugin = Plugin::FindPlugin<ReadNodePlugin>(s_readPlugins, p.extension());
	if (plugin) {
		return plugin->load(_ctx.getPtr(), path);
	}
	else {
		return nullptr;
	}
}
	
bg::scene::Node * NodeLoader::loadPrefab(const std::string & path) {
	return loadScene(path);
}
	
bg::scene::Node * loadScene(bg::base::Context * ctx, const bg::system::Path & path) {
	return NodeLoader(ctx).loadScene(path);
}

bg::scene::Node * loadScene(bg::base::Context * ctx, const std::string & path) {
	return NodeLoader(ctx).loadScene(path);
}

bg::scene::Node * loadPrefab(bg::base::Context * ctx, const bg::system::Path & path) {
	return NodeLoader(ctx).loadPrefab(path);
}

bg::scene::Node * loadPrefab(bg::base::Context * ctx, const std::string & path) {
	return NodeLoader(ctx).loadPrefab(path);
}


NodeWriter::WritePluginVector NodeWriter::s_writePlugins;
bool NodeWriter::s_destroyCallbackRegistered = false;

void NodeWriter::RegisterPlugin(WriteNodePlugin * p) {
	auto it = std::find(s_writePlugins.begin(), s_writePlugins.end(), p);
	if(it == s_writePlugins.end()) {
		s_writePlugins.push_back(p);
		bg::db::Plugin::RegisterDependencies(p);
	}
	if (!s_destroyCallbackRegistered) {
		s_destroyCallbackRegistered = true;
		bg::Engine::AddDestroyCallback([&]() {
			s_writePlugins.clear();
		});
	}
}

NodeWriter::NodeWriter(bg::base::Context * ctx)
	:_ctx(ctx)
{
}

void NodeWriter::writeScene(bg::scene::Node * node, const bg::system::Path & path) {
	WriteNodePlugin * plugin = Plugin::FindPlugin<WriteNodePlugin>(s_writePlugins, path.extension());
	if (plugin) {
		plugin->save(_ctx.getPtr(), path, node);
	}
}


void writeScene(bg::base::Context * ctx, const std::string & path, bg::scene::Node * node) {
	NodeWriter(ctx).writeScene(node, path);
}

void writeScene(bg::base::Context * ctx, const bg::system::Path & path, bg::scene::Node * node) {
	NodeWriter(ctx).writeScene(node, path);
}

}
}
