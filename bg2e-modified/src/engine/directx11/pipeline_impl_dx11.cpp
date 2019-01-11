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


#include <bg/engine/directx_includes.hpp>
#include <bg/wnd/directx_context.hpp>

#include <bg/engine/directx11/pipeline_impl.hpp>

#include <bg/engine/directx11.hpp>
#include <bg/base/definitions.hpp>
#include <bg/engine/directx_includes.hpp>
#include <bg/wnd/window.hpp>
#include <bg/wnd/window_controller.hpp>
#include <bg/base/pipeline.hpp>

namespace bg {
namespace engine {
namespace directx11 {

#if BG2E_DIRECTX_AVAILABLE==1

PipelineImpl::PipelineImpl(bg::base::Pipeline * pipeline)
	:bg::engine::PipelineImpl(pipeline)
	,_renderTargetView(nullptr)
	,_depthStencilState(nullptr)
	,_depthStencilBuffer(nullptr)
	,_depthStencilView(nullptr)
	,_rasterizerState(nullptr)
{
	rebuild();
}

PipelineImpl::~PipelineImpl() {
}

void PipelineImpl::activate() {
	bg::base::Context * ctx = _pipeline->context();
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(ctx);

	if (dxCt) {
		IDXGISwapChain * swapChain;
		ID3D11Device * device;
		ID3D11DeviceContext * deviceContext;
		swapChain = bg::native_cast<IDXGISwapChain*>(dxCt->swapChain());
		device = bg::native_cast<ID3D11Device*>(dxCt->device());
		deviceContext = bg::native_cast<ID3D11DeviceContext*>(dxCt->deviceContext());

		deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
		deviceContext->OMSetDepthStencilState(_depthStencilState, 1);
		deviceContext->RSSetState(_rasterizerState);
		
		bg::math::Viewport & vp = _pipeline->viewport();
		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<float>(vp.width());
		viewport.Height = static_cast<float>(vp.height());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		deviceContext->RSSetViewports(1, &viewport);
	}
}

void PipelineImpl::setViewport() {
	bg::base::Context * ctx = _pipeline->context();
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(ctx);
	
	if (dxCt) {
		IDXGISwapChain * swapChain;
		ID3D11Device * device;
		ID3D11DeviceContext * deviceContext;
		bg::math::Viewport & vp = _pipeline->viewport();
		swapChain = bg::native_cast<IDXGISwapChain*>(dxCt->swapChain());
		device = bg::native_cast<ID3D11Device*>(dxCt->device());
		deviceContext = bg::native_cast<ID3D11DeviceContext*>(dxCt->deviceContext());

		if (isActive()) {
			deviceContext->OMSetRenderTargets(0, 0, 0);
		}
		_renderTargetView->Release();
		_depthStencilBuffer->Release();
		_depthStencilView->Release();
		_depthStencilState->Release();

		if (FAILED(swapChain->ResizeBuffers(2, vp.width(), vp.height(), DXGI_FORMAT_R8G8B8A8_UNORM, 0))) {
			throw bg::base::InvalidStateException("Error resizing viewport. Unexpected Direct3D error.");
		}

		ID3D11Texture2D * buffer = nullptr;
		if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&buffer))) {
			throw bg::base::InvalidStateException("Error resizing viewport. Unexpected Direct3D error.");
		}

		if (FAILED(device->CreateRenderTargetView(buffer, nullptr, &_renderTargetView))) {
			throw bg::base::InvalidStateException("Error resizing viewport. Unexpected Direct3D error.");
		}
		buffer->Release();
		
		createDepthStencilView();

		if (isActive()) {
			deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

			D3D11_VIEWPORT viewport;
			viewport.Width = static_cast<float>(vp.width());
			viewport.Height = static_cast<float>(vp.height());
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			deviceContext->RSSetViewports(1, &viewport);
		}
	}
}

void PipelineImpl::clearBuffers(unsigned int buffers) {
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(_pipeline->context());
	if (dxCt) {
		IDXGISwapChain * swapChain;
		ID3D11Device * device;
		ID3D11DeviceContext * deviceContext;
		swapChain = bg::native_cast<IDXGISwapChain*>(dxCt->swapChain());
		device = bg::native_cast<ID3D11Device*>(dxCt->device());
		deviceContext = bg::native_cast<ID3D11DeviceContext*>(dxCt->deviceContext());

		if (buffers & bg::base::ClearBuffers::kColor) {
			deviceContext->ClearRenderTargetView(_renderTargetView, _pipeline->clearColor().raw());
		}
		if (buffers & bg::base::ClearBuffers::kDepth) {
			deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
	}
}

void PipelineImpl::setDepthTestEnabled() {

}

void PipelineImpl::setBlendEnabled() {
}

void PipelineImpl::setBlendMode() {
/*	switch (mode) {
		case bg::base::BlendMode::kNormal:
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
			break;
		case bg::base::BlendMode::kMultiply:
			glBlendFunc(GL_ZERO,GL_SRC_COLOR);
			glBlendEquation(GL_FUNC_ADD);
			break;
		case bg::base::BlendMode::kAdd:
			glBlendFunc(GL_ONE,GL_ONE);
			glBlendEquation(GL_FUNC_ADD);
			break;
		case bg::base::BlendMode::kSubtract:
			glBlendFunc(GL_ONE,GL_ONE);
			glBlendEquation(GL_FUNC_SUBTRACT);
			break;
		case bg::base::BlendMode::kAlphaAdd:
			glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
			break;
		case bg::base::BlendMode::kAlphaSubtract:
			glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
			glBlendEquation(GL_FUNC_SUBTRACT);
			break;
	}
	*/
}

void PipelineImpl::setCullFace() {
//	e ? glEnable(GL_DEPTH_TEST)  : glDisable(GL_DEPTH_TEST);
}

void PipelineImpl::destroy() {
	if (_renderTargetView)  _renderTargetView->Release();
	if (_depthStencilState) _depthStencilState->Release();
	if (_depthStencilBuffer) _depthStencilBuffer->Release();
	if (_depthStencilView) _depthStencilView->Release();
	if (_rasterizerState) _rasterizerState->Release();
	_renderTargetView = nullptr;
	_depthStencilState = nullptr;
	_depthStencilBuffer = nullptr;
	_depthStencilView = nullptr;
	_rasterizerState = nullptr;
}

void PipelineImpl::rebuild() {
	destroy();
	bg::base::Context * ctx = _pipeline->context();
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(ctx);
	IDXGISwapChain * swapChain;
	ID3D11Device * device;
	ID3D11DeviceContext * deviceContext;

	if (dxCt) {
		ID3D11Texture2D * backBufferPtr = nullptr;
		swapChain = bg::native_cast<IDXGISwapChain*>(dxCt->swapChain());
		device = bg::native_cast<ID3D11Device*>(dxCt->device());
		deviceContext = bg::native_cast<ID3D11DeviceContext*>(dxCt->deviceContext());
		if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr))) {
			backBufferPtr = nullptr;
		}
		if (!backBufferPtr || FAILED(device->CreateRenderTargetView(backBufferPtr, nullptr, &_renderTargetView))) {
			_renderTargetView = nullptr;
		}
		else {
			backBufferPtr->Release();
		}
	}

	createDepthStencilView();

	if (_depthStencilView) {
		D3D11_RASTERIZER_DESC rasterDesc;
		ID3D11RasterizerState * rasterState;
		ZeroMemory(&rasterDesc, sizeof(rasterDesc));
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_FRONT;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;
		if (!FAILED(device->CreateRasterizerState(&rasterDesc, &rasterState))) {
			_rasterizerState = rasterState;
		}
	}
}

void PipelineImpl::createDepthStencilView() {
	bg::base::Context * ctx = _pipeline->context();
	bg::wnd::DirectXContext * dxCt = dynamic_cast<bg::wnd::DirectXContext*>(ctx);
	IDXGISwapChain * swapChain = bg::native_cast<IDXGISwapChain*>(dxCt->swapChain());
	ID3D11Device * device = bg::native_cast<ID3D11Device*>(dxCt->device());
	ID3D11DeviceContext * deviceContext = bg::native_cast<ID3D11DeviceContext*>(dxCt->deviceContext());

	if (_renderTargetView) {
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
		depthBufferDesc.Width = dxCt->window()->rect().width();
		depthBufferDesc.Height = dxCt->window()->rect().height();
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
		if (FAILED(device->CreateTexture2D(&depthBufferDesc, nullptr, &_depthStencilBuffer))) {
			_depthStencilBuffer = nullptr;
		}
	}

	if (_depthStencilBuffer) {
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Front face
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Back face
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xff;

		if (FAILED(device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState))) {
			_depthStencilState = nullptr;
		}
		else {
			deviceContext->OMSetDepthStencilState(_depthStencilState, 1);
		}
	}

	if (_depthStencilState) {
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ID3D11DepthStencilView * depthStencilView;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		if (!FAILED(device->CreateDepthStencilView(bg::native_cast<ID3D11Texture2D*>(_depthStencilBuffer), &depthStencilViewDesc, &depthStencilView))) {
			_depthStencilView = depthStencilView;
		}
	}
}

#else

PipelineImpl::PipelineImpl(bg::base::Pipeline * pipeline)
	:bg::engine::PipelineImpl(pipeline)
{

}

PipelineImpl::~PipelineImpl() {

}

void PipelineImpl::activate() {}
void PipelineImpl::setViewport() {}
void PipelineImpl::clearBuffers(unsigned int buffers) {}
void PipelineImpl::setDepthTestEnabled() {}
void PipelineImpl::setBlendEnabled() {}
void PipelineImpl::setBlendMode() {}
void PipelineImpl::setCullFace() {}
void PipelineImpl::destroy() {}

void PipelineImpl::rebuild() {}
void PipelineImpl::createDepthStencilView() {}

#endif

}
}
}
