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

#ifndef _bg2e_base_poly_list_impl_hpp_
#define _bg2e_base_poly_list_impl_hpp_

#include <bg/base/referenced_pointer.hpp>
#include <bg/base/context.hpp>

namespace bg {

namespace base {
	
class PolyList;

}

namespace engine {

class BG2E_EXPORT PolyListImpl : public bg::base::ReferencedPointer {
public:
	PolyListImpl(bg::base::PolyList *);
	
	virtual void build() = 0;
	virtual void draw() = 0;
	virtual void destroy() = 0;
	
protected:
	virtual ~PolyListImpl();
	
	bg::base::PolyList * _plist;
	
	inline bg::base::PolyList * plist() { return _plist; }
	
	bg::base::Context * ctx();
};

}
}

#endif