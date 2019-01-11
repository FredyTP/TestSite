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

#ifndef _bg2e_audio_opensl_hpp_
#define _bg2e_audio_opensl_hpp_

#include <bg/base/referenced_pointer.hpp>

#include <bg/audio/opensl/types.hpp>

namespace bg {
namespace audio {
namespace opensl {

class BG2E_EXPORT Engine {
public:
    static Engine & Get() {
        if (s_engine==nullptr) {
            s_engine = new Engine();
            s_engine->create();
        }
        return *s_engine;
    }

    template <class T>
    inline T engineObj() { return reinterpret_cast<T>(_engineObj); }
    template <class T>
    inline T engine() { return reinterpret_cast<T>(_engine); }
    template <class T>
    inline T outputMixObj() { return reinterpret_cast<T>(_outputMixObj); }


protected:
    Engine();
    virtual ~Engine();

    static Engine * s_engine;

    void create();
    void destroy();

    ObjectImpl _engineObj;
    EngineImpl _engine;
    ObjectImpl _outputMixObj;
};

}
}
}

#endif
