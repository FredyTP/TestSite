#pragma once
#include <bg\bg2e.hpp>
#include <vector>

namespace gui3d
{


class Rectangle
{
public:
	Rectangle();
	void set(int w, int h, bg::math::Matrix4 &m);

protected:
	
	bg::physics::Plane _plane;
	std::vector<bg::math::Vector3> _RectPoints;
	~Rectangle();
};


}
