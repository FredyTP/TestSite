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


#include <bg/engine/directx11/texture_impl.hpp>

#include <bg/wnd/directx_context.hpp>

namespace bg {
namespace engine {
namespace directx11 {

TextureImpl::TextureImpl(bg::base::Texture * tex)
	:bg::engine::TextureImpl(tex)
#if BG2E_DIRECTX_AVAILABLE==1
	,_dxTexture(nullptr)
	,_dxTextureView(nullptr)
	,_dxSampleState(nullptr)
#endif
{
	
}

TextureImpl::~TextureImpl() {
	
}

void TextureImpl::create(const unsigned char * buffer, int width, int height, int bpp) {
#if BG2E_DIRECTX_AVAILABLE==1
	if (width <= 0 || height <= 0 || bpp <= 0) {
		throw bg::base::InvalidParameterException("Invalid parameter creating DirectX 11 texture.");
	}
	else if (buffer == nullptr) {
		throw bg::base::NullParameterException("Null buffer creating DirectX 11 texture.");
	}

	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(_texture->context());
	if (dxCt) {
		ID3D11Device * device = bg::native_cast<ID3D11Device*>(dxCt->device());
		ID3D11DeviceContext * deviceContext = bg::native_cast<ID3D11DeviceContext*>(dxCt->deviceContext());

		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Height = height;
		textureDesc.Width = width;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;

		// TODO: Texture filter
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &_dxTexture))) {
			throw bg::base::DirectXEngineException("Unexpected error creating DirectX texture.");
		}

		unsigned int rowPitch = width * bpp * sizeof(unsigned char);
		deviceContext->UpdateSubresource(_dxTexture, 0, nullptr, buffer, rowPitch, 0);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		if (FAILED(device->CreateShaderResourceView(_dxTexture, &srvDesc, &_dxTextureView))) {
			throw bg::base::DirectXEngineException("Unexpected error creating DirectX texture view.");
		}

		// TODO: Texture filter
		deviceContext->GenerateMips(_dxTextureView);

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// TODO: texture filter
		samplerDesc.AddressU = _texture->wrapModeU() == base::Texture::kWrapModeRepeat ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = _texture->wrapModeV() == base::Texture::kWrapModeRepeat ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = _texture->wrapModeW() == base::Texture::kWrapModeRepeat ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if (FAILED(device->CreateSamplerState(&samplerDesc, &_dxSampleState))) {
			throw bg::base::DirectXEngineException("Unexpected error creating DirectX texture sampler state.");
		}
	}
#else
	throw bg::base::EngineException("Rendering engine exception creating DirectX 11 texture. Rendering engine not supported in this platform.");
#endif
}

void TextureImpl::destroy() {
#if BG2E_DIRECTX_AVAILABLE==1
	if (_dxTextureView) {
		_dxTextureView->Release();
		_dxTextureView = nullptr;
	}

	if (_dxTexture) {
		_dxTexture->Release();
		_dxTexture = nullptr;
	}

	if (_dxSampleState) {
		_dxSampleState->Release();
		_dxSampleState = nullptr;
	}
#endif
}

}
}
}