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


#include <bg/scene/drawable.hpp>
#include <bg/scene/node.hpp>
#include <bg/system/system.hpp>

#include <bg/db/drawable.hpp>
#include <bg/scene/camera.hpp>

namespace bg {
namespace scene {

ComponentFactory<Drawable> drawableFactory("Drawable");

bg::math::Matrix4 Drawable::s_transformIdentity;

Node * Drawable::InstanceNode(Node * node) {
	ptr<Node> newNode = new Node(node->context(), node->name().empty() ? "" : "copy of " + node->name());
	newNode->setEnabled(node->enabled());
	node->eachComponent([&](Component * comp) {
		Component * newComponent = nullptr;
		Drawable * drw = dynamic_cast<Drawable*>(comp);
		if (drw) {
			newComponent = comp->clone();
		}
		else {
			newComponent = drw->instance(drw->name().empty() ? "" : "copy of " + drw->name());
		}
		newNode->addComponent(newComponent);
	});
	return newNode.release();
}

Drawable::Drawable() {
}

Drawable::Drawable(const std::string & name)
	:_name(name)
{
}

Drawable::~Drawable() {
	
}

Component * Drawable::clone() {
	ptr<Drawable> newInstance = new Drawable(name());
	forEach([&](DrawableElement & elem) {
		bg::base::Context * ctx = elem.polyList->context();
		newInstance->addPolyList(new bg::base::PolyList(ctx,elem.polyList.getPtr()),
								 new bg::base::Material(elem.material.getPtr()),
								 elem.transform);
	});
	
	return newInstance.release();
}

Drawable * Drawable::instance(const std::string & newName) {
	std::string name = newName;
	if (name.empty()) {
		name = "Instance of " + _name;
	}
	ptr<Drawable> newInstance = new Drawable(name);
	forEach([&](DrawableElement & elem) {
		newInstance->addPolyList(elem.polyList.getPtr(),
								 new bg::base::Material(elem.material.getPtr()),
								 elem.transform);
	});
	
	return newInstance.release();
}

void Drawable::addPolyList(bg::base::PolyList * plist, bg::base::Material * mat, const bg::math::Matrix4 & trx) {
	_drawableElements.push_back(DrawableElement{ plist, mat ? mat : new bg::base::Material(), trx });
}

void Drawable::removePolyList(bg::base::PolyList * plist) {
	auto it = std::begin(_drawableElements);
	for (auto item : _drawableElements) {
		if (item.polyList.getPtr()==plist) {
			break;
		}
		++it;
	}
	if (it!=_drawableElements.end()) {
		_drawableElements.erase(it);
	}
}

size_t Drawable::indexOf(bg::base::PolyList * plist) const {
	size_t index = 0;
	for (auto item : _drawableElements) {
		if (item.polyList.getPtr()==plist) {
			break;
		}
		++index;
	}
	return 0;
}

Drawable::DrawableElementVector::iterator Drawable::element(size_t index) {
	if (index<_drawableElements.size()) {
		return _drawableElements.begin() + index;
	}
	return _drawableElements.end();
}

const Drawable::DrawableElementVector::const_iterator Drawable::element(size_t index) const {
	if (index<_drawableElements.size()) {
		return _drawableElements.cbegin() + index;
	}
	return _drawableElements.cend();
}

void Drawable::draw(bg::base::Pipeline * pipeline) {
	if (pipeline->opacityLayer()==bg::base::Pipeline::kOpacityNone) {
		return;
	}
	else {
		for (auto & elem : _drawableElements) {
			if (elem.polyList->visible()) {
				bg::ptr<bg::base::Material> currentMaterial = pipeline->effect()->materialPtr();
				bg::base::MatrixState & state = pipeline->effect()->matrixState();
				state.modelMatrixStack().push();
				state.modelMatrixStack().mult(elem.transform);
				
				if (pipeline->shouldDraw(elem.material.getPtr())) {
					pipeline->effect()->setMaterial(elem.material.getPtr());
					pipeline->draw(elem.polyList.getPtr());
				}
				
				state.modelMatrixStack().pop();
				pipeline->effect()->setMaterial(currentMaterial.getPtr());
			}
		}
	}
}

void Drawable::update(bg::render::LowOverheadPipeline * pipeline, uint32_t currentFrame) {
    if (pipeline->opacityLayer()==bg::base::Pipeline::kOpacityNone) {
        return;
    }
    else {
        for (auto & elem : _drawableElements) {
            if (elem.polyList->visible()) {
                pipeline->matrixState()->modelMatrixStack().push();
                pipeline->matrixState()->modelMatrixStack().mult(elem.transform);
                
                if (pipeline->shouldDraw(elem.material.getPtr())) {
                    pipeline->mapInputBuffer(elem.polyList.getPtr(),elem.material.getPtr());
                    pipeline->updateShaderInputs(elem.polyList.getPtr(), elem.material.getPtr(), currentFrame);
                }
                
                pipeline->matrixState()->modelMatrixStack().pop();
            }
        }
    }
}

void Drawable::recordCommandBuffer(bg::render::LowOverheadPipeline * pipeline, uint32_t currentFrame) {
    if (pipeline->opacityLayer()==bg::base::Pipeline::kOpacityNone) {
        return;
    }
    else {
        for (auto & elem : _drawableElements) {
            if (elem.polyList->visible()) {
                if (pipeline->shouldDraw(elem.material.getPtr())) {
                    pipeline->drawPolyList(elem.polyList.getPtr(), currentFrame);
                }
            }
        }
    }
}

void Drawable::draw(bg::base::RenderQueue * renderQueue, bg::base::MatrixStack & modelMatrixStack) {
    for (auto & elem : _drawableElements) {
        if (elem.polyList->visible()) {
            bg::math::Matrix4 matrix = modelMatrixStack.matrix();
            matrix.mult(elem.transform);
            if (elem.material->transparent()) {
                renderQueue->renderTransparent(elem.polyList.getPtr(), elem.material.getPtr(), matrix);
            }
            else {
                renderQueue->renderOpaque(elem.polyList.getPtr(), elem.material.getPtr(), matrix);
            }
        }
    }
}

void Drawable::deserialize(bg::base::Context * ctx, bg::db::json::Value * componentData, const bg::system::Path & path) {
	using namespace bg::db::json;
	std::string file = Value::String((*componentData)["name"]);
	if (!file.empty()) {
		bg::system::Path vwglbPath = path.pathAddingComponent(file + ".vwglb");
		bg::system::Path bg2Path = path.pathAddingComponent(file + ".bg2");
		bg::ptr<Drawable> drw;
		if (bg2Path.isFile()) {
			drw = bg::db::loadDrawable(ctx, bg2Path);
		}
		else if (vwglbPath.isFile()) {
			drw = bg::db::loadDrawable(ctx, vwglbPath);
		}

		if (drw.valid()) {
			setName(drw->name());

			drw->forEach([&](DrawableElement & elem) {
				addPolyList(elem.polyList.getPtr(), elem.material.getPtr(), elem.transform);
			});
		}
	}
}

bg::db::json::Value * Drawable::serialize(bg::base::Context * ctx, const bg::system::Path & path) {
	using namespace bg::db::json;
	Value * result = new Value();

	if (name().empty()) {
		setName(bg::system::uuid());
	}
	result->setValue("type", "Drawable");
	result->setValue("name", name());
	
	bg::system::Path filePath = path.pathRemovingLastComponent();
	filePath.addComponent(name());
	filePath.addExtension("vwglb");
	bg::db::writeDrawable(ctx, filePath, this);
	return result;
}

}
}
