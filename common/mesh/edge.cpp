#include "edge.hpp"

mesh::edge::edge()
{
}

mesh::edge::edge(vertex * v1, vertex * v2)
{
	this->setVertex(v1, v2);
}

void mesh::edge::setPosition(bg::math::Vector3  newPos)
{

}

void mesh::edge::translate(bg::math::Vector3 translation)
{
	_v1->translate(translation);
	_v2->translate(translation);
}

void mesh::edge::translate(bg::math::Vector3  direction, float distance)
{
	_v1->translate(direction, distance);
	_v2->translate(direction, distance);
}

void mesh::edge::scale(bg::math::Vector3  scaleFactors)
{
}

void mesh::edge::scale(float scale)
{
	this->scale(bg::math::Vector3(scale, scale, scale));
}

void mesh::edge::extrude(bg::math::Vector3 vec)
{
}

void mesh::edge::inset(float scale)
{
}

void mesh::edge::calcPosition()
{

	bg::math::Vector3 aux = _v1->position() + _v2->position();
	aux.scale(0.5f);
	_position = aux;

}

void mesh::edge::setVertex(vertex * v1, vertex * v2)
{
	_v1 = v1;
	_v2 = v2;
}

mesh::edge::~edge()
{
}
