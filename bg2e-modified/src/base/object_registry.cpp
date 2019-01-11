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



#include <bg/base/object_registry.hpp>
#include <bg/base/exception.hpp>

#include <iostream>

namespace bg {
namespace base {

UniqueIdentifiedObject::UniqueIdentifiedObject()
{
	
}

UniqueIdentifiedObject::~UniqueIdentifiedObject() {
	ObjectRegistry::Get().unregisterObject(this);
	_id = "";
}
	
void UniqueIdentifiedObject::setId(const std::string & id) {
	if (id!=_id) {	// Avoid rename the object using the same name (optimization)
		if (id.empty()) {
			ObjectRegistry::Get().unregisterObject(this);
		}
		else {
			ObjectRegistry::Get().registerObject(id, this);
		}
		_id = id;
	}
}

ObjectRegistry * ObjectRegistry::s_objectRegistry = nullptr;

ObjectRegistry::ObjectRegistry()
	:_validIdentifier(std::regex("^[a-zA-Z_][a-zA-Z0-9_-]*$"))
{
	
}

ObjectRegistry::~ObjectRegistry() {
	
}

void ObjectRegistry::clear() {
	_registry.clear();
}

void ObjectRegistry::registerObject(const std::string & newId, UniqueIdentifiedObject * obj) {
	
	// Assert id is not empty and valid
	if (newId.empty() || !isValid(newId)) throw bg::base::InvalidUniqueIdentifierException();
	
	// Assert obj is not null
	else if (!obj) throw bg::base::InvalidParameterException();
	
	// Assert newId is not the same as object id
	else if (obj->id()==newId) std::cerr << "PERFORMANCE WARNING: trying to register the same unique identified object twice." << std::endl;
	
	// Assert the object id is not duplicated
	else if (findObject(newId) != nullptr) throw bg::base::DuplicatedUniqueIdentifierException();
	
	// newId is valid and is not empty
	// obj is not null
	// newId is not duplicated
	// obj may be registered, but using a different id than newId
	else {
		unregisterObject(obj);
		_registry[newId] = obj;
	}
}

void ObjectRegistry::unregisterObject(UniqueIdentifiedObject * obj) {
	if (!obj) throw bg::base::InvalidParameterException("ObjectRegistry::unregisterObject(): the UniqueIdentifiedObject specified is null");
	auto objIt = _registry.find(obj->id());
	if (objIt!=_registry.end()) {
		_registry.erase(objIt);
	}
}
	
}
}
