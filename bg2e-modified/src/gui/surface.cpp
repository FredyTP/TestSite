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

#include <bg/gui/surface.hpp>
#include <bg/base/exception.hpp>
#include <bg/fx/diffuse_texture.hpp>
#include <bg/fx/gui_rect.hpp>
#include <bg/log.hpp>

#include <algorithm>

namespace bg {
namespace gui {

Surface::Surface(bg::base::Context * ctx, Skin * skin)
	:ContextObject(ctx)
	,_scale(1.0f)
	,_skin(skin)
{
	_pipeline = new bg::base::Pipeline(context());
	_pipeline->setEffect(new bg::fx::GuiRect(context()));
	_pipeline->setDepthTest(false);
	_pipeline->setBlend(true);
	_pipeline->setBlendMode(bg::base::kNormal);
}

Surface::~Surface() {
}

void Surface::resize(int w, int h) {
	_pipeline->setViewport(bg::math::Viewport(0,0,w,h));
	_pipeline->effect()->matrixState().projectionMatrixStack()
		.ortho(0, static_cast<float>(w),
			   0, static_cast<float>(h),
			   -10.0f, 10.0f);

	eachWindow([&](Window * wnd) {
		updateLayout(wnd);
	});
}

void Surface::frame(float delta) {
	eachWindow([&](Window * wnd) {
	});
}

void Surface::draw() {
	bg::base::Pipeline * current = bg::base::Pipeline::Current();
	bg::base::Pipeline::SetCurrent(_pipeline.getPtr());
	_pipeline->clearBuffers(bg::base::kDepth);
	bg::math::Viewport vp = _pipeline->viewport();
	bg::base::MatrixState & ms = _pipeline->effect()->matrixState();
	float s = scale();
	eachWindow([&](Window * wnd) {
		if (wnd->visible()) {
			if(scaleChanged()) {
				updateLayout(wnd);
			}
			float sX = static_cast<float>(wnd->position().x());
			float sY = static_cast<float>(wnd->position().y());
			float sH = static_cast<float>(wnd->size().height()) * s;
			ms.modelMatrixStack().push();
			ms.modelMatrixStack()
				.translate(
					sX,
					static_cast<float>(vp.height()) - sY - sH,
					0.0f);
			wnd->draw(_pipeline.getPtr());
			ms.modelMatrixStack().pop();
		}
	});
	_scaleChanged = false;
	bg::base::Pipeline::SetCurrent(current);
}

void Surface::addWindow(Window * wnd) {
	if (!wnd) throw bg::base::NullParameterException("Error adding window to gui::Surface: null window");
	auto it = std::find(_windowVector.begin(), _windowVector.end(), wnd);
	if(it == _windowVector.end()) {
		_windowVector.push_back(wnd);
		wnd->_parentSurface = this;
	}
}

void Surface::removeWindow(Window * wnd) {
	if(!wnd) throw bg::base::NullParameterException("Error removing window to gui::Surface: null window");
	auto it = std::find(_windowVector.begin(), _windowVector.end(), wnd);
	if(it != _windowVector.end()) {
		_windowVector.erase(it);
		wnd->_parentSurface = nullptr;
	}
}

void Surface::removeAllWindows() {
	for (auto & wnd : _windowVector) {
		wnd->_parentSurface = nullptr;
	}
	_windowVector.clear();
}

void Surface::updateLayout(Window * wnd) {
	bg::math::Size2Di surfaceSize = bg::math::Size2Di(
		_pipeline->viewport().width(),
		_pipeline->viewport().height()
	);
	bg::math::Size2Di alignMargin(
		static_cast<int>(static_cast<float>(wnd->alignmentMargin().width()) * scale()),
		static_cast<int>(static_cast<float>(wnd->alignmentMargin().height()) * scale())
	);
	bg::math::Size2Di anchorMargin(
		static_cast<int>(static_cast<float>(wnd->anchorMargin().width()) * scale()),
		static_cast<int>(static_cast<float>(wnd->anchorMargin().height()) * scale())
	);
	bg::math::Size2Di wndSize = bg::math::Size2Di(
		static_cast<int>(static_cast<float>(wnd->size().width()) * scale()),
		static_cast<int>(static_cast<float>(wnd->size().height()) * scale())
	);
	bg::math::Size2Di newSize;
	if (Layout::UpdateSize(wnd->alignment(), wnd->anchor(), surfaceSize, wndSize, alignMargin, anchorMargin, newSize)) {
		// It's important to set the unscaled window size
		wnd->setSize(bg::math::Size2Di(
				static_cast<int>(static_cast<float>(newSize.width()) / scale()),
				static_cast<int>(static_cast<float>(newSize.height()) / scale())
		));
	}
	
	bg::math::Position2Di newPos;
	Layout::GetPosition(wnd->alignment(), surfaceSize, newSize, alignMargin, newPos);
	wnd->setPosition(newPos);
}

bool Surface::keyDown(const bg::base::KeyboardEvent & evt) {
	if (_focusView.valid()) {
		_focusView->keyDown(evt);
		return true;
	}
	return false;
}

bool Surface::keyUp(const bg::base::KeyboardEvent & evt) {
	if (_focusView.valid()) {
		_focusView->keyUp(evt);
		return true;
	}
	return false;
}

bool Surface::charPress(const bg::base::KeyboardEvent & evt) {
	if(_focusView.valid()) {
		_focusView->charPress(evt);
		return true;
	}
	return false;
}

bool Surface::mouseDown(const bg::base::MouseEvent & evt) {
	bool result = false;
	windowInPoint(evt.pos(), [&](Window * wnd, const bg::math::Position2Di & pos) {
		if (wnd->contentView()) {
			result = true;
			bg::base::MouseEvent viewEvt = evt;
			viewEvt.setPos(pos);
			wnd->contentView()->mouseDown(viewEvt);
		}
	});
	return result;
}

bool Surface::mouseDrag(const bg::base::MouseEvent & evt) {
	bool result = false;
	windowInPoint(evt.pos(), [&](Window * wnd, const bg::math::Position2Di & pos) {
		if(wnd->contentView()) {
			result = true;
			bg::base::MouseEvent viewEvt = evt;
			viewEvt.setPos(pos);
			wnd->contentView()->mouseDrag(viewEvt);
		}
	});
	return result;
}

bool Surface::mouseMove(const bg::base::MouseEvent & evt) {
	bool result = false;
	bg::ptr<Window> hitWindow;
	windowInPoint(evt.pos(), [&](Window * wnd, const bg::math::Position2Di & pos) {
		if(wnd->contentView()) {
			hitWindow = wnd;
			result = true;
			bg::base::MouseEvent viewEvt = evt;
			viewEvt.setPos(pos);
			wnd->contentView()->triggerMouseMove(viewEvt);
		}
	});
	for (auto & wnd : _windowVector) {
		if (wnd.getPtr()!=hitWindow.getPtr() && wnd->contentView()) {
			wnd->contentView()->triggerMouseOut();
		}
	}
	return result;
}

bool Surface::mouseUp(const bg::base::MouseEvent & evt) {
	bool result = false;
	windowInPoint(evt.pos(), [&](Window * wnd, const bg::math::Position2Di & pos) {
		if(wnd->contentView()) {
			result = true;
			bg::base::MouseEvent viewEvt = evt;
			viewEvt.setPos(pos);
			wnd->contentView()->mouseUp(viewEvt);
		}
	});
	if(!result) {
		clearFocus();
	}
	return result;
}

bool Surface::mouseWheel(const bg::base::MouseEvent & evt) {
	bool result = false;
	windowInPoint(evt.pos(), [&](Window * wnd, const bg::math::Position2Di & pos) {
		if(wnd->contentView()) {
			result = true;
			bg::base::MouseEvent viewEvt = evt;
			viewEvt.setPos(pos);
			wnd->contentView()->mouseWheel(viewEvt);
		}
	});
	return result;
}

bool Surface::touchStart(const bg::base::TouchEvent & evt) {
	bool result = false;
	bg::math::Position2Di pos = *evt.touches().front();
	windowInPoint(pos, [&](Window * wnd, const bg::math::Position2Di & viewPos) {
		if(wnd->contentView()) {
			result = true;
			bg::base::MouseEvent viewEvt;
			viewEvt.mouse().setButtonMask(bg::base::Mouse::kLeftButton);
			viewEvt.setPos(viewPos);
			wnd->contentView()->mouseDown(viewEvt);
		}
	});
	return result;
}

bool Surface::touchMove(const bg::base::TouchEvent & evt) {
	bool result = false;
	bg::math::Position2Di pos = *evt.touches().front();
	windowInPoint(pos, [&](Window * wnd, const bg::math::Position2Di & viewPos) {
		if(wnd->contentView()) {
			result = true;
			bg::base::MouseEvent viewEvt;
			viewEvt.mouse().setButtonMask(bg::base::Mouse::kLeftButton);
			viewEvt.setPos(viewPos);
			wnd->contentView()->mouseDrag(viewEvt);
		}
	});
	return result;
}

bool Surface::touchEnd(const bg::base::TouchEvent & evt) {
	bool result = false;
	bg::math::Position2Di pos = *evt.touches().front();
	windowInPoint(pos, [&](Window * wnd, const bg::math::Position2Di & viewPos) {
		if(wnd->contentView()) {
			result = true;
			bg::base::MouseEvent viewEvt;
			viewEvt.mouse().setButtonMask(bg::base::Mouse::kLeftButton);
			viewEvt.setPos(viewPos);
			wnd->contentView()->mouseUp(viewEvt);
		}
	});
	return result;
}

void Surface::customEvent(const bg::base::CustomEvent & evt) {
	if(_focusView.valid()) {
		_focusView->customEvent(evt);
	}
}

void Surface::setFocus(Window * wnd) {
	_focusWindow = wnd;
	if (_focusView.valid()) {
		_focusView->focusLost();
	}
	_focusView = nullptr;
}

void Surface::setFocus(View * view) {
	if (_focusView.valid()) {
		_focusView->focusLost();
		_focusView = nullptr;
	}
	_focusView = view;
	if (_focusView.valid()) {
		_focusWindow = _focusView->parentWindow();
		_focusView->focusSet();
	}
}

void Surface::clearFocus() {
	if (_focusView.valid()) {
		_focusView->focusLost();
		_focusView = nullptr;
	}
	_focusWindow = nullptr;
}

}
}
