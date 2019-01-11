#pragma once


#include <event_handler_dsk.hpp>
#include <vr/event_handler.hpp>
#include <bg\bg2e.hpp>
#include <app\Scene.h>
#include <app\AppComponents.hpp>
#include <GeneralConstants.h>
#include <gui3d\Widget.h>
#include <gui3d\GuiManager.h>
#include <extra\Clock.hpp>
#include <Leap.h>



namespace ts
{


class App
{
public:
	
	static App & Get() { return s_app; }

	int run();
	int runVR();

	void init(bg::base::Context *);
	int createWnd();

	void createInitialScene();

	void setViewport(int x, int y, int w, int h);
	//VR FUNCT
	void setVR(bool v) { _isVR = v; }
	bool isVR() { return _isVR; }
	int launchVR();

	//POINTER COMPONENTS
	inline bg::base::Context * context() { return _ctx.getPtr(); }
	inline bg::base::EventHandler * eventHandler() { return _eventHandler.getPtr(); }
	inline Scene *scene() { return _scene.getPtr(); }
	inline gui3d::GuiManager* guiManager() { return _guiManager.getPtr(); }
	inline bg::math::Viewport& viewport() { return _viewport; }

	//TIME FUNCIONS
	uint64_t sessionTime() const { return _timeCounter.sessionTime(); }
	std::string sessionTimeStr() const;

	LocalTime* localTime(){ return &_localTime; }

	Leap::Controller* leapController() { return &_leap_controller; }
	int initLeap();

	void destroy();

protected:

	App();
	virtual ~App();
	static App s_app;

	bool _isVR;

	bg::ptr<bg::base::Context> _ctx;
	bg::ptr<bg::wnd::Window> _window;
	bg::ptr<bg::base::EventHandler> _eventHandler;
	bg::ptr<Scene> _scene;
	bg::math::Viewport _viewport;
	bg::ptr<gui3d::GuiManager> _guiManager;
	void loadPlugins();
	
	sessionTimeCounter _timeCounter;
	LocalTime _localTime;

	Leap::Controller _leap_controller;
};

}