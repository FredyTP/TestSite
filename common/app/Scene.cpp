#include "Scene.h"
#include <app\App.h>
#include <resFinder.hpp>

namespace ts {

	Scene::Scene(bg::base::Context *ctx)
		:ContextObject(ctx)
	{
		_cameras.size();
	}

	void Scene::loadTestSiteScene(const std::string & path)
	{
		open(path);
		_mainCamera->setViewport(ts::App::Get().viewport());
		_sceneRoot->addComponent(ts::App::Get().guiManager());
		for (auto child : _sceneRoot->children())
		{
			if (child->name().compare("SceneRoot") == 0)
			{
				child->addComponent(new geoNodeManager);
			}
		}
	}

	void Scene::createTestScene()
	{
		using namespace bg::scene;
		using namespace bg::math;

		_sceneRoot = new Node(context());
		_sceneRoot->addComponent(ts::App::Get().guiManager());

		Node * cameraNode = new Node(context());
		_sceneRoot->addChild(cameraNode);
		_mainCamera = new Camera();
		cameraNode->addComponent(_mainCamera.getPtr());
		cameraNode->addComponent(new Transform());
		bg::manipulation::OrbitNodeController * orbit = new bg::manipulation::OrbitNodeController;
		cameraNode->addComponent(orbit);
		OpticalProjectionStrategy * projection = new OpticalProjectionStrategy;
		projection->setFocalLength(Scalar(35, distance::mm));
		_mainCamera->setProjectionStrategy(projection);
		orbit->setDistance(Scalar(5, distance::meter));
		orbit->setRotation(Vector2(Scalar(-22.5, trigonometry::deg),
			Scalar(0.0f, trigonometry::deg)));

		//LIGHT
		Node * lightNode = new Node(context());
		_sceneRoot->addChild(lightNode);
		Transform *trx = new Transform();
		trx->matrix()
			.rotate(trigonometry::degreesToRadians(95.0f), -1.0f, 0.0f, 0.0f)
			.rotate(trigonometry::degreesToRadians(45.0f), 0.0f, 1.0f, 0.0f)
			.setPosition(0.0f, 1.0f, 0.0f);
		lightNode->addComponent(trx);
		lightNode->addComponent(new bg::scene::Light);

		//DRAWABLES
		PrimitiveFactory factory(context());
		Node * floor = new Node(context());
		_sceneRoot->addChild(floor);
		floor->addComponent(factory.plane(Scalar(10, distance::meter)));
		floor->drawable()->material(0)->setTexture(bg::db::loadTexture(context(),ts::resources.texture.grass));
		floor->drawable()->material(0)->setTextureScale(bg::math::Vector2(10.0f, 10.0f));

		trx = new Transform();
		trx->matrix()
			.translate(0.0f, Scalar(0, distance::cm), 0.0f);
		floor->addComponent(trx);

		//T_AMB
		Node * t_amb = new Node(context(), "T_AMB1");
		_sceneRoot->addChild(t_amb);


		trx = new Transform();
		trx->matrix()
			.translate(1.0f, 1.0f, 0.0f);
		t_amb->addComponent(trx);

		//FLOW
		Node * flow = new Node(context(), "FLOW1");
		_sceneRoot->addChild(flow);


		trx = new Transform();
		trx->matrix()
			.translate(-1.0f, 1.0f, 0.0f);
		flow->addComponent(trx);

		_geoManager = new geoNodeManager();
		_sceneRoot->addComponent(_geoManager.getPtr());

		

		//SKYBOX HAS MEMORY LEAK//

		/*Skybox *sb = new Skybox;
		sb->setImageFile(bg::base::Texture::kFaceNegativeX, "resources//data//negx.jpg");
		sb->setImageFile(bg::base::Texture::kFacePositiveX, "resources//data//posx.jpg");
															 
		sb->setImageFile(bg::base::Texture::kFaceNegativeY, "resources//data//negy.jpg");
		sb->setImageFile(bg::base::Texture::kFacePositiveY, "resources//data//posy.jpg");
															 
		sb->setImageFile(bg::base::Texture::kFaceNegativeZ, "resources//data//negz.jpg");
		sb->setImageFile(bg::base::Texture::kFacePositiveZ, "resources//data//posz.jpg");
		sb->loadSkybox(context());

		_sceneRoot->addComponent(sb);*/


		bg::db::FontLoader::RegisterPlugin(new bg::db::plugin::ReadFontTrueType());


		bg::ptr<bg::text::Font> _font = bg::db::loadFont(context(), ts::resources.font.nunito_black, 30.0f);

		bg::base::Material * mat = new bg::base::Material();
		mat->setDiffuse(bg::math::Color::White());
		mat->setLightEmission(1.0f);
		mat->setProcessAsTransparent(true);
		mat->setCullFace(false);
		_font->setMaterial(mat);
		_font->setTextScale(0.40f);

		Node *cons = new Node(context());
		_console = new SceneConsole(_font.getPtr());
		cons->addComponent(_console.getPtr());
		cons->addComponent(new bg::scene::Transform);
		cons->transform()->matrix().translate(0.0f, 3.0f, 0.0f);
		_sceneRoot->addChild(cons);
		


		/*Node * wid = new Node(context());
		_sceneRoot->addChild(wid);
		trx = new Transform();
		trx->matrix().translate(0.0f, 2.0f, 0.0f);
		trx->matrix().rotate(1.0f, 1.0f, 1.0f, 0.0f);
		wid->addComponent(trx);
		wid->addComponent(new gui3d::ValueCircle(1, "test"));
		/*bg::base::Material *mat = new bg::base::Material();
		mat->setDiffuse(bg::math::Color::White());
		wid->component<gui3d::Widget>()->setMaterial(mat, 1);
		bg::base::Material *mat2 = new bg::base::Material();
		mat2->setDiffuse(bg::math::Color::Yellow());

		wid->component<gui3d::Widget>()->setMaterial(mat2,2);*/
		//wid->component<gui3d::ValueCircle>()->hide();

		/*bg::base::Material *mate = new bg::base::Material();
		mate->setTexture(bg::db::loadTexture(context(), ts::kColorCircleTexture1));
		mate->setCullFace(false);
		mate->setUnlit(true);
		wid->component<gui3d::ValueCircle>()->setMaterial(mate, -1);*/


		bg::scene::Node *clock = new bg::scene::Node(context());
		_clock = new Clock;
		clock->addComponent(_clock.getPtr());
		clock->addComponent(new bg::scene::Transform);
		clock->transform()->matrix().translate(2.0f, 2.0f, 0.0f);
		_sceneRoot->addChild(clock);
		std::cout << "SCENE BUILD" << std::endl;


	}

	

	Scene::~Scene() {
		_cameras.clear();
		_sceneRoot = nullptr;
		_mainCamera = nullptr;
	}


	void Scene::open(const std::string & path) {
		try {
			if(_cameras.size())
				_cameras.clear();
			bg::ptr<bg::scene::Node> newSceneNode = bg::db::loadScene(context(), path);
			bg::ptr<bg::scene::FindComponentVisitor<bg::scene::Camera>> findCamera = new bg::scene::FindComponentVisitor<bg::scene::Camera>();
			_sceneRoot = newSceneNode.getPtr();
			_currentScenePath = path;
			_sceneRoot->accept(findCamera.getPtr());
			for (auto cam : findCamera->result()) {
				_cameras.push_back(cam);
				if (!cam->projectionStrategy()) {
					auto strategy = new bg::scene::OpticalProjectionStrategy();
					strategy->setNear(0.5f);
					strategy->setFar(1000.0f);
					cam->setProjectionStrategy(strategy);
				}
				if (!cam->node()->component<bg::manipulation::OrbitNodeController>()) {
					auto controller = new bg::manipulation::OrbitNodeController;
					cam->node()->addComponent(controller);
				}
			}

			if (_cameras.size() > 0) {
				_mainCamera = _cameras[0];
			}
			else {
				_mainCamera = nullptr;
			}
		}
		catch (std::runtime_error & err) {
			bg::wnd::MessageBox::Show(bg::wnd::MainLoop::Get()->window(), "Error opening scene", "Could not open scene " + path + "\n" + err.what());
		}
	}

	void Scene::setMainCamera(size_t index) {
		if (_cameras.size() > index) {
			_mainCamera = _cameras[index];
		}
	}

}
