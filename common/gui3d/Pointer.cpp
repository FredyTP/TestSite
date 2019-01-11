#include "Pointer.h"
#include <app\App.h>

namespace gui3d
{
Pointer::Pointer()
{
	_posChanged = false;
}


void Pointer::init()
{
	ts::App::Get().guiManager()->setPointer(this);
	_cam = ts::App::Get().scene()->mainCamera();
}

void Pointer::frame(float delta)
{

}






void Pointer::updateRay()
{
	if(_cam)
		_pRay.setWithCamera(_mousePos, _cam);
}





void Pointer::mouseMove(const bg::base::MouseEvent &evt)
{
	_mousePos = evt.pos();
	_posChanged = true;
	updateRay();
}


Pointer::~Pointer()
{
}

}

namespace ts
{
namespace vr
{
	Pointer::Pointer()
	{
		_posChanged = false;
	}

	void Pointer::init()
	{
		ts::App::Get().guiManager()->setPointer(this);
		_cam = ts::App::Get().scene()->mainCamera();

		_plist = new bg::base::PolyList(context());
		_plist->setDrawMode(bg::base::PolyList::kLines);
		_plist->addVertex(bg::math::Vector3(0.0f, 0.0f, 0.0f));
		_plist->addVertex(bg::math::Vector3(0.0f, 0.0f, -30.0f));
		_plist->addNormal(bg::math::Vector3(1.0f, 0.0f, 0.0f));
		_plist->addNormal(bg::math::Vector3(1.0f, 0.0f, 0.0f));
		_plist->addTexCoord0(bg::math::Vector2(0.0f, 0.0f));
		_plist->addTexCoord0(bg::math::Vector2(1.0f, 1.0f));

		_plist->addIndex(0);
		_plist->addIndex(1);
		_plist->build();

		bg::base::Material *mat = new bg::base::Material;

		mat->setDiffuse(bg::math::Color::Red());
		mat->setLightEmission(0.2f);
		mat->setCastShadows(false);

		_rayDrw = new bg::scene::Drawable();
		_rayDrw->addPolyList(_plist.getPtr(), mat);



	}
	/*void Pointer::init()
	{
		ts::App::Get().guiManager()->setPointer(this);
		_cam = ts::App::Get().scene()->mainCamera();
	}

	void Pointer::frame(float delta)
	{

	}

	void Pointer::updateRay()
	{
		if (_cam)
			_pRay.setWithCamera(_mousePos, _cam);
	}

	void Pointer::mouseMove(const bg::base::MouseEvent &evt)
	{
		_mousePos = evt.pos();
		_posChanged = true;
		updateRay();
	}*/

void ts::vr::Pointer::customEvent(const bg::base::CustomEvent & evt)
{
	
	const ControllerEventData * data = evt.data<ControllerEventData>();
	if (data) {
		_controllerIndex = data->controller()->index();
		switch (data->eventType()) {
		case Controller::kEventButtonRelease:
			if (data->button() == Controller::kButtonIdTouchpad) {
				std::cout << "BUTTON TOUCH release" << std::endl;
			}
			if (data->button() == Controller::kButtonIdTrigger)
			{
				std::cout << "DOWNBUTTON REALEASED" << std::endl;
				auto controller=Controller::Get(_controllerIndex);
				if (r)
				{
					r = false;
					bg::scene::Node* node = VRSystem::Get()->rayNode();
					node->addComponent(_rayDrw.getPtr());
				}
				bg::scene::Node* node = VRSystem::Get()->rayNode();
				node->setEnabled(false);
				
			}
			break;
		case Controller::kEventButtonPress:
			if (data->button() == Controller::kButtonIdTouchpad) {
				std::cout << "BUTTON TOUCH press" << std::endl;
			}
			if (data->button() == Controller::kButtonIdTrigger)
			{
				bg::scene::Node* node = VRSystem::Get()->rayNode();
				node->setEnabled(true);
			}
			else if (data->button() == Controller::kButtonIdMenu) {

			}
			break;
		}
	}
}

Pointer::~Pointer()
{
}

}
}




