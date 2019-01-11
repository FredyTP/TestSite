#pragma once

//TestSite
#include <gui3d\Widget.h>

//Bg2e
#include <bg\bg2e.hpp>

//std
#include <vector>

namespace gui3d
{


class Pointer : public bg::scene::Component
{
public:
	Pointer();
	bg::scene::Component* clone() { return new Pointer; }
	virtual void init();
	virtual void frame(float delta);
	virtual void updateRay();


	void mouseMove(const bg::base::MouseEvent &);
	bg::physics::Ray & ray() { return _pRay; }

	bool posChanged() { return _posChanged; }
	void setPosChanged(bool c) { _posChanged = c; }


protected:
	~Pointer();
	bg::scene::Camera* _cam;
	bg::math::Vector2i _mousePos;
	bool _posChanged;
	bg::physics::Ray _pRay;
	

};

}
namespace ts
{
namespace vr
{
	class Pointer : public gui3d::Pointer
	{
	public:
		Pointer();
		bg::scene::Component* clone() { return new Pointer; }
		virtual void init();
		void customEvent(const bg::base::CustomEvent &evt);

		bg::physics::Ray & ray() { return _pRay; }

		bool posChanged() { return _posChanged; }
		void setPosChanged(bool c) { _posChanged = c; }


	protected:
		~Pointer();
		int _controllerIndex;
		bg::ptr<bg::scene::Drawable> _rayDrw;
		bg::ptr<bg::base::PolyList> _plist;
		bool r = true;

	};





}
}

