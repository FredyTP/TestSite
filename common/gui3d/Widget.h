#pragma once
#include <bg\bg2e.hpp>
#include <GeneralConstants.h>
namespace gui3d
{

typedef enum
{
	kButton = 0,
	kValueBar = 1,
	kValueCircle = 2,
	kTextBar = 3
}widgetType;
typedef enum
{
	kButtonNone = 0, 
	kButtonUp = 1,
	kButtonDown = 2

}mouseButtonEvent;
typedef enum
{
	kLeftButton = 0,
	kMiddleButton = 1,
	kRightButton = 2, 

}buttonMask;

//ADD A WAY TO ENABLE THE DIFFERENT BUTTONS TO BE USED OR NO
//ADD A WAY TO ADD THE MATERIALS//
class Widget : public bg::scene::Component
{
public:
	Widget();
	Widget(float w, float h, std::string ID);
	Widget(bg::math::Vector2 size, std::string ID);
	bg::scene::Component* clone() { return new Widget(_size,"clone"); }
	virtual void init();
	virtual void createPolylist();
	virtual void setMaterial(bg::base::Material *mat, int id);
	virtual void build();
	std::string &getID() { return widgetID; }
	virtual void keyDown(const bg::base::KeyboardEvent &);
	virtual void mouseDown(const bg::base::MouseEvent &);
	virtual void mouseUp(const bg::base::MouseEvent &);

	mouseButtonEvent getButtonEvent();//A GOOD IDEA IS TO USE IT ON FRAME FUNCTION
	void hide();
	void show();

	void frame(float delta) { updatePlane(); }
	virtual bool intersectWithRay(bg::physics::Ray &ray);
	virtual void updatePlane();
	virtual bool isIntersected() { return _isIntersected; }
	widgetType type() { return _type; }

	bg::math::Vector3 pos() { return _plane.origin(); }
protected:
	~Widget();
	virtual void updateMaterial();

	std::string widgetID;

	widgetType _type;
	bg::math::Vector2 _size;
	bg::math::Vector2 _scale;

	bg::ptr<bg::base::PolyList> _plist;

	std::vector<bg::ptr<bg::base::Material> > _mat;

	bg::ptr<bg::scene::Drawable> _drw;

	bg::physics::Plane _plane;
	bool _isIntersected;

	bool _hide;
	mouseButtonEvent _currentButtonEvent;
	buttonMask _currentButtonMask;
	
};	




}


