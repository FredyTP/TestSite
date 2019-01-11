#pragma once



//TestSite
#include <app\SceneConsole.h>
#include <var\TextBox.hpp>
#include <var\TestSiteVariables.hpp>
#include <var\geoVarManager.h>
#include <gui3d\Widget.h>
#include <gui3d\Pointer.h>
#include <extra\GraphicGenerator.h>
#include <var/geoVarHandler.h>

//Bg2e
#include <bg\bg2e.hpp>

//std
#include <vector>


class geoNodeManager : public bg::scene::Component
{
	//ADD THIS COMPONENT TO THE SCENE NODE WHICH IS THE PARENT OF THE BOXES TO GET FILLED;
	//ALLOWS TO SELECT GEOVARS
public:
	geoNodeManager();
	bg::scene::Component* clone()
	{
		return new geoNodeManager;
	}
	void init();
	void frame(float delta);
	void mouseDown(const bg::base::MouseEvent &);
	void keyDown(const bg::base::KeyboardEvent &);

	inline geoVarHandler* selected() { return _selected.getPtr(); }
	
protected:
	~geoNodeManager();

	std::vector<bg::ptr<geoVarHandler>> _varsHandlers;
	geoVarManager _geoManager;
	bg::ptr<bg::text::Font> _font;
	bg::ptr<geoVarHandler> _selected;
};

