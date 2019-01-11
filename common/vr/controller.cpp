
#include "controller.hpp"
#include <vr\vr_system.hpp>

#include <bg/physics/ray.hpp>
#include <bg/physics/plane.hpp>
#include <bg/physics/intersection.hpp>
#include <bg/scene/primitive_factory.hpp>
#include <bg/scene/dynamics.hpp>

#include <bg/log.hpp>


NavigationController::NavigationController(int controllerIndex)
	:_controllerIndex(controllerIndex)
	,_showRay(false)
	,_rayDirection(0.0f, -15.0f, -30.0f)
{
}

NavigationController::~NavigationController() {
}

bg::scene::Component * NavigationController::clone() {
	return new NavigationController(_controllerIndex);
}

void NavigationController::init() {

}

void NavigationController::frame(float delta) {
	if (_showRay) {
		previewNavigation();
	}
}

void NavigationController::draw(bg::base::Pipeline * pipeline) {

}

void NavigationController::customEvent(const bg::base::CustomEvent &evt) {
	const ControllerEventData * data = evt.data<ControllerEventData>();
	if(data && data->controller()->index()==_controllerIndex) {
		switch(data->eventType()) {
		case Controller::kEventButtonRelease:
			if (data->button()==Controller::kButtonIdTouchpad) {
				navigate();
				_showRay = false;
				VRSystem::Get()->hideNavigationPreview();
			}
			break;
		case Controller::kEventButtonPress:
			if (data->button()==Controller::kButtonIdTouchpad) {
				_showRay = true;
			}
			else if (data->button()==Controller::kButtonIdMenu) {
				bg::scene::Node * dyn = bg::base::ObjectRegistry::Get().findObjectOfType<bg::scene::Node>("dynamics-node");
				if (dyn && dyn->dynamics() && dyn->dynamics()->getSimulationState()!=bg::scene::Dynamics::kStatePlaying) {
					dyn->dynamics()->play();
				}
				else if (dyn && dyn->dynamics() && dyn->dynamics()->getSimulationState()==bg::scene::Dynamics::kStatePlaying) {
					dyn->dynamics()->stop();
					//dyn->dynamics()->play();
				}
			}
			break;
		}
	}
}

void NavigationController::previewNavigation() {
	using namespace bg::physics;
	using namespace bg::math;
	if(transform()) {
		Vector4 p0(0.0f, 0.0f, 0.0f, 1.0f);
		p0 = transform()->matrix().multVector(p0);
		Vector4 p1 = transform()->matrix().multVector(_rayDirection);

		Ray ray = Ray::RayWithPoints(p0.xyz(), p1.xyz());
		Plane plane;
		plane.setOrigin(Vector3());
		plane.setNormal(Vector3(0.0f, 1.0f, 0.0f));

		RayToPlaneIntersection intersect = Intersection::RayToPlane(ray, plane);
		if(intersect.intersects()) {
			VRSystem::Get()->previewNavigation(intersect.endPoint());
		}
	}
}

void NavigationController::navigate() {
	using namespace bg::physics;
	using namespace bg::math;
	if (transform()) {
		Vector4 p0(0.0f,0.0f,0.0f,1.0f);
		p0 = transform()->matrix().multVector(p0);
		Vector4 p1 = transform()->matrix().multVector(_rayDirection);

		Ray ray = Ray::RayWithPoints(p0.xyz(), p1.xyz());
		Plane plane;
		plane.setOrigin(Vector3());
		plane.setNormal(Vector3(0.0f, 1.0f, 0.0f));
	
		RayToPlaneIntersection intersect = Intersection::RayToPlane(ray,plane);
		if (intersect.intersects()) {
			bg::log(bg::log::kDebug) << "Intersect in point " << intersect.endPoint().toString() << bg::endl;
			VRSystem::Get()->navigateTo(intersect.endPoint());
		}
	}
}


std::map<uint32_t, bg::ptr<Controller>> Controller::s_controllers;
uint32_t Controller::s_leftControllerIndex = vr::k_unMaxTrackedDeviceCount;
uint32_t Controller::s_rightControllerIndex = vr::k_unMaxTrackedDeviceCount;

Controller * Controller::Get(uint32_t index) {
	return s_controllers[index].getPtr();
}

Controller * Controller::AddNew(VRSystem * sys, uint32_t index, bg::scene::Node * n) {
	bg::ptr<Controller> ctrl = new Controller(sys, index, n);
	n->addComponent(new NavigationController(index));
	s_controllers[index] = ctrl.getPtr();
	if(s_leftControllerIndex == vr::k_unMaxTrackedDeviceCount) {
		s_leftControllerIndex = index;
	}
	else if(s_rightControllerIndex == vr::k_unMaxTrackedDeviceCount) {
		s_rightControllerIndex = index;
	}
	return ctrl.release();
}

void Controller::UpdateAll() {
	for(auto & c : s_controllers) {
		if(c.second.valid()) {
			c.second->update();
		}
	}
}

Controller * Controller::Left() {
	return Get(s_leftControllerIndex);
}

Controller * Controller::Right() {
	return Get(s_rightControllerIndex);
}

uint32_t Controller::LeftControllerIndex() {
	return s_leftControllerIndex;
}

uint32_t Controller::RightControllerIndex() {
	return s_rightControllerIndex;
}

void Controller::Destroy() {
	s_controllers.clear();
}

Controller::Controller(VRSystem * vr, uint32_t index, bg::scene::Node * node)
	:_vrSystem(vr)
	,_index(index)
	,_sceneNode(node)
{
}

void Controller::update() {
	vr::VRControllerState_t controllerState;
	_vrSystem->impl()->GetControllerState(_index,&controllerState,sizeof(controllerState));
	_triggerPosition = controllerState.rAxis[1].x;
	_trackpadPosition.set(controllerState.rAxis[0].x, controllerState.rAxis[0].y);
}

void Controller::updatePose(const bg::math::Matrix4 & mat) {
	_sceneNode->transform()->setMatrix(mat);
	_sceneNode->setEnabled(true);
}

float Controller::trackpadAngle() {
	if(!_trackpadPosition.isZero()) {
		float angle = bg::math::trigonometry::atan2(-_trackpadPosition.x(), _trackpadPosition.y()) + bg::math::kPiOver2;
		if(angle<0.0) {
			angle = bg::math::k2Pi - bg::math::kPiOver2 + (bg::math::kPiOver2 + angle);
		}
		return angle;
	}
	return 0.0f;
}

