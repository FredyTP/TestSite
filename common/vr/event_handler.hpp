
#ifndef _event_handler_hpp_
#define _event_handler_hpp_

#include <bg/bg2e.hpp>

#include <openvr.h>

#include <vr\vr_system.hpp>


class MainEventHandler : public bg::base::EventHandler {
public:
	MainEventHandler(VRSystem* vrSys);
	
	void willCreateContext();
	
	void initGL();
	void reshape(int,int);
	void destroy();
	void frame(float);
	void draw();
	void keyUp(const bg::base::KeyboardEvent &);
	void keyDown(const bg::base::KeyboardEvent &);
	void mouseDown(const bg::base::MouseEvent &);
	void mouseUp(const bg::base::MouseEvent &);
	void mouseMove(const bg::base::MouseEvent &);
	void mouseDrag(const bg::base::MouseEvent &);


	inline bg::scene::InputVisitor * input() { return _inputVisitor.getPtr(); }

	void openFile(const bg::system::Path &);

	bg::scene::Node * stage();

protected:
	virtual ~MainEventHandler();

	bg::base::FPSCounter _fps;

	VRSystem * _vrSystem;

	bg::ptr<bg::render::ClassicRenderer> _renderer;
	bg::ptr<bg::scene::Node> _sceneRoot;
	bg::ptr<bg::scene::Node> _stage;
	bg::ptr<bg::scene::InputVisitor> _inputVisitor;

	void initScene();
	
	bg::ptr<bg::base::Pipeline> _pipeline;
	bg::ptr<bg::wnd::PopUpMenu> _menu;
	bg::ptr<bg::scene::Dynamics> _dynamics;

	
	bg::scene::Node * sceneRoot();

	bg::render::ClassicRenderer * createRenderer();
};

#endif
