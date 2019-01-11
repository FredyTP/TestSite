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


#ifndef _bg2e_audio_buffer_hpp_
#define _bg2e_audio_buffer_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <string>

namespace bg {
namespace audio {

class BG2E_EXPORT Buffer : public bg::base::ReferencedPointer {
public:
	enum Format {
		kFormatUninitialized = 0,
		kFormatMono8 = 1,
		kFormatStereo8,
		kFormatMono16,
		kFormatStereo16
	};

	Buffer();
	
	void create();
	void setBufferData(unsigned char * data, int size, Format fmt, int sampleRate);
	void destroy();

	inline bool valid() const { return _buffer!=0; }

	inline uint32_t bufferImpl() const { return _buffer; }
	inline int size() const { return _size; }
	inline Format format() const { return _format; }
	inline int sampleRate() const { return _sampleRate; }

	inline void setFileName(const std::string & fileName) { _fileName = fileName; }
	inline const std::string & fileName() const { return _fileName; }

protected:
	virtual ~Buffer();

	uint32_t _buffer = 0;
	int _size = 0;
	Format _format = kFormatUninitialized;
	int _sampleRate = 0;
	std::string _fileName;
};


}
}

#endif