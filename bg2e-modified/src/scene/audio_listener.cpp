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


#include <bg/scene/audio_listener.hpp>
#include <bg/scene/transform_visitor.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace scene {

ComponentFactory<AudioListener> audioListenerFactory("AudioListener");

AudioListener::AudioListener()
{
	_listener = new bg::audio::Listener();
	_transformVisitor = new bg::scene::TransformVisitor();
}

AudioListener::~AudioListener() {
	
}

Component * AudioListener::clone() {
	return new AudioListener();
}

void AudioListener::frame(float) {
	if (node() && listener()) {
		_transformVisitor->matrix().identity();
		node()->acceptReverse(_transformVisitor.getPtr());
		bg::math::Matrix4 viewMatrix = _transformVisitor->matrix();
		listener()->setPosition(viewMatrix.multVector(bg::math::Vector3()).xyz());
		viewMatrix.setRow(3, bg::math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		listener()->setDirection(viewMatrix.multVector(bg::math::Vector3(0.0f, 0.0f, -1.0f)).xyz());
		listener()->setUpVector(viewMatrix.multVector(bg::math::Vector3(0.0f, 1.0f, 0.0f)).xyz());
	}
}

void AudioListener::deserialize(bg::base::Context *, bg::db::json::Value *, const bg::system::Path &) {
	// Nothing to do here
}

bg::db::json::Value * AudioListener::serialize(bg::base::Context *, const bg::system::Path &) {
	using namespace bg::db::json;
	Value * listenerData = new Value();
	listenerData->setValue("type","AudioListener");
	return listenerData;
}
	
}
}
