
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

#include <bg/base/buffer.hpp>

namespace bg {
namespace base {

Buffer::Buffer()
    :_buffer(nullptr)
    ,_size(0)
{
}

Buffer::Buffer(unsigned char * buffer, size_t size)
    :_buffer(reinterpret_cast<char *>(buffer))
    ,_size(size)
{
}

Buffer::Buffer(char * buffer, size_t size)
    :_buffer(buffer)
    ,_size(size)
{
}

Buffer::~Buffer() {
    destroy();
}

void Buffer::setBuffer(unsigned char * buffer, size_t size) {
    destroy();
    _buffer = reinterpret_cast<char *>(buffer);
    _size = size;
}

void Buffer::setBuffer(char * buffer, size_t size) {
    destroy();
    _buffer = buffer;
    _size = size;
}

void Buffer::destroy() {
    if (_buffer) {
        delete [] _buffer;
    }
    _buffer = nullptr;
    _size = 0;
}


}
}