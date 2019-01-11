#pragma once

//TestSite
#include <gui3d\Pointer.h>
#include <gui3d\Widget.h>

//Bg2e
#include <bg\bg2e.hpp>

//std
#include <vector>


namespace gui3d
{


//CAMBIAR LA FORMA EN LA QUE SE GESTIONAN LAS RESPUESTAS A EVENTOS//

class GuiManager : public bg::scene::Component
{
public:
	GuiManager();
	bg::scene::Component* clone() { return new GuiManager; }

	void frame(float delta);
	Widget* getWidget(std::string ID);
	void addWidget(gui3d::Widget *w);
	void removeWidget(gui3d::Widget *w);
	void setPointer(gui3d::Pointer * p);
	void setPointer(ts::vr::Pointer *p);
	void clear();
	bool noIntersect() { return _noIntersect; }
	std::vector < bg::ptr<Widget>>& widgets(){ return _widgets; }
protected:
	~GuiManager();
	bool _noIntersect;
	bg::ptr<Pointer> _pointer;
	std::vector<bg::ptr<Widget> > _widgets;
};

}

