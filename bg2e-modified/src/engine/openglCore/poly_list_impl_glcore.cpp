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


#include <bg/engine/openglCore/poly_list_impl.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/engine/ogl_includes.hpp>

namespace bg {
namespace engine {
namespace openglCore {

PolyListImpl::PolyListImpl(bg::base::PolyList * ctx)
	:bg::engine::PolyListImpl(ctx)
	,_vertexArrayObject(0)
	,_vertexBuffer(0)
	,_normalBuffer(0)
	,_texCoord0Buffer(0)
	,_texCoord1Buffer(0)
	,_texCoord2Buffer(0)
	,_colorBuffer(0)
	,_indexBuffer(0)
	,_tangentBuffer(0)
{
	
}

PolyListImpl::~PolyListImpl() {
	
}

void PolyListImpl::build() {
	using namespace bg::engine::opengl;
	gl(ctx())->genVertexArrays(1,&_vertexArrayObject);
	gl(ctx())->bindVertexArray(_vertexArrayObject);

	if (plist()->vertexCount()>0) {
		gl(ctx())->genBuffers(1, &_vertexBuffer);
		gl(ctx())->bindBuffer(kArrayBuffer, _vertexBuffer);
		gl(ctx())->bufferData(kArrayBuffer, sizeof(float)* plist()->vertexCount(), plist()->vertexPointer(), kStaticDraw);
	}
	
	if (plist()->normalCount()>0) {
		gl(ctx())->genBuffers(1, &_normalBuffer);
		gl(ctx())->bindBuffer(kArrayBuffer, _normalBuffer);
		gl(ctx())->bufferData(kArrayBuffer, sizeof(float)* plist()->normalCount(), plist()->normalPointer(), kStaticDraw);
	}
	
	if (plist()->texCoord0Count()>0) {
		gl(ctx())->genBuffers(1, &_texCoord0Buffer);
		gl(ctx())->bindBuffer(kArrayBuffer, _texCoord0Buffer);
		gl(ctx())->bufferData(kArrayBuffer, sizeof(float)* plist()->texCoord0Count(), plist()->texCoord0Pointer(), kStaticDraw);
	}
	
	// TODO: performance improvement. Fix the deferred render pipeline to avoid the need for having
	// a lightmap vbo
	if (plist()->texCoord1Count()==0 && plist()->texCoord0Count()>0) {
		plist()->addTexCoord1Vector(plist()->texCoord0Pointer(), plist()->texCoord0Count());
	}
	
	if (plist()->texCoord1Count()>0) {
		gl(ctx())->genBuffers(1, &_texCoord1Buffer);
		gl(ctx())->bindBuffer(kArrayBuffer, _texCoord1Buffer);
		gl(ctx())->bufferData(kArrayBuffer, sizeof(float)* plist()->texCoord1Count(), plist()->texCoord1Pointer(), kStaticDraw);
	}
	
	if (plist()->texCoord2Count()>0) {
		gl(ctx())->genBuffers(1, &_texCoord2Buffer);
		gl(ctx())->bindBuffer(kArrayBuffer, _texCoord2Buffer);
		gl(ctx())->bufferData(kArrayBuffer, sizeof(float)* plist()->texCoord2Count(), plist()->texCoord2Pointer(), kStaticDraw);
	}
	
	if (plist()->colorCount()>0) {
		gl(ctx())->genBuffers(1, reinterpret_cast<GLuint*>(&_colorBuffer));
		gl(ctx())->bindBuffer(kArrayBuffer, _colorBuffer);
		gl(ctx())->bufferData(kArrayBuffer, sizeof(float)* plist()->colorCount(), plist()->colorPointer(), kStaticDraw);
	}
	
	if (plist()->tangentCount()>0) {
		gl(ctx())->genBuffers(1, reinterpret_cast<GLuint*>(&_tangentBuffer));
		gl(ctx())->bindBuffer(kArrayBuffer, _tangentBuffer);
		gl(ctx())->bufferData(kArrayBuffer, sizeof(float)* plist()->tangentCount(), plist()->tangentPointer(), kStaticDraw);
	}
	
	if (plist()->indexCount()>0) {
		gl(ctx())->genBuffers(1, &_indexBuffer);
		gl(ctx())->bindBuffer(kElementArrayBuffer, _indexBuffer);
		unsigned int *indexPtr = plist()->indexPointer();
		gl(ctx())->bufferData(kElementArrayBuffer, sizeof(int)* plist()->indexCount(), indexPtr, kStaticDraw);
	}
	
	gl(ctx())->bindBuffer(kArrayBuffer, 0);
	gl(ctx())->bindBuffer(kElementArrayBuffer, 0);
	gl(ctx())->bindVertexArray(0);
}

void PolyListImpl::draw() {
	using namespace bg::engine::opengl;
    gl(ctx())->bindVertexArray(_vertexArrayObject);
	gl(ctx())->bindBuffer(kElementArrayBuffer, _indexBuffer);

	uint32_t drawMode = kTriangles;
	switch (plist()->drawMode()) {
		case base::PolyList::kTessellation:
		#if BG2E_OPENGL_ES_AVAILABLE==0		// Ignore tessellation in OpenGL ES. Fallback to GL_TRIANGLES
		{
			static const int patchVertices = 3;
			gl(ctx())->patchParameteri(kPatchVertices, patchVertices);
			drawMode = kPatches;
			break;
		}
		#endif
		case base::PolyList::kTriangles:
			drawMode = kTriangles;
			break;
		case base::PolyList::kLines:
			drawMode = kLines;
			break;
	}
	gl(ctx())->drawElements(drawMode, plist()->indexCount(), kUnsignedInt, 0);
	gl(ctx())->bindBuffer(kElementArrayBuffer, 0);
	gl(ctx())->bindVertexArray(0);
}

void PolyListImpl::destroy() {
	using namespace bg::engine::opengl;
	gl(ctx())->bindVertexArray(0);
	gl(ctx())->bindBuffer(kArrayBuffer, 0);
	gl(ctx())->bindBuffer(kElementArrayBuffer, 0);
	
	if (_vertexBuffer > 0) {
		gl(ctx())->deleteBuffers(1, &_vertexBuffer);
		_vertexBuffer = 0;
	}
	if (_normalBuffer > 0) {
		gl(ctx())->deleteBuffers(1, &_normalBuffer);
		_normalBuffer = 0;
	}
	if (_texCoord0Buffer > 0) {
		gl(ctx())->deleteBuffers(1, &_texCoord0Buffer);
		_texCoord0Buffer = 0;
	}
	if (_texCoord1Buffer > 0) {
		gl(ctx())->deleteBuffers(1, &_texCoord1Buffer);
		_texCoord1Buffer = 0;
	}
	if (_texCoord2Buffer > 0) {
		gl(ctx())->deleteBuffers(1, &_texCoord2Buffer);
		_texCoord2Buffer = 0;
	}
	if (_colorBuffer > 0) {
		gl(ctx())->deleteBuffers(1, &_colorBuffer);
		_colorBuffer = 0;
	}
	if (_indexBuffer > 0) {
		gl(ctx())->deleteBuffers(1, &_indexBuffer);
		_indexBuffer = 0;
	}
	if (_tangentBuffer > 0) {
		gl(ctx())->deleteBuffers(1, &_tangentBuffer);
		_tangentBuffer = 0;
	}
	gl(ctx())->deleteVertexArray(1, &_vertexArrayObject);
}

}
}
}
