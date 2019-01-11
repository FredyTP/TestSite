#include "GuiManager.h"

namespace gui3d
{



GuiManager::GuiManager()
{
}


GuiManager::~GuiManager()
{

}

void GuiManager::frame(float delta)
{
	_noIntersect = true;
	if (_pointer.valid())
	{
		if (_pointer->posChanged())
		{
			for (int i = 0; i < _widgets.size(); i++)
			{
				_widgets[i]->intersectWithRay(_pointer->ray());
				_noIntersect = false;
			}
			_pointer->setPosChanged(false);
		}
	}

}

Widget * GuiManager::getWidget(std::string ID)
{
	for (int i = 0; i < _widgets.size(); i++)
	{
		if (_widgets[i]->getID().compare(ID)==0)
		{
			return _widgets[i].getPtr();
		}
	}
	return nullptr;
}

void GuiManager::addWidget(gui3d::Widget * w)
{
	for (int i = 0; i < _widgets.size(); i++)
	{
		if (_widgets[i].getPtr() == w)
		{
			return;
		}
	}
	_widgets.push_back(w);
}

void GuiManager::removeWidget(gui3d::Widget * w)
{
	for (int i = 0; i < _widgets.size(); i++)
	{
		if (_widgets[i].getPtr() == w)
		{
			_widgets.erase(_widgets.begin() + i);
		}
	}
}

void GuiManager::setPointer(Pointer * p)
{
	if (p != nullptr)
	{
		_pointer = p;
	}
}

void GuiManager::clear()
{
	_pointer = nullptr;
	_widgets.clear();
}

}