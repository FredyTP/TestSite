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


#include <bg/tools/bounding_box.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace tools {

BoundingBox::BoundingBox()
	:_dirty(true)
{
	clear();
}

BoundingBox::BoundingBox(bg::base::PolyList * plist, const bg::math::Matrix4 & trx)
	:_dirty(true)
{
	clear();
	addPolyList(plist,trx);
}

BoundingBox::BoundingBox(bg::scene::Drawable * drw, const bg::math::Matrix4 & trx)
	:_dirty(true)
{
	clear();
	addDrawable(drw,trx);
}

void BoundingBox::clear() {
	_dirty = true;
	_min.set(bg::math::number::maxValue(0.0f));
	_max.set(-bg::math::number::maxValue(0.0f));
}

const bg::math::Vector3 & BoundingBox::min() {
	return _min;
}

const bg::math::Vector3 & BoundingBox::max() {
	return _max;
}

const bg::math::Vector3 & BoundingBox::center() {
	if (_dirty) {
		update();
	}
	return _center;
}

const bg::math::Vector3 & BoundingBox::size() {
	if (_dirty) {
		update();
	}
	return _size;
}

const bg::math::Vector3 & BoundingBox::halfSize() {
	if (_dirty) {
		update();
	}
	return _halfSize;
}

void BoundingBox::addPolyList(bg::base::PolyList * polyList, const bg::math::Matrix4 & trx) {
	using namespace bg::base;
	_dirty = true;
	auto & v = polyList->vertexVector();
	for (auto it = v.begin(); it != v.end(); it += 3) {
		bg::math::Vector3 vec(*it, *(it+1), *(it+2));
		vec = trx.multVector(vec).xyz();
		if (vec.x()<_min.x()) _min.x(vec.x());
		if (vec.y()<_min.y()) _min.y(vec.y());
		if (vec.z()<_min.z()) _min.z(vec.z());
		if (vec.x()>_max.x()) _max.x(vec.x());
		if (vec.y()>_max.y()) _max.y(vec.y());
		if (vec.z()>_max.z()) _max.z(vec.z());
	}
}

void BoundingBox::addDrawable(bg::scene::Drawable * drawable, const bg::math::Matrix4 & trx) {
	using namespace bg::scene;
	drawable->forEach([&](Drawable::DrawableElement & elem) {
		auto transform = trx;
		transform.mult(elem.transform);
		addPolyList(elem.polyList.getPtr(), transform);
	});
}

void BoundingBox::update() {
	_size = _max - _min;
	_halfSize = _size;
	_halfSize.scale(0.5f);
	_center = _min + _halfSize;
	_dirty = false;
}

}
}
