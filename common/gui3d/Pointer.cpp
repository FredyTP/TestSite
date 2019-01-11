#include "Pointer.h"
#include <app\App.h>


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
