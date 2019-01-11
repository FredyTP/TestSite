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


#include <bg/render/render_layer.hpp>

namespace bg {
namespace render {

namespace settings {

const SettingsKey kDebug = "debug";
	
const SettingsKey kEnabled = "enabled";
const SettingsKey kBlur = "blur";
const SettingsKey kSampleRadius = "sampleRadius";
const SettingsKey kMaxDistance = "maxDistance";
const SettingsKey kColor = "color";
const SettingsKey kQuality = "quality";
const SettingsKey kScale = "scale";

}

RenderLayer::RenderLayer(bg::base::Context * ctx, bg::base::Pipeline * pl)
	:bg::base::ContextObject(ctx)
	,_pipeline(pl ? pl:new bg::base::Pipeline(ctx))
	,_drawVisitor(new bg::scene::DrawVisitor())
{
}

RenderLayer::~RenderLayer() {
	
}

void RenderLayer::draw(bg::scene::Node * node, bg::scene::Camera * camera) {
	drawVisitor()->setPipeline(pipeline());
	bg::base::Effect * effect = pipeline()->effect();
	effect->matrixState().projectionMatrixStack()
		.set(camera->projection());
	
	effect->matrixState().viewMatrixStack()
		.identity()
		.mult(camera->viewMatrix());
	
	effect->matrixState().modelMatrixStack()
		.identity();
	
	willDraw(node, camera);
	bg::base::Pipeline::SetCurrent(_pipeline.getPtr());
	performDraw(node, camera);
}

}
}