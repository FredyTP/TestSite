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

#ifndef _bg2e_gui_view_hpp_
#define _bg2e_gui_view_hpp_

#include <bg/data/tree_node.hpp>
#include <bg/base/context_object.hpp>
#include <bg/base/pipeline.hpp>
#include <bg/base/mouse_event.hpp>
#include <bg/base/keyboard_event.hpp>
#include <bg/base/custom_event.hpp>
#include <bg/base/touch_event.hpp>
#include <bg/math/vector.hpp>
#include <bg/base/poly_list.hpp>
#include <bg/tools/texture_frame.hpp>
#include <bg/gui/layout.hpp>
#include <bg/gui/skin.hpp>

namespace bg {
namespace gui {

class View;
class BG2E_EXPORT ViewDecorator : public bg::base::ReferencedPointer {
	friend class View;
public:
	ViewDecorator();
	ViewDecorator(int paddingLeft, int paddingTop, int paddingRight, int paddingBottom);

	// The padding in decorators only works with view layouts
	void setPadding(int left, int top = -1, int right = -1, int bottom = -1);
	void addPadding(int & left, int & top, int & right, int & bottom);

	// Parameters: decoratedView, pipeline and scale
	virtual void draw(View *, bg::base::Pipeline *, float);

	inline std::vector<bg::ptr<bg::gui::View>> & views() { return _views; }
	inline const std::vector<bg::ptr<bg::gui::View>> & views() const { return _views; }

protected:
	virtual ~ViewDecorator();

	int _paddingLeft;
	int _paddingTop;
	int _paddingRight;
	int _paddingBottom;

	// Parameters: decoratedView, pipeline, scale
	virtual void update(View *, bg::base::Pipeline *, float) {}

	// Parameter: decoratedView, view to draw, pipeline, scale
	virtual void drawSubview(View *, View *, bg::base::Pipeline *, float);

	inline void addSubview(bg::gui::View * v) { _views.push_back(v); }

	std::vector<bg::ptr<bg::gui::View>> _views;
};

class Window;
class Surface;
class BG2E_EXPORT View : public bg::data::TreeNode<View>,
						 public bg::base::ContextObject {
	friend class Window;
	friend class ViewDecorator;
public:

	template <class T>
	static T * Create(bg::base::Context * ctx, Skin * skin, Layout * lo) {
		bg::ptr<T> newView = new T(ctx, skin, lo);
		newView->build();
		return newView.release();
	}

	template <class T>
	static T * Create(bg::base::Context * ctx, Skin * skin) {
		bg::ptr<T> newView = new T(ctx, skin);
		newView->build();
		return newView.release();
	}

	// If you need to create a view instance without using the factory method, it's important to
	// call build() method inmediatly after create the View instance
	View(bg::base::Context *, Skin *, Layout * lo = nullptr);

	virtual void build();

	inline void setPosition(const bg::math::Position2Di & pos) { _position = pos; setContentChanged(true); }
	inline const bg::math::Position2Di & position() const { return _position; }
	inline void setSize(const bg::math::Size2Di & size) { _useSkinWidth = false; _useSkinHeight = false; setContentChanged(true); _size = size; }
	inline const bg::math::Size2Di & size() const { return _size; }
	inline void setScroll(const bg::math::Position2Di & pos) { _scroll = pos; setContentChanged(true); }
	inline const bg::math::Position2Di & scroll() const { return _scroll; }
	inline void setScrollSpeed(int s) { _scrollSpeed = s; }
	inline int scrollSpeed() const { return _scrollSpeed; }
	inline void setScrollPadding(int p) { _scrollPadding = p; setContentChanged(true); }
	inline int scrollPadding() const { return _scrollPadding; }
	inline void setScrollEnabled(bool e) { _scrollEnabled = e; }
	inline bool isScrollEnabled() const { return _scrollEnabled; }
	inline void setWidth(int w) { _useSkinWidth = false; setContentChanged(true); _size.width(w); }
	inline int width() const { return _size.width(); }
	inline void setHeight(int h) { _useSkinHeight = false; setContentChanged(true); _size.height(h); }
	inline int height() const { return _size.height(); }
	inline bool visible() const { return _visible; }
	inline void setVisible(bool v) { _visible = v; setContentChanged(true); }
	inline void show() { _visible = true; setContentChanged(true); }
	inline void hide() { _visible = false; setContentChanged(true); }

	inline Layout * layout() { return _layout.getPtr(); }
	inline void setLayout(Layout * lo) {
		if (children().size() > 0) {
			throw bg::base::InvalidStateException("View::setLayout(): Could not change the view layout because the children array is not empty.");
		}
		if (_layout.valid()) {
			_layout->_view = nullptr;
		}
		_layout = lo;
		if (_layout.valid()) {
			_layout->_view = this;
		}
	}

	inline void applySkinProperties(Skin * skin) {
		updateSkinProperties(skin);
		_skinPropertiesUpdated = true;
	}

	inline void setBackgroundColor(const bg::math::Color & bkg) { setContentChanged(true); _skinProperties[Status::kStatusNormal].backgroundColor = bkg; }
	inline void setBackgroundColor(Status ms, const bg::math::Color & bkg) {
		setContentChanged(true);
		if (ms<kMaxMouseStates) {
			_skinProperties[ms].backgroundColor = bkg;
		}
		else {
			for (int state = 0; state < kMaxMouseStates; ++state) {
				_skinProperties[state].backgroundColor = bkg;
			}
		}
	}
	inline const bg::math::Color & backgroundColor(Status ms = Status::kStatusNormal) const { return _skinProperties[ms].backgroundColor; }

	inline void setBorderColor(const bg::math::Color & col) { setContentChanged(true); _skinProperties[Status::kStatusNormal].borderColor = col; }
	inline void setBorderColor(Status ms, const bg::math::Color & col) {
		setContentChanged(true);
		if (ms<kMaxMouseStates) {
			_skinProperties[ms].borderColor = col;
		}
		else {
			for (int state = 0; state < kMaxMouseStates; ++state) {
				_skinProperties[state].borderColor = col;
			}
		}
	}
	inline const bg::math::Color & borderColor(Status ms = Status::kStatusNormal) const { return _skinProperties[ms].borderColor; }
                             
	inline void setBorderWidth(int w) { setContentChanged(true); _skinProperties[Status::kStatusNormal].borderWidth = w; }
	inline void setBorderWidth(Status ms, int w) {
		setContentChanged(true);
		if (ms<kMaxMouseStates) {
			_skinProperties[ms].borderWidth = w;
		}
		else {
			for (int state = 0; state < kMaxMouseStates; ++state) {
				_skinProperties[state].borderWidth = w;
			}
		}
	}
	inline int borderWidth(Status ms = Status::kStatusNormal) const { return _skinProperties[ms].borderWidth; }
                             
	inline void setTextPadding(int p) { setContentChanged(true); _skinProperties[Status::kStatusNormal].padding = p; }
	inline void setTextPadding(Status ms, int p) {
		setContentChanged(true);
		if (ms<kMaxMouseStates) {
			_skinProperties[ms].padding = p;
		}
		else {
			for (int state = 0; state < kMaxMouseStates; ++state) {
				_skinProperties[state].padding = p;
			}
		}
	}
	inline int textPadding(Status ms = Status::kStatusNormal) const { return _skinProperties[ms].padding; }

    inline void setImage(bg::base::Texture * img) { setContentChanged(true); _skinProperties[Status::kStatusNormal].image = img; }
	inline const bg::base::Texture * image(Status ms = Status::kStatusNormal) const { return _skinProperties[ms].image.getPtr(); }
	
	inline void setTextColor(const bg::math::Color & c) { _skinProperties[Status::kStatusNormal].textColor = c; }
	inline void setTextColor(Status ms, const bg::math::Color & c) {
        setContentChanged(true);
        if (ms<kMaxMouseStates) {
            _skinProperties[ms].textColor = c;
        }
        else {
            for (int state = 0; state<kMaxMouseStates; ++state) {
                _skinProperties[state].textColor = c;
            }
        }
    }
	inline const bg::math::Color & textColor(Status ms = Status::kStatusNormal) const { return _skinProperties[ms].textColor; }	

	inline Status status() const { return _status; }

	inline bg::gui::Skin * skin() { return _skin.getPtr(); }

	bg::math::Size2Di contentSize() const;

	void setContentChanged(bool ch) {
		_contentChanged = ch;
		if (_parent) {
			_parent->setContentChanged(true);
		}
	}
	inline bool contentChanged() const { return _contentChanged; }

	virtual void draw(bg::base::Pipeline *);

	inline Window * parentWindow() { return _parentWindow; }
	inline const Window * parentWindow() const { return _parentWindow; }
	Surface * parentSurface();
	const Surface * parentSurface() const;

	virtual void addedToNode(View * parent);
	virtual void removedFromNode(View * parent);

	virtual void keyDown(const bg::base::KeyboardEvent &) {}
	virtual void keyUp(const bg::base::KeyboardEvent &) {}
	virtual void charPress(const bg::base::KeyboardEvent &) {}
	virtual void mouseDown(const bg::base::MouseEvent &);
	virtual void mouseDrag(const bg::base::MouseEvent &);
	virtual void mouseMove(const bg::base::MouseEvent &);
	virtual void mouseOut();
	virtual void mouseUp(const bg::base::MouseEvent &);
	virtual void mouseWheel(const bg::base::MouseEvent &);
	virtual void customEvent(const bg::base::CustomEvent &) {}


	virtual void focusSet() {}
	virtual void focusLost() {}

	void viewInPoint(const bg::math::Position2Di & point, std::function<void(View *, const bg::math::Position2Di &)> cb);

	void triggerMouseMove(const bg::base::MouseEvent & evt);
	void triggerMouseOut();

	inline void addDecorator(ViewDecorator * d) {
		for (auto dec : _decoratorVector) {
			if (dec.getPtr() == d) return;
		}
		_decoratorVector.push_back(d);
	}

	inline void removeDecorator(ViewDecorator * d) {
		auto it = _decoratorVector.begin();
		for (; it != _decoratorVector.end(); ++it) {
			
		}
		if (it != _decoratorVector.end()) {
			_decoratorVector.erase(it);
		}
	}

	inline const std::vector<bg::ptr<ViewDecorator>> & decorators() const { return _decoratorVector; }

protected:
	virtual ~View();

	bg::math::Position2Di _position;
	bg::math::Size2Di _size;
	bg::math::Position2Di _scroll;
	int _scrollSpeed = 30;
	int _scrollPadding = 20;
	bool _scrollEnabled;
	bool _useSkinWidth;
	bool _useSkinHeight;
	bool _contentChanged;
	bool _visible;
	bg::ptr<Skin> _skin;
	Skin::Properties _skinProperties[4];
	bool _skinPropertiesUpdated;
	bg::ptr<Layout> _layout;

	bg::ptr<bg::tools::TextureFrame> _textureFrame;
	bg::ptr<bg::base::PolyList> _rect;
	bg::ptr<bg::base::Material> _mat;
	Window * _parentWindow;
	bool _mouseOutSent = false;

	std::vector<bg::ptr<ViewDecorator>> _decoratorVector;

	static bg::ptr<bg::base::PolyList> s_backgroundRect;
	static bg::base::PolyList * GetBackgroundRect(bg::base::Context *);

	void setParentWindow(Window *);

	virtual void updateContent(bg::base::Pipeline *);
	inline void updateSkinPropertiesIfNeeded() { 
		if (!_skinPropertiesUpdated) {
			updateSkinProperties(skin());
			_skinPropertiesUpdated = true;
		}
	}
	virtual Skin::WidgetType getSkinWidgetType() { return Skin::kView; }
	virtual void updateSkinProperties(Skin * skin);

	void updateTexture(bg::base::Pipeline * pipeline);

	Status _status = kStatusNormal;
};

}
}

#endif

