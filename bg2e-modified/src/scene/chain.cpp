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


#include <bg/scene/chain.hpp>

#include <bg/scene/node.hpp>
#include <bg/scene/transform.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace scene {

ComponentFactory<Chain> chainFactory("Chain");

Chain::Chain() {
	
}

Chain::~Chain() {
	
}

Component * Chain::clone() {
	return new Chain();
}
	
void Chain::willDraw(bg::base::Pipeline *) {
	updateChains();
}

void Chain::willUpdate(bg::render::LowOverheadPipeline *, uint32_t currentFrame) {
    updateChains();
}
	
void Chain::deserialize(bg::base::Context *, bg::db::json::Value *, const bg::system::Path &) {
	// Nothing to do here
}

bg::db::json::Value * Chain::serialize(bg::base::Context *, const bg::system::Path &) {
	return nullptr;
}

void Chain::updateChains() {
    if (node()) {
        bg::math::Matrix4 matrix = bg::math::Matrix4::Identity();
        node()->eachChild([&](Node * child) {
            Transform * trx = child->component<Transform>();
            InputChainJoint * inJoint = child->component<InputChainJoint>();
            OutputChainJoint * outJoint = child->component<OutputChainJoint>();
            
            if (inJoint) {
                inJoint->joint()->applyTransform(matrix);
            }
            else {
                matrix.identity();
            }
            
            if (trx) {
                trx->matrix() = matrix;
            }
            
            if (outJoint) {
                outJoint->joint()->applyTransform(matrix);
            }
        });
    }
}

ChainJoint::ChainJoint()
	:_joint(new bg::physics::LinkJoint())
{
	
}

ChainJoint::ChainJoint(bg::physics::LinkJoint * joint)
	:_joint(joint)
{
		
}


ChainJoint::~ChainJoint() {
	
}

void ChainJoint::deserialize(bg::base::Context *, bg::db::json::Value * componentData, const bg::system::Path & path) {
	using namespace bg::db::json;
	Value * jointData = (*componentData)["joint"];

	if (jointData) {
		bg::math::Vector3 offset = Value::Vec3((*jointData)["offset"]);
		float yaw = Value::Float((*jointData)["yaw"]);
		float pitch = Value::Float((*jointData)["pitch"]);
		float roll = Value::Float((*jointData)["roll"]);
		int order = Value::Int((*jointData)["order"]);

		_joint->setOffset(offset);
		_joint->setYaw(yaw);
		_joint->setPitch(pitch);
		_joint->setRoll(roll);
		if(order == 1) {
			_joint->setTransformOrder(bg::physics::LinkJoint::kOrderTranslateRotate);
		}
		else if(order == 2) {
			_joint->setTransformOrder(bg::physics::LinkJoint::kOrderRotateTranslate);
		}
	}
}

ComponentFactory<InputChainJoint> inputChainFactory("InputChainJoint");

InputChainJoint::InputChainJoint()
	:bg::scene::ChainJoint()
{
	_joint->setTransformOrder(bg::physics::LinkJoint::kOrderRotateTranslate);
}

InputChainJoint::InputChainJoint(bg::physics::LinkJoint * joint)
	:bg::scene::ChainJoint(joint)
{
}

InputChainJoint::~InputChainJoint() {
	
}
	
Component * InputChainJoint::clone() {
	return new InputChainJoint(new physics::LinkJoint(_joint.getPtr()));
}

bg::db::json::Value * InputChainJoint::serialize(bg::base::Context *, const bg::system::Path &) {
	return nullptr;
}


ComponentFactory<OutputChainJoint> outputChainFactory("OutputChainJoint");

OutputChainJoint::OutputChainJoint()
	:bg::scene::ChainJoint()
{
	_joint->setTransformOrder(bg::physics::LinkJoint::kOrderTranslateRotate);
}

OutputChainJoint::OutputChainJoint(bg::physics::LinkJoint * joint)
	:bg::scene::ChainJoint(joint)
{
}

OutputChainJoint::~OutputChainJoint() {
	
}

Component * OutputChainJoint::clone() {
	return new OutputChainJoint(new physics::LinkJoint(_joint.getPtr()));
}

bg::db::json::Value * OutputChainJoint::serialize(bg::base::Context *, const bg::system::Path &) {
	return nullptr;
}


}
}
