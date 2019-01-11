#ifndef _event_handler_dsk_hpp_
#define _event_handler_dsk_hpp_

//TestSite
#include <app\SceneConsole.h>
#include <var\TestSiteVariables.hpp>
#include <var\TextBox.hpp>
#include <var\geoVarManager.h>
#include <var\geoNodeManager.hpp>
#include <extra\Primitives.h>
#include <mesh/mesh.hpp>

#include <Leap/Listener.hpp>

//Bg2e
#include <bg\bg2e.hpp>
#include <bg\net\download.hpp>

//std
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>
#include <exception>
namespace ts
{


namespace dsk
{

class DskEventHandler : public bg::base::EventHandler {
public:
	DskEventHandler();
	virtual void willCreateContext();
	virtual void willDestroyContext();
	// Common events
	void initGL();
	void reshape(int, int);
	void frame(float);
	void draw();

	// Desktop events
	void keyUp(const bg::base::KeyboardEvent &);
	void keyDown(const bg::base::KeyboardEvent &);
	void charPress(const bg::base::KeyboardEvent &);
	void mouseDown(const bg::base::MouseEvent &);
	void mouseDrag(const bg::base::MouseEvent &);
	void mouseMove(const bg::base::MouseEvent &);
	void mouseUp(const bg::base::MouseEvent &);
	void mouseWheel(const bg::base::MouseEvent &);

	// Mobile devices events
	void onMemoryWarning();
	void touchStart(const bg::base::TouchEvent &);
	void touchMove(const bg::base::TouchEvent &);
	void touchEnd(const bg::base::TouchEvent &);
	void sensorEvent(const bg::base::SensorEvent &);

	virtual void buildMenu(bg::wnd::MenuDescriptor & menu);
	virtual void menuSelected(const std::string & title, int32_t identifier);


	void loadScene(std::function<void(void)> AppFunc);
protected:
	virtual ~DskEventHandler();
	void initGui();
	std::string selectedInfoString(geoVarHandler *var);
	bg::ptr<bg::render::Renderer> _renderer;
	bg::ptr<bg::scene::Node> _sceneRoot;
	bg::ptr<bg::scene::Camera> _camera;
	bg::ptr<bg::scene::InputVisitor> _inputVisitor;

	//GUI
	bg::ptr<bg::gui::Surface> _guiSurface;

	bg::ptr<bg::gui::Label> _fpsLabel;
	bg::base::FPSCounter _fpsCounter;
	bg::ptr<bg::gui::Label> _selectedInfo;

	bg::ptr<bg::wnd::PopUpMenu> _selectedMenu;
	bool wasSelected = true;
	

};


}
}

#endif