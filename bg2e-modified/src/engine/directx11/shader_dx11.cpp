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


#include <bg/engine/directx11/shader.hpp>
#include <bg/engine/directx11/poly_list_impl.hpp>
#include <bg/engine/directx11/texture_impl.hpp>

namespace bg {
namespace engine {
namespace directx11 {

ConstantBuffer::ConstantBuffer(bg::base::Context * ctx, unsigned long bufferSize, BufferUsage usage)
	:bg::base::ContextObject(ctx)
	,_bufferSize(bufferSize)
	,_usage(usage)
#if BG2E_DIRECTX_AVAILABLE==1
	,_device(nullptr)
	,_deviceContext(nullptr)
	,_buffer(nullptr)
#endif
{
#if BG2E_DIRECTX_AVAILABLE==1
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(ctx);

	if (dxCt) {
		_device = bg::native_cast<ID3D11Device*>(dxCt->device());
		_deviceContext = bg::native_cast<ID3D11DeviceContext*>(dxCt->deviceContext());

		D3D11_BUFFER_DESC desc;
		desc.Usage = static_cast<D3D11_USAGE>(_usage);
		desc.ByteWidth = _bufferSize;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT res = _device->CreateBuffer(&desc, nullptr, &_buffer);
		if (FAILED(res)) {
			throw bg::base::DirectXEngineException("Error creating constant buffer");
		}
	}
#endif
}

ConstantBuffer::~ConstantBuffer() {
#if BG2E_DIRECTX_AVAILABLE==1
	if (_buffer) {
		_buffer->Release();
	}
#endif
}

Shader::Shader(bg::base::Context * ctx)
	:bg::base::ContextObject(ctx)
{

}

Shader::~Shader() {

}

#if BG2E_DIRECTX_AVAILABLE==1

void Shader::destroy() {
}

ID3D10Blob * Shader::compileShader(const std::string & source, const std::string & entryPoint, const std::string & target) {
	ID3D10Blob * shader = nullptr;
	ID3D10Blob * errorBlob = nullptr;
	if (FAILED(D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr,
						  entryPoint.c_str(), target.c_str(), D3DCOMPILE_ENABLE_STRICTNESS,
						  0, &shader, &errorBlob)))
	{
		std::string error = std::string(static_cast<char*>(errorBlob->GetBufferPointer()));
		errorBlob->Release();
		throw bg::base::ShaderCompileException("Compile shader error: " + error);
	}
	return shader;
}

void VertexShader::create(const std::string & source, const std::string & entryPoint, ShaderTargetVersion version) {
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(context());
	if (dxCt) {
		std::string target;
		getTarget(version, target);
		ID3D11Device * device = bg::native_cast<ID3D11Device*>(dxCt->device());
		ID3D10Blob * shaderBuffer = compileShader(source, entryPoint, target);
		if (FAILED(device->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), nullptr, &_shader))) {
			throw bg::base::ShaderException("Unexpected error creating vertex shader");
		}
		
		// Input layout
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutVector;
		std::string vertexName, normalName, tex0Name, tex1Name, tex2Name, colorName, tangentName;

		for (auto layoutItem : _inputLayoutDescriptor) {
			DXGI_FORMAT fmt;
			char * name = nullptr;
			int index = 0;
			switch (layoutItem.role) {
			case BufferRole::kRoleVertex:
				fmt = DXGI_FORMAT_R32G32B32_FLOAT;
				vertexName = layoutItem.name;
				name = &vertexName[0];
				index = layoutItem.semanticIndex;
				break;
			case BufferRole::kRoleNormal:
				fmt = DXGI_FORMAT_R32G32B32_FLOAT;
				normalName = layoutItem.name;
				name = &normalName[0];
				index = layoutItem.semanticIndex;
				break;
			case BufferRole::kRoleTangent:
				fmt = DXGI_FORMAT_R32G32B32_FLOAT;
				tangentName = layoutItem.name;
				name = &tangentName[0];
				index = layoutItem.semanticIndex;
				break;
			case BufferRole::kRoleTex0:
				fmt = DXGI_FORMAT_R32G32_FLOAT;
				tex0Name = layoutItem.name;
				name = &tex0Name[0];
				index = layoutItem.semanticIndex;
				break;
			case BufferRole::kRoleTex1:
				fmt = DXGI_FORMAT_R32G32_FLOAT;
				tex1Name = layoutItem.name;
				name = &tex1Name[0];
				index = layoutItem.semanticIndex;
				break;
			case BufferRole::kRoleTex2:
				fmt = DXGI_FORMAT_R32G32_FLOAT;
				tex2Name = layoutItem.name;
				name = &tex2Name[0];
				index = layoutItem.semanticIndex;
				break;
			case BufferRole::kRoleColor:
				fmt = DXGI_FORMAT_R32G32B32A32_FLOAT;
				colorName = layoutItem.name;
				name = &colorName[0];
				index = layoutItem.semanticIndex;
				break;
			case BufferRole::kRoleIndex:
				throw bg::base::ShaderException("Invalid buffer role used adding input layout: index buffer.");
			}
			inputLayoutVector.push_back({
				name, static_cast<UINT>(index), static_cast<DXGI_FORMAT>(fmt), static_cast<UINT>(inputLayoutVector.size()),
				inputLayoutVector.size() == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			});
		}

		if (inputLayoutVector.size()) {
			HRESULT error = device->CreateInputLayout(&inputLayoutVector[0], static_cast<UINT>(inputLayoutVector.size()),
				shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), &_inputLayout);
			if (FAILED(error))
			{
				throw bg::base::ShaderException("Error creating input shader layout");
			}
		}

		shaderBuffer->Release();
	}
}

void VertexShader::addInputLayout(BufferRole role) {
	std::string name;
	int index = 0;
	switch (role) {
	case BufferRole::kRoleVertex:
		name = "POSITION";
		break;
	case BufferRole::kRoleNormal:
		name = "NORMAL";
		break;
	case BufferRole::kRoleTex0:
		name = "TEXCOORD";
		break;
	case BufferRole::kRoleTex1:
		name = "TEXCOORD";
		index = 1;
		break;
	case BufferRole::kRoleTex2:
		name = "TEXCOORD";
		index = 2;
		break;
	case BufferRole::kRoleTangent:
		name = "TANGENT";
		break;
	case BufferRole::kRoleColor:
		name = "COLOR";
		break;
	}
	_inputLayoutDescriptor.push_back({ name, role, index });
}

void VertexShader::destroy() {
	Shader::destroy();
	if (_shader) {
		_shader->Release();
		_shader = nullptr;
	}
	_inputLayoutDescriptor.clear();
	if (_inputLayout) {
		_inputLayout->Release();
		_inputLayout = nullptr;
	}
}

void VertexShader::bindPolyList(bg::base::PolyList * plist) {
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(context());
	if (dxCt) {
		ID3D11DeviceContext * deviceContext = bg::native_cast<ID3D11DeviceContext*>(dxCt->deviceContext());
		directx11::PolyListImpl * impl = plist->impl<directx11::PolyListImpl>();
		std::vector<ID3D11Buffer*> bufferPointers;
		std::vector<unsigned int> strides;
		std::vector<unsigned int> offsets;

		for (auto inputDesc : _inputLayoutDescriptor) {
			switch (inputDesc.role) {
			case BufferRole::kRoleVertex:
				if (impl->vertexBuffer()) {
					bufferPointers.push_back(impl->vertexBuffer());
					strides.push_back(sizeof(float) * 3);
					offsets.push_back(0);
				}
				else {
					throw bg::base::InvalidStateException("Trying to bind an invalid poly list. The vertex buffer is required.");
				}
				break;
			case BufferRole::kRoleNormal:
				if (impl->normalBuffer()) {
					bufferPointers.push_back(impl->normalBuffer());
					strides.push_back(sizeof(float) * 3);
					offsets.push_back(0);
				}
				else {
					throw bg::base::InvalidStateException("Trying to bind an invalid poly list. The normal buffer is required.");
				}
				break;
			case BufferRole::kRoleTex0:
				if (impl->texCoord0Buffer()) {
					bufferPointers.push_back(impl->texCoord0Buffer());
					strides.push_back(sizeof(float) * 2);
					offsets.push_back(0);
				}
				else {
					throw bg::base::InvalidStateException("Trying to bind an invalid poly list. At least one texture buffer is required.");
				}
				break;
			case BufferRole::kRoleTex1:
				if (impl->texCoord1Buffer()) {
					bufferPointers.push_back(impl->texCoord1Buffer());
				}
				else if (impl->texCoord0Buffer()) {
					bufferPointers.push_back(impl->texCoord0Buffer());
				}
				else {
					throw bg::base::InvalidStateException("Trying to bind an invalid poly list. At least one texture buffer is required.");
				}
				strides.push_back(sizeof(float) * 2);
				offsets.push_back(0);
				break;
			case BufferRole::kRoleTex2:
				if (impl->texCoord2Buffer()) {
					bufferPointers.push_back(impl->texCoord2Buffer());
				}
				else if (impl->texCoord0Buffer()) {
					bufferPointers.push_back(impl->texCoord0Buffer());
				}
				else {
					throw bg::base::InvalidStateException("Trying to bind an invalid poly list. At least one texture buffer is required.");
				}
				strides.push_back(sizeof(float) * 2);
				offsets.push_back(0);
				break;
			case BufferRole::kRoleColor:
				if (impl->colorBuffer()) {
					bufferPointers.push_back(impl->colorBuffer());
					strides.push_back(sizeof(float) * 4);
					offsets.push_back(0);
				}
				else {
					throw bg::base::InvalidStateException("Trying to bind an invalid poly list. The color buffer is required.");
				}
				break;
			case BufferRole::kRoleTangent:
				if (impl->tangentBuffer()) {
					bufferPointers.push_back(impl->tangentBuffer());
					strides.push_back(sizeof(float) * 3);
					offsets.push_back(0);
				}
				else {
					throw bg::base::InvalidStateException("Trying to bind an invalid poly list. The tangent buffer is required.");
				}
				break;
			}
		}

		deviceContext->IASetVertexBuffers(0, static_cast<UINT>(bufferPointers.size()), &bufferPointers[0], &strides[0], &offsets[0]);
		deviceContext->IASetIndexBuffer(impl->indexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		switch (plist->drawMode()) {
		case bg::base::PolyList::kTessellation:
		case bg::base::PolyList::kTriangles:
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case bg::base::PolyList::kLines:
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		}
	}
	
}

void VertexShader::activate() {
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(context());
	if (dxCt) {
		ID3D11DeviceContext * deviceContext = bg::native_cast<ID3D11DeviceContext*>(dxCt->deviceContext());
		int index = 0;
		for (auto constantBuffer : _constantBufferVector) {
			ID3D11Buffer * buffer = constantBuffer->buffer();
			deviceContext->VSSetConstantBuffers(index++, 1, &buffer);
		}
		deviceContext->IASetInputLayout(_inputLayout);
		deviceContext->VSSetShader(_shader, nullptr, 0);
	}
}

void PixelShader::create(const std::string & source, const std::string & entryPoint, ShaderTargetVersion version) {
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(context());
	if (dxCt) {
		std::string target;
		getTarget(version, target);
		ID3D11Device * device = bg::native_cast<ID3D11Device*>(dxCt->device());
		ID3D10Blob * shaderBuffer = compileShader(source, entryPoint, target);
		if (FAILED(device->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), nullptr, &_shader))) {
			throw bg::base::ShaderException("Unexpected error creating pixel shader");
		}
		shaderBuffer->Release();
	}
}

void PixelShader::activate() {
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(context());
	if (dxCt) {
		ID3D11DeviceContext * deviceContext = bg::native_cast<ID3D11DeviceContext*>(dxCt->deviceContext());
		int index = 0;
		for (auto constantBuffer : _constantBufferVector) {
			ID3D11Buffer * buffer = constantBuffer->buffer();
			deviceContext->PSSetConstantBuffers(index++, 1, &buffer);
		}
		deviceContext->PSSetShader(_shader, nullptr, 0);
	}
}

void PixelShader::setTexture(int slot, int numTextures, bg::base::Texture * tex) {
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(context());
	if (dxCt) {
		ID3D11DeviceContext * deviceContext = bg::native_cast<ID3D11DeviceContext*>(dxCt->deviceContext());
		bg::engine::directx11::TextureImpl * impl = tex->impl<bg::engine::directx11::TextureImpl>();
		ID3D11ShaderResourceView * shaderResView = impl->dxTextureView();
		ID3D11SamplerState * samplerState = impl->dxSampleState();
		deviceContext->PSSetShaderResources(slot, numTextures, &shaderResView);
		deviceContext->PSSetSamplers(slot, numTextures, &samplerState);
	}
}

void PixelShader::destroy() {
	Shader::destroy();
	if (_shader) {
		_shader->Release();
		_shader = nullptr;
	}
}


#else

// Shader
void Shader::destroy() {}

// Vertex shader
void VertexShader::create(const std::string & source, const std::string & entryPoint, ShaderTargetVersion version) {}
void VertexShader::destroy() {}
void VertexShader::activate() {}
void VertexShader::addInputLayout(BufferRole fmt) {}
void VertexShader::bindPolyList(bg::base::PolyList *) {}

// Pixel shader
void PixelShader::create(const std::string & source, const std::string & entryPoint, ShaderTargetVersion version) {}
void PixelShader::activate() {}
void PixelShader::setTexture(int slot, int numTextures, bg::base::Texture * tex) {}
void PixelShader::destroy() {}

#endif

}
}
}