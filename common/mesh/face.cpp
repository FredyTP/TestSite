#include "face.hpp"

mesh::face::face()
{

}

void mesh::face::setPosition(bg::math::Vector3 newPos)
{

}

void mesh::face::translate(bg::math::Vector3 translation)
{
	for (auto &v : _vertex)
	{
		v->translate(translation);
	}
}

void mesh::face::translate(bg::math::Vector3 direction, float distance)
{
	for (auto &v : _vertex)
	{
		v->translate(direction, distance);
	}
}

void mesh::face::scale(bg::math::Vector3 scaleFactors)
{
	bg::math::Vector3 vec;
	for (auto &v : _vertex)
	{
		vec = v->position();
		vec.sub(this->_position);
		vec.set(vec.x()*scaleFactors.x(), vec.y()*scaleFactors.y(), vec.z()*scaleFactors.z());
		vec.add(this->_position);
		v->setPosition(vec);
	}
}

void mesh::face::scale(float scale)
{
	this->scale(bg::math::Vector3(scale, scale, scale));
}

void mesh::face::extrude(bg::math::Vector3 vec)
{
}

void mesh::face::inset(float scale)
{
}

void mesh::face::calcPosition()
{
	bg::math::Vector3 aux;
	for (auto &v : _vertex)
	{
		aux.add(v->position());
	}
	aux.scale(1 / (float)_sides);
	_position = aux;
}

void mesh::face::calcNormal()
{
	bg::math::Vector3 v1 = _vertex[1]->position();
	v1.sub(_vertex[0]->position());

	bg::math::Vector3 v2 = _vertex[2]->position();
	v2.sub(_vertex[0]->position());

	_normal = v1;
	_normal.cross(v2);
	_normal.normalize();
}

mesh::face::~face()
{
	_vertex.clear();
}
