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


#ifndef _bg2e_engine_directx11_pipeline_impl_hpp_
#define _bg2e_engine_directx11_pipeline_impl_hpp_

#include <bg/export.hpp>
#include <bg/engine/pipeline_impl.hpp>
#include <bg/engine/directx_includes.hpp>

namespace bg {
namespace engine {
namespace directx11 {

class BG2E_EXPORT PipelineImpl : public bg::engine::PipelineImpl {
public:
	PipelineImpl(bg::base::Pipeline * pipeline);
	
	virtual void activate();
	virtual void setViewport();
	virtual void clearBuffers(unsigned int buffers);
	virtual void setDepthTestEnabled();
	virtual void setBlendEnabled();
	virtual void setBlendMode();
	virtual void setCullFace();
	virtual void destroy();
	
protected:
	virtual ~PipelineImpl();

	void rebuild();
	void createDepthStencilView();

#if BG2E_DIRECTX_AVAILABLE==1
	ID3D11RenderTargetView * _renderTargetView;
	ID3D11DepthStencilState * _depthStencilState;
	ID3D11Texture2D * _depthStencilBuffer;
	ID3D11DepthStencilView * _depthStencilView;
	ID3D11RasterizerState * _rasterizerState;
#endif

};

}
}
}

#endif