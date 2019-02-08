#pragma once

//TEST SITE
#include <extra/Primitives.h>
#include <vr/controller.hpp>
 
//BG2E
#include <bg/bg2e.hpp>
#include <bg/wnd/win32_window.hpp>


//STD
#include <vector>
#include <string>



namespace ts
{
namespace gui3ds
{


	class ControllerMenu : public bg::scene::Component
	{
	public:
		ControllerMenu(const std::string mName, const unsigned int mID);
		bg::scene::Component *clone() { return new ControllerMenu(_name,_id); }

		void addSubMenu(const std::string mName, const unsigned int mID);

		void init();
		
		//CONFIG FUNCTIONS
		void setColors(const bg::math::Color normal, const bg::math::Color selected, const bg::math::Color accepted);
		void closeWhenSelected(bool close);

		//HIERARCHY FUNCTIONS
		void addSubMenu(ControllerMenu* subMenu);

		//GET FUNCTIONS
		ControllerMenu* subMenu(std::string mName);
		Controller* controller() { return _controller; }

		//CONSULT FUNC
		const std::string & name() { return _name; }
		const unsigned int id() { return _id; }

		bool hasSubMenus(){ return _subMenus.size() > 0; }
		bool hasFocus() { return _focus; }
		void attachToController(Controller *vrController);

		void keyDown(const bg::base::KeyboardEvent &ev);

		//CONTROL FUNC
		void open();
		void close();
		void closeAll();

		void show();
		void hide();

		void nextSubMenu();
		void prevSubMenu();

		void select();
		void deselect();

		void choose();





	private:
		std::vector<bg::ptr<ControllerMenu> > _subMenus;
		ControllerMenu *parent;
		std::vector<bg::math::Color> _colors;

		//CONFIG INFO
		std::string _name;
		unsigned int _id;

		//STATE INFO
		bool _getFocus;
		bool _focus;
		unsigned int _selected;

		uint32_t _controllerIndex;
		Controller * _controller;
	};




}
}

