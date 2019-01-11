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

#include <bg/platform.hpp>

#include <bg/audio/opensl/engine.hpp>

#include <bg/base/exception.hpp>

#include <bg/engine.hpp>

#if BG2E_OPENSLES_AVAILABLE==1

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#endif


namespace bg {
namespace audio {
namespace opensl {

Engine * Engine::s_engine = nullptr;

Engine::Engine()
    :_engineObj(nullptr)
    ,_engine(nullptr)
    ,_outputMixObj(nullptr)
{

}

Engine::~Engine() {

}

void Engine::create() {
#if BG2E_OPENSLES_AVAILABLE==1
    const SLuint32 engineMixIIDCount = 1;
    const SLInterfaceID engineMixIIDs[] = { SL_IID_ENGINE };
    const SLboolean engineMixReqs[] = { SL_BOOLEAN_TRUE };
    const SLuint32 outputMixIIDCount = 0;
    const SLInterfaceID outputMixIIDs[] = {};
    const SLboolean outputMixReqs[] = {};

    SLObjectItf engineObj;
    SLEngineItf engine;
    SLObjectItf outputMixObj;
    bg::base::AudioCreateException except("Error initializing OpenSL ES engine.");

    bg::assert(slCreateEngine(&engineObj, 0, nullptr, engineMixIIDCount, engineMixIIDs, engineMixReqs)==SL_RESULT_SUCCESS, except);
    bg::assert((*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE)==SL_RESULT_SUCCESS, except);
    bg::assert((*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engine)==SL_RESULT_SUCCESS, except);
    bg::assert((*engine)->CreateOutputMix(engine,&outputMixObj,outputMixIIDCount,outputMixIIDs,outputMixReqs)==SL_RESULT_SUCCESS, except);
    bg::assert((*outputMixObj)->Realize(outputMixObj,SL_BOOLEAN_FALSE)==SL_RESULT_SUCCESS, except);

    _engineObj = reinterpret_cast<ObjectImpl>(engineObj);
    _engine = reinterpret_cast<EngineImpl>(engine);
    _outputMixObj = reinterpret_cast<ObjectImpl>(outputMixObj);

    bg::Engine::AddDestroyCallback([&]() {
        destroy();
        delete Engine::s_engine;
        Engine::s_engine = nullptr;
    });
#endif
}

void Engine::destroy() {
#if BG2E_OPENSLES_AVAILABLE==1
    SLObjectItf engineObj = reinterpret_cast<SLObjectItf>(_engineObj);
    SLObjectItf outputMixObj = reinterpret_cast<SLObjectItf>(_outputMixObj);

    if (outputMixObj!=nullptr) {
        (*outputMixObj)->Destroy(outputMixObj);
    }
    if (engineObj!=nullptr) {
        (*engineObj)->Destroy(engineObj);
    }
    _outputMixObj = nullptr;
    _engineObj = nullptr;
    _engine = nullptr;
#endif
}

}
}
}
