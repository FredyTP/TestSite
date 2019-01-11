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


#ifndef _bg2e_base_referenced_pointer_hpp_
#define _bg2e_base_referenced_pointer_hpp_

#include <bg/export.hpp>

#include <vector>
#include <algorithm>
#include <functional>

#include <map>
#include <string>

namespace bg {
namespace base {

class BG2E_EXPORT ReferencedPointer;

}

template <class T> T native_cast(void * nativePtr) { return static_cast<T>(nativePtr); }
typedef void * plain_ptr;



template <class T>
class ptr {
public:
	ptr() :_ptr(nullptr) {}
	ptr(T * p) :_ptr(p) { if (_ptr) _ptr->inc_ref(); }
	ptr(const ptr & p)  :_ptr(p._ptr) { if (_ptr) _ptr->inc_ref(); }
	~ptr() { if (_ptr) _ptr->dec_ref(); _ptr = nullptr; }

	ptr& operator=(const ptr& p) {
		assign(p);
		return *this;
	}
	inline ptr & operator=(T * p) {
		if (_ptr==p) {
			return *this;
		}
		T * tmp_ptr = _ptr;
		_ptr = p;
		if (_ptr) _ptr->inc_ref();
		if (tmp_ptr) tmp_ptr->dec_ref();
		return *this;
	}
	bool operator==(const ptr & p) const { return _ptr==p._ptr; }
	bool operator==(const T * p) const { return _ptr==p; }
	friend bool operator==(const T* p, const ptr & rp) { return p==rp._ptr; }

	bool operator < (const ptr& p) const { return _ptr<p._ptr; }

	T& operator*() const { return *_ptr; }
	T* operator->() const { return _ptr; }

	bool operator!() const { return _ptr==nullptr; }
	bool valid() const { return _ptr!=nullptr; }
	bool isNull() const { return _ptr==nullptr; }
	T * release() { T * tmp=_ptr; if (_ptr) _ptr->dec_ref_nodelete(); _ptr=nullptr; return tmp; }
	void swap(ptr & p) { T * tmp = _ptr; _ptr=p._ptr; p._ptr = tmp; }

	bool operator==(T * p) const { return _ptr==p; }

	T * getPtr() { return _ptr; }
	const T * getPtr() const { return _ptr; }

	T * operator->() { return _ptr; }

protected:
	void assign(const ptr<T>& p) {
		if (_ptr==p._ptr) return;
		T * tmp = _ptr;
		_ptr = p._ptr;
		if (_ptr) _ptr->inc_ref();
		if (tmp) tmp->dec_ref();
	}

	T * _ptr;
};

namespace base {
	
class BG2E_EXPORT ReferencedPointer {
public:
    typedef std::function<void(ReferencedPointer *)> DeleteClosure;
    typedef std::function<void(ReferencedPointer *, int oldRef, int newRef)> ReferencesChangedClosure;
    
	ReferencedPointer();

	void inc_ref();
	void dec_ref();
	void dec_ref_nodelete();

	inline int getReferences() const { return _ref; }
    inline void onDelete(const std::string & id, DeleteClosure c) { _deleteClosure[id] = c; }

protected:
	virtual ~ReferencedPointer();

	int _ref;
    std::map<std::string,DeleteClosure> _deleteClosure;
    
    void callDeleteClosure();
};

}
}

#endif
