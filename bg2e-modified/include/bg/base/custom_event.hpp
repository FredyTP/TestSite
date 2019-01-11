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

#ifndef _bg_base_custom_event_hpp_
#define _bg_base_custom_event_hpp_

#include <bg/base/referenced_pointer.hpp>
namespace bg {
namespace base {

class CustomEventData : public ReferencedPointer {
public:
	CustomEventData() {}

protected:
	virtual ~CustomEventData() {}
};

class CustomEvent {
public:
	CustomEvent() {}
	CustomEvent(CustomEventData * d) :_data(d) {}

	inline void operator=(const CustomEvent & e) {
		_data = e._data;
	}
	inline bool operator==(const CustomEvent & e) {
		return _data==e._data;
	}

	inline CustomEventData * data() { return _data.getPtr(); }
	inline const CustomEventData * data() const { return _data.getPtr(); }
	template <class T>
	inline T * data() { return dynamic_cast<T*>(_data.getPtr()); }
	template <class T>
	inline const T * data() const { return dynamic_cast<const T*>(_data.getPtr()); }

protected:
	bg::ptr<CustomEventData> _data;
};

}
}

#endif
