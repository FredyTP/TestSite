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

#ifndef _bg2e_lua_base_hpp_
#define _bg2e_lua_base_hpp_

#include <bg/export.hpp>
#include <bg/base/exception.hpp>

#include <string>
#include <functional>

namespace bg {
namespace lua {

typedef void * LuaState;
typedef int (*LuaFunction)(void*);

extern BG2E_EXPORT LuaState newState();
extern BG2E_EXPORT void closeState(LuaState);
extern BG2E_EXPORT bool openFile(LuaState, const std::string &);
extern BG2E_EXPORT bool loadString(LuaState, const std::string &);

extern BG2E_EXPORT void pushNil(LuaState &);

extern BG2E_EXPORT void push(LuaState, bool v);
extern BG2E_EXPORT void push(LuaState, short v);
extern BG2E_EXPORT void push(LuaState, int v);
extern BG2E_EXPORT void push(LuaState, long v);
extern BG2E_EXPORT void push(LuaState, long long v);
extern BG2E_EXPORT void push(LuaState, unsigned short v);
extern BG2E_EXPORT void push(LuaState, unsigned int v);
extern BG2E_EXPORT void push(LuaState, unsigned long v);
extern BG2E_EXPORT void push(LuaState, unsigned long long v);
extern BG2E_EXPORT void push(LuaState, float v);
extern BG2E_EXPORT void push(LuaState, double v);
extern BG2E_EXPORT void push(LuaState, char v);
extern BG2E_EXPORT void push(LuaState, std::string & v);
extern BG2E_EXPORT void push(LuaState, const std::string & v);

extern BG2E_EXPORT bool readBool(LuaState, int);
extern BG2E_EXPORT short readShort(LuaState, int);
extern BG2E_EXPORT int readInt(LuaState, int);
extern BG2E_EXPORT long readLong(LuaState, int);
extern BG2E_EXPORT long long readLongLong(LuaState, int);
extern BG2E_EXPORT unsigned short readUShort(LuaState, int);
extern BG2E_EXPORT unsigned int readUInt(LuaState, int);
extern BG2E_EXPORT unsigned long readULong(LuaState, int);
extern BG2E_EXPORT unsigned long long readULongLong(LuaState, int);
extern BG2E_EXPORT float readFloat(LuaState, int);
extern BG2E_EXPORT double readDouble(LuaState, int);
extern BG2E_EXPORT char readChar(LuaState, int);
extern BG2E_EXPORT const std::string & readString(LuaState, int);

extern BG2E_EXPORT bool readBool(LuaState, int, bool &);
extern BG2E_EXPORT short readShort(LuaState, int, bool &);
extern BG2E_EXPORT int readInt(LuaState, int, bool &);
extern BG2E_EXPORT long readLong(LuaState, int, bool &);
extern BG2E_EXPORT long long readLongLong(LuaState, int, bool &);
extern BG2E_EXPORT unsigned short readUShort(LuaState, int, bool &);
extern BG2E_EXPORT unsigned int readUInt(LuaState, int, bool &);
extern BG2E_EXPORT unsigned long readULong(LuaState, int, bool &);
extern BG2E_EXPORT unsigned long long readULongLong(LuaState, int, bool &);
extern BG2E_EXPORT float readFloat(LuaState, int, bool &);
extern BG2E_EXPORT double readDouble(LuaState, int, bool &);
extern BG2E_EXPORT char readChar(LuaState, int, bool &);
extern BG2E_EXPORT const std::string & readString(LuaState, int, bool &);

extern BG2E_EXPORT void pop(LuaState, int);
extern BG2E_EXPORT void getGlobal(LuaState, const std::string &);
extern BG2E_EXPORT void setGlobal(LuaState, const std::string &);
extern BG2E_EXPORT void call(LuaState, int numArgs, int numRet);

extern BG2E_EXPORT void pushLightUserData(LuaState, void *);
extern BG2E_EXPORT void pushClosure(LuaState, LuaFunction, int);

template <class T> struct _id {};

template <class T>
inline void checkGet(_id<T>, LuaState l, T t) {
	readInt(l, 0);
}

inline short checkGet(_id<short>, LuaState l, const int index) {
	bool status = true;
	auto res = readShort(l, index, status);
	if(!status) {
		throw bg::base::LuaTypeException();
	}
	return res;
}

inline int checkGet(_id<int>, LuaState l, const int index) {
	bool status = true;
	auto res = readInt(l, index, status);
	if(!status) {
		throw bg::base::LuaTypeException();
	}
	return res;
}

inline long checkGet(_id<long>, LuaState l, const int index) {
	bool status = true;
	auto res = readLong(l, index, status);
	if(!status) {
		throw bg::base::LuaTypeException();
	}
	return res;
}

inline long long checkGet(_id<long long >, LuaState l, const int index) {
	bool status = true;
	auto res = readLongLong(l, index, status);
	if(!status) {
		throw bg::base::LuaTypeException();
	}
	return res;
}

inline unsigned short checkGet(_id<unsigned short>, LuaState l, const int index) {
	bool status = true;
	auto res = readUShort(l, index, status);
	if(!status) {
		throw bg::base::LuaTypeException();
	}
	return res;
}

inline unsigned int checkGet(_id<unsigned int>, LuaState l, const int index) {
	bool status = true;
	auto res = readUInt(l, index, status);
	if(!status) {
		throw bg::base::LuaTypeException();
	}
	return res;
}

inline unsigned long checkGet(_id<unsigned long>, LuaState l, const int index) {
	bool status = true;
	auto res = readULong(l, index, status);
	if(!status) {
		throw bg::base::LuaTypeException();
	}
	return res;
}

inline unsigned long long checkGet(_id<unsigned long long>, LuaState l, const int index) {
	bool status = true;
	auto res = readULongLong(l, index, status);
	if(!status) {
		throw bg::base::LuaTypeException();
	}
	return res;
}

inline float checkGet(_id<float>, LuaState l, const int index) {
	bool status = true;
	auto res = readFloat(l, index, status);
	if(!status) {
		throw bg::base::LuaTypeException();
	}
	return res;
}

inline double checkGet(_id<double>, LuaState l, const int index) {
	bool status = true;
	auto res = readDouble(l, index, status);
	if(!status) {
		throw bg::base::LuaTypeException();
	}
	return res;
}

inline char checkGet(_id<char>, LuaState l, const int index) {
	bool status = true;
	auto res = readChar(l, index, status);
	if(!status) {
		throw bg::base::LuaTypeException();
	}
	return res;
}

inline std::string checkGet(_id<std::string>, LuaState l, const int index) {
	bool status = true;
	auto res = readString(l, index, status);
	if(!status) {
		throw bg::base::LuaTypeException();
	}
	return res;
}

struct BaseFunction {
	virtual ~BaseFunction() {}
	virtual int apply(LuaState) = 0;
};

extern BG2E_EXPORT int functionDispatcher(LuaState l);

template <std::size_t...Is>
struct Indices {};

template <std::size_t N, std::size_t...Is>
struct IndicesBuilder : IndicesBuilder<N-1, N-1, Is...> {};

template <std::size_t...Is>
struct IndicesBuilder<0, Is...> {
	using type = Indices<Is...>;
};

template <class ...T, std::size_t...N>
std::tuple<T...> getArgs(LuaState l, Indices<N...>) {
	return std::make_tuple(checkGet(_id<T>{}, l, N+1)...);
}

template <class...T>
std::tuple<T...> getArgs(LuaState l) {
	constexpr std::size_t numArgs = sizeof...(T);
	return getArgs<T...>(l, typename IndicesBuilder<numArgs>::type());
}

template <class Ret, class...Args, std::size_t...N>
Ret lift(std::function<Ret(Args...)> fn, std::tuple<Args...> args, Indices<N...>) {
	return fn(std::get<N>(args)...);
}

template <class Ret, class...Args>
Ret lift(std::function<Ret(Args...)> fn, std::tuple<Args...> args) {
	return lift(fn, args, typename IndicesBuilder<sizeof...(Args)>::type());
}



}
}

#endif
