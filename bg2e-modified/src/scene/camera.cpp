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


#include <bg/scene/camera.hpp>

#include <bg/math/utils.hpp>

namespace bg {
namespace scene {

ProjectionStrategy * ProjectionStrategy::Factory(bg::db::json::Value * jsonData) {
	using namespace bg::db::json;
	ProjectionStrategy * result = nullptr;
	if (jsonData) {
		std::string type = Value::String((*jsonData)["type"]);
		if (type=="PerspectiveProjectionMethod") {
			result = new PerspectiveProjectionStrategy();
		}
		else if (type=="OpticalProjectionMethod") {
			result = new OpticalProjectionStrategy();
		}
        else if (type=="OrthographicProjectionStrategy") {
            result = new OrthographicProjectionStrategy();
        }
		if (result) {
			result->deserialize(jsonData);
		}
	}
	return result;
}

ProjectionStrategy::ProjectionStrategy()
	:MatrixStrategy()
	,_near(0.3f)
	,_far(1000.0f)
{
}

ProjectionStrategy::ProjectionStrategy(bg::math::Matrix4 * mat)
	:MatrixStrategy(mat)
	,_near(0.3f)
	,_far(1000.0f)
{
}

ProjectionStrategy::~ProjectionStrategy() {
}


PerspectiveProjectionStrategy::PerspectiveProjectionStrategy()
	:ProjectionStrategy()
	,_fov(50.0f)
{
}

PerspectiveProjectionStrategy::PerspectiveProjectionStrategy(bg::math::Matrix4 * mat)
	:ProjectionStrategy(mat)
	,_fov(50.0f)
{
}

PerspectiveProjectionStrategy::~PerspectiveProjectionStrategy() {
}

ProjectionStrategy * PerspectiveProjectionStrategy::clone() {
	PerspectiveProjectionStrategy * result = new PerspectiveProjectionStrategy();
	result->_near = _near;
	result->_far = _far;
	result->_fov = _fov;
	return result;
}

void PerspectiveProjectionStrategy::apply() {
	if(target()) {
		target()->perspective(_fov, viewport().aspectRatio(), near(), far());
	}
}

bg::db::json::Value * PerspectiveProjectionStrategy::serialize() {
	bg::db::json::Value * value = new bg::db::json::Value();
	value->setValue("type", "PerspectiveProjectionMethod");
	value->setValue("near", _near);
	value->setValue("far", _far);
	value->setValue("fov", _fov);
	return value;
}

void PerspectiveProjectionStrategy::deserialize(bg::db::json::Value * data) {
	using namespace bg::db::json;
	_near = Value::Float((*data)["near"]);
	_far = Value::Float((*data)["far"]);
	_fov = Value::Float((*data)["fov"]);
}


OpticalProjectionStrategy::OpticalProjectionStrategy()
	:ProjectionStrategy()
	,_focalLength(50.0f)
	,_frameSize(35.0f)
{
}

OpticalProjectionStrategy::OpticalProjectionStrategy(bg::math::Matrix4 * mat)
	:ProjectionStrategy(mat)
	,_focalLength(50.0f)
	,_frameSize(35.0f)
{
}

OpticalProjectionStrategy::~OpticalProjectionStrategy() {
}

ProjectionStrategy * OpticalProjectionStrategy::clone() {
	OpticalProjectionStrategy * result = new OpticalProjectionStrategy();
	result->_near = _near;
	result->_far = _far;
	result->_focalLength = _focalLength;
	result->_frameSize = _frameSize;
	return result;
}

void OpticalProjectionStrategy::apply() {
	if(target()) {
		float fov = 2.0f * bg::math::trigonometry::atan(_frameSize / (_focalLength * 2.0f));
		fov = bg::math::trigonometry::radiansToDegrees(fov);
		target()->perspective(fov, viewport().aspectRatio(), near(), far());
	}
}

bg::db::json::Value * OpticalProjectionStrategy::serialize() {
	bg::db::json::Value * value = new bg::db::json::Value();
	value->setValue("type", "OpticalProjectionMethod");
	value->setValue("focalLength", _focalLength);
	value->setValue("frameSize", _frameSize);
	value->setValue("near", _near);
	value->setValue("far", _far);
	return value;
}

void OpticalProjectionStrategy::deserialize(bg::db::json::Value * data) {
	using namespace bg::db::json;
	_focalLength = Value::Float((*data)["focalLength"]);
	_frameSize = Value::Float((*data)["frameSize"]);
	_near = Value::Float((*data)["near"]);
	_far = Value::Float((*data)["far"]);
}

OrthographicProjectionStrategy::OrthographicProjectionStrategy()
    :bg::scene::ProjectionStrategy()
    ,_viewWidth(100.0f)
{

}

OrthographicProjectionStrategy::OrthographicProjectionStrategy(bg::math::Matrix4 * mat)
    :bg::scene::ProjectionStrategy(mat)
    ,_viewWidth(100.0f)
{

}
    
OrthographicProjectionStrategy::~OrthographicProjectionStrategy() {
}
    
ProjectionStrategy * OrthographicProjectionStrategy::clone() {
    OrthographicProjectionStrategy * result = new OrthographicProjectionStrategy();
    result->_viewWidth = _viewWidth;
    result->_near = _near;
    result->_far = _far;
    return result;
}
    
void OrthographicProjectionStrategy::apply() {
    if (target()) {
        auto viewHeight = viewport().aspectRatio() * _viewWidth;
        auto w2 = _viewWidth / 2.0f;
        auto h2 = viewHeight / 2.0f;
        target()->ortho(-h2, h2, -w2, w2, _near, _far);
    }
}
    
bg::db::json::Value * OrthographicProjectionStrategy::serialize() {
    auto value = new bg::db::json::Value();
    value->setValue("type", "OrthographicProjectionStrategy");
    value->setValue("viewWidht", _viewWidth);
    value->setValue("near", _near);
    value->setValue("far", _far);
    return value;
}

void OrthographicProjectionStrategy::deserialize(bg::db::json::Value * data) {
    using namespace bg::db::json;
    _viewWidth = Value::Float((*data)["viewWidth"]);
    _far = Value::Float((*data)["far"]);
    _near = Value::Float((*data)["near"]);
}

    

ComponentFactory<Camera> cameraFactory("Camera");

Camera::Camera()
	:_projection(bg::math::Matrix4::Perspective(60.0f, 1.0f, 0.1f, 100.0f))
	,_viewport(0,0,512,512)
	,_visitor(new TransformVisitor())
	,_rebuildTransform(true)
	,_rebuildViewMatrix(true)
    ,_rebuildPosition(true)
	,_clearBuffers(bg::base::kColorDepth)
	,_focus(5.0f)
{
}

Camera::~Camera() {
}

Component * Camera::clone() {
	ptr<Camera> newCam = new Camera();
	newCam->_projection = _projection;
	newCam->_viewport = _viewport;
	newCam->_visitor->matrix() = _visitor->matrix();
	newCam->_viewMatrix = _viewMatrix;
	newCam->_rebuildTransform = _rebuildTransform;
	newCam->_rebuildViewMatrix = _rebuildViewMatrix;
    newCam->_rebuildPosition = _rebuildPosition;
	newCam->_clearBuffers = _clearBuffers;
	newCam->_focus = _focus;
	_projectionStrategy = _projectionStrategy.valid() ? _projectionStrategy->clone() : nullptr;
	return newCam.release();
}

void Camera::setViewport(const bg::math::Viewport & vp) {
	_viewport = vp;
	if (_projectionStrategy.valid()) {
		_projectionStrategy->setViewport(_viewport);
		_projectionStrategy->apply();
	}
}

void Camera::setProjectionStrategy(ProjectionStrategy * strategy) {
	_projectionStrategy = strategy;
	if (_projectionStrategy.valid()) {
		_projectionStrategy->setTarget(&_projection);
	}
}

const bg::math::Matrix4 & Camera::modelMatrix() {
	if (_rebuildTransform && node()) {
		_visitor->matrix().identity();
		node()->acceptReverse(_visitor.getPtr());
		_rebuildTransform = false;
	}
	return _visitor->matrix();
}

const bg::math::Matrix4 & Camera::viewMatrix() {
	if (_rebuildViewMatrix) {
		_viewMatrix = modelMatrix();
		_viewMatrix.invert();
		_rebuildViewMatrix = false;
	}
	return _viewMatrix;
}

const bg::math::Vector3 & Camera::worldPosition() {
    if (_rebuildPosition) {
        _position = modelMatrix().multVector(bg::math::Vector3(0.0f)).xyz();
        _rebuildPosition = false;
    }
    return _position;
}

void Camera::frame(float) {
	_rebuildTransform = true;
	_rebuildViewMatrix = true;
    _rebuildPosition = true;
}

void Camera::deserialize(bg::base::Context *, bg::db::json::Value * componentData, const bg::system::Path & path) {
	using namespace bg::db::json;
	bg::math::Matrix4 projection;
	ProjectionStrategy * projectionStrategy = ProjectionStrategy::Factory((*componentData)["projectionMethod"]);
	if (projectionStrategy) {
		setProjectionStrategy(projectionStrategy);
	}
	else {
		projection = Value::Mat4((*componentData)["projection"]);
		setProjection(projection);
	}
}

bg::db::json::Value * Camera::serialize(bg::base::Context *, const bg::system::Path &) {
	using namespace bg::db::json;
	Value * cameraData = new Value();
	cameraData->setValue("type","Camera");
	if (_projectionStrategy.getPtr()) {
		cameraData->setValue("projectionMethod", _projectionStrategy->serialize());
	}
	else {
		cameraData->setValue("projection", _projection);
	}
	return cameraData;
}

}
}
