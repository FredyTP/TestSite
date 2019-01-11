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

#ifndef _bg2e_gui_window_hpp_
#define _bg2e_gui_window_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/base/context_object.hpp>
#include <bg/gui/view.hpp>
#include <bg/gui/draw_visitor.hpp>
#include <bg/gui/frame_visitor.hpp>
#include <bg/gui/layout.hpp>

namespace bg {
namespace gui {

class Surface;
class BG2E_EXPORT Window : public bg::base::ReferencedPointer,
						   public bg::base::ContextObject {
	friend class Surface;
public:
	Window(Surface *);
	inline void setContentView(View * v) { _contentView = v; _contentView->setParentWindow(this); }
	inline View * contentView() { return _contentView.getPtr(); }
	inline const View * contentView() const { return _contentView.getPtr(); }

	void create();
	void create(View * contentView);

	inline void setAlignment(Alignment align) { _sizeChanged = true; _alignment = align; }
	inline Alignment alignment() const { return _alignment; }
	inline void setAnchor(Anchor anchor) { _sizeChanged = true; _anchor = anchor; }
	inline Anchor anchor() const { return _anchor; }
	inline void setAlignmentMargin(const bg::math::Size2Di & m) { _sizeChanged = true; _alignMargin = m; }
	inline const bg::math::Size2Di & alignmentMargin() const { return _alignMargin; }
	inline void setAnchorMargin(const bg::math::Size2Di & m) { _sizeChanged = true; _anchorMargin = m; }
	inline const bg::math::Size2Di & anchorMargin() const { return _anchorMargin; }
	inline void setPosition(const bg::math::Position2Di & pos) { _position = pos; }
	inline const bg::math::Position2Di & position() const { return _position; }
	inline bg::math::Position2Di & position() { return _position; }
	inline void setSize(const bg::math::Size2Di & size) { _sizeChanged = true; _size = size; }
	inline const bg::math::Size2Di & size() const { return _size; }
	inline Surface * parentSurface() { return _parentSurface; }
	inline const Surface * parentSurface() const { return _parentSurface; }
	inline bool visible() const { return _visible; }
	inline void setVisible(bool v) { _visible = v; }
	inline void show() { _visible = true; }
	inline void hide() { _visible = false; }

	Skin * skin();
	const Skin * skin() const;

	virtual void init();
	virtual void draw(bg::base::Pipeline *);

protected:
	virtual ~Window();

	Alignment _alignment = kAlignTopLeft;
	Anchor _anchor = kAnchorNone;
	bg::math::Position2Di _position;
	bg::math::Size2Di _size;
	bg::math::Size2Di _alignMargin;
	bg::math::Size2Di _anchorMargin;
	bool _sizeChanged;
	Surface * _parentSurface;
	bool _visible;

	bg::ptr<View> _contentView;
	bg::ptr<DrawVisitor> _drawVisitor;
	bg::ptr<FrameVisitor> _frameVisitor;
};

}
}

#endif
