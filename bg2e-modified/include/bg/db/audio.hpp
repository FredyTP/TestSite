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


#ifndef _bg2e_db_audio_hpp_
#define _bg2e_db_audio_hpp_

#include <bg/audio/context.hpp>
#include <bg/audio/buffer.hpp>
#include <bg/db/plugin.hpp>

namespace bg {
namespace db {

typedef AudioPlugin<bg::audio::Buffer> ReadAudioPlugin;

class BG2E_EXPORT AudioLoader {
public:
	typedef std::vector<bg::ptr<ReadAudioPlugin> > ReadPluginVector;
	
	static void RegisterPlugin(ReadAudioPlugin *);
	
	AudioLoader(bg::audio::Context *);
	
	inline bg::audio::Buffer * load(const bg::system::Path & p) { return load(p.text()); }
	bg::audio::Buffer * load(const std::string &);
	
protected:
	bg::ptr<bg::audio::Context> _ctx;
	
	static ReadPluginVector s_readPlugins;
	static bool s_destroyCallbackRegistered;
};

/*
 *	Quick loader functions: loads a drawable with default configuration
 */
extern BG2E_EXPORT bg::audio::Buffer * loadAudio(bg::audio::Context *, const bg::system::Path &);
extern BG2E_EXPORT bg::audio::Buffer * loadDrawable(bg::audio::Context *, const std::string &);

}
}

#endif
