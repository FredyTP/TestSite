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


#include <bg/tools/drawable_tools.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace tools {

DrawableTools::DrawableTools(bg::scene::Drawable * drw)
	:ToolBase(drw)
{
}

DrawableTools & DrawableTools::clone(const std::string & newName) {
	_target = dynamic_cast<bg::scene::Drawable*>(_target->clone());
	_target->setName(newName);
	return *this;
}

DrawableTools & DrawableTools::replacePolyList(size_t index, bg::base::PolyList * plist) {
	if (index<_target->drawableElements().size()) {
		_target->drawableElements()[index].polyList = plist;
	}
	return *this;
}

DrawableTools & DrawableTools::replaceMaterial(size_t index, bg::base::Material * mat) {
	if (index<_target->drawableElements().size()) {
		_target->drawableElements()[index].material = mat;
	}
	return *this;
}

DrawableTools & DrawableTools::replaceTransform(size_t index, const bg::math::Matrix4 & trx) {
	if (index<_target->drawableElements().size()) {
		_target->drawableElements()[index].transform = trx;
	}
	return *this;
 }

DrawableTools & DrawableTools::setGroupVisible(const std::string & name, bool visibility) {
	_target->forEach([&](scene::Drawable::DrawableElement & e) {
		if (e.polyList->groupName()==name) {
			e.polyList->setVisible(visibility);
		}
	});
	return *this;
}

DrawableTools & DrawableTools::setVisibleByName(const std::string & name, bool visibility) {
	_target->some([&](scene::Drawable::DrawableElement & e) {
		if (e.polyList->name()==name) {
			e.polyList->setVisible(visibility);
			return true;
		}
		return false;
	});
	return *this;
}


}
}
