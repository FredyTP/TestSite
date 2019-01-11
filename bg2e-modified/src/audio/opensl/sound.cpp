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

#include <bg/audio/opensl/sound.hpp>
#include <bg/base/exception.hpp>
#include <bg/log.hpp>

#if BG2E_OPENSLES_AVAILABLE==1

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

#endif

namespace bg {
namespace audio {
namespace opensl {

#if BG2E_OPENSLES_AVAILABLE==1
void soundCallback(SLBufferQueueItf obj, void * context) {

    Sound * sound = reinterpret_cast<Sound*>(context);
    if (sound) {
        sound->stop();
        if (sound->loop()) {
            sound->play();
        }
    }
}
#endif

Sound::Sound()
    :_playerObj(nullptr)
    ,_player(nullptr)
    ,_bufferQueueObj(nullptr)
    ,_loop(false)
{

}

Sound::~Sound() {
    destroy();
}

void Sound::create(bg::base::Buffer * buffer, int channels, SampleRate samplesPerSec, SampleFormat bitsPerSample, SampleFormat containerSize) {
#if BG2E_OPENSLES_AVAILABLE==1
    SLDataLocator_AndroidSimpleBufferQueue dataLocatorIn;
    dataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
    dataLocatorIn.numBuffers = 1;


    SLDataFormat_PCM dataFormat;
    dataFormat.formatType = SL_DATAFORMAT_PCM;
    dataFormat.numChannels = channels;
    dataFormat.samplesPerSec = samplesPerSec;
    dataFormat.bitsPerSample = bitsPerSample;
    dataFormat.containerSize = containerSize;
    if (channels==1) {
        dataFormat.channelMask = SL_SPEAKER_FRONT_CENTER;
    }
    else if (channels==2) {
        dataFormat.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
    }
    dataFormat.endianness = SL_BYTEORDER_LITTLEENDIAN;

    SLDataSource dataSource;
    dataSource.pLocator = &dataLocatorIn;
    dataSource.pFormat = &dataFormat;

    SLDataLocator_OutputMix dataLocatorOut;
    dataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
    dataLocatorOut.outputMix = Engine::Get().outputMixObj<SLObjectItf>();

    SLDataSink dataSink;
    dataSink.pLocator = &dataLocatorOut;
    dataSink.pFormat = nullptr;

    const SLuint32 soundPlayerIIDCount = 2;
    const SLInterfaceID soundPlayerIIDs[] = {
        SL_IID_PLAY,
        SL_IID_BUFFERQUEUE
    };
    const SLboolean soundPlayerReqs[] = {
        SL_BOOLEAN_TRUE,
        SL_BOOLEAN_TRUE
    };

    SLObjectItf  playerObj;
    SLPlayItf player;
    SLBufferQueueItf bufferQueueObj;
    bg::base::AudioException except("Error initializing OpenSL ES sound source.");

    SLEngineItf engine = Engine::Get().engine<SLEngineItf>();
    bg::assert((*engine)->CreateAudioPlayer(engine,
                                            &playerObj,
                                            &dataSource,
                                            &dataSink,
                                            soundPlayerIIDCount,
                                            soundPlayerIIDs,
                                            soundPlayerReqs)==SL_RESULT_SUCCESS,except);

    bg::assert((*playerObj)->Realize(playerObj,SL_BOOLEAN_FALSE)==SL_RESULT_SUCCESS,except);
    bg::assert((*playerObj)->GetInterface(playerObj, SL_IID_PLAY,&player)==SL_RESULT_SUCCESS,except);
    bg::assert((*playerObj)->GetInterface(playerObj, SL_IID_BUFFERQUEUE, &bufferQueueObj)==SL_RESULT_SUCCESS,except);
    bg::assert((*bufferQueueObj)->RegisterCallback(bufferQueueObj,soundCallback,this)==SL_RESULT_SUCCESS,except);
    bg::assert((*player)->SetCallbackEventsMask(player,SL_PLAYEVENT_HEADATEND)==SL_RESULT_SUCCESS,except);


    _buffer = buffer;
    _numChannels = channels;
    _samplesPerSec = samplesPerSec;
    _bitsPerSample = bitsPerSample;
    _containerSize = containerSize;
    _playerObj = reinterpret_cast<ObjectImpl>(playerObj);
    _player = reinterpret_cast<PlayImpl>(player);
    _bufferQueueObj = reinterpret_cast<BufferQueueImpl>(bufferQueueObj);
#endif
}

Sound::PlayState Sound::playState() const {
#if BG2E_OPENSLES_AVAILABLE==1
    SLPlayItf player = reinterpret_cast<SLPlayItf>(_player);
    SLuint32 playerState;
    (*player)->GetPlayState(player, &playerState);
    if (playerState==SL_PLAYSTATE_PLAYING) {
        return kStatePlaying;
    }
    else if (playerState==SL_PLAYSTATE_PAUSED) {
        return kStatePaused;
    }
    else {
        return kStateStop;
    }
#else
	return kStateStop;
#endif
}

void Sound::play() {
#if BG2E_OPENSLES_AVAILABLE==1
    if (!_buffer.valid() || !_buffer->valid()) {
        throw bg::base::AudioException("Error playing sound: invalid sound buffer.");
    }

    if (!_playerObj) {
        throw bg::base::AudioException("Error playing sound: the player object is not initialized.");
    }
    SLPlayItf player = reinterpret_cast<SLPlayItf>(_player);
    SLBufferQueueItf queue = reinterpret_cast<SLBufferQueueItf>(_bufferQueueObj);

    switch (playState()) {
    case kStateStop: {
        int16_t *buffer = reinterpret_cast<int16_t *>(_buffer->buffer());
        off_t length = _buffer->size();
        (*player)->SetPlayState(player,SL_PLAYSTATE_PLAYING);
        (*queue)->Clear(queue);
        (*queue)->Enqueue(queue,buffer,length);
        break;
    }
    case kStatePlaying:
        break;
    case kStatePaused:
        (*player)->SetPlayState(player,SL_PLAYSTATE_PLAYING);
        break;
    }
#endif
}

void Sound::pause() {
#if BG2E_OPENSLES_AVAILABLE==1
    if (playState()==kStatePlaying) {
        SLPlayItf player = reinterpret_cast<SLPlayItf>(_player);
        (*player)->SetPlayState(player,SL_PLAYSTATE_PAUSED);
    }
#endif
}

void Sound::stop() {
#if BG2E_OPENSLES_AVAILABLE==1
    PlayState state = playState();
    if (state==kStatePlaying || state==kStatePaused) {
        SLBufferQueueItf queue = reinterpret_cast<SLBufferQueueItf>(_bufferQueueObj);
        SLPlayItf player = reinterpret_cast<SLPlayItf>(_player);
        (*player)->SetPlayState(player, SL_PLAYSTATE_STOPPED);
        (*queue)->Clear(queue);
    }
#endif
}

void Sound::destroy() {
#if BG2E_OPENSLES_AVAILABLE==1
    if (_buffer.valid() && _playerObj) {
        SLPlayItf player = reinterpret_cast<SLPlayItf>(_player);
        SLObjectItf playerObj = reinterpret_cast<SLObjectItf>(_playerObj);
        (*player)->SetPlayState(player,SL_PLAYSTATE_PAUSED);
        (*playerObj)->Destroy(playerObj);
        _player = nullptr;
        _playerObj = nullptr;
        _bufferQueueObj = nullptr;
        _buffer = nullptr;
    }
#endif
}

}
}
}