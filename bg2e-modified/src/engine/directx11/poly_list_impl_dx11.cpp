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


#include <bg/engine/directx11/poly_list_impl.hpp>
#include <bg/base/poly_list.hpp>

namespace bg {
namespace engine {
namespace directx11 {

PolyListImpl::PolyListImpl(bg::base::PolyList * ctx)
	:bg::engine::PolyListImpl(ctx)
#if BG2E_DIRECTX_AVAILABLE==1
	,_vertexBuffer(nullptr)
	,_normalBuffer(nullptr)
	,_texCoord0Buffer(nullptr)
	,_texCoord1Buffer(nullptr)
	,_texCoord2Buffer(nullptr)
	,_colorBuffer(nullptr)
	,_tangentBuffer(nullptr)
	,_indexBuffer(nullptr)
	,_vertexCount(0)
	,_normalCount(0)
	,_texCoord0Count(0)
	,_texCoord1Count(0)
	,_texCoord2Count(0)
	,_colorCount(0)
	,_tangentCount(0)
	,_indexCount(0)
#endif
{
}

PolyListImpl::~PolyListImpl() {
	
}

#if BG2E_DIRECTX_AVAILABLE==1
template <class T>
ID3D11Buffer * createBuffer(ID3D11Device * device, UINT flags, const T * vector, int length) {
	ID3D11Buffer * buffer = nullptr;

	D3D11_BUFFER_DESC bufferDescriptor;
	D3D11_SUBRESOURCE_DATA bufferData;

	bufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
	bufferDescriptor.ByteWidth = sizeof(T) * length;
	bufferDescriptor.BindFlags = flags;
	bufferDescriptor.CPUAccessFlags = 0;
	bufferDescriptor.MiscFlags = 0;
	bufferDescriptor.StructureByteStride = 0;

	bufferData.pSysMem = vector;
	bufferData.SysMemPitch = 0;
	bufferData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&bufferDescriptor, &bufferData, &buffer))) {
		return nullptr;
	}
	return buffer;
}

void PolyListImpl::build() {
	destroy();
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(_plist->context());

	if (dxCt) {
		ID3D11Device * device = bg::native_cast<ID3D11Device*>(dxCt->device());
		if (plist()->vertexCount()>0) {
			_vertexBuffer = createBuffer(device, D3D11_BIND_VERTEX_BUFFER, plist()->vertexPointer(), plist()->vertexCount());
	//		_bufferPointers.push_back(_vertexBuffer);
	//		_strides.push_back(sizeof(float) * 3);
	//		_offsets.push_back(0);
		}

		if (plist()->normalCount()>0) {
			_normalBuffer = createBuffer(device, D3D11_BIND_VERTEX_BUFFER, plist()->normalPointer(), plist()->normalCount());
	//		_bufferPointers.push_back(_normalBuffer);
	//		_strides.push_back(sizeof(float) * 3);
	//		_offsets.push_back(0);
		}

		if (plist()->texCoord0Count()>0) {
			_texCoord0Buffer = createBuffer(device, D3D11_BIND_VERTEX_BUFFER, plist()->texCoord0Pointer(), plist()->texCoord0Count());
	//		_bufferPointers.push_back(_texCoord0Buffer);
	//		_strides.push_back(sizeof(float) * 2);
	//		_offsets.push_back(0);
		}

		if (plist()->texCoord1Count()>0) {
			_texCoord1Buffer = createBuffer(device, D3D11_BIND_VERTEX_BUFFER, plist()->texCoord1Pointer(), plist()->texCoord1Count());
	//		_bufferPointers.push_back(_texCoord1Buffer);
	//		_strides.push_back(sizeof(float) * 2);
	//		_offsets.push_back(0);
		}

		if (plist()->texCoord2Count()>0) {
			_texCoord2Buffer = createBuffer(device, D3D11_BIND_VERTEX_BUFFER, plist()->texCoord2Pointer(), plist()->texCoord2Count());
	//		_bufferPointers.push_back(_texCoord2Buffer);
	//		_strides.push_back(sizeof(float) * 2);
	//		_offsets.push_back(0);
		}

		if (plist()->colorCount()>0) {
			_colorBuffer = createBuffer(device, D3D11_BIND_VERTEX_BUFFER, plist()->colorPointer(), plist()->colorCount());
	//		_bufferPointers.push_back(_colorBuffer);
	//		_strides.push_back(sizeof(float) * 4);
	//		_offsets.push_back(0);
		}

		if (plist()->tangentCount()>0) {
			_tangentBuffer = createBuffer(device, D3D11_BIND_VERTEX_BUFFER, plist()->tangentPointer(), plist()->tangentCount());
	//		_bufferPointers.push_back(_tangentBuffer);
	//		_strides.push_back(sizeof(float) * 3);
	//		_offsets.push_back(0);
		}

		if (plist()->indexCount()>0) {
			_indexBuffer = createBuffer(device, D3D11_BIND_INDEX_BUFFER, plist()->indexPointer(), plist()->indexCount());
		}
	}
}

void PolyListImpl::draw() {
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(plist()->context());

	if (dxCt) {
		ID3D11DeviceContext * deviceContext = bg::native_cast<ID3D11DeviceContext*>(dxCt->deviceContext());

		

		deviceContext->DrawIndexed(plist()->indexCount(), 0, 0);
	}
}

void PolyListImpl::destroy() {
	if (_vertexBuffer) {
		_vertexBuffer->Release();
		_vertexBuffer = nullptr;
	}
	if (_normalBuffer) {
		_normalBuffer->Release();
		_normalBuffer = nullptr;
	}
	if (_texCoord0Buffer) {
		_texCoord0Buffer->Release();
		_texCoord0Buffer = nullptr;
	}
	if (_texCoord1Buffer) {
		_texCoord1Buffer->Release();
		_texCoord1Buffer = nullptr;
	}
	if (_texCoord2Buffer) {
		_texCoord2Buffer->Release();
		_texCoord2Buffer = nullptr;
	}
	if (_colorBuffer) {
		_colorBuffer->Release();
		_colorBuffer = nullptr;
	}
	if (_tangentBuffer) {
		_tangentBuffer->Release();
		_tangentBuffer = nullptr;
	}
	if (_indexBuffer) {
		_indexBuffer->Release();
		_indexBuffer = nullptr;
	}
	
	_normalCount = 0;
	_texCoord0Count = 0;
	_texCoord1Count = 0;
	_texCoord2Count = 0;
	_colorCount = 0;
	_tangentCount = 0;
	_indexCount = 0;
}

#else

void PolyListImpl::build() {}
void PolyListImpl::draw() {}
void PolyListImpl::destroy() {}

#endif

}
}
}
