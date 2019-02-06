
#include "event_handler.hpp"
#include <vr\vr_model.hpp>

#include <openvr.h>

#include <thread>

#include <bg/engine/openglCore/texture_impl.hpp>

#include <app/App.h>

std::string getTrackedDeviceString(vr::IVRSystem * vr, vr::TrackedDeviceIndex_t dev, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError * peError = nullptr) {
	uint32_t unRequiredBufferLength = vr->GetStringTrackedDeviceProperty(dev, prop, nullptr, 0, peError);
	if (unRequiredBufferLength==0) return "";
	char * pchBuffer = new char[unRequiredBufferLength];
	unRequiredBufferLength = vr->GetStringTrackedDeviceProperty(dev, prop, pchBuffer, unRequiredBufferLength, peError);
	std::string result = pchBuffer;
	delete [] pchBuffer;
	return result;
}

MainEventHandler::MainEventHandler(VRSystem* vrSys)
	:_vrSystem(vrSys)
{
}

MainEventHandler::~MainEventHandler() {
	ts::App::Get().destroy();
	delete _vrSystem;
}

void MainEventHandler::willCreateContext() {

	if (bg::engine::OpenGLCore::Supported()) {
		bg::Engine::Init(new bg::engine::OpenGLCore());
	}
	else {
		throw bg::base::CompatibilityException("No such suitable rendering engine.");
	}
}

void MainEventHandler::willDestroyContext() {


	_inputVisitor = nullptr;
	_sceneRoot = nullptr;
	_inputVisitor = nullptr;
	_renderer = nullptr;


	ts::App::Get().destroy();

	bg::base::EventHandler::willDestroyContext();
}

void MainEventHandler::initGL() {
	using namespace bg::engine;
	bg::Engine::Get()->initialize(context());
	//context()->setVsyncEnabled(true);
	ts::App::Get().init(context());
	
	_inputVisitor = new bg::scene::InputVisitor();

	_pipeline = new bg::base::Pipeline(context());

	initScene();

	_vrSystem->setControllerEventClosure([&](Controller * controller, Controller::EventType type, Controller::ButtonIdentifier button) {
		_inputVisitor->customEvent(sceneRoot(), bg::base::CustomEvent(new ControllerEventData(controller, type, button)));
	});

	_renderer = createRenderer();
}

void MainEventHandler::reshape(int w, int h) {
	_pipeline->setViewport(bg::math::Viewport(0, 0, w, h));
}

void MainEventHandler::destroy() {
	Controller::Destroy();
	_vrSystem->destroy();
	bg::Engine::Destroy();
}

void MainEventHandler::frame(float delta) {
	_renderer->frame(_sceneRoot.getPtr(), delta);
	if (_fps.frame(delta)) {
//		bg::log(bg::log::kDebug) << _fps.fps() << " FPS" << bg::endl;
	}
}

void MainEventHandler::draw() {
	_vrSystem->frame();

	// Draw VR
	_vrSystem->draw(_renderer.getPtr(),sceneRoot());

	// Preview pipeline
	bg::base::Pipeline::SetCurrent(_pipeline.getPtr());
	_pipeline->drawTexture(_vrSystem->leftSurface());

	context()->swapBuffers();
}

void MainEventHandler::keyUp(const bg::base::KeyboardEvent & evt) {
	if (evt.keyboard().key() == bg::base::Keyboard::kKeyEsc) {
		bg::wnd::MainLoop::Get()->quit(0);
	}
	else {
		if (evt.keyboard().key() == bg::base::Keyboard::kKeyAdd) {
			float s = _vrSystem->scale();
			_vrSystem->setScale(s + 0.001f);
			_vrSystem->navigateTo(bg::math::Vector3(0.0f));
			bg::log(bg::log::kDebug) << "Scene scale set to " << _vrSystem->scale() << bg::endl;
		}
		else if (evt.keyboard().key() == bg::base::Keyboard::kKeySub) {
			float s = _vrSystem->scale();
			_vrSystem->setScale(s - 0.001f);
			_vrSystem->navigateTo(bg::math::Vector3(0.0f));
			bg::log(bg::log::kDebug) << "Scene scale set to " << _vrSystem->scale() << bg::endl;
		}
		else if (evt.keyboard().key() == bg::base::Keyboard::kKeySpace) {
			using namespace bg::scene;
			bg::ptr<FindComponentVisitor<Light>> findLight = new FindComponentVisitor<Light>();
			sceneRoot()->accept(findLight.getPtr());
			for(auto l : findLight->result()) {
				if(l->light()->type() == bg::base::Light::kTypeDirectional) {
					float b = l->light()->shadowBias();
					l->light()->setShadowBias(b * 1.5f);
					bg::log(bg::log::kDebug) << l->light()->shadowBias() << bg::endl;
				}
			}
		}
		_inputVisitor->keyUp(sceneRoot(), evt);
	}
}

void MainEventHandler::keyDown(const bg::base::KeyboardEvent & evt) {
	_inputVisitor->keyDown(sceneRoot(), evt);
}

void MainEventHandler::mouseDown(const bg::base::MouseEvent & evt) {
	_inputVisitor->mouseDown(sceneRoot(), evt);
}

void MainEventHandler::mouseUp(const bg::base::MouseEvent & evt) {
	_inputVisitor->mouseUp(sceneRoot(), evt);
	/*
	enum MenuOptions {
		kOpenFile = 1//,
//		kDisableSSAO,
//		kSSAOQualityLow,
//		kSSAOQualityMedium,
//		kSSAOQualityHigh,
//		kSSAOQualityExtreme,
//		kDisableRaytracer,
//		kRaytracerQualityLow,
//		kRaytracerQualityMedium,
//		kRaytracerQualityHigh,
//		kRaytracerQualityExtreme
	};
	
	if(evt.mouse().getReleasedButton() == bg::base::Mouse::kRightButton) {
		using namespace bg::render::settings;
		bool ssao = _renderer->boolSettingValue(kAmbientOcclusion, kEnabled);
		int ssaoSamples = _renderer->intSettingValue(kAmbientOcclusion, kKernelSize);
		bool raytracing = _renderer->boolSettingValue(kRaytracer, kEnabled);
		bg::base::Quality rtQuality = static_cast<bg::base::Quality>(_renderer->intSettingValue(kRaytracer, kQuality));

		_menu = bg::wnd::PopUpMenu::New();

		_menu->addMenuItem({ kOpenFile, "Open scene" });
/*
		bg::base::Quality aoQuality = bg::base::kQualityLow;
		switch(ssaoSamples) {
		case 8:
			aoQuality = bg::base::kQualityLow;
			break;
		case 16:
			aoQuality = bg::base::kQualityMedium;
			break;
		case 24:
			aoQuality = bg::base::kQualityMedium;
			break;
		case 32:
			aoQuality = bg::base::kQualityExtreme;
			break;
		}
		if(ssao) {
			_menu->addMenuItem({ kDisableSSAO, "Disable Ambient Occlusion" });
		}
		if(aoQuality != bg::base::kQualityLow || !ssao) {
			_menu->addMenuItem({ kSSAOQualityLow, "AO Quality Low" });
		}
		if(aoQuality != bg::base::kQualityMedium || !ssao) {
			_menu->addMenuItem({ kSSAOQualityMedium, "AO Quality Medium" });
		}
		if(aoQuality != bg::base::kQualityHigh || !ssao) {
			_menu->addMenuItem({ kSSAOQualityHigh, "AO Quality High" });
		}
		if(aoQuality != bg::base::kQualityExtreme || !ssao) {
			_menu->addMenuItem({ kSSAOQualityExtreme, "AO Quality Extreme" });
		}

		if(raytracing) {
			_menu->addMenuItem({ kDisableRaytracer, "Disable Reflections" });
		}
		if(rtQuality != bg::base::kQualityLow || !raytracing) {
			_menu->addMenuItem({ kRaytracerQualityLow, "Reflections Quality Low" });
		}
		if(rtQuality != bg::base::kQualityMedium || !raytracing) {
			_menu->addMenuItem({ kRaytracerQualityMedium, "Reflections Quality Medium" });
		}
		if(rtQuality != bg::base::kQualityHigh || !raytracing) {
			_menu->addMenuItem({ kRaytracerQualityHigh, "Reflections Quality High" });
		}
		if(rtQuality != bg::base::kQualityExtreme || !raytracing) {
			_menu->addMenuItem({ kRaytracerQualityExtreme, "Reflections Quality Extreme" });
		}
*/
	/*	_menu->setPosition(evt.pos());
		_menu->show([&](bg::wnd::MenuItemIdentifier selected) {
			using namespace bg::render::settings;
			switch(selected) {
			case kOpenFile:
			{
				bg::ptr<bg::wnd::FileDialog> fileDialog = bg::wnd::FileDialog::OpenFileDialog();
				fileDialog->addFilter("vitscnj");
				fileDialog->addFilter("vitscn");
				if(fileDialog->show()) {
					openFile(bg::system::Path(fileDialog->getResultPath()));
				}
			}
			break;
/*			case kDisableSSAO:
				_renderer->setRenderSetting(kAmbientOcclusion, kEnabled, false);
				break;
			case kSSAOQualityLow:
				_renderer->setRenderSetting(kAmbientOcclusion, kEnabled, true);
				_renderer->setRenderSetting(kAmbientOcclusion, kKernelSize, 8);
				_renderer->setRenderSetting(kAmbientOcclusion, kBlur, 1);
				break;
			case kSSAOQualityMedium:
				_renderer->setRenderSetting(kAmbientOcclusion, kEnabled, true);
				_renderer->setRenderSetting(kAmbientOcclusion, kKernelSize, 16);
				_renderer->setRenderSetting(kAmbientOcclusion, kBlur, 1);
				break;
			case kSSAOQualityHigh:
				_renderer->setRenderSetting(kAmbientOcclusion, kEnabled, true);
				_renderer->setRenderSetting(kAmbientOcclusion, kKernelSize, 24);
				_renderer->setRenderSetting(kAmbientOcclusion, kBlur, 2);
				break;
			case kSSAOQualityExtreme:
				_renderer->setRenderSetting(kAmbientOcclusion, kEnabled, true);
				_renderer->setRenderSetting(kAmbientOcclusion, kKernelSize, 32);
				_renderer->setRenderSetting(kAmbientOcclusion, kBlur, 4);
				break;
			case kDisableRaytracer:
				_renderer->setRenderSetting(kRaytracer, kEnabled, false);
				break;
			case kRaytracerQualityLow:
				_renderer->setRenderSetting(kRaytracer, kEnabled, true);
				_renderer->setRenderSetting(kRaytracer, kQuality, bg::base::kQualityLow);
				break;
			case kRaytracerQualityMedium:
				_renderer->setRenderSetting(kRaytracer, kEnabled, true);
				_renderer->setRenderSetting(kRaytracer, kQuality, bg::base::kQualityMedium);
				break;
			case kRaytracerQualityHigh:
				_renderer->setRenderSetting(kRaytracer, kEnabled, true);
				_renderer->setRenderSetting(kRaytracer, kQuality, bg::base::kQualityHigh);
				break;
			case kRaytracerQualityExtreme:
				_renderer->setRenderSetting(kRaytracer, kEnabled, true);
				_renderer->setRenderSetting(kRaytracer, kQuality, bg::base::kQualityExtreme);
				break;

			}
		});
	}
	*/
}

void MainEventHandler::mouseMove(const bg::base::MouseEvent & evt) {
	_inputVisitor->mouseMove(sceneRoot(), evt);
}

void MainEventHandler::mouseDrag(const bg::base::MouseEvent & evt) {
	_inputVisitor->mouseDrag(sceneRoot(), evt);
}

void MainEventHandler::openFile(const bg::system::Path & path) {
	using namespace bg::scene;
	using namespace bg::math;
	Node * newSceneRoot = nullptr;
	if (_dynamics.valid() && _dynamics->node()) {
		_dynamics->node()->setId("");
	}
	try {
		newSceneRoot = bg::db::loadScene(context(), path);
		bg::ptr<FindComponentVisitor<Light>> findLight= new FindComponentVisitor<Light>();
		newSceneRoot->accept(findLight.getPtr());
		for (auto l : findLight->result()) {
			if (l->light()->type()==bg::base::Light::kTypeDirectional) {
				l->light()->setShadowBias(0.00000001f);
				l->light()->setShadowStrength(0.5f);
			}
		}
		bg::ptr<FindComponentVisitor<Dynamics>> findDynamics = new FindComponentVisitor<Dynamics>();
		newSceneRoot->accept(findDynamics.getPtr());
		_dynamics = nullptr;
		for (auto d : findDynamics->result()) {
			_vrSystem->initPhysics(d);
			_dynamics = d;
			_dynamics->node()->setId("dynamics-node");
			break;
		}
		bg::ptr<FindComponentVisitor<RigidBody>> findRigidBody = new FindComponentVisitor<RigidBody>();
		newSceneRoot->accept(findRigidBody.getPtr());
		for (auto rb : findRigidBody->result()) {
			rb->body()->setMass(1.0f);
			rb->body()->setKinematic(true);
		}
 	}
	catch (std::runtime_error & e) {
		bg::log(bg::log::kWarning) << "Could not load file " << path << bg::endl;
	}
	

	if (!newSceneRoot) {
		newSceneRoot = new Node(context());
		Node * s2 = new Node(context());
		newSceneRoot->addChild(s2);
		Node * sceneNode = new Node(context());
		sceneNode->addComponent(new Transform());
		sceneNode->transform()->setTransformStrategy(new TRSTransformStrategy());
		s2->addChild(sceneNode);

		PrimitiveFactory primitive(context());
		Node * floor = new Node(context());
		floor->addComponent(primitive.plane(30.0f));
		sceneNode->addChild(floor);

		Node * cube = new Node(context());
		sceneNode->addChild(cube);
		cube->addComponent(primitive.cube());
		cube->addComponent(new Transform(Matrix4::Translation(0.0f, 1.5f, -4.5f)));

		Node * mainLight = new Node(context());
		mainLight->setId("main-light");
		mainLight->addComponent(new Light());
		mainLight->addComponent(new Transform(Matrix4::Rotation(kPiOver4, -1.0f, 0.0f, 0.0f)));
		TRSTransformStrategy *trx = new TRSTransformStrategy();

		
		trx->setTranslate(Vector3(0,3,0));
		trx->setRotateX(-0.78);
		trx->setRotateY(0.78);
		trx->setRotationOrder(TRSTransformStrategy::kOrderXYZ);
		trx->setScale(Vector3(1.0));

		mainLight->transform()->setTransformStrategy(trx);
		s2->addChild(mainLight);
	}
	
	Node * stage = this->stage();
	stage->clearChildren();
	stage->addChild(newSceneRoot);
	_vrSystem->resetNavigation();

	_vrSystem->navigateTo(bg::math::Vector3(0.0f));
}

void MainEventHandler::initScene() {
	using namespace bg::render;
	using namespace bg::scene;
	using namespace bg::math;
	using namespace bg::render::settings;

	
	// Create scene root and stage by calling this function
	sceneRoot()->addChild(stage());

	bg::system::Path path = bg::system::Path::AppDir();
    if (bg::system::currentPlatform()==bg::system::kMac) {
        path = bg::system::Path::ResourcesDir();
    }

	stage()->addChild(ts::App::Get().scene()->sceneRoot());
	_vrSystem->init(sceneRoot());
}

bg::scene::Node * MainEventHandler::sceneRoot() {
	if(_sceneRoot.isNull()) {
		_sceneRoot = new bg::scene::Node(context());
		_sceneRoot->setId("scene-root");
	}
	return _sceneRoot.getPtr();
}

bg::scene::Node * MainEventHandler::stage() {
	if (_stage.isNull()) {
		_stage = new bg::scene::Node(context());
		_stage->setId("stage");
		float scale = _vrSystem->scale();
		_stage->addComponent(new bg::scene::Transform(bg::math::Matrix4::Scale(scale,scale,scale)));
	}
	return _stage.getPtr();
}

bg::render::ClassicRenderer * MainEventHandler::createRenderer() {
	using namespace bg::render;
	using namespace bg::render::settings;
	using namespace bg::math;
	bg::ptr<bg::render::Renderer> renderer = Renderer::Create(context(), Renderer::kRenderPathForward);
	renderer->setClearColor(Color(0.05f,0.07f,0.11f,1.0f));

	renderer->setRenderSetting(kAmbientOcclusion, kEnabled, false);
	renderer->setRenderSetting(kAmbientOcclusion, kKernelSize, 8);
	renderer->setRenderSetting(kAmbientOcclusion, kBlur, 1);
	renderer->setRenderSetting(kRaytracer, kEnabled, true);
	renderer->setRenderSetting(kRaytracer, kQuality, bg::base::kQualityMedium);
	renderer->setRenderSetting(kShadowMap, kShadowMapSize, Size2Di(3000));

	return dynamic_cast<bg::render::ClassicRenderer*>(renderer.release());
}
