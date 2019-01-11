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

#ifndef _bg2e_tools_texture_frame_hpp_
#define _bg2e_tools_texture_frame_hpp_

#include <bg/base/texture.hpp>
#include <bg/base/pipeline.hpp>

namespace bg {
namespace tools {

class BG2E_EXPORT TextureFrame :	public bg::base::ContextObject,
									public bg::base::ReferencedPointer {
public:
	typedef std::function<void(bg::base::Pipeline*)> DrawClosure;
	typedef std::function<void(bg::base::Pipeline*,const bg::math::Size2Di &)> ResizeClosure;

	TextureFrame(bg::base::Context *);

	void setSize(const bg::math::Size2Di &);
	inline const bg::math::Size2Di & size() const { return _size; }

	inline bg::base::Pipeline * pipeline() { return _pipeline.getPtr(); }
	inline const bg::base::Pipeline * pipeline() const { return _pipeline.getPtr(); }


	inline bg::base::Texture * texture() { return _surface.valid() ? _surface->texture() : nullptr; }

	inline void onResize(ResizeClosure resizeClosure) { _resizeClosure = resizeClosure; }
	inline void onDraw(DrawClosure drawClosure) { _drawClosure = drawClosure; }

	void update();

protected:
	virtual ~TextureFrame();

	bg::ptr<bg::base::Pipeline> _pipeline;
	bg::ptr<bg::base::TextureRenderSurface> _surface;
	bg::math::Size2Di _size;
	DrawClosure _drawClosure;
	ResizeClosure _resizeClosure;
};

}
}

#endif
