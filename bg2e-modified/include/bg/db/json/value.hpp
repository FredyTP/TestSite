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

#ifndef _bg2e_db_json_value_hpp_
#define _bg2e_db_json_value_hpp_

#include <bg/base/referenced_pointer.hpp>

#include <bg/math/matrix.hpp>
#include <bg/math/vector.hpp>

#include <string>
#include <vector>
#include <map>
#include <functional>


namespace bg {
namespace db {
namespace json {

class BG2E_EXPORT Value : public bg::base::ReferencedPointer {
public:
	enum Type {
		kNull = 0,
		kNumber,
		kString,
		kBool,
		kArray,
		kObject
	};
	
	typedef std::vector<ptr<Value> > ValueArray;
	typedef std::map<std::string, ptr<Value> > ValueMap;
	
	Value() :_type(kNull), _boolVal(false), _numberVal(0.0) {}
	Value(Type t) :_type(t), _boolVal(false), _numberVal(0.0) {}
	Value(bool v) :_type(kBool), _boolVal(v), _numberVal(0.0) {}
	Value(const char * v) :_type(kString), _boolVal(false), _stringVal(v), _numberVal(0.0) {}
	Value(char v) :_type(kString), _boolVal(false), _numberVal(0.0) { _stringVal = v; }
	Value(const std::string & v) :_type(kString), _boolVal(false), _stringVal(v), _numberVal(0.0) {}
	Value(short v) :_type(kNumber), _boolVal(false), _numberVal(static_cast<double>(v)) {}
	Value(int v) :_type(kNumber), _boolVal(false), _numberVal(static_cast<double>(v)) {}
	Value(long v) :_type(kNumber), _boolVal(false), _numberVal(static_cast<double>(v)) {}
	Value(long long v) :_type(kNumber), _boolVal(false), _numberVal(static_cast<double>(v)) {}
	Value(unsigned short v) :_type(kNumber), _boolVal(false), _numberVal(static_cast<double>(v)) {}
	Value(unsigned int v) :_type(kNumber), _boolVal(false), _numberVal(static_cast<double>(v)) {}
	Value(unsigned long v) :_type(kNumber), _boolVal(false), _numberVal(static_cast<double>(v)) {}
	Value(unsigned long long v) :_type(kNumber), _boolVal(false), _numberVal(static_cast<double>(v)) {}
	Value(float v) :_type(kNumber), _boolVal(false), _numberVal(static_cast<double>(v)) {}
	Value(double v) :_type(kNumber), _boolVal(false), _numberVal(v) {}
	Value(const ValueArray & arr) :_type(kArray), _boolVal(false), _numberVal(0.0), _array(arr) {}
	Value(const ValueMap & map) :_type(kObject), _boolVal(false), _numberVal(0.0), _map(map) {}
	
	inline Type type() const { return _type; }
	
	inline bool boolValue() const { return _boolVal; }
	inline const std::string & stringValue() const { return _stringVal; }
	
	inline short shortValue() const { return static_cast<short>(_numberVal); }
	inline int intValue() const { return static_cast<int>(_numberVal); }
	inline long longValue() const { return static_cast<long>(_numberVal); }
	inline long long llongValue() const { return static_cast<long long>(_numberVal); }
	inline unsigned short ushortValue() const { return static_cast<unsigned short>(_numberVal); }
	inline unsigned int uintValue() const { return static_cast<unsigned int>(_numberVal); }
	inline unsigned long ulongValue() const { return static_cast<unsigned long>(_numberVal); }
	inline unsigned long long ullongValue() const { return static_cast<unsigned long long>(_numberVal); }
	inline float floatValue() const { return static_cast<float>(_numberVal); }
	inline double numberValue() const { return _numberVal; }
	
	inline const ValueArray & array() const { return _array; }
	inline ValueArray & array() { return _array; }
	inline const ValueMap & map() const { return _map; }
	inline ValueMap & map() { return _map; }
	
	
	static bool Bool(Value * val, bool def = false);
	static std::string String(Value * val, const std::string & def = "");
	
	static short Short(Value * val, short def = 0);
	static int Int(Value * val, int def = 0);
	static long Long(Value * val, long def = 0);
	static long long LLong(Value * val, long long def = 0);
	static unsigned short UShort(Value * val, unsigned short def = 0);
	static unsigned int UInt(Value * val, unsigned int def = 0);
	static unsigned long ULong(Value * val, unsigned long def = 0);
	static unsigned long long ULLong(Value * val, unsigned long long def = 0);
	static float Float(Value * val, float def = 0.0f);
	static double number(Value * val, double def = 0.0);
	
	static bg::math::Vector2 Vec2(Value * val, const bg::math::Vector2 & def = bg::math::Vector2(0.0f,0.0f));
	static bg::math::Vector3 Vec3(Value * val, const bg::math::Vector3 & def = bg::math::Vector3(0.0f,0.0f,0.0f));
	static bg::math::Vector4 Vec4(Value * val, const bg::math::Vector4 & def = bg::math::Vector4(0.0f,0.0f,0.0f,0.0f));
	static bg::math::Color Color(Value * val, const bg::math::Color & def = bg::math::Color::White());
	static bg::math::Matrix3 Mat3(Value * val, const bg::math::Matrix3 & def = bg::math::Matrix3::Identity());
	static bg::math::Matrix4 Mat4(Value * val, const bg::math::Matrix4 & def = bg::math::Matrix4::Identity());
	static void FloatVector(Value * val, bg::math::FloatVector & result);
	static bg::math::Vector2i Vec2i(Value * val, const bg::math::Vector2i & def = bg::math::Vector2i(0, 0));
	static bg::math::Vector3i Vec3i(Value * val, const bg::math::Vector3i & def = bg::math::Vector3i(0, 0, 0));
	static bg::math::Vector4i Vec4i(Value * val, const bg::math::Vector4i & def = bg::math::Vector4i(0, 0, 0, 0));
	
	
	inline void setValue(bool b) { _type = kBool; _boolVal = b; }
	inline void setValue(const char * v) { _type = kString; _stringVal = v; }
	inline void setValue(char v) { _type = kString; _stringVal = v; }
	inline void setValue(const std::string & v) { _type = kString; _stringVal = v; }
	inline void setValue(short v) { _type = kNumber; _numberVal = static_cast<double>(v); }
	inline void setValue(int v) { _type = kNumber; _numberVal = static_cast<double>(v); }
	inline void setValue(long v) { _type = kNumber; _numberVal = static_cast<double>(v); }
	inline void setValue(long long v) { _type = kNumber; _numberVal = static_cast<double>(v); }
	inline void setValue(unsigned short v) { _type = kNumber; _numberVal = static_cast<double>(v); }
	inline void setValue(unsigned int v) { _type = kNumber; _numberVal = static_cast<double>(v); }
	inline void setValue(unsigned long v) { _type = kNumber; _numberVal = static_cast<double>(v); }
	inline void setValue(unsigned long long v) { _type = kNumber; _numberVal = static_cast<double>(v); }
	inline void setValue(float v) { _type = kNumber; _numberVal = static_cast<double>(v); }
	inline void setValue(double v) { _type = kNumber; _numberVal = v; }
	inline void setValue(const ValueArray & v) { _type = kArray; _array = v; }
	inline void setValue(const ValueMap & v) { _type = kObject; _map = v; }
	
	// Specific array functions
	inline void push(bool v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(const char * v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(char v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(const std::string & v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(short v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(int v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(long v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(long long v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(unsigned short v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(unsigned int v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(unsigned long v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(unsigned long long v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(float v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(double v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(const ValueArray & v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(const ValueMap & v) { _type = kArray; _array.push_back(new Value(v)); }
	inline void push(Value * val) { _type = kArray; _array.push_back(val ? val:new Value(kNull)); }
	
	// Specific object functions
	inline void setValue(const std::string & key, bool v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, const char * v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, char v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, const std::string & v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, short v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, int v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, long v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, long long v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, unsigned short v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, unsigned int v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, unsigned long v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, unsigned long long v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, float v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, double v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, const ValueArray & v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, const ValueMap & v) { _type = kObject; _map[key] = new Value(v); }
	inline void setValue(const std::string & key, Value * val) { _type = kObject; _map[key] = val ? val:new Value(kNull); }
	inline void setValue(const std::string & key, const bg::math::Vector2 & v) {
		setValue(key, ValueArray{ new Value(v.x()), new Value(v.y()) });
	}

	inline void setValue(const std::string & key, const bg::math::Vector3 & v) {
		setValue(key, ValueArray{ new Value(v.x()), new Value(v.y()), new Value(v.z()) });
	}

	inline void setValue(const std::string & key, const bg::math::Vector4 & v) {
		setValue(key, ValueArray{ new Value(v.x()), new Value(v.y()), new Value(v.z()), new Value(v.w()) });
	}

	inline void setValue(const std::string & key, const bg::math::Vector2i & v) {
		setValue(key, ValueArray{ new Value(v.x()), new Value(v.y()) });
	}

	inline void setValue(const std::string & key, const bg::math::Vector3i & v) {
		setValue(key, ValueArray{ new Value(v.x()), new Value(v.y()), new Value(v.z()) });
	}

	inline void setValue(const std::string & key, const bg::math::Vector4i & v) {
		setValue(key, ValueArray{ new Value(v.x()), new Value(v.y()), new Value(v.z()), new Value(v.w()) });
	}

	inline void setValue(const std::string & key, const bg::math::Matrix3 & v) {
		setValue(key, ValueArray{
			new Value(v.get00()), new Value(v.get01()), new Value(v.get02()),
			new Value(v.get10()), new Value(v.get11()), new Value(v.get12()),
			new Value(v.get20()), new Value(v.get21()), new Value(v.get22())
		});
	}

	inline void setValue(const std::string & key, const bg::math::Matrix4 & v) {
		setValue(key, ValueArray{
			new Value(v.get00()), new Value(v.get01()), new Value(v.get02()), new Value(v.get03()),
			new Value(v.get10()), new Value(v.get11()), new Value(v.get12()), new Value(v.get13()),
			new Value(v.get20()), new Value(v.get21()), new Value(v.get22()), new Value(v.get23()),
			new Value(v.get30()), new Value(v.get31()), new Value(v.get32()), new Value(v.get33())
		});
	}
	
	inline void setValue(const std::string & key, const bg::math::FloatVector & values) {
		ValueArray array;
		for (auto v : values) {
			array.push_back(new Value(v));
		}
		setValue(key,array);
	}
	
	inline Value * operator[](size_t i) { return _array.size()>i ? _array[i].getPtr():nullptr; }
	
	inline Value * operator[](const std::string & key) { return _map[key].getPtr(); }
	
	// Iterate array items
	inline void eachItem(std::function<void (Value *, size_t, const ValueArray &)> lambda) {
		size_t i = 0;
		for (auto item : _array) {
			lambda(item.getPtr(), i++, _array);
		}
	}
	
	inline void eachItem(std::function<void (Value *, size_t)> lambda) {
		size_t i = 0;
		for (auto item : _array) {
			lambda(item.getPtr(), i++);
		}
	}
	
	inline void eachItem(std::function<void (Value *)> lambda) {
		for (auto item : _array) {
			lambda(item.getPtr());
		}
	}
	
	// Iterate object items
	inline void eachItem(std::function<void (const std::string &, Value *, size_t, const ValueMap &)> lambda) {
		size_t i = 0;
		for (auto item : _map) {
			lambda(item.first, item.second.getPtr(), i++, _map);
		}
	}
	
	inline void eachItem(std::function<void (const std::string &, Value *, size_t)> lambda) {
		size_t i = 0;
		for (auto item : _map) {
			lambda(item.first, item.second.getPtr(), i++);
		}
	}
	
	inline void eachItem(std::function<void (const std::string &, Value *)> lambda) {
		for (auto item : _map) {
			lambda(item.first, item.second.getPtr());
		}
	}
	
	void loadFromStream(std::istream & input, bool strict);
	
	void writeToStream(std::ostream & stream);

protected:
	virtual ~Value() {}
	
	Type _type;
	bool _boolVal;
	std::string _stringVal;
	double _numberVal;
	ValueArray _array;
	ValueMap _map;
};

}
}
}

#endif
