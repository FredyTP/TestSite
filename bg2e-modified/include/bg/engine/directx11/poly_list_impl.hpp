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

#ifndef _bg_engine_directx11_poly_list_impl_hpp_
#define _bg_engine_directx11_poly_list_impl_hpp_

#include <bg/engine/poly_list_impl.hpp>
#include <bg/engine/directx_includes.hpp>
#include <vector>

namespace bg {
namespace engine {
namespace directx11 {

class BG2E_EXPORT PolyListImpl : public bg::engine::PolyListImpl {
public:
	PolyListImpl(bg::base::PolyList *);

	virtual void build();
	virtual void draw();
	virtual void destroy();

#if BG2E_DIRECTX_AVAILABLE==1
	inline ID3D11Buffer * vertexBuffer() { return _vertexBuffer; }
	inline ID3D11Buffer * normalBuffer() { return _normalBuffer; }
	inline ID3D11Buffer * texCoord0Buffer() { return _texCoord0Buffer; }
	inline ID3D11Buffer * texCoord1Buffer() { return _texCoord1Buffer; }
	inline ID3D11Buffer * texCoord2Buffer() { return _texCoord2Buffer; }
	inline ID3D11Buffer * colorBuffer() { return _colorBuffer; }
	inline ID3D11Buffer * tangentBuffer() { return _tangentBuffer; };
	inline ID3D11Buffer * indexBuffer() { return _indexBuffer; }
#else
	inline void * _vertexBuffer() { return nullptr; }
	inline void * _normalBuffer() { return nullptr; }
	inline void * _texCoord0Buffer() { return nullptr; }
	inline void * _texCoord1Buffer() { return nullptr; }
	inline void * _texCoord2Buffer() { return nullptr; }
	inline void * _colorBuffer() { return nullptr; }
	inline void * _tangentBuffer() { return nullptr; }
	inline void * _indexBuffer() { return nullptr; }
#endif
protected:
	virtual ~PolyListImpl();

#if BG2E_DIRECTX_AVAILABLE==1
	ID3D11Buffer * _vertexBuffer;
	ID3D11Buffer * _normalBuffer;
	ID3D11Buffer * _texCoord0Buffer;
	ID3D11Buffer * _texCoord1Buffer;
	ID3D11Buffer * _texCoord2Buffer;
	ID3D11Buffer * _colorBuffer;
	ID3D11Buffer * _tangentBuffer;
	ID3D11Buffer * _indexBuffer;

	int _vertexCount;
	int _normalCount;
	int _texCoord0Count;
	int _texCoord1Count;
	int _texCoord2Count;
	int _colorCount;
	int _tangentCount;
	int _indexCount;

//	std::vector<ID3D11Buffer*> _bufferPointers;
//	std::vector<unsigned int> _strides;
//	std::vector<unsigned int> _offsets;
#endif
};

}
}
}

#endif
