
#ifndef _vr_system_hpp_
#define _vr_system_hpp_

#include <openvr.h>
#include <string>

#include <vr\vr_model.hpp>
#include <vr\controller.hpp>

#include <bg/scene/node.hpp>
#include <bg/scene/camera.hpp>
#include <bg/render/renderer.hpp>
#include <bg/render/shadow_map.hpp>

class VRSystem {
public:
	typedef std::function<void(Controller*, Controller::EventType, Controller::ButtonIdentifier)> ControllerEventClosure;

	static VRSystem * Get() { return s_singleton; }

	VRSystem();

	void init(bg::scene::Node * sceneRoot);
	void initPhysics(bg::scene::Dynamics * dynamics);
	void frame();

	inline bg::scene::Camera * leftCamera() { return _leftCamera.getPtr(); }
	inline bg::scene::Camera * rightCamera() { return _rightCamera.getPtr(); }
	inline bg::base::TextureRenderSurface * leftSurface() { return _leftSurface.getPtr(); }
	inline bg::base::TextureRenderSurface * rightSurface() { return _rightSurface.getPtr(); }

	void navigateTo(const bg::math::Vector3 & point);
	void resetNavigation();
	void previewNavigation(const bg::math::Vector3 & point);
	void hideNavigationPreview();

	void draw(bg::render::ClassicRenderer * renderer, bg::scene::Node * sceneRoot);
	void destroy();

	inline vr::IVRSystem * impl() { return _vrSystem; }

	inline void setControllerEventClosure(ControllerEventClosure c) {
		_controllerClosure = c;
	}

	inline float scale() const { return _scale; }
	inline void setScale(float s) { _scale = s; }

	bg::scene::Node* leftControllerPhysics(){return _leftControllerPhysics.getPtr(); }
	bg::scene::Node* rightControllerPhysics() {return  _rightControllerPhysics.getPtr(); }
	bg::scene::Node* navigationNode() { return _navigationTargetNode.getPtr(); };

	bg::scene::Node* rayNode() { return _rayNode.getPtr(); }

protected:

	std::string getTrackedDeviceString(vr::IVRSystem * vr, vr::TrackedDeviceIndex_t dev, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError * peError = nullptr);

	vr::IVRSystem * _vrSystem;
	vr::IVRRenderModels * _vrRenderModels;
	std::string _driver;
	std::string _display;
	vr::TrackedDevicePose_t _trackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	bg::math::Matrix4 _matDevicePose[vr::k_unMaxTrackedDeviceCount];
	char _deviceClass[vr::k_unMaxTrackedDeviceCount];
	bool _showTrackedDevice[vr::k_unMaxTrackedDeviceCount];
	int _validPoseCount = 0;
	std::string _poseClasses;

	float _scale = 1.0f;

	bg::ptr<bg::scene::Node> _headNode;
	bg::math::Matrix4 _hdmPose;
	bg::math::Matrix4 _leftCameraPos;
	bg::math::Matrix4 _rightCameraPos;

	bg::math::Matrix4 _leftProjection;
	bg::math::Matrix4 _rightProjection;

	int _controllersFound = 0;
	std::map<std::string, bg::ptr<bg::scene::Drawable>> _modelDrawables;
	ControllerEventClosure _controllerClosure;

	bg::ptr<bg::scene::Node> _navigationTargetNode;
	bg::ptr<bg::scene::Node> _leftControllerPhysics;
	bg::ptr<bg::scene::Node> _rightControllerPhysics;

	bg::ptr<bg::scene::Node> _rayNode;

	float _near;
	float _far;

	uint32_t _renderWidth;
	uint32_t _renderHeight;

	bg::scene::Node * _sceneRoot;
	bg::ptr<bg::scene::Camera> _leftCamera;
	bg::ptr<bg::scene::Camera> _rightCamera;
	bg::ptr<bg::base::TextureRenderSurface> _leftSurface;
	bg::ptr<bg::base::TextureRenderSurface> _rightSurface;

	void initControllers(bg::scene::Node * sceneRoot);
	void initCameras(bg::scene::Node * sceneRoot);
	void updateMatrixPose();
	void processVREvent(const vr::VREvent_t & event);

	bg::math::Matrix4 getHMDProjection(vr::Hmd_Eye);
	bg::math::Matrix4 getHMDPose(vr::Hmd_Eye);

	inline bg::math::Matrix4 fromSteamVRMatrix(const vr::HmdMatrix34_t & matPose) {
		return bg::math::Matrix4(
			matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
			matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
			matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
			matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
		);
	}

	static VRSystem * s_singleton;
};

#endif
