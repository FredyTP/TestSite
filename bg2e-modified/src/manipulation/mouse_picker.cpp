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


#include <bg/manipulation/mouse_picker.hpp>
#include <bg/scene/camera.hpp>

namespace bg {
namespace manipulation {

MousePicker::MousePicker(bg::base::Context * ctx)
	:ContextObject(ctx)
{
	
}

MousePicker::~MousePicker() {
	
}

SelectableItem * MousePicker::pick(bg::scene::Node * sceneRoot, bg::scene::Camera * camera, const bg::math::Position2Di & position) {
	using namespace bg::base;
	ptr<SelectableItem> result;
	Pipeline * current = Pipeline::Current();
	Pipeline::SetCurrent(pipeline());
	pipeline()->setViewport(camera->viewport());
	
	pipeline()->clearBuffers(ClearBuffers::kColorDepth);
	
	_effect->matrixState().projectionMatrixStack().set(camera->projection());
	_effect->matrixState().viewMatrixStack().set(camera->viewMatrix());
	
	// Prevent draw
	pipeline()->setOpacityLayer(Pipeline::kOpacityNone);
	sceneRoot->accept(_drawVisitor.getPtr());
	
	unsigned char * buffer = nullptr;
	bg::math::Viewport vp(position.x(),
						  camera->viewport().height() - position.y(),
						  1,1);
	buffer = _renderSurface->readPixels(vp);
	
	if (buffer) {
		ptr<FindPickIdVisitor> findVisitor = new FindPickIdVisitor();
		ptr<Image> img = new Image();
		img->setData(buffer, bg::math::Size2Di(1), 4, Image::kFormatRGBA);
		bg::math::Color pixel;
		img->color(position.x(), camera->viewport().height() - position.y(),pixel);
		findVisitor->setTarget(ColorPickId(pixel));
		sceneRoot->accept(findVisitor.getPtr());
		result = findVisitor->result();
	}
	
	Pipeline::SetCurrent(current);
	return result.release();
}

bg::base::Pipeline * MousePicker::pipeline() {
	if (_pipeline.isNull()) {
		_pipeline = new bg::base::Pipeline(context());
		_effect = new bg::fx::ColorPickerEffect(context());
		_effect->build();
		_pipeline->setEffect(_effect.getPtr());
		_renderSurface = new bg::base::TextureRenderSurface(context());
		_renderSurface->create();
		_pipeline->setRenderSurface(_renderSurface.getPtr());
		_pipeline->setClearColor(bg::math::Color::Transparent());
		drawVisitor()->setPipeline(_pipeline.getPtr());
	}
	return _pipeline.getPtr();
}

bg::scene::DrawVisitor * MousePicker::drawVisitor() {
	if (_drawVisitor.isNull()) {
		_drawVisitor = new bg::scene::DrawVisitor();
	}
	return _drawVisitor.getPtr();
}


}
}
