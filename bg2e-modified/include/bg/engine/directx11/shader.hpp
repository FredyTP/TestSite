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


#ifndef _bg2e_engine_directx11_shader_hpp_
#define _bg2e_engine_directx11_shader_hpp_

#include <bg/base/context_object.hpp>
#include <bg/engine/directx_includes.hpp>
#include <bg/base/exception.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/base/texture.hpp>

#include <string>
#include <vector>
#include <map>

namespace bg {
namespace engine {
namespace directx11 {

#if BG2E_DIRECTX_AVAILABLE==1

enum BufferUsage {
	kUsageDefault = D3D11_USAGE_DEFAULT,
	kUsageInmutable = D3D11_USAGE_IMMUTABLE,
	kUsageDynamic = D3D11_USAGE_DYNAMIC,
	kUsageStaging = D3D11_USAGE_STAGING
};

#else

enum BufferUsage {
	kUsageDefault,
	kUsageInmutable,
	kUsageDynamic,
	kUsageStaging
};

#endif

enum BufferRole {
	kRoleVertex = 1,
	kRoleNormal,
	kRoleTex0,
	kRoleTex1,
	kRoleTex2,
	kRoleColor,
	kRoleTangent,
	kRoleIndex
};

class BG2E_EXPORT ConstantBuffer : public bg::base::ContextObject, public bg::base::ReferencedPointer {
public:
	ConstantBuffer(bg::base::Context * ctx, unsigned long bufferSize, BufferUsage usage);

	template <class T>
	T * beginMap() {
		T * dataPtr = nullptr;
#if BG2E_DIRECTX_AVAILABLE==1
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (FAILED(_deviceContext->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
			throw bg::base::ShaderException("Error mapping shader resource");
		}

		dataPtr = static_cast<T*>(mappedResource.pData);
#endif
		return dataPtr;
	}

	void endMap(unsigned int bufferNumber) {
#if BG2E_DIRECTX_AVAILABLE==1
		_deviceContext->Unmap(_buffer, 0);
		_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_buffer);
#endif
	}

#if BG2E_DIRECTX_AVAILABLE==1
	ID3D11Buffer * buffer() { return _buffer; }
#else
	void * buffer() { return nullptr; }
#endif

protected:
	virtual ~ConstantBuffer();
	
	unsigned long _bufferSize;
	BufferUsage _usage;

#if BG2E_DIRECTX_AVAILABLE==1
	ID3D11Device * _device;
	ID3D11DeviceContext * _deviceContext;
	ID3D11Buffer * _buffer;
#endif
};

enum ShaderTargetVersion {
	kShaderVersion50,
	kShaderVersion41,
	kShaderVersion40
};

class BG2E_EXPORT Shader : public bg::base::ContextObject, public bg::base::ReferencedPointer {
public:
	Shader(bg::base::Context * ctx);

	virtual void create(const std::string & source, const std::string & entryPoint, ShaderTargetVersion version = ShaderTargetVersion::kShaderVersion50) = 0;
	virtual void destroy();
	virtual void activate() = 0;

	inline void addConstantBuffer(ConstantBuffer * constantBuffer) { _constantBufferVector.push_back(constantBuffer); }
	inline std::vector<bg::ptr<ConstantBuffer> > & constantBuffers() { return _constantBufferVector; }

protected:
	virtual ~Shader();

#if BG2E_DIRECTX_AVAILABLE==1
	ID3D10Blob * compileShader(const std::string & source, const std::string & entryPoint, const std::string & target);
#endif

	std::vector<bg::ptr<ConstantBuffer> > _constantBufferVector;

	virtual void getTarget(ShaderTargetVersion v, std::string & target) = 0;
};

struct InputLayoutDescriptor {
	std::string name;
	BufferRole role;
	int semanticIndex;
};
class BG2E_EXPORT VertexShader : public Shader {
public:
	VertexShader(bg::base::Context * ctx)
		:Shader(ctx)
#if BG2E_DIRECTX_AVAILABLE==1
		,_shader(nullptr)
		,_inputLayout(nullptr)
#endif
	{}

	void addInputLayout(BufferRole fmt);

	virtual void create(const std::string & source, const std::string & entryPoint, ShaderTargetVersion version = ShaderTargetVersion::kShaderVersion50);

	void bindPolyList(bg::base::PolyList * plist);
	virtual void activate();

	virtual void destroy();

public:
	virtual ~VertexShader() {}

#if BG2E_DIRECTX_AVAILABLE==1
	ID3D11VertexShader * _shader;

	std::vector<InputLayoutDescriptor> _inputLayoutDescriptor;
	ID3D11InputLayout * _inputLayout;
#endif

	virtual void getTarget(ShaderTargetVersion v, std::string & target) {
		switch (v) {
		case kShaderVersion40:
			target = "vs_4_0";
			break;
		case kShaderVersion41:
			target = "vs_4_1";
			break;
		case kShaderVersion50:
			target = "vs_5_0";
			break;
		}
	}
};

// TODO: Hull shader,

class BG2E_EXPORT PixelShader : public Shader {
public:
	PixelShader(bg::base::Context * ctx)
		:Shader(ctx)
#if BG2E_DIRECTX_AVAILABLE==1
		,_shader(nullptr)
#endif
	{}

	virtual void create(const std::string & source, const std::string & entryPoint, ShaderTargetVersion version = ShaderTargetVersion::kShaderVersion50);
	virtual void destroy();
	virtual void activate();
	virtual void setTexture(int slot, int numTextures, bg::base::Texture * tex);

public:
	virtual ~PixelShader() {}

#if BG2E_DIRECTX_AVAILABLE==1
	ID3D11PixelShader * _shader;
#endif

	virtual void getTarget(ShaderTargetVersion v, std::string & target) {
		switch (v) {
		case kShaderVersion40:
			target = "ps_4_0";
			break;
		case kShaderVersion41:
			target = "ps_4_1";
			break;
		case kShaderVersion50:
			target = "ps_5_0";
			break;
		}
	}
};


}
}
}

#endif