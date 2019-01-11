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

#ifndef _bg2e_base_buffer_hpp_
#define _bg2e_base_buffer_hpp_

#include <bg/base/referenced_pointer.hpp>

namespace bg {
namespace base {

class BG2E_EXPORT Buffer : public ReferencedPointer {
public:
    Buffer();
    Buffer(unsigned char * buffer, size_t size);
    Buffer(char * buffer, size_t size);

    inline bool valid() const { return _buffer!=nullptr; }
    inline bool isNull() const { return _buffer==nullptr; }
    inline size_t size() const { return _size; }
    inline const char * buffer() const { return _buffer; }
    inline char * buffer() { return _buffer; }

    void setBuffer(unsigned char * buffer, size_t size);
    void setBuffer(char * buffer, size_t size);
    void destroy();

protected:
    virtual ~Buffer();

    char * _buffer;
    size_t _size;
};

}
}
#endif
