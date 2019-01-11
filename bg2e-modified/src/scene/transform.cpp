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


#include <bg/scene/transform.hpp>
#include <bg/scene/node.hpp>

#include <bg/scene/transform_visitor.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace scene {

TransformStrategy * TransformStrategy::Factory(bg::db::json::Value * jsonData) {
	using namespace bg::db::json;
	TransformStrategy * result = nullptr;
	if (jsonData) {
		std::string type = Value::String((*jsonData)["type"]);
		if (type=="TRSTransformStrategy") {
			result = new TRSTransformStrategy();
		}
		if (result) {
			result->deserialize(jsonData);
		}
	}
	return result;
}

TransformStrategy::TransformStrategy()	
	:MatrixStrategy()
{
}

TransformStrategy::TransformStrategy(bg::math::Matrix4 * target)
	:MatrixStrategy(target)
{
}

TransformStrategy::~TransformStrategy() {
}


TRSTransformStrategy::TRSTransformStrategy()
	:TransformStrategy()
	,_rotateX(0.0f)
	,_rotateY(0.0f)
	,_rotateZ(0.0f)
	,_scale(1.0f)
	,_rotationOrder(kOrderXYZ)
{
}

TRSTransformStrategy::TRSTransformStrategy(bg::math::Matrix4 * mat)
	:TransformStrategy(mat)
	, _rotateX(0.0f)
	, _rotateY(0.0f)
	, _rotateZ(0.0f)
	, _scale(1.0f)
	, _rotationOrder(kOrderXYZ)
{
}

TRSTransformStrategy::~TRSTransformStrategy() {
}

TransformStrategy * TRSTransformStrategy::clone() {
	TRSTransformStrategy * result = new TRSTransformStrategy();
	result->_translate = _translate;
	result->_rotateX = _rotateX;
	result->_rotateY = _rotateY;
	result->_rotateZ = _rotateZ;
	result->_scale = _scale;
	result->_rotationOrder= _rotationOrder;
	return result;
}

void TRSTransformStrategy::apply() {
	if (target()) {
		target()->identity()
			.translate(_translate);

		switch(_rotationOrder) {
		case kOrderXYZ:
			target()->rotate(_rotateX, 1.0f, 0.0f, 0.0f)
					.rotate(_rotateY, 0.0f, 1.0f, 0.0f)
					.rotate(_rotateZ, 0.0f, 0.0f, 1.0f);
			break;
		case kOrderXZY:
			target()->rotate(_rotateX, 1.0f, 0.0f, 0.0f)
				.rotate(_rotateZ, 0.0f, 0.0f, 1.0f)
				.rotate(_rotateY, 0.0f, 1.0f, 0.0f);
			break;
		case kOrderYXZ:
			target()->rotate(_rotateY, 0.0f, 1.0f, 0.0f)
				.rotate(_rotateX, 1.0f, 0.0f, 0.0f)
				.rotate(_rotateZ, 0.0f, 0.0f, 1.0f);
			break;
		case kOrderYZX:
			target()->rotate(_rotateY, 0.0f, 1.0f, 0.0f)
				.rotate(_rotateZ, 0.0f, 0.0f, 1.0f)
				.rotate(_rotateX, 1.0f, 0.0f, 0.0f);
			break;
		case kOrderZYX:
			target()->rotate(_rotateZ, 0.0f, 0.0f, 1.0f)
				.rotate(_rotateY, 0.0f, 1.0f, 0.0f)
				.rotate(_rotateX, 1.0f, 0.0f, 0.0f);
			break;
		case kOrderZXY:
			target()->rotate(_rotateZ, 0.0f, 0.0f, 1.0f)
				.rotate(_rotateX, 1.0f, 0.0f, 0.0f)
				.rotate(_rotateY, 0.0f, 1.0f, 0.0f);
			break;
		}

		target()->scale(_scale);
	}
}

bg::db::json::Value * TRSTransformStrategy::serialize() {
	using namespace bg::db::json;
	Value * result = new Value();
	result->setValue("type","TRSTransformStrategy");
	result->setValue("translate", _translate);
	result->setValue("rotateX", _rotateX);
	result->setValue("rotateY", _rotateY);
	result->setValue("rotateZ", _rotateZ);
	result->setValue("scale", _scale);
	switch(_rotationOrder) {
	case kOrderXYZ:
		result->setValue("rotationOrder", "kOrderXYZ");
		break;
	case kOrderXZY:
		result->setValue("rotationOrder", "kOrderXZY");
		break;
	case kOrderYXZ:
		result->setValue("rotationOrder", "kOrderYXZ");
		break;
	case kOrderYZX:
		result->setValue("rotationOrder", "kOrderYZX");
		break;
	case kOrderZYX:
		result->setValue("rotationOrder", "kOrderZYX");
		break;
	case kOrderZXY:
		result->setValue("rotationOrder", "kOrderZXY");
		break;
	}
	return result;
}

void TRSTransformStrategy::deserialize(bg::db::json::Value * value) {
	using namespace bg::db::json;
	if (value) {
		_translate = Value::Vec3((*value)["translate"]);
		_rotateX = Value::Float((*value)["rotateX"]);
		_rotateY = Value::Float((*value)["rotateY"]);
		_rotateZ = Value::Float((*value)["rotateZ"]);
		_scale = Value::Vec3((*value)["scale"]);
		std::string rotationOrder = Value::String((*value)["rotationOrder"]);
		if (rotationOrder=="kOrderXYZ") {
			_rotationOrder = kOrderXYZ;
		}
		else if(rotationOrder == "kOrderXZY") {
			_rotationOrder = kOrderXZY;
		}
		else if(rotationOrder == "kOrderYXZ") {
			_rotationOrder = kOrderYXZ;
		}
		else if(rotationOrder == "kOrderYZX") {
			_rotationOrder = kOrderYZX;
		}
		else if(rotationOrder == "kOrderZYX") {
			_rotationOrder = kOrderZYX;
		}
		else if(rotationOrder == "kOrderZXY") {
			_rotationOrder = kOrderZXY;
		}
	}
}



ComponentFactory<Transform> transformFactory("Transform");

Transform::Transform()
	:_matrix(bg::math::Matrix4::Identity())
	,_globalMatrixUpdated(false)
{
}

Transform::Transform(const bg::math::Matrix4 & trx)
	:_matrix(trx)
{
}

Transform::~Transform() {
}

Component * Transform::clone() {
	return new Transform(_matrix);
}

const bg::math::Matrix4 & Transform::globalMatrix() {
	if (!_globalMatrixUpdated) {
		bg::ptr<TransformVisitor> visitor = new TransformVisitor();
		if (node()) {
			node()->acceptReverse(visitor.getPtr());
			_globalMatrix = visitor->matrix();
		}
		else {
			_globalMatrix = _matrix;
		}
		_globalMatrixUpdated = true;
	}
	return _globalMatrix;
}

void Transform::willDraw(bg::base::Pipeline * pipeline) {
	if (node() && node()->enabled() && pipeline->effect()) {
		pipeline->effect()->matrixState().modelMatrixStack().push();
		pipeline->effect()->matrixState().modelMatrixStack().mult(_matrix);
	}
}

void Transform::didDraw(bg::base::Pipeline * pipeline) {
	if (node() && node()->enabled() && pipeline->effect()) {
		pipeline->effect()->matrixState().modelMatrixStack().pop();
	}
	_globalMatrixUpdated = false;
}

void Transform::willUpdate(bg::render::LowOverheadPipeline * pipeline, uint32_t currentFrame) {
    pipeline->matrixState()->modelMatrixStack().push();
    pipeline->matrixState()->modelMatrixStack().mult(_matrix);
}

void Transform::didUpdate(bg::render::LowOverheadPipeline * pipeline, uint32_t currentFrame) {
    pipeline->matrixState()->modelMatrixStack().pop();
    _globalMatrixUpdated = false;
}

void Transform::willUpdate(bg::base::MatrixStack & viewMatrixStack) {
	viewMatrixStack.push();
	viewMatrixStack.mult(_matrix);
}

void Transform::didUpdate(bg::base::MatrixStack & viewMatrixStack) {
	viewMatrixStack.pop();
}

void Transform::setTransformStrategy(TransformStrategy * strategy) {
	_transformStrategy = strategy;
	if (_transformStrategy.valid()) {
		_transformStrategy->setTarget(&_matrix);
		_transformStrategy->apply();
	}
}

void Transform::deserialize(bg::base::Context *, bg::db::json::Value * componentData, const bg::system::Path & path) {
	using namespace bg::db::json;
	bg::math::Matrix4 matrix;
	TransformStrategy * strategy = TransformStrategy::Factory((*componentData)["transformStrategy"]);
	if (strategy) {
		setTransformStrategy(strategy);
	}
	else {
		this->setMatrix(Value::Mat4((*componentData)["transformMatrix"]));
	}
}

bg::db::json::Value * Transform::serialize(bg::base::Context *, const bg::system::Path &) {
	using namespace bg::db::json;
	Value * jsonData = new Value();
	jsonData->setValue("type","Transform");
	if (_transformStrategy.valid()) {
		jsonData->setValue("transformStrategy", _transformStrategy->serialize());
	}
	else {
		jsonData->setValue("transformMatrix", _matrix);
	}
	return jsonData;
}

}
}
