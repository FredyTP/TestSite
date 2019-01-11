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

DskEventHandler::~DskEventHandler() {
}

void DskEventHandler::initGui()
{
	using namespace bg::gui;
	bg::ptr<Skin> skin = bg::db::loadSkin(context(), "resources//data//default.bg2skin");

	_guiSurface = new Surface(context(), skin.getPtr());
	_guiSurface->setId("surface");



	//FPS COUNTER
	Window *wnd = new Window(_guiSurface.getPtr());
	wnd->create();
	wnd->setSize(bg::math::Size2Di(200, 100));
	wnd->setAlignment(bg::gui::kAlignTopRight);
	wnd->setVisible(true);

	_fpsLabel = View::Create<Label>(context(), skin.getPtr());
	_fpsLabel->paragraph().setAlignment(bg::text::Paragraph::kVAlignCenter);
	_fpsLabel->paragraph().setAlignment(bg::text::Paragraph::kHAlignCenter);

	wnd->contentView()->setBackgroundColor(bg::math::Color::Transparent());
	wnd->contentView()->addChild(_fpsLabel.getPtr());


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


	wnd = new Window(_guiSurface.getPtr());
	wnd->create();
	wnd->setSize(bg::math::Size2Di(250, 190));
	wnd->setAlignment(bg::gui::kAlignTopLeft);
	wnd->setVisible(true);

	auto textIn = View::Create<bg::gui::TextInput>(context(), skin.getPtr());
	textIn->setBorderColor(bg::math::Color::Black());
	textIn->setBackgroundColor(bg::math::Color::White());
	textIn->setScrollEnabled(true);
	textIn->setHeight(50);

	wnd->contentView()->setBackgroundColor(bg::math::Color(0.7f, 0.8f, 1.0f, 1.0f));
	wnd->contentView()->addChild(textIn);

	

	

	
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
	_renderer->setRenderSetting("shadowMap", "shadowMapSize", bg::math::Size2Di(4096));

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
		_inputVisitor = new bg::scene::InputVisitor;
	}
	this->initGui();


}
void DskEventHandler::frame(float delta) {
	_renderer->frame(_sceneRoot.getPtr(), delta);

	_fpsCounter.frame(delta);
	if (_fpsCounter.valueUpdated())
	{
		_fpsLabel->setText("FPS: " + std::to_string(_fpsCounter.fps()));
	}

	//NOT OPTIMIZED!!!!! OPTIMIZE... IN A FUTURE ... MAYBE... NOT REALLY IMPORTANT... :)
	geoVarHandler *selection = ts::App::Get().scene()->geoManager()->selected();

	if (selection == nullptr)
	{

		_selectedInfo->setText("");
		_selectedInfo->parentNode()->hide();
		if (wasSelected)
		{
			wasSelected = false;
			HWND _hWnd = bg::native_cast<HWND>(reinterpret_cast<bg::wnd::Win32Window*>(bg::wnd::MainLoop::Get()->window())->hWnd());
			EnableMenuItem(GetMenu(_hWnd), 2, MF_BYPOSITION | MF_GRAYED);
			DrawMenuBar(_hWnd);

		}

	}
	else
	{

		_selectedInfo->parentNode()->show();
		_selectedInfo->setText(this->selectedInfoString(selection));
		if (!wasSelected)
		{
			wasSelected = true;
			HWND _hWnd = bg::native_cast<HWND>(reinterpret_cast<bg::wnd::Win32Window*>(bg::wnd::MainLoop::Get()->window())->hWnd());
			EnableMenuItem(GetMenu(_hWnd), 2, MF_BYPOSITION | MF_ENABLED);
			DrawMenuBar(_hWnd);
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
	}
	
}

void DskEventHandler::buildMenu(bg::wnd::MenuDescriptor & menu)
{

	//SOON, ADD A MENUS TO CONFIGURATE THE BOXES.. ETC^^// FONT,COLOR,POSITION...IDK U KNOW
	//REIMPLEMENT DE WHOLE SISTEM OF THE MENUS//
	bg::wnd::PopUpMenu * file = bg::wnd::PopUpMenu::New("File");
	file->addMenuItem({ bg::wnd::kCodeOpen, "Open" });
	file->addMenuItem({ bg::wnd::kCodeClose, "Close" });
	file->addMenuItem({ bg::wnd::kCodeCustom + 666 , "Launch VR-NotWorking" });

	if (bg::system::currentPlatform() != bg::system::kMac) {
		file->addMenuItem({ bg::wnd::kCodeQuit, "Exit" });
	}
	bg::wnd::PopUpMenu * edit = bg::wnd::PopUpMenu::New("Edit");
	edit->addMenuItem({ bg::wnd::kCodeCopy, "Copy" });
	edit->addMenuItem({ bg::wnd::kCodePaste, "Paste" });
	edit->addMenuItem({ bg::wnd::kCodeCustom + 100, "Custom command" });

	bg::wnd::PopUpMenu * selection = bg::wnd::PopUpMenu::New("Selection");

	bg::wnd::PopUpMenu * graphic = bg::wnd::PopUpMenu::New("Graphic");
	graphic->addMenuItem({ bg::wnd::kCodeCustom + 101, "Show" ,bg::base::KeyboardShortcut(1,bg::base::Keyboard::kKeyS) });
	graphic->addMenuItem({ bg::wnd::kCodeCustom + 102, "Hide" });
	graphic->addMenuItem({ bg::wnd::kCodeCustom + 103, "Start" });
	graphic->addMenuItem({ bg::wnd::kCodeCustom + 104, "Stop" });
	graphic->addMenuItem({ bg::wnd::kCodeCustom + 105, "Clear" });

	bg::wnd::PopUpMenu * text = bg::wnd::PopUpMenu::New("Text");
	text->addMenuItem({ bg::wnd::kCodeCustom + 106, "Color" });

	selection->addSubMenu(graphic);
	selection->addSubMenu(text);

	menu.push_back(file);
	menu.push_back(edit);
	menu.push_back(selection);
}

void DskEventHandler::menuSelected(const std::string & title, int32_t identifier)
{
	std::cout << title << " selected. Identifier: " << identifier << std::endl;
	if (identifier == bg::wnd::kCodeQuit) {
		bg::wnd::MainLoop::Get()->quit(0);
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
