#include "App.h"
#include <resFinder.hpp>

namespace ts
{

App App::s_app;

void App::setViewport(int x, int y, int w, int h)
{
	_viewport.set(x, y, w, h);
}

int App::launchVR()
{

	this->destroy();

	bg::ptr<bg::wnd::Window> window = bg::wnd::Window::New();

	if (!window.valid()) {
		std::cerr << "Platform not supported" << std::endl;
		return -1;
	}
	_window = window;

	window->setRect(bg::math::Rect(1920 / 2 - 400, 1080 / 2 - 400, 800, 800));
	window->setTitle("TestSite");


	std::cout << "HEY LETS ChecK IF VR" << std::endl;
	VRSystem *vrSystem = new VRSystem();

	if(this->isVR())
	{
		window->setIcon(ts::resources.icon.vr);
		_eventHandler = new MainEventHandler(vrSystem);
	}
	else
	{
		window->setIcon(ts::resources.icon.vr);
		_eventHandler = new ts::dsk::DskEventHandler();
		std::cout << "OH ITS VR::: JEJE NOT" << std::endl;
	}
	
	window->create();
	bg::wnd::MainLoop::Get()->setWindow(window.getPtr());
	setViewport(0, 0, window->rect().w(), window->rect().z());
	return 0;

}

App::App()
{
	_isVR = false;
}

int App::run()
{
	Get().createWnd();
	int status = bg::wnd::MainLoop::Get()->run();
	this->destroy();
	return status;
}

int App::runVR()
{
	Get().launchVR();
	int status = bg::wnd::MainLoop::Get()->run();
	this->destroy();
	return status;
}

void App::init(bg::base::Context *ctx)
{
	_ctx = ctx;
	loadPlugins();
	_guiManager = new gui3d::GuiManager();
	this->initLeap();
	createInitialScene();
}

std::string App::sessionTimeStr() const
{
	uint64_t aux = this->sessionTime();
	std::vector<std::string> times = { "day","hour","minute","second" };
	std::vector<uint64_t> values(4,0);
	//DAYS
	values[0] = aux / (24 * 3600);
	aux = aux % (24 * 3600);
	//HOURS
	values[1] = aux / 3600;
	aux = aux % 3600;
	//MINUTES
	values[2] = aux / 60;
	aux = aux % 60;
	//SECONS
	values[3] = aux;
	std::string out;
	for (int i = 0; i < values.size(); i++)
	{
		if (values[i] != 0)
		{
			out += std::to_string(values[i]);
			out += " ";
			out += times[i];
			if (values[i] > 1)
			{
				out += "s";
			}
			out += " ";
		}
	}
	if(out.size()>0)
		out.pop_back();
	return out;
	
}

int App::initLeap()
{
	_leap_controller.setPolicy(Leap::Controller::POLICY_ALLOW_PAUSE_RESUME);
	_leap_controller.enableGesture(Leap::Gesture::TYPE_CIRCLE);
	_leap_controller.enableGesture(Leap::Gesture::TYPE_SWIPE, true);
	return 0; //OK
}

void App::destroy()
{
	_ctx = nullptr;
	_scene = nullptr;
	_eventHandler = nullptr;
	_window = nullptr;
	if(_guiManager.valid())
	_guiManager->clear();
	_guiManager = nullptr;
	
}

int App::createWnd()
{
	{
		bg::ptr<bg::wnd::Window> window = bg::wnd::Window::New();

		if (!window.valid()) {
			std::cerr << "Platform not supported" << std::endl;
			return -1;
		}
		_window = window;
		
		window->setRect(bg::math::Rect(1920/2-400,1080/2-400,800,800));
		window->setTitle("TestSite");
		

		/*std::cout << "HEY LETS ChecK IF VR" << std::endl;
		VRSystem *vrSystem = new VRSystem();

		if(this->isVR())
		{
			window->setIcon(ts::resources.icon.vr);
			_eventHandler = new MainEventHandler(vrSystem);		
		}
		else
		{*/
		window->setIcon(ts::resources.icon.main);
		_eventHandler = new ts::dsk::DskEventHandler();
	//	}

		window->setEventHandler(_eventHandler.getPtr());
		window->create();
		bg::wnd::MainLoop::Get()->setWindow(window.getPtr());
		setViewport(0, 0, window->rect().w(), window->rect().z());
	}
	return 0;
}

void App::createInitialScene()
{
	_guiManager->clear();
	_guiManager = nullptr;
	_guiManager = new gui3d::GuiManager();
	_scene = nullptr;
	_scene = new Scene(context());
	_scene->createTestScene();
	_scene->mainCamera()->setViewport(_viewport);
	_scene->sceneRoot()->addComponent(new AppComponent);
}



App::~App()
{
	_ctx = nullptr;
}

void App::loadPlugins()
{
	bg::db::AudioLoader::RegisterPlugin(new bg::db::plugin::ReadWavAudio());

	bg::db::DrawableLoader::RegisterPlugin(new bg::db::plugin::ReadDrawableBg2());
	bg::db::DrawableLoader::RegisterPlugin(new bg::db::plugin::ReadDrawableObj());
	bg::db::DrawableWriter::RegisterPlugin(new bg::db::plugin::WriteDrawableBg2());

	bg::db::FontLoader::RegisterPlugin(new bg::db::plugin::ReadFontTrueType());

	bg::db::NodeLoader::RegisterPlugin(new bg::db::plugin::ReadScene());
	bg::db::NodeLoader::RegisterPlugin(new bg::db::plugin::ReadPrefabBg2());
	bg::db::NodeWriter::RegisterPlugin(new bg::db::plugin::WritePrefabBg2());
	bg::db::NodeWriter::RegisterPlugin(new bg::db::plugin::WriteScene());

	bg::db::GuiSkinLoader::RegisterPlugin(new bg::db::plugin::ReadGuiSkinBg2());
}


}