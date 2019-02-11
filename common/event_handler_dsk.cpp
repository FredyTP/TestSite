#include <event_handler_dsk.hpp>
#include <app\App.h>
#include <resFinder.hpp>
#include <bg/wnd/win32_Window.hpp>

namespace ts
{
namespace dsk
{
	



DskEventHandler::DskEventHandler()
{

}

void DskEventHandler::rightClickMenu(const bg::base::MouseEvent & evt)
{

	//TRY TO CHANGE THE CODE OF BG2 BACK TO TFM_LEFTCLICK//
	 _clickMenu= bg::wnd::PopUpMenu::New();
	 _clickMenu->setPosition(evt.pos());
	 _clickMenu->addItem("open", 1000);
	 _clickMenu->show([&](bg::wnd::MenuItemIdentifier id)
	{
		if (id == 1000)
		{
			bg::ptr<bg::wnd::FileDialog> fileDialog = bg::wnd::FileDialog::OpenFileDialog();
			fileDialog->addFilter("txt");
			if (fileDialog->show()) {
				std::cout << fileDialog->getResultPath() << std::endl;
				std::cout << "WTFFF" << std::endl;
			}
			else
			{
				std::cout << "mo way WTFFF" << std::endl;
			}
		}
	});
}

DskEventHandler::~DskEventHandler() {
}

void DskEventHandler::initGui()
{
	using namespace bg::gui;
	bg::system::Path path = bg::system::Path::AppDir();
	path.addComponent("resources/data/default.bg2skin");
	bg::ptr<Skin> skin = bg::db::loadSkin(context(),path);

	_guiSurface = new Surface(context(), skin.getPtr());
	_guiSurface->setId("surface");



	//FPS COUNTER
	Window *wnd = new Window(_guiSurface.getPtr());
	wnd->create();
	wnd->setSize(bg::math::Size2Di(200, 50));
	wnd->setAlignment(bg::gui::kAlignTopRight);
	wnd->setVisible(true);

	_fpsLabel = View::Create<Label>(context(), skin.getPtr());
	_fpsLabel->paragraph().setAlignment(bg::text::Paragraph::kVAlignCenter);
	_fpsLabel->paragraph().setAlignment(bg::text::Paragraph::kHAlignCenter);
	_fpsLabel->setFont(bg::db::loadFont(context(), ts::resources.font.nunito_black, 10));
	_fpsLabel->setSize(bg::math::Size2Di(90, 50));
	_fpsLabel->setPosition(bg::math::Position2Di(90, 0));

	_netLabel = View::Create<Label>(context(), skin.getPtr());
	_netLabel->paragraph().setAlignment(bg::text::Paragraph::kVAlignCenter);
	_netLabel->paragraph().setAlignment(bg::text::Paragraph::kHAlignCenter);
	_netLabel->setFont(bg::db::loadFont(context(), ts::resources.font.nunito_black, 11));
	_netLabel->setSize(bg::math::Size2Di(90, 50));
	_netLabel->setPosition(bg::math::Position2Di(0, 0));
	_netLabel->setText("Net: Off");
	_netLabel->setTextColor(bg::math::Color(1.0f, 0.4f, 0.4f, 1.0f));

	wnd->contentView()->setBackgroundColor(bg::math::Color::Transparent());
	wnd->contentView()->addChild(_fpsLabel.getPtr());
	wnd->contentView()->addChild(_netLabel.getPtr());

	//INFO OF THE SELECTED TARGET
	wnd = new Window(_guiSurface.getPtr());
	wnd->create();
	wnd->setSize(bg::math::Size2Di(250, 190));
	wnd->setAlignment(bg::gui::kAlignBottomLeft);
	wnd->setVisible(true);

	
	_selectedInfo = View::Create<Label>(context(), skin.getPtr());
	_selectedInfo->paragraph().setAlignment(bg::text::Paragraph::kVAlignBottom);
	_selectedInfo->paragraph().setAlignment(bg::text::Paragraph::kHAlignLeft);
	_selectedInfo->setPosition(bg::math::Position2Di(20, 20));
	_selectedInfo->setFont(bg::db::loadFont(context(), ts::resources.font.nunito_black, 11));
	_selectedInfo->setSize(bg::math::Size2Di(250 - 40, 190 - 40));

	wnd->contentView()->setBackgroundColor(bg::math::Color(0.7f, 0.8f, 1.0f, 0.2f));
	wnd->contentView()->addChild(_selectedInfo.getPtr());	
}

std::string DskEventHandler::selectedInfoString(geoVarHandler * var)
{
	std::string info;
	info = "Name: " + var->variable().name + "\n";
	info += "Graphic State:\n";
	info += "\t->";
	if (var->graphic()->isHidden())
	{
		info += "Hidden";
	}
	else
	{
		info += "Visible";
	}
	info += "\n";
	info += "\t->";
	if (var->graphic()->isRunning())
	{
		info += "Running";
	}
	else
	{
		info += "Paused";
	}
	info += "\n";
	info += "\t->Points: " + std::to_string(var->graphic()->size());

	return info;
}

void DskEventHandler::willCreateContext() {
	if (bg::engine::OpenGLCore::Supported()) {
		bg::Engine::Init(new bg::engine::OpenGLCore());
	}
	else {
		throw new bg::base::CompatibilityException("Your system is not supported.");
	}
}
void DskEventHandler::willDestroyContext() {


	_camera = nullptr;
	_inputVisitor = nullptr;
	_sceneRoot = nullptr;
	_inputVisitor = nullptr;
	_renderer = nullptr;


	ts::App::Get().destroy();

	bg::base::EventHandler::willDestroyContext();
}
void DskEventHandler::initGL() {
	//LEAP CONFIG//


	bg::Engine::Get()->initialize(context());
	_renderer = bg::render::Renderer::Create(context(), bg::render::Renderer::kRenderPathForward);
	_renderer->setRenderSetting("shadowMap", "shadowMapSize", bg::math::Size2Di(2048));

	std::vector<bg::system::Screen> screens = bg::system::Screen::ScreenList();
	for (auto & s : screens) {
		bg::log(bg::log::kDebug) << "Screen " << s.id() << ": "
			<< s.name() << ", res: " << s.size() << ", scale: " << s.scale() << bg::endl;
	}
	ts::App::Get().init(context());

	{
		using namespace bg::scene;
		using namespace bg::math;
		_sceneRoot = ts::App::Get().scene()->sceneRoot();
		_camera = ts::App::Get().scene()->mainCamera();

		_camera->node()->component<bg::manipulation::OrbitNodeController>()->setCenter(_camera->node()->transform()->matrix().position() + bg::math::Vector3(0.0f, 0.0f, -1.0f));
		using namespace bg::math;

		Node * cn = new Node(context());
		
		base = new ts::gui3ds::ControllerMenu("base", 0);
		cn->addComponent(base);
		base->addSubMenu("1", 103);
		base->addSubMenu("2", 2);
		base->addSubMenu("3", 3);
		base->addSubMenu("2", 2);
		base->addSubMenu("3", 3);
		base->addSubMenu("2", 2);
		base->addSubMenu("3", 3);
		base->addSubMenu("2", 2);

		base->subMenu("2")->addSubMenu("21", 21);
		base->subMenu("2")->addSubMenu("22", 22);
		base->subMenu("2")->addSubMenu("23", 23);
		base->subMenu("2")->subMenu("22")->addSubMenu("221", 221);
		base->subMenu("2")->subMenu("22")->addSubMenu("222", 222);

		base->subMenu("3")->addSubMenu("31", 31);
		base->subMenu("3")->addSubMenu("32", 32);
		base->subMenu("3")->addSubMenu("33", 33);
		base->subMenu("3")->addSubMenu("34", 34);

		_sceneRoot->addChild(cn);
		base->close();
		_inputVisitor = new bg::scene::InputVisitor;
	}
	std::cout << "INNIT GUI" << std::endl;
	this->initGui();
	std::cout << "INNIT GLED" << std::endl;


}
void DskEventHandler::frame(float delta) {
	_renderer->frame(_sceneRoot.getPtr(), delta);

	_fpsCounter.frame(delta);
	if (_fpsCounter.valueUpdated())
	{
		_fpsLabel->setText("FPS: " + std::to_string(_fpsCounter.fps()));
		if(_fpsCounter.fps() >= 60)
		{
			_fpsLabel->setTextColor(bg::math::Color(0.1f, 1.0f, 0.1f, 1.0f));
		}
		else if (_fpsCounter.fps() < 60 && _fpsCounter.fps() >= 24)
		{
			_fpsLabel->setTextColor(bg::math::Color(1.0f, 0.6f, 0.05f, 1.0f));
		}
		else
		{
			_fpsLabel->setTextColor(bg::math::Color(1.0f, 0.01f, 0.01f, 1.0f));
		}
	}

	//NOT OPTIMIZED!!!!! OPTIMIZE... IN A FUTURE ... MAYBE... NOT REALLY IMPORTANT... :)
	geoVarHandler *selection = ts::App::Get().scene()->geoManager()->selected();
	if (ts::App::Get().scene()->geoManager()->manager()->isConnected())
	{
		if (!wasConnected)
		{
			wasConnected = true;
			_netLabel->setText("Net: On");
			_netLabel->setTextColor(bg::math::Color(0.1f, 1.0f, 0.1f, 1.0f));
			
		}
	}
	else
	{
		if (wasConnected)
		{
			wasConnected = false;
			_netLabel->setText("Net: Off");
			_netLabel->setTextColor(bg::math::Color(1.0f, 0.01f, 0.01f, 1.0f));
		}
	}

	if (selection == nullptr)
	{
		_selectedInfo->setText("");
		_selectedInfo->parentNode()->hide();
		if (wasSelected)
		{
			wasSelected = false;
			_selectedMenu->setEnabled(false);
			bg::wnd::MainLoop::Get()->window()->drawMenuBar();
		}
	}
	else
	{
		_selectedInfo->parentNode()->show();
		_selectedInfo->setText(this->selectedInfoString(selection));
		if (!wasSelected)
		{
			wasSelected = true;
			_selectedMenu->setEnabled(true);
			bg::wnd::MainLoop::Get()->window()->drawMenuBar();
		}
	}



}
void DskEventHandler::reshape(int w, int h) {
	_camera->setViewport(bg::math::Viewport(0, 0, w, h));
	_guiSurface->resize(w, h);
	ts::App::Get().setViewport(0, 0, w, h);

}

void DskEventHandler::draw() {
	_renderer->draw(_sceneRoot.getPtr(), _camera.getPtr());
	_guiSurface->draw();
	context()->swapBuffers();
}
void DskEventHandler::onMemoryWarning()
{
}
void DskEventHandler::touchStart(const bg::base::TouchEvent & evt) {
	if (!_guiSurface->touchStart(evt)) {
		_inputVisitor->touchStart(_sceneRoot.getPtr(), evt);
	}
}

void DskEventHandler::touchMove(const bg::base::TouchEvent & evt) {
	if (!_guiSurface->touchMove(evt)) {
		_inputVisitor->touchMove(_sceneRoot.getPtr(), evt);
	}
}

void DskEventHandler::touchEnd(const bg::base::TouchEvent & evt) {
	if (!_guiSurface->touchEnd(evt)) {
		_inputVisitor->touchEnd(_sceneRoot.getPtr(), evt);
	}
}

void DskEventHandler::sensorEvent(const bg::base::SensorEvent &)
{
}
// Desktop events
void DskEventHandler::keyUp(const bg::base::KeyboardEvent & evt) {
	if (evt.keyboard().key() == bg::base::Keyboard::kKeyEsc) {
		bg::wnd::MainLoop::Get()->quit(0);
	}
	else if (!_guiSurface->keyUp(evt)) {
		_inputVisitor->keyUp(_sceneRoot.getPtr(), evt);
	}
}

void DskEventHandler::keyDown(const bg::base::KeyboardEvent & evt) {
	if (!_guiSurface->keyDown(evt)) {
		if (evt.keyboard().key() == evt.keyboard().kKeyO)
		{
			//base->open();
		}
		_inputVisitor->keyDown(_sceneRoot.getPtr(), evt);
	}
}

void DskEventHandler::charPress(const bg::base::KeyboardEvent & evt) {
	if (!_guiSurface->charPress(evt)) {
		_inputVisitor->charPress(_sceneRoot.getPtr(), evt);
	}
}

void DskEventHandler::mouseDown(const bg::base::MouseEvent & evt) {
	if (!_guiSurface->mouseDown(evt)) {
		_inputVisitor->mouseDown(_sceneRoot.getPtr(), evt);
		
	}
}

void DskEventHandler::mouseMove(const bg::base::MouseEvent & evt) {
	if (!_guiSurface->mouseMove(evt)) {
		_inputVisitor->mouseMove(_sceneRoot.getPtr(), evt);

	}
}

void DskEventHandler::mouseDrag(const bg::base::MouseEvent & evt) {
	if (!_guiSurface->mouseDrag(evt)) {
		_inputVisitor->mouseDrag(_sceneRoot.getPtr(), evt);
	}
}

void DskEventHandler::mouseWheel(const bg::base::MouseEvent & evt) {
	if (!_guiSurface->mouseWheel(evt)) {
		_inputVisitor->mouseWheel(_sceneRoot.getPtr(), evt);
	}
}

void DskEventHandler::mouseUp(const bg::base::MouseEvent & evt) {
	if (!_guiSurface->mouseUp(evt))
	{
		_inputVisitor->mouseUp(_sceneRoot.getPtr(),evt);
		if (evt.mouse().getReleasedButton() == evt.mouse().kRightButton)
		{
			///this->rightClickMenu(evt);
		}
		
	}
	
}

void DskEventHandler::buildMenu(bg::wnd::PopUpMenu * menu)
{

	//SOON, ADD A MENUS TO CONFIGURATE THE BOXES.. ETC^^// FONT,COLOR,POSITION...IDK U KNOW
	//REIMPLEMENT DE WHOLE SISTEM OF THE MENUS//
	bg::wnd::PopUpMenu * file = bg::wnd::PopUpMenu::New("File");
	file->addItem("Open",bg::wnd::kCodeOpen);
	file->addItem("Save", bg::wnd::kCodeSave);
	file->addItem("Close", bg::wnd::kCodeClose);
	//file->addMenuItem({ bg::wnd::kCodeCustom + 666 , "Launch VR-NotWorking" });

	if (bg::system::currentPlatform() != bg::system::kMac) {
		file->addItem("Exit", bg::wnd::kCodeQuit);
	}
	bg::wnd::PopUpMenu * network = bg::wnd::PopUpMenu::New("Network");
	network->addItem("Connect", bg::wnd::kCodeCustom+107);
	network->addItem("Disconnect", bg::wnd::kCodeCustom + 108);

	_selectedMenu = bg::wnd::PopUpMenu::New("Selection");

	bg::wnd::PopUpMenu * graphic = bg::wnd::PopUpMenu::New("Graphic");
	graphic->addItem("Save", bg::wnd::kCodeCustom + 111);
	graphic->addSubMenu("", bg::wnd::PopUpMenu::kSeparator);
	graphic->addItem("Show", bg::wnd::kCodeCustom + 101);
	graphic->addItem("Hide",bg::wnd::kCodeCustom + 102);
	graphic->addSubMenu("", bg::wnd::PopUpMenu::kSeparator);
	graphic->addItem("Run", bg::wnd::kCodeCustom + 103);
	graphic->addItem("Stop",bg::wnd::kCodeCustom + 104);
	graphic->addSubMenu("", bg::wnd::PopUpMenu::kSeparator);
	graphic->addItem("Clear",bg::wnd::kCodeCustom + 105);

	bg::wnd::PopUpMenu * text = bg::wnd::PopUpMenu::New("Text");
	text->addItem("Color", bg::wnd::kCodeCustom + 106);

	_selectedMenu->addSubMenu(graphic);
	_selectedMenu->addSubMenu(text);

	menu->addSubMenu(file);
	menu->addSubMenu(network);
	menu->addSubMenu(_selectedMenu.getPtr());
}

void DskEventHandler::menuSelected(const std::string & title, int32_t identifier)
{
	std::cout << title << " selected. Identifier: " << identifier << std::endl;
	if (identifier == bg::wnd::kCodeQuit) {
		bg::wnd::MainLoop::Get()->quit(0);
	}
	if (identifier == bg::wnd::kCodeSave)
	{
		bg::ptr<bg::wnd::FileDialog> diag = bg::wnd::FileDialog::SaveFileDialog();
		diag->addFilter("tsvar");
		if (diag->show())
		{
			bg::system::Path path(diag->getResultPath());
			if (path.exists())
			{
				bg::wnd::MessageBox box;
				auto response=box.Show(nullptr, "WARNING", "The file already exists\n Truncate its content?", bg::wnd::MessageBox::kButtonOkCancel);
				switch (response)
				{
				case bg::wnd::MessageBox::kResponseOk:
					ts::App::Get().sFileManager().save(path, app::SaveFileManager::kGeoVarNow, ts::App::Get().scene()->geoManager()->manager());
					break;
				case bg::wnd::MessageBox::kResponseCancel:
					break;
				default:
					break;
				}
			}
			else
			{
				ts::App::Get().sFileManager().save(path, app::SaveFileManager::kGeoVarNow, ts::App::Get().scene()->geoManager()->manager());
			}

			
		}
	}
	if (identifier == bg::wnd::kCodeCustom + 111)
	{
		bg::ptr<bg::wnd::FileDialog> diag = bg::wnd::FileDialog::SaveFileDialog();
		diag->addFilter("tsgr");
		if (diag->show())
		{
			bg::system::Path path(diag->getResultPath());
			if (path.exists())
			{
				bg::wnd::MessageBox box;
				auto response = box.Show(nullptr, "WARNING", "The file already exists\n Truncate its content?", bg::wnd::MessageBox::kButtonOkCancel);
				switch (response)
				{
				case bg::wnd::MessageBox::kResponseOk:
					ts::App::Get().sFileManager().save(path, app::SaveFileManager::kGeoGraph,ts::App::Get().scene()->geoManager()->selected()->graphic());
					break;
				case bg::wnd::MessageBox::kResponseCancel:
					break;
				default:
					break;
				}
			}
			else
			{
				ts::App::Get().sFileManager().save(path, app::SaveFileManager::kGeoGraph, ts::App::Get().scene()->geoManager()->selected()->graphic());
			}


		}
	}
	if (identifier == bg::wnd::kCodeCustom + 107)
	{
		ts::App::Get().scene()->geoManager()->manager()->connect(); //THIS FUNCTION ALREADY CONTROLS IF THE MANAGER IS ALREADY RUNNING
	}
	else if (identifier == bg::wnd::kCodeCustom + 108)
	{
		ts::App::Get().scene()->geoManager()->manager()->disconnect(); //THIS FUNCTION ALREADY CONTROLS IF THE MANAGER IS ALREADY STOP
	}
	geoVarHandler *selection = ts::App::Get().scene()->geoManager()->selected();
	if (selection != nullptr)
	{
		if (identifier == bg::wnd::kCodeCustom + 101)
		{
			selection->graphic()->show();
		}
		else if (identifier == bg::wnd::kCodeCustom + 102)
		{
			selection->graphic()->hide();
		}
		else if (identifier == bg::wnd::kCodeCustom + 103)
		{
			selection->graphic()->start();
		}
		else if (identifier == bg::wnd::kCodeCustom + 104)
		{
			selection->graphic()->pause();
		}
		else if (identifier == bg::wnd::kCodeCustom + 105)
		{
			selection->graphic()->clear();
		}
		else if (identifier == bg::wnd::kCodeCustom + 106)
		{
			//MODIFY COLOR
		}

	}


}

void DskEventHandler::loadScene(std::function<void(void)> AppFunc)
{
	_sceneRoot = nullptr;
	_camera = nullptr;
	AppFunc();
	_sceneRoot = ts::App::Get().scene()->sceneRoot();
	_camera = ts::App::Get().scene()->mainCamera();
}

}
}
