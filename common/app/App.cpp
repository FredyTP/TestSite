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
		window->setIcon(ts::resources.icon.vr.text());
		_eventHandler = new MainEventHandler(vrSystem);
	}
	else
	{
		window->setIcon(ts::resources.icon.vr.text());
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
	_lfManager.run();
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
#ifdef _LEAP_H
	this->initLeap();
#endif //_LEAP_H
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

#ifdef _LEAP_H
int App::initLeap()
{
	_leap_controller.setPolicy(Leap::Controller::POLICY_ALLOW_PAUSE_RESUME);
	_leap_controller.enableGesture(Leap::Gesture::TYPE_CIRCLE);
	_leap_controller.enableGesture(Leap::Gesture::TYPE_SWIPE, true);
	return 0; //OK
}
#endif
void App::destroy()
{
	bg::system::Path path = bg::system::Path::AppDir();
	path.addComponent("._tsRunningFile.temp");
	path.remove();
	_lfManager.stop();

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
		

		std::cout << "HEY LETS ChecK IF VR" << std::endl;
		VRSystem *vrSystem = new VRSystem();

		if(this->isVR())
		{
			window->setIcon(ts::resources.icon.vr.text());
			_eventHandler = new MainEventHandler(vrSystem);		
		}
		else
		{
			window->setIcon(ts::resources.icon.main.text());
			_eventHandler = new ts::dsk::DskEventHandler();
		}

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

int App::processArgs(int argc, char* argv[])
{
	bg::system::Path path = bg::system::Path::AppDir();
	path.addComponent("._tsRunningFile.temp");
	//CHECKS IF THE RUNNING FILE HAS BEEN CREATED
	//IF IT EXIST, MEANS THAT TESTSITE IS ALREADY RUNNING
	if (path.exists())
	{
		bg::wnd::MessageBox box;
		if (argc == 1)
			box.Show(nullptr, "Error", "TestSite is Already Running");
		else
		{

			//CREATES A FILE TO INDICATE THAT SOMETHING HAS TO BE LOADED ON MAIN APP;
			bg::system::Path toLoadPath = bg::system::Path::AppDir();
			toLoadPath.addComponent(".toLoadArchive.txt");

			std::ifstream fileQueue;
			int tryCont = 0;
			using namespace std::chrono_literals;
			do
			{
				fileQueue.open(toLoadPath.text());
				tryCont++;
				std::this_thread::sleep_for(100ms);
				if (tryCont > 20)
				{
					box.Show(nullptr, "Error", "Not Able To Create The File, Queue is full");
					return -2;
				}

			} while (fileQueue.is_open());
			std::ofstream file_to_load;

			file_to_load.open(toLoadPath.text());
			if (file_to_load.is_open())
			{
				file_to_load << argv[1] << std::endl;
			}
			file_to_load.close();
		}
		return -1;
	}
	else
	{
		//CREATES THE FILE THAT SAYS WE ARE RUNNING
		std::ofstream file;
		file.open(path.text());
		file.close();
		if (argc > 1)
		{
			//OPEN THE FILE
		}
	}
	return 1;

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