#pragma once

//TestSite
#include <gui3d\Widget.h>

//Bg2e
#include <bg\bg2e.hpp>
namespace gui3d
{

bg::math::Vector4 HSVtoRGB(float fH, float fS, float fV);

class ValueCircle : public Widget
{
public:
	ValueCircle(float r, std::string ID, int sides = 20);
	virtual void init();
	virtual void createPolylist();
	virtual void setMaterial(bg::base::Material *mat, int id);
	virtual void build();

	virtual bool intersectWithRay(bg::physics::Ray &ray);
	virtual void updatePlane();
	virtual bool isIntersected() { return _isIntersected; }

	bg::math::Vector2 value() { return _value; }
	
protected:
	~ValueCircle();
	virtual void updateMaterial();
	bg::math::Matrix4 _matrix;

	float _radius;
	bg::math::Vector2 _value;
	int _sides;
};



}

