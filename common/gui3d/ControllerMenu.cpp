#include "ControllerMenu.hpp"

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
		snode->addComponent(fac.sphere(1.0f));
		snode->drawable()->material(0)->setDiffuse(bg::math::Color::Blue());
		
		snode->addComponent(new Transform);
		snode->transform()->matrix().translate(2 * _subMenus.size(), 1.0f, 1.0f);

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
		_subMenus[_selected]->deselect();
		if (_selected < _subMenus.size() - 1)
		{
			_selected++;
		}
		else
		{
			_selected = 0;
		}
		_subMenus[_selected]->select();

		
	}

	void ControllerMenu::prevSubMenu()
	{
		_subMenus[_selected]->deselect();
		if (_selected > 0)
		{
			_selected--;
		}
		else
		{
			_selected = _subMenus.size() - 1;
		}
		_subMenus[_selected]->select();
	}

	void ControllerMenu::select()
	{
		node()->drawable()->material(0)->setDiffuse(bg::math::Color::Orange());
		//CHANGE THE COLOR BLA BLA BLA//
	}

	void ControllerMenu::deselect()
	{
		node()->drawable()->material(0)->setDiffuse(bg::math::Color::Blue());
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