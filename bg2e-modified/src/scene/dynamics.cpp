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


#include <bg/scene/dynamics.hpp>

namespace bg {
namespace scene {

ComponentFactory<Dynamics> dynamicsFactory("Dynamics");

Dynamics::Dynamics()
	:_world(new bg::physics::World)
	,_state(kStateStopped)
{
	_world->_dynamics = this;
}
	
Dynamics::~Dynamics() {
	
}

Component * Dynamics::clone() {
	Dynamics * newComp = new Dynamics();
	newComp->_state = _state;
	return newComp;
}
	
void Dynamics::frame(float delta) {
	if (_state==kStatePlaying) {
		_world->simulationStep(delta);
	}
}
	
void Dynamics::play() {
	if (_state==kStatePaused) {
		_state = kStatePlaying;
	}
	else if (_state==kStateStopped) {
		_state = kStatePlaying;
		_world->beginSimulation(node());
	}
}

void Dynamics::pause() {
	if (_state==kStatePlaying) {
		_state = kStatePaused;
	}
}

void Dynamics::stop() {
	if (_state!=kStateStopped) {
		_state = kStateStopped;
		_world->endSimulation();
	}
}

void Dynamics::deserialize(bg::base::Context *, bg::db::json::Value * componentData, const bg::system::Path & path) {
	using namespace bg::db::json;
	_world->setGravity(Value::Vec3((*componentData)["gravity"]));
}

bg::db::json::Value * Dynamics::serialize(bg::base::Context * ctx, const bg::system::Path & path) {
	using namespace bg::db::json;
	Value * result = new Value();
	result->setValue("type", "Dynamics");
	result->setValue("gravity",_world->gravity());
	return result;
}

void DynamicsVisitor::visit(bg::scene::Node * node) {
	Dynamics * dyn = node->component<Dynamics>();
	if (dyn) {
		_dynamics.push_back(dyn);
	}
}
	
void DynamicsVisitor::play() {
	for (auto d : _dynamics) {
		d->play();
	}
	_state = Dynamics::kStatePlaying;
}

void DynamicsVisitor::pause() {
	for (auto d : _dynamics) {
		d->pause();
	}
	_state = Dynamics::kStatePaused;
}

void DynamicsVisitor::stop() {
	for (auto d : _dynamics) {
		d->stop();
	}
	_state = Dynamics::kStateStopped;
}

}
}