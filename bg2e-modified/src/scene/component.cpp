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


#include <bg/scene/component.hpp>
#include <bg/scene/scene_object.hpp>
#include <bg/scene/node.hpp>

#include <bg/scene/audio_listener.hpp>
#include <bg/scene/audio_source.hpp>
#include <bg/scene/transform.hpp>
#include <bg/scene/camera.hpp>
#include <bg/scene/light.hpp>
#include <bg/scene/drawable.hpp>
#include <bg/scene/collider.hpp>
#include <bg/scene/rigid_body.hpp>
#include <bg/scene/dynamics.hpp>

#include <bg/db/json/value.hpp>

#include <map>

namespace bg {
namespace scene {

ComponentRegistry::ComponentRegistry() {}
ComponentRegistry::~ComponentRegistry() {}

ComponentRegistry * ComponentRegistry::s_singleton = nullptr;


Component::Component()
	:_sceneObject(nullptr)
	,_initialized(false)
	,_ignoreSerialize(false)
{

}

Component::~Component()
{

}

Node * Component::node() {
	return dynamic_cast<Node*>(sceneObject());
}

Component * Component::Factory(bg::base::Context * ctx, bg::db::json::Value * componentData, const bg::system::Path & resourcePath) {
	using namespace bg::db::json;
	std::string type = Value::String((*componentData)["type"]);
	ptr<Component> component = ComponentRegistry::Get()->instantiate(type);
	if (component.valid()) {
		component->deserialize(ctx, componentData, resourcePath);
	}
	return component.release();
}

bg::base::Context * Component::context() {
	return node() ? node()->context() : nullptr;
}

AudioListener * Component::audioListener() {
	return node() ? node()->component<AudioListener>() : nullptr;
}

AudioSource * Component::audioSource() {
	return node() ? node()->component<AudioSource>() : nullptr;
}

Transform * Component::transform() {
	return node() ? node()->component<Transform>() : nullptr;
}

Camera * Component::camera() {
	return node() ? node()->component<Camera>() : nullptr;
}

Light * Component::light() {
	return node() ? node()->component<Light>() : nullptr;
}

Drawable * Component::drawable() {
	return node() ? node()->component<Drawable>() : nullptr;
}

Collider * Component::collider() {
	return node() ? node()->component<Collider>() : nullptr;
}

RigidBody * Component::rigidBody() {
	return node() ? node()->component<RigidBody>() : nullptr;
}

Dynamics * Component::dynamics() {
	return node() ? node()->component<Dynamics>() : nullptr;
}

}
}
