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


#include <bg/scene/light.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace scene {

ComponentFactory<Light> lightFactory("Light");

std::vector<Light*> Light::s_activeLights;

Light::Light()
	:_light(new bg::base::Light)
	,_visitor(new  TransformVisitor)
	,_rebuildTransform(true)
    ,_rebuildPosition(true)
    ,_rebuildDirection(true)
{
}

Light::~Light() {
}

Component * Light::clone() {
	ptr<Light> newLight = new Light();
	newLight->_visitor->matrix() = _visitor->matrix();
	newLight->_light = new bg::base::Light(_light.getPtr());
	newLight->_rebuildTransform = _rebuildTransform;
    newLight->_rebuildPosition = _rebuildPosition;
    newLight->_rebuildDirection = _rebuildDirection;
	return newLight.release();
}

void Light::addedToNode(SceneObject *) {
	s_activeLights.push_back(this);
    _rebuildTransform = true;
    _rebuildPosition = true;
    _rebuildDirection = true;
}

void Light::removedFromNode(SceneObject *) {
	auto it = std::find(s_activeLights.begin(), s_activeLights.end(), this);
	if (it!=s_activeLights.end()) {
		s_activeLights.erase(it);
	}
    _rebuildTransform = true;
    _rebuildPosition = true;
    _rebuildDirection = true;
}

bg::math::Matrix4 & Light::transform() {
	if(/*_rebuildTransform && */node()) {
		_visitor->matrix().identity();
		node()->acceptReverse(_visitor.getPtr());
		_rebuildTransform = false;
        _rebuildPosition = true;
        _rebuildDirection = true;
	}
	return _visitor->matrix();
}

const bg::math::Vector3 & Light::worldPosition() {
    if (_rebuildPosition) {
        _worldPosition = transform().multVector(bg::math::Vector3(0.0f)).xyz();
        _rebuildPosition = false;
    }
    return _worldPosition;
}

const bg::math::Vector3 & Light::worldDirection() {
    if (_rebuildDirection) {
        _worldDirection = transform()
            .rotation()
            .multVector(light()->direction()).xyz();
        _worldDirection.normalize();
        _rebuildDirection = false;
    }
    return _worldDirection;
}

void Light::frame(float) {
	_rebuildTransform = true;
    _rebuildPosition = true;
    _rebuildDirection = true;
}

void Light::deserialize(bg::base::Context *, bg::db::json::Value * componentData, const bg::system::Path & path) {
	using namespace bg::db::json;
	std::string lightType = Value::String((*componentData)["lightType"]);
	light()->setAmbient(Value::Vec4((*componentData)["ambient"]));
	light()->setDiffuse(Value::Vec4((*componentData)["diffuse"]));
	light()->setSpecular(Value::Vec4((*componentData)["specular"]));
	light()->setConstantAttenuation(Value::Float((*componentData)["constantAtt"]));
	light()->setLinearAttenuation(Value::Float((*componentData)["linearAtt"]));
	light()->setQuadraticAttenuation(Value::Float((*componentData)["expAtt"]));
	light()->setSpotCutoff(Value::Float((*componentData)["spotCutoff"]));
	light()->setShadowStrength(Value::Float((*componentData)["shadowStrength"]));
	light()->setCutoffDistance(Value::Float((*componentData)["cutoffDistance"]));
	light()->setProjection(Value::Mat4((*componentData)["projection"]));
	light()->setCastShadows(Value::Bool((*componentData)["castShadows"]));
	light()->setShadowBias(Value::Float((*componentData)["shadowBias"]));

	if (lightType =="kTypeDirectional") {
		light()->setType(bg::base::Light::kTypeDirectional);
	}
	else if(lightType == "kTypePoint") {
		light()->setType(bg::base::Light::kTypePoint);
	}
	else if(lightType == "kTypeSpot") {
		light()->setType(bg::base::Light::kTypeSpot);
	}
	else if(lightType == "kTypeDisabled") {
		light()->setType(bg::base::Light::kTypeDisabled);
	}

}

bg::db::json::Value * Light::serialize(bg::base::Context *, const bg::system::Path &) {
	using namespace bg::db::json;
	Value * jsonData = new Value();
	jsonData->setValue("type","Light");
	std::string typeString;
	switch (this->light()->type()) {
	case bg::base::Light::kTypeDirectional:
		typeString = "kTypeDirectional";
		break;
	case bg::base::Light::kTypeSpot:
		typeString = "kTypeSpot";
		break;
	case bg::base::Light::kTypePoint:
		typeString = "kTypePoint";
		break;
	case bg::base::Light::kTypeDisabled:
		typeString = "kTypeDisabled";
		break;
	}
	jsonData->setValue("lightType", typeString);
	jsonData->setValue("ambient", light()->ambient());
	jsonData->setValue("diffuse", light()->diffuse());
	jsonData->setValue("specular", light()->specular());
	jsonData->setValue("constantAtt", light()->constantAttenuation());
	jsonData->setValue("linearAtt", light()->linearAttenuation());
	jsonData->setValue("expAtt", light()->quadraticAttenuation());
	jsonData->setValue("spotCutoff", light()->spotCutoff());
	jsonData->setValue("shadowStrength", light()->shadowStrength());
	jsonData->setValue("cutoffDistance", light()->cutoffDistance());
	jsonData->setValue("projection", light()->projection());
	jsonData->setValue("castShadows", light()->castShadows());
	jsonData->setValue("shadowBias", light()->shadowBias());
	return jsonData;
}

}
}
