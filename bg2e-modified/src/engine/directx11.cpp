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


#include <bg/engine/directx11.hpp>
#include <bg/wnd/directx_context.hpp>

#include <bg/engine/directx11/pipeline_impl.hpp>
#include <bg/engine/directx11/poly_list_impl.hpp>
#include <bg/engine/directx11/texture_impl.hpp>
#include <bg/engine/directx11/render_surface_impl.hpp>


namespace bg {
namespace engine {

bool DirectX11::Supported() {
	
#if BG2E_DIRECTX_AVAILABLE==1
	return true;
#else
	return false;
#endif
}

DirectX11::DirectX11()
{
	_name = "DirectX-11";
	
	// TODO: Implementation objects
}
	
void DirectX11::initialize(bg::base::Context * ctx) {
	// TODO: initialize DirectX
}

bool DirectX11::supportsFeature(bg::base::Context *, EngineFeature feature) {
	return true;
}

bg::engine::PipelineImpl * DirectX11::createPipeline(bg::base::Pipeline * pipeline) {
	return new directx11::PipelineImpl(pipeline);
}

bg::engine::PolyListImpl * DirectX11::createPolyList(bg::base::PolyList * plist) {
	return new bg::engine::directx11::PolyListImpl(plist);
}

bg::engine::TextureImpl * DirectX11::createTextureImpl(bg::base::Texture * tex) {
	return new bg::engine::directx11::TextureImpl(tex);
}

bg::engine::RenderSurfaceImpl * DirectX11::createRenderSurfaceImpl(bg::base::RenderSurface * renderSurface, engine::RenderSurfaceImpl::RenderSurfaceTarget target) {
	return new bg::engine::directx11::RenderSurfaceImpl(renderSurface, target);
}

}
}
