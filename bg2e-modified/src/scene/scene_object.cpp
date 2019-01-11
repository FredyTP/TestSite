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

#include <bg/scene/scene_object.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace scene {

SceneObject::SceneObject(bg::base::Context * ctx)
	:bg::base::ContextObject(ctx)
	,_name("")
	,_enabled(true)
{
}

SceneObject::SceneObject(bg::base::Context * ctx, const std::string & name)
	: bg::base::ContextObject(ctx)
	,_name(name)
	,_enabled(true)
{
}

SceneObject::~SceneObject() {
	for (auto & c : _componentMap) {
		if (c.second.valid())  c.second->removedFromNode(this);
	}
	_componentMap.clear();
}

void SceneObject::reshape(bg::base::Pipeline * pipeline, int w, int h) {
	for (auto & c : _componentMap) {
		if (c.second.valid())  c.second->reshape(pipeline,w,h);
	}
}

void SceneObject::willDraw(bg::base::Pipeline * pipeline) {
	for (auto & c : _componentMap) {
		if (c.second.valid())  c.second->willDraw(pipeline);
	}
}

void SceneObject::draw(bg::base::Pipeline * pipeline) {
	for (auto & c : _componentMap) {
		if (c.second.valid())  c.second->draw(pipeline);
	}
}

void SceneObject::didDraw(bg::base::Pipeline * pipeline) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->didDraw(pipeline);
	}
}

void SceneObject::frame(float delta) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  {
			c.second->checkInit();
			c.second->frame(delta);
		}
	}
}

void SceneObject::willUpdate(bg::render::LowOverheadPipeline * pipeline, uint32_t currentFrame) {
    for(auto & c : _componentMap) {
        if (c.second.valid())  c.second->willUpdate(pipeline, currentFrame);
    }
}

void SceneObject::update(bg::render::LowOverheadPipeline * pipeline, uint32_t currentFrame) {
    for(auto & c : _componentMap) {
        if (c.second.valid())  c.second->update(pipeline, currentFrame);
    }
}

void SceneObject::didUpdate(bg::render::LowOverheadPipeline * pipeline, uint32_t currentFrame) {
    for(auto & c : _componentMap) {
        if (c.second.valid())  c.second->didUpdate(pipeline, currentFrame);
    }
}

void SceneObject::willUpdate(bg::base::MatrixStack & viewMatrixStack) {
    for(auto & c : _componentMap) {
        if (c.second.valid())  c.second->willUpdate(viewMatrixStack);
    }
}

void SceneObject::draw(bg::base::RenderQueue * renderQueue, bg::base::MatrixStack & modelMatrixStack) {
    for(auto & c : _componentMap) {
        if (c.second.valid())  c.second->draw(renderQueue, modelMatrixStack);
    }
}
    
void SceneObject::didUpdate(bg::base::MatrixStack & modelMatrixStack) {
    for(auto & c : _componentMap) {
        if (c.second.valid())  c.second->didUpdate(modelMatrixStack);
    }
}
    
void SceneObject::recordCommandBuffer(bg::render::LowOverheadPipeline * pipeline, uint32_t currentFrame) {
    for(auto & c : _componentMap) {
        if (c.second.valid())  c.second->recordCommandBuffer(pipeline, currentFrame);
    }
}

void SceneObject::destroy() {
	std::vector<Component*> destroyComponents;
	for(auto & c : _componentMap) {
		destroyComponents.push_back(c.second.getPtr());
	}

	for (auto & comp : destroyComponents) {
		ptr<Component> preventDelete = comp;
		removeComponent(comp);
		comp->destroy();
	}
}

void SceneObject::keyUp(const bg::base::KeyboardEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->keyUp(evt);
	}
}

void SceneObject::keyDown(const bg::base::KeyboardEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->keyDown(evt);
	}
}

void SceneObject::charPress(const bg::base::KeyboardEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->charPress(evt);
	}
}

void SceneObject::mouseDown(const bg::base::MouseEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->mouseDown(evt);
	}
}

void SceneObject::mouseDrag(const bg::base::MouseEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->mouseDrag(evt);
	}
}

void SceneObject::mouseMove(const bg::base::MouseEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->mouseMove(evt);
	}
}

void SceneObject::mouseUp(const bg::base::MouseEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->mouseUp(evt);
	}
}

void SceneObject::mouseWheel(const bg::base::MouseEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->mouseWheel(evt);
	}
}

void SceneObject::touchStart(const bg::base::TouchEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->touchStart(evt);
	}
}

void SceneObject::touchMove(const bg::base::TouchEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->touchMove(evt);
	}
}

void SceneObject::touchEnd(const bg::base::TouchEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->touchEnd(evt);
	}
}

void SceneObject::sensorEvent(const bg::base::SensorEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->sensorEvent(evt);
	}
}

void SceneObject::customEvent(const bg::base::CustomEvent & evt) {
	for(auto & c : _componentMap) {
		if (c.second.valid())  c.second->customEvent(evt);
	}
}

}
}
