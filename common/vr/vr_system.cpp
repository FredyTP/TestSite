
#include "vr_system.hpp"

#include <bg/bg2e.hpp>
#include <bg/scene/transform_visitor.hpp>

#include <bg/engine/openglCore/texture_impl.hpp>

#include <thread>
#include <app/App.h>

class ControllerPhysicsComponent : public bg::scene::Component {
public:
	ControllerPhysicsComponent(bool leftController) :_leftController(leftController) {
	}

	Component * clone() { return new ControllerPhysicsComponent(_leftController); }

	void init() {
		using namespace bg::scene;
		Collider * col = collider();
		if (col) {
			col->addCollisionClosure([&](const bg::physics::CollisionData & collision) {
				if (_catchedObject.valid()) return;	// Avoid capture two objects

				Node * target = collision.nodeB;
				RigidBody * rb = target->rigidBody();
				ControllerPhysicsComponent * controllerComponent = target->component<ControllerPhysicsComponent>();
				if(rb && !controllerComponent && _catchObject) {
					bg::log(bg::log::kDebug) << "Collision with physics object" << bg::endl;
					catchObject(target);
				}
			});
		}
	}

	void frame(float) {
		using namespace bg::scene;
		using namespace bg::math;
		RigidBody * rb = nullptr;
		if (_catchedObject.valid() && (rb = _catchedObject->rigidBody()) && transform()) {
			Matrix4 newTrx = transform()->matrix();
			newTrx.mult(_p);
			newTrx.mult(_initialMatrix);
//			newTrx.translate(_offset);
			rb->body()->setTransform(newTrx);
		}
	}

	void customEvent(const bg::base::CustomEvent &evt) {
		const ControllerEventData * data = evt.data<ControllerEventData>();
		int thisControllerIndex = _leftController ? Controller::LeftControllerIndex() :
													Controller::RightControllerIndex();
		if(data && data->controller()->index() == thisControllerIndex) {
			switch(data->eventType()) {
			case Controller::kEventButtonRelease:
				if(data->button() == Controller::kButtonIdSide) {
					releaseObject();
					_catchObject = false;
				}
				break;
			case Controller::kEventButtonPress:
				if(data->button() == Controller::kButtonIdSide) {
					_catchObject = true;
				}
				break;
			}
		}
	}

protected:
	virtual ~ControllerPhysicsComponent() {
	}

	bool _leftController;
	bool _catchObject = false;
	bg::ptr<bg::scene::Node> _catchedObject;
	bg::math::Vector3 _offset;
	float _mass;
	bg::math::Vector3 _linearFactor;
	bg::math::Vector3 _angularFactor;
	bg::math::Matrix4 _initialMatrix;
	bg::math::Matrix4 _p;

	void catchObject(bg::scene::Node * obj) {
		using namespace bg::scene;
		using namespace bg::math;
		Transform * trx = obj->transform();
		RigidBody * rb = obj->rigidBody();
		Transform * thisTrx = transform();
		if (trx && rb && thisTrx) {
			_catchedObject = obj;
			Matrix4 rotationMatrix = trx->globalMatrix().rotation();
			Vector3 position = trx->globalMatrix().position();
			Vector3 thisPosition = thisTrx->globalMatrix().position();
			_p = thisTrx->globalMatrix();
			_p.invert();
			_initialMatrix = trx->globalMatrix();

			_offset = thisPosition - position;
			_mass = rb->body()->mass();
			_linearFactor = rb->body()->linearFactor();
			_angularFactor = rb->body()->angularFactor();
			rb->body()->setMass(0.0f);
			rb->body()->setLinearFactor(Vector3(0.0f));
			rb->body()->setLinearVelocity(Vector3(0.0f));
			rb->body()->setAngularFactor(Vector3(0.0f));
			rb->body()->setAngularVelocity(Vector3(0.0f));
		}
	}

	void releaseObject() {
		if (_catchedObject.valid()) {
			_catchedObject->rigidBody()->body()->setMass(_mass);
			_catchedObject->rigidBody()->body()->setLinearFactor(_linearFactor);
			_catchedObject->rigidBody()->body()->setAngularFactor(_angularFactor);
			_catchedObject = nullptr;
		}
	}
};

class ArrowComponent : public bg::scene::Component {
public:
	bg::scene::Component * clone() {
		return new ArrowComponent();
	}

	virtual void frame(float delta) {
		float minHeight = 0.0f;
		float maxHeight = 0.40f;
		_alpha += delta;
		if (_alpha>bg::math::k2Pi) {
			_alpha = 0.0f;
		}
		if (_heightUp) {
			_height += delta * 0.7f;
		}
		else {
			_height -= delta * 0.7f;
		}
		if (_height<minHeight) {
			_height = minHeight;
			_heightUp = true;
		}
		else if (_height>maxHeight) {
			_height = maxHeight;
			_heightUp = false;
		}

		if (transform()) {
			transform()->matrix()
				.identity()
				.rotate(_alpha, 0.0f, 1.0f, 0.0f)
				.translate(0.0f, _height, 0.0f);
		}
	}

protected:
	virtual ~ArrowComponent() {}

	float _alpha = 0.0f;
	float _height = 0.0f;
	bool _heightUp = true;
};

VRSystem * VRSystem::s_singleton = nullptr;

VRSystem::VRSystem()
{
	s_singleton = this;
	vr::EVRInitError err = vr::VRInitError_None;
	_vrSystem = vr::VR_Init(&err, vr::VRApplication_Scene);
	//_scale = 1.0f;
	if(err!= vr::VRInitError_None) {
		_vrSystem = nullptr;
		ts::App::Get().setVR(false);
		std::cout << "NO VR" << std::endl;
		//throw bg::base::InitException("Unable to initialize VR rintime: " + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(err)));
	}
	else
	{
		ts::App::Get().setVR(true);
	}
}


void VRSystem::init(bg::scene::Node * sceneRoot) {
	vr::EVRInitError eError = vr::VRInitError_None;
	_vrRenderModels = static_cast<vr::IVRRenderModels *>(vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError));
	if(!_vrRenderModels) {
		_vrSystem = nullptr;
		vr::VR_Shutdown();
		throw bg::base::InitException("Unable to get render model interface: " + std::string(vr::VR_GetVRInitErrorAsEnglishDescription(eError)));
	}

	_driver = getTrackedDeviceString(_vrSystem, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
	_display = getTrackedDeviceString(_vrSystem, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);

	std::memset(_deviceClass, 0, sizeof(char) * vr::k_unMaxTrackedDeviceCount);

	_near = 0.1f;
	_far = 30.0f;



	bg::wnd::MainLoop::Get()->addCustomLoopClosure([&]() {
		vr::VREvent_t event;
		while(_vrSystem->PollNextEvent(&event, sizeof(event))) {
			processVREvent(event);
		}
	});

	vr::EVRInitError peError = vr::VRInitError_None;
	int elapsed = 0;
	while(!vr::VRCompositor()) {
		bg::wnd::MessageBox::Response res = bg::wnd::MessageBox::Show(nullptr,"Open SteamVR",
			"Please, open SteamVR and click OK to continue, or press Cancel to quit.",
			bg::wnd::MessageBox::kButtonOkCancel);
		if (res==bg::wnd::MessageBox::kResponseCancel)
		{
			throw bg::base::InitException("Could not initialize VR compositor.");
		}
	}

	// Init tracking devices
	_controllersFound = 0;
	initControllers(sceneRoot);
	

	// Init cameras
	initCameras(sceneRoot);
}

void VRSystem::initPhysics(bg::scene::Dynamics * dynamics) {
	using namespace bg::scene;
	if (dynamics && dynamics->node()) {
		float colliderSize = 0.1f;
		_leftControllerPhysics = new Node(dynamics->context());
		_leftControllerPhysics->addComponent(new Transform(bg::math::Matrix4::Translation(0.0f, -1.5f, 0.0f)));
		_leftControllerPhysics->addComponent(new Collider(new bg::physics::BoxColliderShape(colliderSize)));
		_leftControllerPhysics->addComponent(new RigidBody());
		_leftControllerPhysics->rigidBody()->body()->setMass(0.0f);
		_leftControllerPhysics->rigidBody()->body()->setLinearFactor(bg::math::Vector3());
		_leftControllerPhysics->rigidBody()->body()->setAngularFactor(bg::math::Vector3());
		_leftControllerPhysics->addComponent(new ControllerPhysicsComponent(true));
	//	_leftControllerPhysics->addComponent(PrimitiveFactory(dynamics->context()).cube(colliderSize));
		_rightControllerPhysics = new Node(dynamics->context());
		_rightControllerPhysics->addComponent(new Transform(bg::math::Matrix4::Translation(0.0f, -1.5f, 0.0f)));
		_rightControllerPhysics->addComponent(new Collider(new bg::physics::BoxColliderShape(colliderSize)));
		_rightControllerPhysics->addComponent(new RigidBody());
	//	_rightControllerPhysics->addComponent(PrimitiveFactory(dynamics->context()).cube(colliderSize));
		_rightControllerPhysics->rigidBody()->body()->setMass(0.0f);
		_rightControllerPhysics->rigidBody()->body()->setLinearFactor(bg::math::Vector3());
		_rightControllerPhysics->rigidBody()->body()->setAngularFactor(bg::math::Vector3());
		_rightControllerPhysics->addComponent(new ControllerPhysicsComponent(false));
		dynamics->node()->addChild(_leftControllerPhysics.getPtr());
		dynamics->node()->addChild(_rightControllerPhysics.getPtr());

	}
}

void VRSystem::frame() {
	Controller::UpdateAll();

	_leftCamera->setProjection(getHMDProjection(vr::Eye_Left));
	_leftCamera->transform()->setMatrix(_matDevicePose[vr::k_unTrackedDeviceIndex_Hmd]);
	_leftCamera->transform()->matrix().mult(getHMDPose(vr::Eye_Left));

	_rightCamera->setProjection(getHMDProjection(vr::Eye_Right));
	_rightCamera->transform()->setMatrix(_matDevicePose[vr::k_unTrackedDeviceIndex_Hmd]);
	_rightCamera->transform()->matrix().mult(getHMDPose(vr::Eye_Right));

	updateMatrixPose();
}

void VRSystem::navigateTo(const bg::math::Vector3 & point) {
	using namespace bg::scene;
	Node * stage = bg::base::ObjectRegistry::Get().findObjectOfType<Node>("stage");
	if (stage) {
		bg::math::Vector3 currentPos = stage->transform()->matrix().position();
		bg::math::Vector3 translation = currentPos - point;
		stage->transform()->matrix()
			.identity()
			.translate(translation)
			.scale(_scale, _scale, _scale);
	}
}

void VRSystem::resetNavigation() {
	using namespace bg::scene;
	Node * stage = bg::base::ObjectRegistry::Get().findObjectOfType<Node>("stage");
	if(stage) {
		stage->transform()->matrix()
			.identity()
			.scale(_scale, _scale, _scale);
	}
}

void VRSystem::previewNavigation(const bg::math::Vector3 & point) {
	using namespace bg::scene;
	_navigationTargetNode->setEnabled(true);
	_navigationTargetNode->transform()->matrix()
		.identity()
		.translate(point);
}

void VRSystem::hideNavigationPreview() {
	_navigationTargetNode->setEnabled(false);
}

void VRSystem::draw(bg::render::ClassicRenderer * renderer, bg::scene::Node * sceneRoot) {
	renderer->pipeline()->setRenderSurface(_leftSurface.getPtr());
	renderer->draw(sceneRoot, _leftCamera.getPtr());

	bg::base::Texture * tex = _leftSurface->texture();
	GLuint texName = tex->impl<bg::engine::openglCore::TextureImpl>()->textureName();
	vr::Texture_t leftEyeTex = { (void*)texName, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };

	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTex);

	renderer->pipeline()->setRenderSurface(_rightSurface.getPtr());
	renderer->draw(sceneRoot, _rightCamera.getPtr());

	tex = _rightSurface->texture();
	texName = tex->impl<bg::engine::openglCore::TextureImpl>()->textureName();
	vr::Texture_t rightEyeTex = { (void*)texName, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
	
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTex);
}

void VRSystem::destroy() {
	vr::VR_Shutdown();
}

void VRSystem::initControllers(bg::scene::Node * sceneRoot) {
	using namespace bg::scene;
	bg::base::Context * context = sceneRoot->context();
	for(vr::TrackedDeviceIndex_t index = vr::k_unTrackedDeviceIndex_Hmd + 1; index<vr::k_unMaxTrackedDeviceCount && _controllersFound<2; index += 1) {
		if(_vrSystem->GetTrackedDeviceClass(index) == vr::TrackedDeviceClass_Controller) {
			++_controllersFound;
			std::string sRenderModelName = getTrackedDeviceString(_vrSystem, index, vr::Prop_RenderModelName_String);
			Node * modelNode = new Node(context);
			_rayNode = new Node(context);
			Transform *trx = new Transform;
			trx->matrix().translate(0.0f, -0.025f, 0.0f);
			_rayNode->addComponent(trx);
			
			modelNode->addChild(_rayNode.getPtr());
			modelNode->addComponent(new Transform());
			Controller::AddNew(this,index,modelNode);

			sceneRoot->addChild(modelNode);

			Drawable * modelDrawable = _modelDrawables[sRenderModelName].getPtr();
			if(!modelDrawable) {
				modelDrawable = VRModelFactory(context).loadModel(sRenderModelName);
				_modelDrawables[sRenderModelName] = modelDrawable;
				modelDrawable->setName("Controller A");
				modelNode->addComponent(modelDrawable);
			}
			else {
				modelNode->addComponent(modelDrawable->instance("Controller B"));
			}
		}
	}

	// Target point to preview the navigation positon
	bg::system::Path arrowPath = bg::system::Path::AppDir();
    if (bg::system::currentPlatform() == bg::system::kMac) {
        arrowPath = bg::system::Path::ResourcesDir();
    }
	arrowPath.addComponent("resources/data/arrow.bg2");
	bg::scene::Node * targetNode = new bg::scene::Node(sceneRoot->context());
	targetNode->addComponent(bg::db::loadDrawable(sceneRoot->context(),arrowPath));
	targetNode->component<bg::scene::Drawable>()->material(0)->setDiffuse(bg::math::Color::Red());
	targetNode->component<bg::scene::Drawable>()->material(0)->setReceiveShadows(false);
	targetNode->component<bg::scene::Drawable>()->material(0)->setReflectionAmount(0.3f);
	targetNode->addComponent(new bg::scene::Transform());
	targetNode->addComponent(new ArrowComponent());
	_navigationTargetNode = new bg::scene::Node(sceneRoot->context());
	_navigationTargetNode->setEnabled(true);
	_navigationTargetNode->addComponent(new bg::scene::Transform());
	_navigationTargetNode->addChild(targetNode);
	_navigationTargetNode->setEnabled(false);
	sceneRoot->addChild(_navigationTargetNode.getPtr());
}

void VRSystem::initCameras(bg::scene::Node * sceneRoot) {
	using namespace bg::scene;
	using namespace bg::math;
	bg::base::Context * context = sceneRoot->context();
	_vrSystem->GetRecommendedRenderTargetSize(&_renderWidth, &_renderHeight);

	Node * headNode = new Node(context);
	_headNode = headNode;
	sceneRoot->addChild(headNode);

	Node * leftCameraNode = new Node(context);
	leftCameraNode->setId("left-camera");
	leftCameraNode->addComponent(new Camera());
	leftCameraNode->addComponent(new Transform());
	_leftCamera = leftCameraNode->camera();
	headNode->addChild(leftCameraNode);
	_leftCamera->setProjection(getHMDProjection(vr::Eye_Left));
	_leftCamera->setViewport(Viewport(0, 0, _renderWidth, _renderHeight));

	_leftSurface = new bg::base::TextureRenderSurface(context);
	_leftSurface->create();
	_leftSurface->setSize(bg::math::Size2Di(_renderWidth, _renderHeight));

	Node * rightCameraNode = new Node(context);
	rightCameraNode->setId("right-camera");
	rightCameraNode->addComponent(new Camera());
	rightCameraNode->addComponent(new Transform());
	_rightCamera = rightCameraNode->camera();
	headNode->addChild(rightCameraNode);
	_rightCamera->setProjection(getHMDProjection(vr::Eye_Right));
	_rightCamera->setViewport(Viewport(0, 0, _renderWidth, _renderHeight));

	_rightSurface = new bg::base::TextureRenderSurface(context);
	_rightSurface->create();
	_rightSurface->setSize(bg::math::Size2Di(_renderWidth, _renderHeight));
}

void VRSystem::updateMatrixPose() {
	vr::VRCompositor()->WaitGetPoses(_trackedDevicePose, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
	_validPoseCount = 0;
	_poseClasses = "";
	for(auto device = 0; device<vr::k_unMaxTrackedDeviceCount; ++device) {
		if(_trackedDevicePose[device].bPoseIsValid) {
			++_validPoseCount;
			_matDevicePose[device] = fromSteamVRMatrix(_trackedDevicePose[device].mDeviceToAbsoluteTracking);
			if(_deviceClass[device] == 0) {
				switch(_vrSystem->GetTrackedDeviceClass(device)) {
				case vr::TrackedDeviceClass_Controller:			_deviceClass[device] = 'C'; break;
				case vr::TrackedDeviceClass_HMD:				_deviceClass[device] = 'H'; break;
				case vr::TrackedDeviceClass_Invalid:			_deviceClass[device] = 'I'; break;
			//	case vr::TrackedDeviceClass_Other:				_deviceClass[device] = 'O'; break;
				case vr::TrackedDeviceClass_TrackingReference:	_deviceClass[device] = 'T'; break;
				default:										_deviceClass[device] = '?';
				}
			}
			_poseClasses += _deviceClass[device];
			if(Controller::Get(device)) {
				Controller * ctrl = Controller::Get(device);
				ctrl->updatePose(_matDevicePose[device]);
				using namespace bg::scene;
				bg::math::Matrix4 initialTrx;
				initialTrx.identity();
				Node * stage = bg::base::ObjectRegistry::Get().findObjectOfType<Node>("stage");
				if(stage) {
					initialTrx = stage->transform()->matrix();
					initialTrx.invert();
				}
				initialTrx.mult(_matDevicePose[device]);
				
				if (ctrl->index() == Controller::LeftControllerIndex() && _leftControllerPhysics.valid() && _leftControllerPhysics->rigidBody()) {
					_leftControllerPhysics->rigidBody()->body()->setTransform(initialTrx);
				}
				else if (ctrl->index() == Controller::RightControllerIndex() && _rightControllerPhysics.valid() && _rightControllerPhysics->rigidBody())
					_rightControllerPhysics->rigidBody()->body()->setTransform(initialTrx); {
				}
			}
		}
	}

	if(_trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
		_hdmPose = _matDevicePose[vr::k_unTrackedDeviceIndex_Hmd];
	}
}

void VRSystem::processVREvent(const vr::VREvent_t & event) {
	using namespace bg::scene;
	using namespace bg::math;
	Controller * controller = nullptr;
	Controller::EventType type = Controller::kEventNone;
	Controller::ButtonIdentifier button = Controller::kButtonIdNone;
	if(event.eventType == vr::VREvent_ButtonPress) {
		controller = Controller::Get(event.trackedDeviceIndex);
		if (controller) {
			switch (event.data.controller.button) {
			case vr::k_EButton_ApplicationMenu:
				controller->_menuButton = Controller::kButtonPressed;
				button = Controller::kButtonIdMenu;
				break;
			case vr::k_EButton_Axis0:
				controller->_trackpadButton = Controller::kButtonPressed;
				button = Controller::kButtonIdTouchpad;
				break;
			case vr::k_EButton_Axis1:
				controller->_triggerButton = Controller::kButtonPressed;
				button = Controller::kButtonIdTrigger;
				break;
			case vr::k_EButton_Dashboard_Back:
				controller->_sideButton = Controller::kButtonPressed;
				button = Controller::kButtonIdSide;
				break;
			}
			type = Controller::kEventButtonPress;
		}
	}
	else if(event.eventType == vr::VREvent_ButtonUnpress) {
		controller = Controller::Get(event.trackedDeviceIndex);
		if(controller) {
			switch(event.data.controller.button) {
			case vr::k_EButton_ApplicationMenu:
				controller->_menuButton = Controller::kButtonReleased;
				button = Controller::kButtonIdMenu;
				break;
			case vr::k_EButton_Axis0:
				controller->_trackpadButton = Controller::kButtonReleased;
				button = Controller::kButtonIdTouchpad;
				break;
			case vr::k_EButton_Axis1:
				controller->_triggerButton = Controller::kButtonReleased;
				button = Controller::kButtonIdTrigger;
				break;
			case vr::k_EButton_Dashboard_Back:
				controller->_sideButton = Controller::kButtonReleased;
				button = Controller::kButtonIdSide;
				break;
			}
			type = Controller::kEventButtonRelease;
		}
	}
	else if(event.eventType == vr::VREvent_ButtonTouch) {
		controller = Controller::Get(event.trackedDeviceIndex);
		if(controller) {
			switch(event.data.controller.button) {
			case vr::k_EButton_ApplicationMenu:
				controller->_menuButton = Controller::kButtonTouched;
				button = Controller::kButtonIdMenu;
				break;
			case vr::k_EButton_Axis0:
				controller->_trackpadButton = Controller::kButtonTouched;
				button = Controller::kButtonIdTouchpad;
				break;
			case vr::k_EButton_Axis1:
				controller->_triggerButton = Controller::kButtonTouched;
				button = Controller::kButtonIdTrigger;
				break;
			case vr::k_EButton_Dashboard_Back:
				controller->_sideButton = Controller::kButtonTouched;
				button = Controller::kButtonIdSide;
				break;
			}
			type = Controller::kEventButtonTouch;
		}
	}
	else if(event.eventType == vr::VREvent_ButtonUntouch) {
		controller = Controller::Get(event.trackedDeviceIndex);
		if(controller) {
			switch(event.data.controller.button) {
			case vr::k_EButton_ApplicationMenu:
				controller->_menuButton = Controller::kButtonReleased;
				button = Controller::kButtonIdMenu;
				break;
			case vr::k_EButton_Axis0:
				controller->_trackpadButton = Controller::kButtonReleased;
				button = Controller::kButtonIdTouchpad;
				break;
			case vr::k_EButton_Axis1:
				controller->_triggerButton = Controller::kButtonReleased;
				button = Controller::kButtonIdTrigger;
				break;
			case vr::k_EButton_Dashboard_Back:
				controller->_sideButton = Controller::kButtonReleased;
				button = Controller::kButtonIdSide;
				break;
			}
			type = Controller::kEventButtonUntouch;
		}
	}
	if (controller && _controllerClosure) {
		_controllerClosure(controller,type,button);
	}
}

std::string VRSystem::getTrackedDeviceString(vr::IVRSystem * vr, vr::TrackedDeviceIndex_t dev, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError * peError) {
	uint32_t unRequiredBufferLength = vr->GetStringTrackedDeviceProperty(dev, prop, nullptr, 0, peError);
	if(unRequiredBufferLength == 0) return "";
	char * pchBuffer = new char[unRequiredBufferLength];
	unRequiredBufferLength = vr->GetStringTrackedDeviceProperty(dev, prop, pchBuffer, unRequiredBufferLength, peError);
	std::string result = pchBuffer;
	delete[] pchBuffer;
	return result;
}


bg::math::Matrix4 VRSystem::getHMDProjection(vr::Hmd_Eye eye) {
	vr::HmdMatrix44_t mat = _vrSystem->GetProjectionMatrix(eye, _near, _far);
	return bg::math::Matrix4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
}

bg::math::Matrix4 VRSystem::getHMDPose(vr::Hmd_Eye eye) {
	vr::HmdMatrix34_t matEye = _vrSystem->GetEyeToHeadTransform(eye);
	bg::math::Matrix4 result(
		matEye.m[0][0], matEye.m[1][0], matEye.m[2][0], 0.0,
		matEye.m[0][1], matEye.m[1][1], matEye.m[2][1], 0.0,
		matEye.m[0][2], matEye.m[1][2], matEye.m[2][2], 0.0,
		matEye.m[0][3], matEye.m[1][3], matEye.m[2][3], 1.0f
	);
	return result;
}
