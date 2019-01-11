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

#include <bg/tools/texture_frame.hpp>

namespace bg {
namespace tools {

TextureFrame::TextureFrame(bg::base::Context * ctx)
	:ContextObject(ctx)
	,_size(512)
{
	_pipeline = new bg::base::Pipeline(ctx);
	_surface = new bg::base::TextureRenderSurface(ctx);
	_surface->create();
	_surface->setResizeOnViewportChanged(true);
	_pipeline->setRenderSurface(_surface.getPtr());
	_pipeline->setViewport(bg::math::Viewport(0, 0, _size.width(), _size.height()));
}

TextureFrame::~TextureFrame() {
}

void TextureFrame::setSize(const bg::math::Size2Di & size) {
	_size = size;
	_pipeline->setViewport(bg::math::Viewport(0, 0, _size.width(), _size.height()));
	if (_resizeClosure) {
		_resizeClosure(_pipeline.getPtr(), size);
	}
}

void TextureFrame::update() {
	if (_drawClosure) {
		bg::base::Pipeline * prev = bg::base::Pipeline::Current();
		bg::base::Pipeline::SetCurrent(_pipeline.getPtr());

		_drawClosure(_pipeline.getPtr());

		bg::base::Pipeline::SetCurrent(prev);
	}
}


}
}