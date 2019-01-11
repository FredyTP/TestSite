#pragma once

//TestSite
#include <gui3d\Widget.h>

//Bg2e
#include <bg\bg2e.hpp>

//std
#include <vector>

class Pointer : public bg::scene::Component
{
public:
	Pointer();
	bg::scene::Component* clone() { return new Pointer; }
	void init();
	void frame(float delta);
	void updateRay();


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

