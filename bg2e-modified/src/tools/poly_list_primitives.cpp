/*
 *	bg2 engine license
 *	Copyright (c) 2016 Fernando Serrano <ferserc1@gmail.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *	of the Software, and to permit persons to whom the Software is furnished to do
 *	so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 *	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <bg/tools/poly_list_primitives.hpp>

namespace bg {
namespace tools {

PolyListPrimitives::PolyListPrimitives(bg::base::Context * ctx)
	:ContextObject(ctx)
{
	
}

bg::base::PolyList * PolyListPrimitives::cube(float width, float height, float depth) {
	bg::ptr<bg::base::PolyList> plist = new bg::base::PolyList(context());
	
	float x = width/2.0f;
	float y = height/2.0f;
	float z = depth/2.0f;
	
	std::vector<float> position = {
		x,-y,-z, -x,-y,-z, -x, y,-z,  x, y,-z,		// back face
		x,-y, z,  x,-y,-z,  x, y,-z,  x, y, z,		// right face
		-x,-y, z,  x,-y, z,  x, y, z, -x, y, z, 	// front face
		-x,-y,-z, -x,-y, z, -x, y, z, -x, y,-z,		// left face
		-x, y, z,  x, y, z,  x, y,-z, -x, y,-z,		// top face
		x,-y, z, -x,-y, z, -x,-y,-z,  x,-y,-z		// bottom face
	};
	
	std::vector<float> normal = {
		0.0f, 0.0f,-1.0f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,-1.0f,		// back face
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,		// right face
		0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,		// front face
		-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,		// left face
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,		// top face
		0.0f,-1.0f, 0.0f,  0.0f,-1.0f, 0.0f,  0.0f,-1.0f, 0.0f,  0.0f,-1.0f, 0.0f		// bottom face
	};
	
	std::vector<float> texCoord = {
		0.0f, 0.0f,	1.0f, 0.0f,	1.0f, 1.0f,	0.0f,1.0f,
		0.0f, 0.0f,	1.0f, 0.0f,	1.0f, 1.0f,	0.0f,1.0f,
		0.0f, 0.0f,	1.0f, 0.0f,	1.0f, 1.0f,	0.0f,1.0f,
		0.0f, 0.0f,	1.0f, 0.0f,	1.0f, 1.0f,	0.0f,1.0f,
		0.0f, 0.0f,	1.0f, 0.0f,	1.0f, 1.0f,	0.0f,1.0f,
		0.0f, 0.0f,	1.0f, 0.0f,	1.0f, 1.0f,	0.0f,1.0f
	};
	
	std::vector<unsigned int> index = {
		0, 1, 2,	 2, 3, 0,
		4, 5, 6,	 6, 7, 4,
		8, 9,10,	10,11, 8,
		12,13,14,	14,15,12,
		16,17,18,	18,19,16,
		20,21,22,	22,23,20
	};
	
	plist->addVertexVector(&position[0], position.size());
	plist->addNormalVector(&normal[0], normal.size());
	plist->addTexCoord0Vector(&texCoord[0], texCoord.size());
	plist->addIndexVector(&index[0], index.size());
	
	plist->build();
	return plist.release();
}

bg::base::PolyList * PolyListPrimitives::plane(float width, float depth, Alignment align, const bg::math::Vector2 & offset) {
	bg::ptr<bg::base::PolyList> plist = new bg::base::PolyList(context());
	
	float x = width / 2.0f;
	float y = depth / 2.0f;
	float xMin = -x + offset.x();
	float xMax = x + offset.x();
	float yMin = -y + offset.y();
	float yMax = y + offset.y();
	
	std::vector<float> position;
	std::vector<float> normal;
	std::vector<float> texCoord0;
	
	switch (align) {
	case kAlignXPlane:
		position = {
			0.0f, xMax, yMin,
			0.0f, xMin, yMin,
			0.0f, xMin, yMax,
			0.0f, xMin, yMax,
			0.0f, xMax, yMax,
			0.0f, xMax, yMin
		};
		
		normal = {
			1.0f,0.0f,0.0f,
			1.0f,0.0f,0.0f,
			1.0f,0.0f,0.0f,
			1.0f,0.0f,0.0f,
			1.0f,0.0f,0.0f,
			1.0f,0.0f,0.0f
		};
		
		texCoord0 = {
			0.0f,0.0f,
			1.0f,0.0f,
			1.0f,1.0f,
			1.0f,1.0f,
			0.0f,1.0f,
			0.0f,0.0f
		};
		
		break;
	case kAlignYPlane:
		position = {
			 xMin,0.0f, yMin,
			 xMax,0.0f, yMin,
			 xMax,0.0f, yMax,
			 xMax,0.0f, yMax,
			 xMin,0.0f, yMax,
			 xMin,0.0f, yMin
		};
		
		normal = {
			0.0f,1.0f,0.0f,
			0.0f,1.0f,0.0f,
			0.0f,1.0f,0.0f,
			0.0f,1.0f,0.0f,
			0.0f,1.0f,0.0f,
			0.0f,1.0f,0.0f
		};
		
		texCoord0 = {
			0.0f,0.0f,
			1.0f,0.0f,
			1.0f,1.0f,
			1.0f,1.0f,
			0.0f,1.0f,
			0.0f,0.0f
		};
		break;
	case kAlignZPlane:
		position = {
			 xMax, yMin,0.0f,
			 xMin, yMin,0.0f,
			 xMin, yMax,0.0f,
			 xMin, yMax,0.0f,
			 xMax, yMax,0.0f,
			 xMax, yMin,0.0f
		};
		
		normal = {
			0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f,
			0.0f,0.0f,1.0f
		};
		
		texCoord0 = {
			1.0f,0.0f,
			0.0f,0.0f,
			0.0f,1.0f,
			0.0f,1.0f,
			1.0f,1.0f,
			1.0f,0.0f
		};
		break;
	}
	
	
	
	std::vector<unsigned int > index = { 2,1,0,5,4,3 };
	
	plist->addVertexVector(&position[0], position.size());
	plist->addNormalVector(&normal[0], normal.size());
	plist->addTexCoord0Vector(&texCoord0[0], texCoord0.size());
	plist->addIndexVector(&index[0], index.size());
	
	plist->build();
	return plist.release();
}

bg::base::PolyList * PolyListPrimitives::sphere(float radius, float slc, float stacks) {
	using namespace bg::base;
	using namespace bg::math;
	bg::ptr<bg::base::PolyList> plist = new bg::base::PolyList(context());
	
	int slices = static_cast<int>(slc) + 1;
	const float R = 1.0f/static_cast<float>(stacks-1);
	const float S = 1.0f/static_cast<float>(slices-1);
	float r, s;
	
	std::vector<float> vertex;
	std::vector<float> normal;
	std::vector<float> texCoord;
	std::vector<unsigned int> index;
	
	for(r = 0.0f; r < stacks; r++) for(s = 0.0f; s < slices; s++) {
		const float y = trigonometry::sin( -kPiOver2 + kPi * r * R );
		const float x = trigonometry::cos(2*kPi * s * S) * trigonometry::sin(kPi * r * R);
		const float z = trigonometry::sin(2*kPi * s * S) * trigonometry::sin(kPi * r * R);
		texCoord.push_back(r * R); texCoord.push_back(s * S);
		normal.push_back(x); normal.push_back(y); normal.push_back(z);
		vertex.push_back(x * radius); vertex.push_back(y * radius); vertex.push_back(z * radius);
	}
	
	for(r = 0.0f; r < stacks - 1.0f; r++) for(s = 0.0f; s < slices - 1.0f; s++) {
		unsigned int i1 = static_cast<unsigned int>(r) * slices + static_cast<unsigned int>(s);
		unsigned int i2 = static_cast<unsigned int>(r) * slices + (static_cast<unsigned int>(s) + 1);
		unsigned int i3 = (static_cast<unsigned int>(r) + 1) * slices + (static_cast<unsigned int>(s) + 1);
		unsigned int i4 = (static_cast<unsigned int>(r) + 1) * slices + static_cast<unsigned int>(s);
		index.push_back(i1); index.push_back(i4); index.push_back(i3);
		index.push_back(i3); index.push_back(i2); index.push_back(i1);
	}
	
	plist->addVertexVector(&vertex[0], vertex.size());
	plist->addNormalVector(&normal[0], normal.size());
	plist->addTexCoord0Vector(&texCoord[0], texCoord.size());
	plist->addIndexVector(&index[0], index.size());
	plist->build();
	
	return plist.release();
}

}
}
