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

#ifndef _bg2e_scene_audio_source_hpp_
#define _bg2e_scene_audio_source_hpp_

#include <bg/scene/component.hpp>
#include <bg/audio/source.hpp>
#include <bg/scene/transform_visitor.hpp>

namespace bg {
namespace scene {

class BG2E_EXPORT AudioSource : public Component {
public:
	AudioSource();
	
	virtual Component * clone();
	
	virtual void frame(float);
	
	virtual void deserialize(bg::base::Context *, bg::db::json::Value *, const bg::system::Path &);
	virtual bg::db::json::Value * serialize(bg::base::Context *, const bg::system::Path &);

	inline void setSource(bg::audio::Source * src) { _source = src; }
	inline bg::audio::Source * source() { return _source.getPtr(); }
	inline const bg::audio::Source * source() const { return _source.getPtr(); }
	
protected:
	virtual ~AudioSource();
	
	bg::ptr<bg::audio::Source> _source;
	bg::ptr<bg::scene::TransformVisitor> _transformVisitor;
};

}
}

#endif
