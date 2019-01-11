#include "vertex.hpp"

mesh::vertex::vertex()
{
	_index = 0;
}

mesh::vertex::vertex(bg::math::Vector3 pos)
{
	_index = 0;
	_position = pos;
}

void mesh::vertex::setPosition(bg::math::Vector3 newPos)
{
	_position = newPos;
}

void mesh::vertex::translate(bg::math::Vector3 translation)
{
	_position.add(translation);
}

void mesh::vertex::translate(bg::math::Vector3 direction, float distance)
{
	bg::math::Vector3 dir = direction;
	dir.normalize();
	dir.scale(distance);
	_position.add(dir);
}

void mesh::vertex::scale(bg::math::Vector3  scaleFactors)
{
}

void mesh::vertex::scale(float scale)
{
	this->scale(bg::math::Vector3(scale, scale, scale));
}

void mesh::vertex::extrude(bg::math::Vector3 vec)
{
}

void mesh::vertex::inset(float scale)
{
}

void mesh::vertex::calcPosition()
{
	//_position = _position;
}

mesh::vertex::~vertex()
{

}
