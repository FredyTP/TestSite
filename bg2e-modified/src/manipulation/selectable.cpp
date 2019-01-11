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


#include <bg/manipulation/selectable.hpp>

#include <bg/scene/drawable.hpp>

#include <bg/fx/color_picker_fx.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace manipulation {

bg::scene::ComponentFactory<Selectable> selectableFactory("Selectable");

uint32_t ColorPickId::s_genValue = 0;

void ColorPickId::generateIdentifier() {
	_colorId.uint32 = ++s_genValue;
}


bool Selectable::s_selectMode = false;

ColorPickId Selectable::GetIdentifier() {
	ColorPickId pickId;
	pickId.generateIdentifier();
	return pickId;
}

Selectable::Selectable()
{
	
}

Selectable::~Selectable() {
	
}

bg::scene::Component * Selectable::clone() {
	return new Selectable();
}

void Selectable::init() {
	using namespace bg::scene;
	_selectablePlist.clear();
	
	if (drawable()) {
		drawable()->forEach([&](Drawable::DrawableElement & elem) {
			_selectablePlist.push_back(new SelectableItem(GetIdentifier(),
													elem.polyList.getPtr(),
													elem.material.getPtr(),
													drawable(),
													node(),
													SelectableItem::kPlist,
													elem.transform));
		});
	}
}
	
void Selectable::draw(bg::base::Pipeline * pipeline) {
	bg::fx::ColorPickerEffect * effect = dynamic_cast<bg::fx::ColorPickerEffect*>(pipeline->effect());
	if (effect) {
		for (auto item : _selectablePlist) {
			effect->matrixState().modelMatrixStack().push();
			effect->matrixState().modelMatrixStack().mult(item->trx());
			
			effect->setColorPickId(item->id());
			bg::base::Pipeline::OpacityLayer layer = pipeline->opacityLayer();
			pipeline->setOpacityLayer(bg::base::Pipeline::kOpacityAll);
			pipeline->draw(item->polyList());
			pipeline->setOpacityLayer(layer);
			
			effect->matrixState().modelMatrixStack().pop();
		}
	}
}

SelectableItem * Selectable::findId(const bg::manipulation::ColorPickId &id) {
	bg::ptr<bg::base::PolyList> result;
	for (auto pl : _selectablePlist) {
		if (pl->id()==id) {
			result = pl->polyList();
			return pl.getPtr();
		}
	}
	return nullptr;
}

}
}
