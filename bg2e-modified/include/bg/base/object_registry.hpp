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



#ifndef _bg2e_base_object_registry_hpp_
#define _bg2e_base_object_registry_hpp_

#include <bg/export.hpp>

#include <string>
#include <unordered_map>
#include <regex>

namespace bg {
namespace base {

class BG2E_EXPORT UniqueIdentifiedObject {
public:
	UniqueIdentifiedObject();
	virtual ~UniqueIdentifiedObject();
	
	void setId(const std::string & id);
	inline const std::string & id() const { return _id; }
	
protected:
	std::string _id = "";
};

class BG2E_EXPORT ObjectRegistry {
	friend class UniqueIdentifiedObject;
public:
	static ObjectRegistry & Get() {
		if (!s_objectRegistry) {
			s_objectRegistry = new ObjectRegistry();
		}
		return *s_objectRegistry;
	}
	
	inline void setValidIdentifierExpression(const std::string & exp) { setValidIdentifierExpression(std::regex(exp)); }
	inline void setValidIdentifierExpression(const std::regex & exp) { _validIdentifier = exp; }

	inline bool isValid(const std::string & id) { return std::regex_match(id, _validIdentifier); }

	void clear();
	
	
	inline UniqueIdentifiedObject * findObject(const std::string & id) {
		if (_registry.find(id)!=_registry.end()) {
			return _registry[id];
		}
		else {
			return nullptr;
		}
	}
	
	template <class T>
	inline T * findObjectOfType(const std::string & id) {
		return dynamic_cast<T*>(findObject(id));
	}
	
protected:
	ObjectRegistry();
	~ObjectRegistry();
	
	void registerObject(const std::string & newId, UniqueIdentifiedObject * obj);
	void unregisterObject(UniqueIdentifiedObject * obj);
	
	std::unordered_map<std::string, UniqueIdentifiedObject*> _objects;
	
	static ObjectRegistry * s_objectRegistry;
	std::regex _validIdentifier;
	std::unordered_map<std::string, UniqueIdentifiedObject*> _registry;
};


}
}

#endif
