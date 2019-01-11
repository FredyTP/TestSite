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


#include <bg/db/plugin/wav_audio.hpp>
#include <bg/base/exception.hpp>

#include <fstream>

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#elif BG2E_OPENAL_AVAILABLE==1
#include <AL/al.h>
#include <AL/alc.h>
#endif

namespace bg {
namespace db {
namespace plugin {

namespace wav {

struct RIFF_Header {
	char chunkID[4];
	int chunkSize;
	char format[4];
};

struct WAVE_Format {
	char subChunkID[4];
	int subChunkSize;
	short audioFormat;
	short numChannels;
	int sampleRate;
	int byteRate;
	short blockAlign;
	short bitsPerSampe;
};

struct WAVE_Data {
	char subChunkID[4];
	int subChunk2Size;
};

void checkAudioError(const std::string & errorMsg) {
#if BG2E_OPENAL_AVAILABLE==1
	if (alGetError()!=AL_NO_ERROR) {
		throw bg::base::AudioCreateException(errorMsg);
	}
#endif
}

bg::audio::Buffer * loadWavFile(const std::string & filename) {
	std::ifstream soundFile;
	WAVE_Format wave_format;
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	unsigned char * data;
	bg::ptr<bg::audio::Buffer> buffer;
    memset(&wave_format, 0, sizeof(WAVE_Format));

	try {
		soundFile.open(filename, std::ios::binary);
		if (!soundFile.is_open()) {
			throw "Could not load sound file at path " + filename;
		}

		soundFile.read(reinterpret_cast<char*>(&riff_header), sizeof(RIFF_Header));
		if (riff_header.chunkID[0] != 'R' ||
			riff_header.chunkID[1] != 'I' ||
			riff_header.chunkID[2] != 'F' ||
			riff_header.chunkID[3] != 'F' ||
			riff_header.format[0] != 'W' || 
			riff_header.format[1] != 'A' ||
			riff_header.format[2] != 'V' ||
			riff_header.format[3] != 'E')
		{
			throw "Invalid RIFF or WAVE header";
		}

		soundFile.read(reinterpret_cast<char*>(&wave_format), sizeof(WAVE_Format));
		if (wave_format.subChunkID[0] != 'f' ||
			wave_format.subChunkID[1] != 'm' || 
			wave_format.subChunkID[2] != 't' || 
			wave_format.subChunkID[3] != ' ')
		{
			throw "Invalid WAVE format";
		}

		if (wave_format.subChunkSize>16) {
			soundFile.seekg(sizeof(short), std::ios_base::cur);
		}

		char dataChunkMark = '\0';
		bool dataFound = false;
		while (!dataFound) {
			while (dataChunkMark!='d') {
				if(soundFile.eof()) {
					throw "Invalid WAVE file: unexpected EOF found searching wave data.";
				}
				soundFile.read(&dataChunkMark, sizeof(char));
			}
			soundFile.seekg(-1, std::ios_base::cur);
			soundFile.read(reinterpret_cast<char*>(&wave_data), sizeof(WAVE_Data));
			if (wave_data.subChunkID[0] == 'd' &&
				wave_data.subChunkID[1] == 'a' &&
				wave_data.subChunkID[2] == 't' &&
				wave_data.subChunkID[3] == 'a')
			{
				dataFound = true;
			}
			else {
				soundFile.seekg(-static_cast<int>(sizeof(WAVE_Data)) +  1, std::ios_base::cur);	// move to the position behind 'd'
			}
		}

		data = new unsigned char[wave_data.subChunk2Size];

		soundFile.read(reinterpret_cast<char*>(data), wave_data.subChunk2Size);
		if (!soundFile) {
			throw "Error reading data buffer. Unexpected EOF found.";
		}

		int size = wave_data.subChunk2Size;
		int sampleRate = wave_format.sampleRate;
		bg::audio::Buffer::Format format;
		if (wave_format.numChannels == 1 && wave_format.bitsPerSampe == 8) {
			format = bg::audio::Buffer::kFormatMono8;
		}
		else if(wave_format.numChannels == 1 && wave_format.bitsPerSampe == 16) {
			format = bg::audio::Buffer::kFormatMono16;
		}
		else if(wave_format.numChannels == 2 && wave_format.bitsPerSampe == 8) {
			format = bg::audio::Buffer::kFormatStereo8;
		}
		else if(wave_format.numChannels == 2 && wave_format.bitsPerSampe == 16) {
			format = bg::audio::Buffer::kFormatStereo16;
		}
        else {
            format = bg::audio::Buffer::kFormatUninitialized;
        }

		buffer = new bg::audio::Buffer();
		buffer->create();
		buffer->setBufferData(data, size, format, sampleRate);

		soundFile.close();
	}
	catch (std::string & err) {
		if (soundFile.is_open()) {
			soundFile.close();
		}
		throw bg::base::AudioCreateException(err);
	}
	catch (bg::base::AudioException & ex) {
		throw ex;
	}

	return buffer.release();
}

}

ReadWavAudio::ReadWavAudio()
{
}

ReadWavAudio::~ReadWavAudio() {
}

bool ReadWavAudio::supportFileType(const std::string & extension) {
	return extension=="wav";
}

bg::audio::Buffer * ReadWavAudio::load(bg::audio::Context *, const bg::system::Path & path) {
	bg::ptr<bg::audio::Buffer> buffer = wav::loadWavFile(path.text());
	if (buffer.valid()) {
		buffer->setFileName(path.text());
	}
	return buffer.release();
}

}
}
}
