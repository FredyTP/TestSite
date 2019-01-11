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


#include <bg/db/drawable.hpp>

#include <bg/scene/camera.hpp>

#include <bg/engine.hpp>

namespace bg {
namespace db {


DrawableLoader::ReadPluginVector DrawableLoader::s_readPlugins;
bool DrawableLoader::s_destroyCallbackRegistered = false;

void DrawableLoader::RegisterPlugin(ReadDrawablePlugin * p) {
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
	
DrawableLoader::DrawableLoader(bg::base::Context * ctx)
	:_ctx(ctx)
{
}
	
bg::scene::Drawable * DrawableLoader::load(const std::string & path) {
	bg::system::Path p(path);
	ReadDrawablePlugin * plugin = Plugin::FindPlugin<ReadDrawablePlugin>(s_readPlugins, p.extension());
	if (plugin) {
		bg::ptr<bg::scene::Drawable> drw = plugin->load(_ctx.getPtr(), path);
		return drw.release();
	}
	else {
		return nullptr;
	}
}

bg::scene::Drawable * loadDrawable(bg::base::Context * ctx, const bg::system::Path & path) {
	return DrawableLoader(ctx).load(path);
}

bg::scene::Drawable * loadDrawable(bg::base::Context * ctx, const std::string & path) {
	return DrawableLoader(ctx).load(path);
}


DrawableWriter::WritePluginVector DrawableWriter::s_writePlugins;
bool DrawableWriter::s_destroyCallbackRegistered = false;

void DrawableWriter::RegisterPlugin(WriteDrawablePlugin * p) {
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

DrawableWriter::DrawableWriter(bg::base::Context * ctx)
	:_ctx(ctx)
{
}

void DrawableWriter::write(const std::string & path, bg::scene::Drawable * drw) {
	bg::system::Path p(path);
	WriteDrawablePlugin * plugin = Plugin::FindPlugin<WriteDrawablePlugin>(s_writePlugins, p.extension());
	if (plugin) {
		plugin->save(_ctx.getPtr(), path, drw);
	}
	else {
		throw bg::base::NoSuchPluginException();
	}
}

void writeDrawable(bg::base::Context * ctx, const bg::system::Path & p, bg::scene::Drawable * drw) {
	DrawableWriter(ctx).write(p, drw);
}

void writeDrawable(bg::base::Context * ctx, const std::string & p, bg::scene::Drawable * drw) {
	DrawableWriter(ctx).write(p, drw);
}

}
}
