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

#ifndef _bg2e_audio_opensl_sound_hpp_
#define _bg2e_audio_opensl_sound_hpp_

#include <bg/audio/opensl/engine.hpp>
#include <bg/base/buffer.hpp>

namespace bg {
namespace audio {
namespace opensl {

class BG2E_EXPORT Sound : public bg::base::ReferencedPointer {
public:

    enum DataFormat {
        kDataFormatMime         = 0x00000001,
        kDataFormatPCM          = 0x00000002,
        kDataFormatReserved3    = 0x00000003
    };

    enum SampleFormat {
        kSampleFormatFixed8          = 0x0008,
        kSampleFormatFixed16         = 0x0010,
        kSampleFormatFixed20         = 0x0014,
        kSampleFormatFixed24         = 0x0018,
        kSampleFormatFixed28         = 0x001C,
        kSampleFormatFixed32         = 0x0020
    };

    enum SampleRate {
        kSampleRate8000             = 8000000,
        kSampleRate11025            = 11025000,
        kSampleRate12000            = 12000000,
        kSampleRate16000            = 16000000,
        kSampleRate22050            = 22050000,
        kSampleRate24000            = 24000000,
        kSampleRate32000            = 32000000,
        kSampleRate44100            = 44100000,
        kSampleRate48000            = 48000000,
        kSampleRate64000            = 64000000,
        kSampleRate88200            = 88200000,
        kSampleRate96000            = 96000000,
        kSampleRate192000           = 192000000
    };

    enum PlayState {
        kStateStop = 0,
        kStatePlaying = 1,
        kStatePaused = 2
    };

    Sound();

    void create(bg::base::Buffer * buffer, int channels, SampleRate samplesPerSec, SampleFormat bitsPerSample, SampleFormat containerSize);

    PlayState playState() const;

    void play();
    void pause();
    void stop();

    inline void setLoop(bool l) { _loop = l; }
    inline bool loop() const { return _loop; }

protected:
    virtual ~Sound();

    void destroy();

    bg::ptr<bg::base::Buffer> _buffer;
    int _numChannels;
    SampleRate _samplesPerSec;
    SampleFormat _bitsPerSample;
    SampleFormat _containerSize;

	ObjectImpl  _playerObj;
    PlayImpl _player;
    BufferQueueImpl _bufferQueueObj;

    bool _loop;
};

}
}
}

#endif
