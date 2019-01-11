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

#ifndef _bg2e_gui_surface_hpp_
#define _bg2e_gui_surface_hpp_

#include <bg/base/context_object.hpp>
#include <bg/base/referenced_pointer.hpp>
#include <bg/base/pipeline.hpp>
#include <bg/base/mouse_event.hpp>
#include <bg/base/keyboard_event.hpp>
#include <bg/base/touch_event.hpp>
#include <bg/base/custom_event.hpp>
#include <bg/base/pipeline.hpp>
#include <bg/base/object_registry.hpp>

#include <bg/gui/view.hpp>
#include <bg/gui/window.hpp>
#include <bg/gui/skin.hpp>

namespace bg {
namespace gui {

class BG2E_EXPORT Surface : public bg::base::ContextObject,
							public bg::base::ReferencedPointer,
							public bg::base::UniqueIdentifiedObject {
public:
	Surface(bg::base::Context *, Skin *);

	inline const Skin * skin() const { return  _skin.getPtr(); }
	inline Skin * skin() { return  _skin.getPtr(); }
	inline bg::text::Font * skinFont() { return _skin.valid() ? _skin->font() : nullptr; }
	inline const bg::text::Font * skinFont() const { return _skin.valid() ? _skin->font() : nullptr; }
	inline void setScale(float s) { _scaleChanged = _scale!=s || _scaleChanged; _scale = s; }
	inline float scale() const { return _scale; }
	inline bool scaleChanged() const { return _scaleChanged; }

	void resize(int, int);
	void frame(float);
	void draw();
	virtual bool keyDown(const bg::base::KeyboardEvent &);
	virtual bool keyUp(const bg::base::KeyboardEvent &);
	virtual bool charPress(const bg::base::KeyboardEvent &);
	virtual bool mouseDown(const bg::base::MouseEvent &);
	virtual bool mouseDrag(const bg::base::MouseEvent &);
	virtual bool mouseMove(const bg::base::MouseEvent &);
	virtual bool mouseUp(const bg::base::MouseEvent &);
	virtual bool mouseWheel(const bg::base::MouseEvent &);
	virtual bool touchStart(const bg::base::TouchEvent &);
	virtual bool touchMove(const bg::base::TouchEvent &);
	virtual bool touchEnd(const bg::base::TouchEvent &);
	virtual void customEvent(const bg::base::CustomEvent &);

	void addWindow(Window *);
	void removeWindow(Window *);
	void removeAllWindows();
	void updateLayout(Window *);

	inline void eachWindow(std::function<void(Window*)> cb) {
		for (auto & wnd : _windowVector) {
			cb(wnd.getPtr());
		}
	}

	inline void reverseEachWindow(std::function<void(Window*)> cb) {
		for (auto it = _windowVector.rbegin(); it!=_windowVector.rend(); ++it) {
			cb((*it).getPtr());
		}
	}

	inline void windowInPoint(const bg::math::Position2Di & point, std::function<void(Window *, const bg::math::Position2Di &)> cb) {
		using namespace bg::math;
		for (auto it = _windowVector.rbegin(); it!=_windowVector.rend(); ++it) {
			Window * wnd = (*it).getPtr();
			if (wnd->visible()) {
				Size2Di size(
					static_cast<int>(static_cast<float>(wnd->size().width()) * scale()),
					static_cast<int>(static_cast<float>(wnd->size().height()) * scale())
				);
				Position2Di pos = wnd->position();
				Rect rect(pos.x(), pos.y(), size.width(), size.height());
				if(rect.containsPoint(point)) {
					cb(wnd, point - pos);
				}
			}
		}
	}

	void setFocus(Window * wnd);
	void setFocus(View * view);
	void clearFocus();

protected:
	virtual ~Surface();

	bg::ptr<bg::base::Pipeline> _pipeline;
	std::vector<bg::ptr<Window>> _windowVector;
	bg::ptr<Window> _focusWindow;
	bg::ptr<View> _focusView;
	float _scale;
	bool _scaleChanged = true;

	bg::ptr<Skin> _skin;
};

}
}

#endif
