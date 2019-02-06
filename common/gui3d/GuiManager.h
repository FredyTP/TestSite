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
	void setPointer(Pointer * p);
	void setPointer(ts::vr::Pointer *p);
	void removePointer() { _pointer = nullptr; }
	void clear();
	void sort();
	bool intersects() { return _intersects; }
	Widget* firstIntersected() { if (_intersects) return _first; return nullptr; }
	std::vector<Widget*> intersected() { return _intersWidgets; }
	std::vector<bg::ptr<Widget> >& widgets(){ return _widgets; }

	Pointer* pointer() { return _pointer.getPtr(); }
protected:
	~GuiManager();
	bool _intersects;
	std::vector<Widget*> _intersWidgets;
	Widget* _first;
	bg::ptr<Pointer> _pointer;
	Widget* _intersected;
	std::vector<bg::ptr<Widget> > _widgets;
};

}

