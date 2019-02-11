#include "ControllerMenu.hpp"
#include <resFinder.hpp>

namespace ts
{
namespace gui3ds
{





	ControllerMenu::ControllerMenu(const std::string mName, const unsigned int mID)
	{
		_name = mName;
		_id = mID;

		_getFocus = false;
		_focus = false;
		_selected = 0;

	}

	void ControllerMenu::closeWhenSelected(bool close)
	{
		
	}


	void ControllerMenu::addSubMenu(const std::string mName, const unsigned int mID)
	{
		using namespace bg::scene;
		bg::ptr<ControllerMenu> _newMenu = new ControllerMenu(mName, mID);
		Node* snode = new Node(context());
		snode->addComponent(_newMenu.getPtr());
		
		PrimitiveFactory fac(context());
		/*auto dr = getSurface([](float t, float v)->bg::math::Vector3
		{
			
			return bg::math::Vector3(t,v,cos(t));
		})*/
		snode->addComponent(bg::db::loadDrawable(context(), ts::resources.model.CtrlTile));
		snode->drawable()->material(0)->setDiffuse(bg::math::Color(0.1f, 0.3f, 1.0f, 0.6f));
		snode->drawable()->material(0)->setLightEmission(0.1f);
		snode->addComponent(new Transform);	
		snode->transform()->matrix().rotate(-bg::math::k2Pi / 8.0f*_subMenus.size(), 0.0f, 1.0f, 0.0f);
		snode->transform()->matrix().translate(0.0f, 0.13f, 0.0f);


		this->addSubMenu(_newMenu.getPtr());
	}

	void ControllerMenu::init()
	{
	}

	void ControllerMenu::setColors(const bg::math::Color normal, const bg::math::Color selected, const bg::math::Color accepted)
	{
	}

	void ControllerMenu::addSubMenu(ControllerMenu * subMenu)
	{
		if (subMenu)
		{
			subMenu->parent = this;
			node()->addChild(subMenu->node());
			_subMenus.push_back(subMenu);
			subMenu->hide();
		}
		
	}

	ControllerMenu * ControllerMenu::subMenu(std::string mName)
	{
		for (auto& sm : _subMenus)
		{
			if (sm->name()==mName)
			{
				return sm.getPtr();
			}
		}
		return nullptr;
	}

	void ControllerMenu::attachToController(Controller * vrController)
	{
		if(vrController)
		{
			_controller = vrController;
			_controllerIndex = _controller->index();
		}
	}

	void ControllerMenu::keyDown(const bg::base::KeyboardEvent & ev)
	{
		if (hasSubMenus())
		{
			if (parent == nullptr)
			{ //NOT HERE, WHERE THE CTRL IS ASSOCIATED
				if (ev.keyboard().key() == ev.keyboard().kKeyO)
				{
					this->open();
				}
			}
			if (_getFocus)
			{
				_getFocus = false;
				_focus = true;
			}
			else if (hasFocus())
			{
				if (ev.keyboard().key() == ev.keyboard().kKeyLeft)
				{
					prevSubMenu();
				}
				else if (ev.keyboard().key() == ev.keyboard().kKeyRight)
				{
					nextSubMenu();
				}
				else if (ev.keyboard().key() == ev.keyboard().kKeyUp)
				{
					this->choose();
				}
				else if (ev.keyboard().key() == ev.keyboard().kKeyDown)
				{
					this->close();
				}
			}
		}

	}

	void ControllerMenu::open()
	{
		if (parent) parent->_focus = false;
		_getFocus = true;
		for (auto& sb : _subMenus)
		{
			sb->show();
		}
		_subMenus[_selected]->select();
	}

	void ControllerMenu::close()
	{
		_subMenus[_selected]->deselect();
		_focus = false;
		for (auto& sb : _subMenus)
		{
			sb->hide();
		}
		if (parent) parent->_focus = true;
	}

	void ControllerMenu::closeAll()
	{
		ControllerMenu *p = this;
		while (p)
		{
			p->close();
			p = p->parent;
		}
	}

	void ControllerMenu::show()
	{
		node()->setEnabled();
	}

	void ControllerMenu::hide()
	{
		node()->setDisabled();
	}

	void ControllerMenu::nextSubMenu()
	{
		this->selectSubMenu(_selected + 1);	
	}

	void ControllerMenu::prevSubMenu()
	{
		this->selectSubMenu(_selected - 1);
	}

	void ControllerMenu::selectSubMenu(int subM)
	{
		_subMenus[_selected]->deselect();
		_selected = subM % _subMenus.size();
		_subMenus[_selected]->select();
	}

	void ControllerMenu::select()
	{
		node()->drawable()->material(0)->setDiffuse(bg::math::Color(1.0f, 0.7f, 0.3f, 0.7f));
		//CHANGE THE COLOR BLA BLA BLA//
	}

	void ControllerMenu::deselect()
	{
		node()->drawable()->material(0)->setDiffuse(bg::math::Color(0.1f, 0.3f, 1.0f, 0.6f));
		//CHANGE BACK THE COLOR BLABLALA
	}

	void ControllerMenu::choose()
	{
		bg::wnd::MainLoop::Get()->window()->windowController()->eventHandler()
			->menuSelected(_subMenus[_selected]->_name, _subMenus[_selected]->_id);
		if (_subMenus[_selected]->hasSubMenus())
		{
			_subMenus[_selected]->open();
		}
		else
		{
			this->closeAll();
		}
		//SEND THE COMAND TO EVENT HANDLER eh->menuEvent(_name,_id)//

	}

}
}