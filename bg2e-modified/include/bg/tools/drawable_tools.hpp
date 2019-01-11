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


#ifndef _bg2e_tools_drawable_tools_hpp_
#define _bg2e_tools_drawable_tools_hpp_

#include <bg/scene/drawable.hpp>
#include <bg/tools/tool_base.hpp>

namespace bg {
namespace tools {

class BG2E_EXPORT DrawableTools : public ToolBase<bg::scene::Drawable> {
public:
	DrawableTools(bg::scene::Drawable * drw);

	DrawableTools & clone(const std::string & newName);
	
	DrawableTools & replacePolyList(size_t index, bg::base::PolyList * plist);
	
	DrawableTools & replaceMaterial(size_t index, bg::base::Material * mat);
	
	DrawableTools & replaceTransform(size_t index, const bg::math::Matrix4 & trx);
	
	DrawableTools & setGroupVisible(const std::string & name, bool visibility = true);
	inline DrawableTools & hideGroup(const std::string & name) { setGroupVisible(name, false); return *this; }
	inline DrawableTools & showGroup(const std::string & name) { setGroupVisible(name, true); return *this; }
	
	DrawableTools & setVisibleByName(const std::string & name, bool visibility = true);
	
	inline DrawableTools & showByName(const std::string & name) { setVisibleByName(name, false); return *this; }
	inline DrawableTools & hideByName(const std::string & name) { setVisibleByName(name, true); return *this; }
};

}
}

#endif