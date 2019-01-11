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


#include <bg/physics/world.hpp>

#include <bg/scene/dynamics.hpp>


#include <bg/scene/Transform.hpp>

#include <bg/math/utils.hpp>


namespace bg {
namespace physics {

World::World()
	:_gravity(0.0f)
	,_isRunning(false)
	,_dynamics(nullptr)
{
}

World::~World() {
}

void World::setGravity(const bg::math::Vector3 &g) {
	_gravity = g;
}

void World::beginSimulation(bg::scene::Node *) {
}

void World::simulationStep(float) {
}

void World::endSimulation() {
}

void World::removeNode(bg::scene::Node *) {
}

void World::addNode(bg::scene::Node *) {
}

bg::scene::Dynamics * World::sceneComponent() {
	return nullptr;
}

bg::physics::RayCastResult * World::rayTest(const bg::physics::Ray &) {
	return nullptr;
}

}
}
 
