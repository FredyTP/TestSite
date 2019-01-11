#ifndef _controller_hpp_
#define _controller_hpp_

#include <bg/scene/node.hpp>
#include <bg/base/custom_event.hpp>

class NavigationController : public bg::scene::Component {
public:
	NavigationController(int controllerIndex);

	bg::scene::Component * clone();
	void init();
	void frame(float);
	void draw(bg::base::Pipeline * pipeline);
	void customEvent(const bg::base::CustomEvent &evt);

protected:
	virtual ~NavigationController();

	int _controllerIndex;
	bool _showRay;

	void previewNavigation();
	void navigate();

	bg::math::Vector3 _rayDirection;
};

class VRSystem;
class Controller : public bg::base::ReferencedPointer {
	friend class VRSystem;
public:
	enum EventType {
		kEventNone = 0,
		kEventButtonPress = 1,
		kEventButtonRelease,
		kEventButtonTouch,
		kEventButtonUntouch
	};
	enum ButtonStatus {
		kButtonReleased = 0,
		kButtonTouched,
		kButtonPressed
	};

	enum ButtonIdentifier {
		kButtonIdNone = 0,
		kButtonIdTrigger = 1,
		kButtonIdSide,
		kButtonIdMenu,
		kButtonIdTouchpad
	};

	static Controller * Get(uint32_t index);
	static Controller * AddNew(VRSystem * sys, uint32_t index, bg::scene::Node * n);
	static void UpdateAll();
	static Controller * Left();
	static Controller * Right();
	static uint32_t LeftControllerIndex();
	static uint32_t RightControllerIndex();
	static void Destroy();

	void update();

	void updatePose(const bg::math::Matrix4 &);

	inline bg::scene::Node * sceneNode() { return _sceneNode.getPtr(); }

	inline uint32_t index() const { return _index; }
	inline const bg::math::Vector2 & trackpadPosition() const { return _trackpadPosition; }
	inline float triggerPosition() const { return _triggerPosition; }
	float trackpadAngle();

	inline ButtonStatus triggerButtonStatus() const { return _triggerButton; }
	inline ButtonStatus trackpadButtonStatus() const { return _trackpadButton; }
	inline ButtonStatus sideButton() const { return _sideButton; }
	inline ButtonStatus menuButton() const { return _menuButton; }

protected:
	Controller(VRSystem *, uint32_t index, bg::scene::Node *);
	virtual ~Controller() {}

	VRSystem * _vrSystem;
	uint32_t _index;
	bg::ptr<bg::scene::Node> _sceneNode;
	ButtonStatus _triggerButton = kButtonReleased;
	ButtonStatus _trackpadButton = kButtonReleased;
	ButtonStatus _sideButton = kButtonReleased;
	ButtonStatus _menuButton = kButtonReleased;

	static std::map<uint32_t,bg::ptr<Controller>> s_controllers;
	static uint32_t s_leftControllerIndex;
	static uint32_t s_rightControllerIndex;

	bg::math::Vector2 _trackpadPosition;
	float _triggerPosition;
};

class ControllerEventData : public bg::base::CustomEventData {
public:
	ControllerEventData(Controller * c, Controller::EventType evtType, Controller::ButtonIdentifier id) :_controller(c), _type(evtType), _button(id) {}

	inline Controller * controller() { return _controller; }
	inline const Controller * controller() const { return _controller; }
	inline Controller::EventType eventType() const { return _type; }
	inline Controller::ButtonIdentifier button() const { return _button; }

protected:
	virtual ~ControllerEventData() {}
	Controller * _controller;
	Controller::EventType _type;
	Controller::ButtonIdentifier _button;
};

#endif