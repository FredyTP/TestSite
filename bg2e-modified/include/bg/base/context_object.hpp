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


#ifndef _bg2e_base_context_object_hpp_
#define _bg2e_base_context_object_hpp_


#include <bg/base/referenced_pointer.hpp>
#include <bg/base/context.hpp>
#include <bg/base/exception.hpp>

#if BG2E_ANDROID==1
// this std::type_info foward declaration is required in Android to compile
namespace std { class type_info; }
#endif

#include <typeinfo>
#include <exception>

namespace bg {
namespace base {

class ContextObject {
public:
	ContextObject(Context * context) :_context(context) { }
	
	static Context * GetContext(ContextObject * obj) { return obj ? obj->context():nullptr; }
	
	inline Context * context() { return _context; }
	inline const Context * context() const { return _context; }
	
	inline void setContext(Context * c) { _context = c; }

protected:
	Context * _context;
};

typedef std::unordered_map<size_t, ptr<ContextObject> > ContextObjectMap;

template <class T>
class ContextObjectRegistry {
public:
	inline T * get(Context * ctx) { return _objects[typeid(ctx).hash_code()].getPtr(); }
	
	inline void set(T * obj) { _objects[typeid(obj->context()).hash_code()] = obj; }
	
	inline bool find(Context * ctx) { return _objects.find(typeid(ctx).hash_code()) != _objects.end(); }
	inline bool find(T * obj) { return _objects.find(typeid(obj->context()).hash_code()) != _objects.end(); }
	
	inline void remove(Context * ctx) {
		typename std::unordered_map<size_t, ptr<T> >::iterator it = _objects.find(typeid(ctx).hash_code());
		if (it!=_objects.end()) {
			_objects.erase(it);
		}
	}
	inline void remove(T * obj) {
		typename std::unordered_map<size_t, ptr<T> >::iterator it = _objects.find(typeid(obj->context()).hash_code());
		if (it!=_objects.end()) {
			_objects.erase(it);
		}
	}
	
	inline void clear() {
		_objects.clear();
	}
	
protected:
	std::unordered_map<size_t, ptr<T> > _objects;
};

}
}

#endif
