#include "ValueCircle.h"
#include <app/App.h>

namespace gui3d
{

bg::math::Vector4 HSVtoRGB(float fH, float fS, float fV) {
	float fC = fV * fS; // Chroma
	float fHPrime = fmod(fH / 60.0f, 6.0f);
	float fX = fC * (1.0f - fabs(fmod(fHPrime, 2.0f) - 1.0f));
	float fM = fV - fC;
	bg::math::Vector4 color;
	if (0 <= fHPrime && fHPrime < 1) {
		color.x(fC);
		color.y(fX);
		color.z(0);
	}
	else if (1 <= fHPrime && fHPrime < 2) {
		color.x(fX);
		color.y(fC);
		color.z(0);
	}
	else if (2 <= fHPrime && fHPrime < 3) {
		color.x(0);
		color.y(fC);
		color.z(fX);
	}
	else if (3 <= fHPrime && fHPrime < 4) {
		color.x(0);
		color.y(fX);
		color.z(fC);
	}
	else if (4 <= fHPrime && fHPrime < 5) {
		color.x(fX);
		color.y(0);
		color.z(fC);
	}
	else if (5 <= fHPrime && fHPrime < 6) {
		color.x(fC);
		color.y(0);
		color.z(fX);
	}
	else {
		color.x(0);
		color.y(0);
		color.z(0);
	}

	color = color + bg::math::Vector4(fM, fM, fM, 1);
	return color;
}



ValueCircle::ValueCircle(float r, std::string ID, int sides)
{
	_size.set(r*2,r*2);
	_radius = r;
	_isIntersected = false;
	widgetID = ID;
	_hide = true;
	_mat.resize(1);
	_mat[0] = new bg::base::Material();
	_mat[0]->setDiffuse(bg::math::Color::Blue());
	_mat[0]->setCullFace(true);
	_mat[0]->setLightEmission(0.2f);
	_sides = sides;
}

void ValueCircle::init()
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

void ValueCircle::createPolylist()
{
	using namespace bg::math;

	_plist = new bg::base::PolyList(context());
	_plist->setDrawMode(bg::base::PolyList::kTriangles);

	//add center//
	_plist->addVertex(Vector3(0.0f, 0.0f, 0.0f));
	_plist->addNormal(Vector3(0.0f, 0.0f, 1.0f));
	_plist->addTexCoord0(Vector2(0.5f, 0.5f));
	float x;
	float y;
	for (int i = 1; i <= _sides + 1; i++)
	{
		x = (float)(i) / _sides* bg::math::k2Pi;
		y = trigonometry::sin(x);
		x = trigonometry::cos(x);
		_plist->addVertex(Vector3(x, y, 0.0f));
		_plist->addNormal(Vector3(0.0f, 0.0f, 1.0f));
		_plist->addTexCoord0(Vector2(x / 2 + 0.5f, y / 2 + 0.5f));
		if (i > 1)
		{
			_plist->addTriangle(0, i - 1, i);
		}
		
	}
	_plist->build();

}

void ValueCircle::setMaterial(bg::base::Material * mat, int id)
{
	_mat[0] = mat;
}

void ValueCircle::build()
{
	_drw = new bg::scene::Drawable();
	_drw->addPolyList(_plist.getPtr(), _mat[0].getPtr());
}

bool ValueCircle::intersectWithRay(bg::physics::Ray & ray)
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
		if (dist.magnitude() <= _radius)
		{
			_intersects = true;
			bg::math::Vector4 val(inter.x(),inter.y(),inter.z(),1);
			val = _matrix.multVector(val);
			_value = val.xy();
			bg::math::Color color = HSVtoRGB(bg::math::trigonometry::atan2(_value.x(), _value.y()),_value.magnitude(), 1.0f);
			//node()->drawable()->material(0)->setDiffuse(color);
		}
			
	}
	if (_intersects != _isIntersected)
	{
		_isIntersected = _intersects;
		this->updateMaterial();
	}



	return _intersects;
}

void ValueCircle::updatePlane()
{
	bg::ptr<bg::scene::TransformVisitor> visitor = new bg::scene::TransformVisitor;
	node()->acceptReverse(visitor.getPtr());
	_matrix = visitor->matrix();
	_matrix.invert();

	_scale = visitor->matrix().scale().xy();
	_plane.setOrigin(visitor->matrix().position());
	_plane.setNormal(visitor->matrix().forwardVector());
}

void ValueCircle::updateMaterial()
{

}

ValueCircle::~ValueCircle()
{
}






}