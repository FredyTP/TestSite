#pragma once


//TestSite
#include <gui3d/Widget.h>
#include <gui3d/Pointer.h>
#include <gui3d\ValueCircle.h>
#include <app\SceneConsole.h>
#include <GeneralConstants.h>
#include <extra\PosAnimation.h>
#include <extra\RotAnimation.h>
#include <extra/Clock.hpp>
#include <extra/GraphicGenerator.h>
#include <var/geoNodeManager.hpp>
#include <Leap/HandController.hpp>


//Bg2e
#include <bg\bg2e.hpp>
#include <bg\scene\skybox.hpp>

//std
#include <vector>

namespace ts
{


class Scene : public bg::base::ContextObject, public bg::base::ReferencedPointer {
public:
	Scene(bg::base::Context *);

	void loadTestSiteScene(const std::string& path);
	void createTestScene();

	inline bool ready() const { return _sceneRoot.valid() && _mainCamera.valid(); }

	void open(const std::string & path);

	inline bg::scene::Node * sceneRoot() { return _sceneRoot.getPtr(); }
	inline bg::scene::Camera * mainCamera() { return _mainCamera.getPtr(); }
	inline ts::Clock * clock() { return _clock.getPtr(); }
	inline const std::vector<bg::scene::Camera*> & cameras() const { return _cameras; }
	inline SceneConsole* console() { return _console.getPtr(); }
	inline geoNodeManager* geoManager() { return _geoManager.getPtr(); }
	void setMainCamera(size_t index);

protected:
	virtual ~Scene();

	bg::ptr<bg::scene::Node> _sceneRoot;
	bg::ptr<bg::scene::Camera> _mainCamera;
	std::vector<bg::scene::Camera*> _cameras;
	bg::ptr<ts::Clock> _clock;
	bg::ptr<SceneConsole> _console;
	bg::ptr<geoNodeManager> _geoManager;
	std::string _currentScenePath;
};


}
