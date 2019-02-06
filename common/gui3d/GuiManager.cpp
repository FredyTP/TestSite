#include "GuiManager.h"
#include <app/App.h>

namespace gui3d
{



GuiManager::GuiManager()
{
	_intersects = false;
	_first = nullptr;
}


GuiManager::~GuiManager()
{

}

void GuiManager::frame(float delta)
{
	if (_pointer.valid())
	{
		if (_pointer->posChanged())
		{
			_intersects = false;
			_intersWidgets.clear();
			for (int i = 0; i < _widgets.size(); i++)
			{
				if (_widgets[i]->intersectWithRay(_pointer->ray()))
				{
					_intersects = true;
					_intersWidgets.push_back(_widgets[i].getPtr());
				}			
			}
			if(!ts::App::Get().isVR())
				_pointer->setPosChanged(false);
			if (_intersects) this->sort();
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
void GuiManager::setPointer(ts::vr::Pointer * p)
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

void GuiManager::sort()
{
	float minDist = std::numeric_limits<float>().max();
	bg::math::Vector3 dist;
	_first = _intersWidgets[0];
	for (auto* w : _intersWidgets)
	{
		dist = w->pos();
		dist.sub(_pointer->ray().start());
		float dis = dist.magnitude();
		if (dis < minDist)
		{
			minDist = dis;
			_first = w;
		}
	}
}

}