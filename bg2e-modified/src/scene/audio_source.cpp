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


#include <bg/scene/audio_source.hpp>
#include <bg/scene/transform_visitor.hpp>

#include <bg/scene/camera.hpp>

namespace bg {
namespace scene {

AudioSource::AudioSource()
{
	_source = new bg::audio::Source();
	_source->create();
	_transformVisitor = new bg::scene::TransformVisitor();
}

AudioSource::~AudioSource() {
	
}
	

Component * AudioSource::clone(){
	bg::ptr<AudioSource> newInstance = new AudioSource();
	newInstance->setSource(source());
	return newInstance.getPtr();
}

void AudioSource::frame(float){
	if (node() && source()) {
		_transformVisitor->matrix().identity();
		node()->acceptReverse(_transformVisitor.getPtr());
		source()->setPosition(_transformVisitor->matrix().multVector(bg::math::Vector3()).xyz());
	}
}

void AudioSource::deserialize(bg::base::Context *, bg::db::json::Value *, const bg::system::Path &){
	// TODO: deserialize source and it's audio buffer
	// NOTE: It's important to deserialize the initial source status:
	//	looping mode
	//	gain
	//	pitch
}


bg::db::json::Value * AudioSource::serialize(bg::base::Context *, const bg::system::Path &){
	// TODO: serialize source and it's audio buffer
	// NOTE: It's important to serialize the initial source status:
	//	looping mode
	//	gain
	//	pitch
	return nullptr;
}


	
	
}
}
