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

#include <bg/system/system.hpp>

#include "btBulletDynamicsCommon.h"

#include <bg/scene/transform.hpp>

#include <bg/math/utils.hpp>


namespace bg {
namespace physics {

namespace bt {

btDefaultCollisionConfiguration * config(World * w) {
	return bg::native_cast<btDefaultCollisionConfiguration*>(w->collisionConfiguration);
}

btCollisionDispatcher * dispatcher(World * w) {
	return bg::native_cast<btCollisionDispatcher*>(w->collisionDispatcher);
}

btDbvtBroadphase * pairCache(World * w) {
	return bg::native_cast<btDbvtBroadphase*>(w->overlappingPairCache);
}

btSequentialImpulseConstraintSolver * solver(World * w) {
	return bg::native_cast<btSequentialImpulseConstraintSolver*>(w->solver);
}

btDiscreteDynamicsWorld * world(World * w) {
	return bg::native_cast<btDiscreteDynamicsWorld*>(w->dynamicsWorld);
}

}

World::World()
	:_gravity(0.0f)
	,_isRunning(false)
	,_dynamics(nullptr)
{
	btDefaultCollisionConfiguration * conf = new btDefaultCollisionConfiguration();
	btCollisionDispatcher * dispatcher = new btCollisionDispatcher(conf);
	btDbvtBroadphase * pairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver * s = new btSequentialImpulseConstraintSolver();
	btDiscreteDynamicsWorld * dw = new btDiscreteDynamicsWorld(dispatcher,
		pairCache,
		s,
		conf);

	collisionConfiguration = conf;
	collisionDispatcher = dispatcher;
	overlappingPairCache = pairCache;
	solver = s;
	dynamicsWorld = dw;
}

World::~World() {
	endSimulation();
	
	delete bt::world(this);
	delete bt::dispatcher(this);
	delete bt::pairCache(this);
	delete bt::solver(this);
	delete bt::config(this);

	dynamicsWorld = nullptr;
	solver = nullptr;
	overlappingPairCache = nullptr;
	collisionDispatcher = nullptr;
	collisionConfiguration = nullptr;
}

void World::setGravity(const bg::math::Vector3 &g) {
	_gravity = g;
	bt::world(this)->setGravity(btVector3(g.x(),g.y(),g.z()));
}

void World::beginSimulation(bg::scene::Node *physicsNode) {
	scene::Dynamics * sim = physicsNode ? physicsNode->component<bg::scene::Dynamics>() : nullptr;
	
	if (!_isRunning && sim) {
		_isRunning = true;
		
		sim->eachShape([&](bg::scene::Node *node, bg::scene::Collider *, bg::scene::RigidBody *) {
			addNode(node);
		});
	}
}

void World::simulationStep(float delta) {
	if (_isRunning) {
        float minT = 1.0f / static_cast<float>(_minFramerate);
        float simT = 1.0f / static_cast<float>(_targetFramerate);
        int steps = static_cast<int>(ceil(minT/simT));
		bt::world(this)->stepSimulation(delta,steps);
		int collisionObjects = bt::world(this)->getNumCollisionObjects();
		int numManifolds = bt::world(this)->getDispatcher()->getNumManifolds();
		for (int i=0; i<numManifolds; ++i) {
			btPersistentManifold* contactManifold = bt::world(this)->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
			const btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());
			size_t bodyA = reinterpret_cast<size_t>(btRigidBody::upcast(obA));
			size_t bodyB = reinterpret_cast<size_t>(btRigidBody::upcast(obB));
			bg::scene::Node * nodeA = _colliderMap[bodyA].getPtr();
			bg::scene::Node * nodeB = _colliderMap[bodyB].getPtr();
		
			if (contactManifold->getNumContacts()>0) {
				const btVector3 & posA = contactManifold->getContactPoint(0).getPositionWorldOnA();
				const btVector3 & posB = contactManifold->getContactPoint(0).getPositionWorldOnB();
				int lifeTime = contactManifold->getContactPoint(0).getLifeTime();
				float impulse = contactManifold->getContactPoint(0).getAppliedImpulse();
				const btVector3 & normal = contactManifold->getContactPoint(0).m_normalWorldOnB;
				
				nodeA->component<bg::scene::Collider>()->onCollision(CollisionData{
					nodeA,
					nodeB,
					bg::math::Vector3(posA.x(),posA.y(),posA.z()),
					bg::math::Vector3(posB.x(),posB.y(),posB.z()),
					lifeTime,
					impulse,
					bg::math::Vector3(normal.x(),normal.y(),normal.z())
				});
				nodeB->component<bg::scene::Collider>()->onCollision(CollisionData{
					nodeB,
					nodeA,
					bg::math::Vector3(posB.x(),posB.y(),posB.z()),
					bg::math::Vector3(posA.x(),posA.y(),posA.z()),
					lifeTime,
					impulse,
					bg::math::Vector3(normal.x(),normal.y(),normal.z())
				});
			}
		
		}
		
		for (int j=collisionObjects-1; j>=0; --j) {
			btCollisionObject * obj = bt::world(this)->getCollisionObjectArray()[j];
			btRigidBody * body = btRigidBody::upcast(obj);
			bg::scene::Node * node = _colliderMap[reinterpret_cast<size_t>(body)].getPtr();
			bg::scene::Collider * collider = node->component<bg::scene::Collider>();
			if (collider && collider->transform()) {
				btTransform trans;
				btMotionState * motionState = nullptr;
				if (body && (motionState=body->getMotionState())!=nullptr) {
					motionState->getWorldTransform(trans);
				}
				else {
					trans = obj->getWorldTransform();
				}
				
				float rawMat[16];
				trans.getOpenGLMatrix(rawMat);
				bg::math::Matrix4 m(rawMat);
				m.scale(collider->transform()->matrix().scale());
				collider->transform()->setMatrix(m);
			}
		}
	}

}

void World::endSimulation() {
	if (_isRunning) {
		for (auto collider : _colliderMap) {
			removeNode(collider.second.getPtr());
		}

		// Assert that all the world is clean
		if (bt::world(this)->getNumCollisionObjects()>0) {
			std::cerr << "WARNING: error cleaning physics world. Not all physics objects were removed." << std::endl;
		}
		_colliderMap.clear();
		_isRunning = false;
	}

}

void World::removeNode(bg::scene::Node * node) {
	bg::scene::Node * parent = node->parent();
	bg::scene::Collider * collider = node->component<bg::scene::Collider>();
	bg::scene::Dynamics * dynamics = parent ? parent->component<bg::scene::Dynamics>() : nullptr;
	World * dynWorld = dynamics ? dynamics->world() : nullptr;

	if (dynWorld==this && collider) {
		bg::scene::RigidBody * rb = node->component<bg::scene::RigidBody>();
		btCollisionObject * obj = bg::native_cast<btCollisionObject*>(collider->shape()->collisionObject());

		// Remove rigid body
		btRigidBody * body = btRigidBody::upcast(obj);
		if(body && body->getMotionState()) {
			delete body->getMotionState();
		}

		// Remove object from simulation loop
		bt::world(this)->removeCollisionObject(obj);

		// Remove collision object
		delete obj;

		// Clean up scene objects
		collider->shape()->endSimulation();
		if(rb) {
			rb->body()->setImpl(nullptr, nullptr);
			rb->body()->endSimulation();
		}
		if(collider->transform()) {
			if(collider->transform()->transformStrategy()) {
				collider->transform()->transformStrategy()->apply();
			}
		}
	}
}

void World::addNode(bg::scene::Node * node) {
	bg::scene::Node * parent = node->parent();
	bg::scene::Collider * collider = node->component<bg::scene::Collider>();
	bg::scene::RigidBody * rigidBodyComponent = node->component<bg::scene::RigidBody>();
	bg::scene::Dynamics * dynamics = parent ? parent->component<bg::scene::Dynamics>() : nullptr;
	
	if (sceneComponent()==dynamics && collider) {
		btCollisionShape * shape = bg::impl_cast<btCollisionShape*>(collider->shape()->beginSimulation());

		btTransform bt_transform;
		bt_transform.setIdentity();
		bg::scene::Transform * trx = collider->transform();
		if(trx) {
			bt_transform.setFromOpenGLMatrix(trx->matrix().raw());
			bg::math::Vector3 scale = trx->matrix().scale();
			shape->setLocalScaling(btVector3(scale.x(), scale.y(), scale.z()));
		}

		btScalar mass(0.0f);
		btVector3 localInertia(0.0f, 0.0f, 0.0f);
		if(rigidBodyComponent && rigidBodyComponent->body()->mass()>0.0f) {
			mass = rigidBodyComponent->body()->mass();
			shape->calculateLocalInertia(mass, localInertia);
		}

		btDefaultMotionState * myMotionState = new btDefaultMotionState(bt_transform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
		btRigidBody * body = new btRigidBody(rbInfo);

		collider->shape()->setSimulationLoopObjects(body,this);
		_colliderMap[reinterpret_cast<size_t>(body)] = collider->node();
		bt::world(this)->addRigidBody(body);
        if (rigidBodyComponent) {
            rigidBodyComponent->body()->setImpl(body, this);
        }
	}
}

bg::scene::Dynamics * World::sceneComponent() {
	return _dynamics;
}

bg::physics::RayCastResult * World::rayTest(const bg::physics::Ray & ray) {
	bg::ptr<bg::physics::RayCastResult> result;
	btVector3 start(ray.start().x(), ray.start().y(), ray.start().z());
	btVector3 end(ray.end().x(), ray.end().y(), ray.end().z());
	btCollisionWorld::ClosestRayResultCallback rayCallback(start,end);
	
	bt::world(this)->rayTest(start, end, rayCallback);
	if (rayCallback.hasHit()) {
		const btCollisionObject * obj = rayCallback.m_collisionObject;
		btVector3 point = rayCallback.m_hitPointWorld;
		btVector3 normal = rayCallback.m_hitNormalWorld;
		size_t bodyKey = reinterpret_cast<size_t>(btRigidBody::upcast(obj));
		bg::scene::Node * node = _colliderMap[bodyKey].getPtr();
		result = new bg::physics::RayCastResult(node,
												bg::math::Vector3(point.x(), point.y(), point.z()),
												bg::math::Vector3(normal.x(), normal.y(), normal.z()));
	}
	
	return result.release();
}

}
}
 
