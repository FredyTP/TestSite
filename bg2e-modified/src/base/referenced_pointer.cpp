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


#include <bg/base/referenced_pointer.hpp>

#include <iostream>
#include <vector>

#define DEBUG_MEMORY_LEAKS !BG2E_ANDROID

namespace bg {
namespace base {

class ReferencedPointerManager {
public:
#if DEBUG_MEMORY_LEAKS==1
	ReferencedPointerManager() {
		
	}
#else
	ReferencedPointerManager() :_references(0) {
		
	}
#endif
	
	virtual ~ReferencedPointerManager() {
		checkReferences();
	}
	
#if DEBUG_MEMORY_LEAKS==1
	void addReference(ReferencedPointer * ptr) {
		_references.push_back(ptr);
	}
	
	void removeReference(ReferencedPointer * ptr) {
		std::vector<ReferencedPointer*>::iterator it = std::find(_references.begin(), _references.end(), ptr);
		if (it!=_references.end()) {
			_references.erase(it);
		}
	}
#else
	void addReference(ReferencedPointer * ptr) {
		++_references;
	}
	
	void removeReference(ReferencedPointer * ptr) {
		--_references;
	}
#endif
	
	void checkReferences() {

#if DEBUG_MEMORY_LEAKS==1
		if (_references.size()!=0) std::cerr << "WARNING: Possible memory leak. " << _references.size() << " undeleted references found." << std::endl;
#else
		if (_references>0) std::cerr << "WARNING: Possible memory leak. " << _references << " undeleted references found." << std::endl;
#endif
	}
	
protected:
#if DEBUG_MEMORY_LEAKS==1
	std::vector<ReferencedPointer*> _references;
#else
	int _references;
#endif
};

#if DEBUG_MEMORY_LEAKS==1
static ReferencedPointerManager s_referencedPointerManager;
#endif

ReferencedPointer::ReferencedPointer() :_ref(0) {
#if DEBUG_MEMORY_LEAKS==1
	s_referencedPointerManager.addReference(this);
#endif
}
	

ReferencedPointer::~ReferencedPointer() {
    
#if DEBUG_MEMORY_LEAKS==1
	s_referencedPointerManager.removeReference(this);
#endif
    callDeleteClosure();
}
	
void ReferencedPointer::inc_ref() {
	++_ref;
}

void ReferencedPointer::dec_ref() {
	--_ref;
	if (_ref<=0) delete this;
}

void ReferencedPointer::dec_ref_nodelete() {
	--_ref;
}

void ReferencedPointer::callDeleteClosure() {
    for (auto & c : _deleteClosure) {
        if (c.second) {
            c.second(this);
        }
    }
}

}
}

