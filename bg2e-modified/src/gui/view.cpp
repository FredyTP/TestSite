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

#include <bg/gui/view.hpp>
#include <bg/fx/diffuse_texture.hpp>
#include <bg/tools/poly_list_primitives.hpp>
#include <bg/tools/texture_cache.hpp>
#include <bg/gui/window.hpp>
#include <bg/gui/surface.hpp>
#include <bg/fx/gui_rect.hpp>
#include <bg/system/system.hpp>

#include <bg/engine.hpp>

namespace bg {
namespace gui {


ViewDecorator::ViewDecorator()
	:_paddingLeft(0)
	,_paddingTop(0)
	,_paddingRight(0)
	,_paddingBottom(0)
{
}

ViewDecorator::ViewDecorator(int paddingLeft, int paddingTop, int paddingRight, int paddingBottom)
	:_paddingLeft(paddingLeft)
	,_paddingTop(paddingTop)
	,_paddingRight(paddingRight)
	,_paddingBottom(paddingBottom)
{
}

ViewDecorator::~ViewDecorator() {
}

// The padding in decorators only works with view layouts
void ViewDecorator::setPadding(int left, int top, int right, int bottom) {
	_paddingLeft = left;
	_paddingTop = top >= 0 ? top : left;
	_paddingRight = right >= 0 ? right : left;
	_paddingBottom = bottom >= 0 ? bottom : left;
}

void ViewDecorator::addPadding(int & left, int & top, int & right, int & bottom) {
	left += _paddingLeft;
	top += _paddingTop;
	right += _paddingRight;
	bottom += _paddingBottom;
}

void ViewDecorator::draw(View * decoratedView, bg::base::Pipeline * pipeline, float scale) {
	update(decoratedView, pipeline, scale);

	for (auto & view : _views) {
		drawSubview(decoratedView, view.getPtr(), pipeline, scale);
	}
}


void ViewDecorator::drawSubview(View * decoratedView, View * view, bg::base::Pipeline * pipeline, float scale) {
	view->updateSkinPropertiesIfNeeded();
	view->_parentWindow = decoratedView->parentWindow();
	bg::base::MatrixStack & mat = pipeline->effect()->matrixState().modelMatrixStack();
	mat.push();
	mat.translate(
		static_cast<float>(static_cast<float>(view->position().x()) * scale),
		static_cast<float>(static_cast<float>(view->position().y()) * scale),
		0.0f
	);
	view->draw(pipeline);
	mat.pop();
}



bg::ptr<bg::base::PolyList> View::s_backgroundRect;

bg::base::PolyList * View::GetBackgroundRect(bg::base::Context * context) {
	if (s_backgroundRect.isNull()) {
		using namespace bg::tools;
		bg::Engine::AddDestroyCallback([]() {
			s_backgroundRect = nullptr;
		});

		s_backgroundRect = PolyListPrimitives(context)
			.plane(1.0f, 1.0f, PolyListPrimitives::kAlignZPlane, bg::math::Vector2(0.5f, 0.5f));
	}
	return s_backgroundRect.getPtr();
}

View::View(bg::base::Context * ctx, Skin * skin, Layout * lo)
	:ContextObject(ctx)
	,_position(0,0)
	,_size(200,200)
	,_scrollEnabled(false)
	,_useSkinWidth(true)
	,_useSkinHeight(true)
	,_contentChanged(true)
	,_visible(true)
	,_skin(skin)
	,_skinPropertiesUpdated(false)
	,_layout(lo)
	,_parentWindow(nullptr)
{
	if (_layout.valid()) {
		_layout->_view = this;
	}
}

View::~View() {
}

void View::build() {
	using namespace bg::tools;

	_textureFrame = new bg::tools::TextureFrame(context());
	_textureFrame->pipeline()->setEffect(new bg::fx::GuiRect(context()));
	_textureFrame->onDraw([&](bg::base::Pipeline * pipeline) {
		updateTexture(pipeline);
	});

	_textureFrame->onResize([&](bg::base::Pipeline * pipeline, const bg::math::Size2Di & size) {
		pipeline->setViewport(bg::math::Viewport(0, 0, size.width(), size.height()));
		pipeline->effect()->matrixState().projectionMatrixStack()
			.ortho(0.0f, static_cast<float>(size.width()), 0.0f, static_cast<float>(size.height()), -10.0f, 10.0f);
	});
	_textureFrame->pipeline()->setClearColor(bg::math::Color::White());
	_textureFrame->pipeline()->setBlend(true);
	_textureFrame->pipeline()->setBlendMode(bg::base::kNormal);
	_textureFrame->pipeline()->setDepthTest(false);

	_rect = PolyListPrimitives(context())
		.plane(1.0f, 1.0f, PolyListPrimitives::kAlignZPlane, bg::math::Vector2(0.5f, 0.5f));

	_mat = new bg::base::Material();
	_mat->setTexture(_textureFrame->texture());

	applySkinProperties(skin());
}

bg::math::Size2Di View::contentSize() const {
	bg::math::Size2Di contentSize;
	for (auto child : _children) {
		bg::math::Size2Di size = child->size();
		bg::math::Position2Di pos = child->position();
		if ((pos.x() + size.width()) > contentSize.width()) {
			contentSize.width(pos.x() + size.width());
		}
		if ((pos.y() + size.height()) > contentSize.height()) {
			contentSize.height(pos.y() + size.height());
		}
	}
	return contentSize;
}

void View::draw(bg::base::Pipeline * pipeline) {
	updateSkinPropertiesIfNeeded();
	if (!visible()) return;
	if (_layout.valid()) {
		_layout->beginLayout();
		for (auto c : children()) {
			_layout->setNext(c.getPtr());
		}
	}
	float s = 1.0f;
	if (parentWindow() && parentWindow()->parentSurface()) {
		setContentChanged(_contentChanged || parentWindow()->parentSurface()->scaleChanged());
		s = parentWindow()->parentSurface()->scale();
	}
	if (contentChanged()) {
		_textureFrame->setSize(bg::math::Size2Di(
			static_cast<int>(static_cast<float>(_size.width()) * s),
			static_cast<int>(static_cast<float>(_size.height()) * s)
		));
		_textureFrame->update();
		_contentChanged = false;
	}
	bg::ptr<bg::base::Material> m = pipeline->effect()->materialPtr();
	bg::fx::GuiRect * guiRect = dynamic_cast<bg::fx::GuiRect*>(pipeline->effect());

	if (guiRect) {
		guiRect->setFrameSize(_textureFrame->size());
		guiRect->setBorderColor(_skinProperties[_status].borderColor);
		guiRect->setBorderWidth(
			static_cast<int>(
				static_cast<float>(_skinProperties[_status].borderWidth) * s
			)
		);
	}

	pipeline->effect()->setMaterial(_mat.getPtr());
	pipeline->effect()->matrixState().modelMatrixStack()
		.push()
		.scale(
			static_cast<float>(_size.width()) * s,
			static_cast<float>(_size.height()) * s,
			0.0f);
	pipeline->draw(_rect.getPtr());
	pipeline->effect()->setMaterial(m.getPtr());
	pipeline->effect()->matrixState().modelMatrixStack()
		.pop();
}

Surface * View::parentSurface() {
	return parentWindow() ? parentWindow()->parentSurface() : nullptr;
}

const Surface * View::parentSurface() const {
	return parentWindow() ? parentWindow()->parentSurface() : nullptr;
}

void View::addedToNode(View * parent) {
	_parentWindow = parent->_parentWindow;
}

void View::removedFromNode(View *) {
	_parentWindow = nullptr;
}

void View::setParentWindow(Window * wnd) {
	_parentWindow = wnd;
	eachChild([&](View * child) {
		child->setParentWindow(wnd);
	});
}

void View::mouseDown(const bg::base::MouseEvent & evt) {
	viewInPoint(evt.pos(),[&](View * child, const bg::math::Position2Di & pos) {
		bg::base::MouseEvent newEvt = evt;
		newEvt.setPos(pos);
		child->mouseDown(newEvt);
	});
	_status = kStatusPress;
	setContentChanged(true);
}

void View::mouseDrag(const bg::base::MouseEvent & evt) {
	viewInPoint(evt.pos(), [&](View * child, const bg::math::Position2Di & pos) {
		bg::base::MouseEvent newEvt = evt;
		newEvt.setPos(pos);
		child->mouseDrag(newEvt);
	});
}

void View::mouseMove(const bg::base::MouseEvent & evt) {
	if (_status != kStatusHover) {
		_status = kStatusHover;
		setContentChanged(true);
	}
}

void View::mouseOut() {
	if (_status != kStatusNormal) {
		_status = kStatusNormal;
		setContentChanged(true);
	}
}

void View::mouseUp(const bg::base::MouseEvent & evt) {
	bool childClicked = false;
	viewInPoint(evt.pos(), [&](View * child, const bg::math::Position2Di & pos) {
		bg::base::MouseEvent newEvt = evt;
		newEvt.setPos(pos);
		child->mouseUp(newEvt);
		childClicked = true;
	});
	if (bg::system::isMobile()) {
		_status = kStatusNormal;
	}
	else {
		_status = kStatusHover;
	}
	setContentChanged(true);
	if (!childClicked && parentSurface()) {
		parentSurface()->clearFocus();
	}
}

void View::mouseWheel(const bg::base::MouseEvent & evt) {
	if (_scrollEnabled) {
		bg::math::Size2Di contentSize = this->contentSize();
		bg::math::Position2Di scroll(
			_scroll.x() + static_cast<int>(evt.delta().x()) * _scrollSpeed,
			_scroll.y() + static_cast<int>(evt.delta().y()) * _scrollSpeed
		);
		if (scroll.x() < 0) {
			scroll.x(0);
		}
		if (scroll.y() < 0) {
			scroll.y(0);
		}
		if ((scroll.y() + _size.height()) > (contentSize.height() + _scrollPadding)) {
			scroll.y(contentSize.height() - _size.height() + _scrollPadding);
		}
		if ((scroll.x() + _size.width()) > (contentSize.width() + _scrollPadding)) {
			scroll.x(contentSize.width() - _size.width() + _scrollPadding);
		}
		setScroll(scroll);
	}
	else {
		viewInPoint(evt.pos(), [&](View * child, const bg::math::Position2Di & pos) {
			bg::base::MouseEvent newEvt = evt;
			newEvt.setPos(pos);
			child->mouseWheel(newEvt);
		});
	}
}

void View::viewInPoint(const bg::math::Position2Di & point, std::function<void(View *, const bg::math::Position2Di &)> cb) {
	using namespace bg::math;
	float scale = 1.0f;
	if (parentWindow() && parentWindow()->parentSurface()) {
		scale = parentWindow()->parentSurface()->scale();
	}
	for (auto it = _children.rbegin(); it != _children.rend(); ++it) {
		View * view = (*it).getPtr();
		if (view->visible()) {
			Size2Di size(
				static_cast<int>(static_cast<float>(view->size().width()) * scale),
				static_cast<int>(static_cast<float>(view->size().height()) * scale)
			);
			Position2Di pos = view->position();
			pos.x(static_cast<int>(static_cast<float>(pos.x() - _scroll.x()) * scale));
			pos.y(static_cast<int>(static_cast<float>(pos.y() - _scroll.y()) * scale));
			Rect rect(pos.x(), pos.y(),size.width(),size.height());
			if(rect.containsPoint(point)) {
				cb(view, point - pos);
			}
		}
	}
	for (auto dec : _decoratorVector) {
		for (auto it = dec->views().rbegin(); it != dec->views().rend(); ++it) {
			View * view = (*it).getPtr();
			if (view->visible()) {
				Size2Di size(
					static_cast<int>(static_cast<float>(view->size().width()) * scale),
					static_cast<int>(static_cast<float>(view->size().height()) * scale)
				);
				Position2Di pos = view->position();
				// Decorator views aren't affected by scroll
				pos.x(static_cast<int>(static_cast<float>(pos.x()) * scale));
				pos.y(static_cast<int>(static_cast<float>(pos.y()) * scale));
				Rect rect(pos.x(), pos.y(), size.width(), size.height());
				if (rect.containsPoint(point)) {
					cb(view, point - pos);
				}
			}
		}
	}
}

void View::triggerMouseMove(const bg::base::MouseEvent & evt) {
	bg::ptr<View> targetView;
	viewInPoint(evt.pos(), [&](View * child, const bg::math::Position2Di & pos) {
		targetView = child;
		bg::base::MouseEvent newEvt = evt;
		newEvt.setPos(pos);
		child->mouseMove(newEvt);
		child->triggerMouseMove(newEvt);
	});
	for (auto & ch : _children) {
		if (ch.getPtr()!=targetView.getPtr()) {
			ch->triggerMouseOut();
		}
	}
	for (auto & dec : _decoratorVector) {
		for (auto & v : dec->views()) {
			if (v.getPtr() != targetView.getPtr()) {
				v->triggerMouseOut();
			}
		}
	}
	_mouseOutSent = false;
}

void View::triggerMouseOut() {
	if(!_mouseOutSent) {
		mouseOut();
		_mouseOutSent = true;
	}
	for(auto & ch : _children) {
		ch->triggerMouseOut();
	}
	for (auto & dec : _decoratorVector) {
		for (auto & v : dec->views()) {
			v->triggerMouseOut();
		}
	}
}

void View::updateContent(bg::base::Pipeline * pipeline) {
	pipeline->setClearColor(_skinProperties[_status].backgroundColor);
	pipeline->clearBuffers();
	bg::base::Texture * tex = _skinProperties[status()].image.getPtr();
	if (tex) pipeline->drawTexture(tex);
}

void View::updateSkinProperties(Skin * s) {
	for (auto ms = 0; ms < Status::kMaxMouseStates; ++ms) {
		_skinProperties[ms] = s->widgetProperties(getSkinWidgetType(), static_cast<Status>(ms));
	}
	if (_useSkinWidth) {
		_size.width(_skinProperties[kStatusNormal].defaultSize.width());
	}
	if (_useSkinHeight) {
		_size.height(_skinProperties[kStatusNormal].defaultSize.height());
	}
}

void View::updateTexture(bg::base::Pipeline * pipeline) {
	float s = (parentWindow() && parentWindow()->parentSurface()) ?
						parentWindow()->parentSurface()->scale() : 1.0f;
	updateContent(pipeline);
	bg::base::MatrixStack & mat = pipeline->effect()->matrixState().modelMatrixStack();
	mat.identity();
	bg::math::Size2Di parentSize = bg::math::Size2Di(
		static_cast<int>(static_cast<float>(size().width()) * s),
		static_cast<int>(static_cast<float>(size().height()) * s)
	);
	for (auto & ch : children()) {
		ch->updateSkinPropertiesIfNeeded();
		ch->_parentWindow = _parentWindow;
		mat.push();
		bg::math::Position2Di pos = ch->position();
		bg::math::Size2Di size = ch->size();
		float sX = static_cast<float>(pos.x() - _scroll.x()) * s;
		float sY = static_cast<float>(pos.y() - _scroll.y()) * s;
		//float sW = static_cast<float>(size.width()) * s;
		float sH = static_cast<float>(size.height()) * s;
		mat.translate(
			sX,
			static_cast<float>(parentSize.height()) - sH - sY,
			0.0f);
		ch->draw(pipeline);
		mat.pop();
	}

	for (auto d : _decoratorVector) {
		d->draw(this, pipeline,s);
	}
	setContentChanged(false);
}

}
}
