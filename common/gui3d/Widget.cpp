#include "Widget.h"
#include <app\App.h>

namespace gui3d
{




Widget::Widget()
{
}

Widget::Widget(float w, float h, std::string ID)
{
	_size.set(w, h);
	_isIntersected = false;
	widgetID = ID;
	_hide = true;
	_mat.resize(3);
	_mat[0] = new bg::base::Material();
	_mat[0]->setDiffuse(bg::math::Color::Blue());
	_mat[0]->setCullFace(false);
}

Widget::Widget(bg::math::Vector2 size, std::string ID)
{
	_size=size;
	_isIntersected = false;
	widgetID = ID;
	_hide = true;
	_mat.resize(3);
	_mat[0] = new bg::base::Material();
	_mat[0]->setDiffuse(bg::math::Color::Blue());
	_mat[0]->setCullFace(false);

}

void Widget::init()
{
	_initialized = true;
	ts::App::Get().guiManager()->addWidget(this);
	this->createPolylist();
	this->build();
	this->updatePlane();
	if (!_hide)
	{
		node()->addComponent(_drw.getPtr());
	}
}

void Widget::createPolylist()
{
	using namespace bg::math;
	float halfw = _size.x() / 2;
	float halfh = _size.y() / 2;
	_plist = new bg::base::PolyList(context());
	_plist->setDrawMode(bg::base::PolyList::kTriangles);

	_plist->addVertex(Vector3(halfw, halfh, 0));
	_plist->addVertex(Vector3(-halfw, halfh, 0));
	_plist->addVertex(Vector3(-halfw, -halfh, 0));
	_plist->addVertex(Vector3(halfw, -halfh, 0));

	_plist->addNormal(Vector3(0.0f, 0.0f, 1.0f));
	_plist->addNormal(Vector3(0.0f, 0.0f, 1.0f));
	_plist->addNormal(Vector3(0.0f, 0.0f, 1.0f));
	_plist->addNormal(Vector3(0.0f, 0.0f, 1.0f));

	_plist->addTexCoord0(Vector2(1.0f, 1.0f));
	_plist->addTexCoord0(Vector2(0.0f, 1.0f));
	_plist->addTexCoord0(Vector2(0.0f, 0.0f));
	_plist->addTexCoord0(Vector2(1.0f, 0.0f));

	_plist->addTriangle(0, 1, 3);
	_plist->addTriangle(1, 2, 3);

	_plist->build();
}

void Widget::setMaterial(bg::base::Material * mat, int id)
{
	if (mat == nullptr)
	{
		return;
	}
	if (id < _mat.size() && id >= 0)
	{
		_mat[id] = mat;
	}

}

void Widget::build()
{
	_drw = new bg::scene::Drawable();
	_drw->addPolyList(_plist.getPtr(), _mat[0].getPtr());
}

void Widget::keyDown(const bg::base::KeyboardEvent &ev)
{

}

void Widget::mouseDown(const bg::base::MouseEvent &evt)
{
	if (_isIntersected)
	{
		if (evt.mouse().buttonMask() == evt.mouse().kLeftButton)
		{
			_currentButtonEvent = kButtonDown;
			_currentButtonMask = kLeftButton;
			this->updateMaterial();
		}
		else if (evt.mouse().buttonMask() == evt.mouse().kMiddleButton)
		{
			_currentButtonEvent = kButtonDown;
			_currentButtonMask = kMiddleButton;
			this->updateMaterial();
		}
		else if (evt.mouse().buttonMask() == evt.mouse().kRightButton)
		{
			_currentButtonEvent = kButtonDown;
			_currentButtonMask = kRightButton;
			this->updateMaterial();
		}
	}
	else
	{
		_currentButtonEvent = kButtonNone;
	}
}

void Widget::mouseUp(const bg::base::MouseEvent &evt)
{
	if (_isIntersected)
	{
		_currentButtonEvent = kButtonUp;
		this->updateMaterial();
	}
	else
	{
		_currentButtonEvent = kButtonNone;
	}
	
}

mouseButtonEvent Widget::getButtonEvent()
{
	mouseButtonEvent aux = _currentButtonEvent;
	_currentButtonEvent = kButtonNone;
	return aux;
}

void Widget::hide()
{
	if (!_hide)
	{
		_hide = true;

		if (_drw->node() != nullptr)
		{
			node()->removeComponent(_drw.getPtr());
		}
	}
}

void Widget::show()
{
	if (_hide)
	{
		_hide = false;
		if(_initialized)
			node()->addComponent(_drw.getPtr());
	}
}

bool Widget::intersectWithRay(bg::physics::Ray & ray)
{
	bool _intersects;
	bg::math::Vector3 p0(_plane.origin());
	bg::math::Vector3 n(_plane.normal());
	bg::math::Vector3 l0(ray.start());
	bg::math::Vector3 l(ray.vector());
	float num = p0.sub(l0).dot(n);
	float den = l.dot(n);
	float distance = 0;

	if (den == 0) {
		_intersects = false;
	}
	else if ((distance = num / den)>ray.magnitude()) {
		_intersects = false;
	}
	else if (distance<0.0f) {	// Intersects behind the ray direction
		_intersects = false;
	}
	else {
		_intersects = false;
		bg::math::Vector3 inter;
		inter = ray.pointAtDistance(distance);
		bg::math::Vector3 dist = inter - _plane.origin();
		if (dist.magnitude() <= (_size*_scale).magnitude() / 2)
			_intersects = true;
	}
	if (_intersects !=_isIntersected)
	{
		_isIntersected = _intersects;
		this->updateMaterial();
	}


	
	return _intersects;
}

void Widget::updatePlane()
{
	bg::ptr<bg::scene::TransformVisitor> visitor = new bg::scene::TransformVisitor;
	node()->acceptReverse(visitor.getPtr());
	
	_scale = visitor->matrix().scale().xy();
	_plane.setOrigin(visitor->matrix().position());
	_plane.setNormal(visitor->matrix().forwardVector());
}

Widget::~Widget()
{

}

void Widget::updateMaterial()
{
	if (!_hide)
	{
		if (_isIntersected)
		{
			if (_currentButtonEvent == kButtonDown)
			{
				_drw->removePolyList(_plist.getPtr());
				_drw->addPolyList(_plist.getPtr(), _mat[2].getPtr());
				node()->addComponent(_drw.getPtr());
			}
			else
			{
				_drw->removePolyList(_plist.getPtr());
				_drw->addPolyList(_plist.getPtr(), _mat[1].getPtr());
			
			}
		}
		else
		{
			_drw->removePolyList(_plist.getPtr());
			_drw->addPolyList(_plist.getPtr(), _mat[0].getPtr());
			
		}
	}

}

}
