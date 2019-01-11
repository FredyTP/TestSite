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


#include <bg/wnd/directx_context.hpp>

#include <bg/system/system.hpp>
#include <bg/engine/directx_includes.hpp>
#include <bg/wnd/win32_window.hpp>
#include <bg/wnd/window_controller.hpp>

namespace bg {
namespace wnd {

#if BG2E_DIRECTX_AVAILABLE==1

bool DirectXContext::createContext()
{
	Win32Window * win = dynamic_cast<Win32Window*>(_window);
	if (!win) return false;

	bg::math::Rect winRect = _window->rect();
	HWND hwnd = bg::native_cast<HWND>(win->win_Wnd());
	
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = winRect.width();
	swapChainDesc.BufferDesc.Height = winRect.height();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	IDXGISwapChain * swapChain;
	ID3D11Device * device;
	ID3D11DeviceContext * deviceContext;

	UINT creationFlags = 0;
	#ifdef _DEBUG
	creationFlags = D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,creationFlags, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &deviceContext);
	_swapChain = swapChain;
	_device = device;
	_deviceContext = deviceContext;

	return true;
}

void DirectXContext::swapBuffers()
{
	// TODO: vsync Present(1, 0);
	bg::native_cast<IDXGISwapChain*>(_swapChain)->Present(0, 0);
}

void DirectXContext::destroy()
{
	bg::native_cast<IDXGISwapChain*>(_swapChain)->Release();
	bg::native_cast<ID3D11Device*>(_device)->Release();
	bg::native_cast<ID3D11DeviceContext*>(_deviceContext)->Release();
	_swapChain = nullptr;
	_device = nullptr;
	_deviceContext = nullptr;
}

#else

bool DirectXContext::createContext()
{
	return false;
}

void DirectXContext::swapBuffers()
{

}

void DirectXContext::destroy()
{

}

#endif

}
}