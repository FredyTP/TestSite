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

#ifndef _bg2e_android_asset_hpp_
#define _bg2e_android_asset_hpp_

#include <bg/base/referenced_pointer.hpp>

namespace bg {
namespace android {

class BG2E_EXPORT Asset : public bg::base::ReferencedPointer {
public:
    Asset();
    Asset(int32_t desc, off_t start, off_t length);

    inline int32_t descriptor() const { return _descriptor; }
    inline off_t start() const { return _start; }
    inline off_t length() const { return _length; }
    inline void setData(int32_t desc, off_t start, off_t length) {
        _descriptor = desc;
        _start = start;
        _length = length;
    }

    inline bool valid() const { return _descriptor!=-1 && _length!=0; }

    void close();

protected:
    virtual ~Asset();

    int32_t _descriptor;
    off_t _start;
    off_t _length;
};

}
}

#endif
