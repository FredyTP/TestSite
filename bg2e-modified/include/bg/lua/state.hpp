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


#ifndef _bg2e_lua_state_hpp_
#define _bg2e_lua_state_hpp_

#include <bg/lua/function.hpp>
#include <bg/system/path.hpp>

#include <iostream>
#include <string>
#include <tuple>
#include <memory>
#include <map>

namespace bg {
namespace lua {

class State {
public:
	State() :_luaState(nullptr)
	{
		_luaState = bg::lua::newState();
	}

	State(const State &) = delete;
	State & operator = (const State &) = delete;
	State(State && other) :_luaState(other._luaState)
	{
		other._luaState = nullptr;
	}

	~State() {
		if(_luaState) {
			_functions.clear();
			bg::lua::closeState(_luaState);
			_luaState = nullptr;
		}
	}

	bool open(const bg::system::Path & path) { return load(path.text()); }
	bool open(const std::string & file) {
		return bg::lua::openFile(_luaState, file);
	}
	bool load(const std::string & script) { return bg::lua::loadString(_luaState, script); }
	

	void push() { bg::lua::pushNil(_luaState); }
	void push(bool v) { bg::lua::push(_luaState, v); }
	void push(short v) { bg::lua::push(_luaState, v); }
	void push(int v) { bg::lua::push(_luaState, v); }
	void push(long v) { bg::lua::push(_luaState, v); }
	void push(long long v) { bg::lua::push(_luaState, v); }
	void push(unsigned short v) { bg::lua::push(_luaState, v); }
	void push(unsigned int v) { bg::lua::push(_luaState, v); }
	void push(unsigned long v) { bg::lua::push(_luaState, v); }
	void push(unsigned long long v) { bg::lua::push(_luaState, v); }
	void push(float v) { bg::lua::push(_luaState, v); }
	void push(double v) { bg::lua::push(_luaState, v); }
	void push(char v) { bg::lua::push(_luaState, v); }
	void push(std::string v) { bg::lua::push(_luaState, v); }

	template <class T, class ... Ts>
	void push(const T value, const Ts...values) {
		push(value);
		push(values...);
	}

	template <class T>
	T read(int index) const {
		return 0;
	}

	template <size_t, class ... Ts>
	struct _pop {
		typedef std::tuple<Ts...> type;

		// base case: tuple with one element
		template <class T>
		static std::tuple<T> Pop(const State & s, const int index) {
			return std::make_tuple(s.read<T>(index));
		}

		// inductive case
		template <class T1, class T2, class ... Rest>
		static std::tuple<T1, T2, Rest...> Pop(const State & s, const int index) {
			std::tuple<T1> head = std::make_tuple(s.read<T1>(index));
			return std::tuple_cat(head, Pop<T2, Rest...>(s, index + 1));
		}

		static type Apply(State &s) {
			auto ret = Pop<Ts...>(s, 1);
			s.pop(sizeof...(Ts));
			return ret;
		}
	};

	template <class T>
	struct _pop<1, T> {
		typedef T type;
		static type Apply(State & s) {
			T ret = s.read<T>(-1);
			s.pop(1);
			return ret;
		}
	};

	template <class ...Ts>
	struct _pop<0, Ts...> {
		typedef void type;
		static type Apply(State &s);
	};

	template <class ...T>
	typename _pop<sizeof...(T), T...>::type pop() {
		return _pop<sizeof...(T), T...>::Apply(*this);
	}

	void pop(int index) { bg::lua::pop(_luaState, index); }

	template <class ... Ret, class ... Args>
	typename _pop<sizeof...(Ret), Ret...>::type call(const std::string & fun, const Args&...args) {
		bg::lua::getGlobal(_luaState, fun);
		const int nArgs = sizeof...(Args);
		const int nRet = sizeof...(Ret);

		push(args...);

		bg::lua::call(_luaState,nArgs, nRet);

		return pop<Ret...>();
	}

	template <class Ret, class...Args>
	void registerFunction(const std::string & name, std::function<Ret(Args...)> fn) {
		auto tmp = std::unique_ptr<BaseFunction>{ new Function<1, Ret, Args...>{_luaState, name, fn}};
		_functions.insert(std::make_pair(name, std::move(tmp)));
	}

	template <class Ret, class...Args>
	void registerFunction(const std::string & name, Ret(*fun)(Args...)) {
		auto tmp = std::unique_ptr<BaseFunction>{ new Function<1, Ret, Args...>{ _luaState, name, fun}};
		_functions.insert(std::make_pair(name, std::move(tmp)));
	}

protected:
	LuaState _luaState;
	std::map<std::string, std::unique_ptr<BaseFunction>> _functions;
};

template <>
bool State::read(int index) const {
	return bg::lua::readBool(_luaState, index);
}

template <>
short State::read(int index) const {
	return bg::lua::readShort(_luaState, index);
}

template <>
int State::read(int index) const {
	return bg::lua::readInt(_luaState, index);
}

template <>
long State::read(int index) const {
	return bg::lua::readLong(_luaState, index);
}

template <>
long long State::read(int index) const {
	return bg::lua::readLongLong(_luaState, index);
}

template <>
unsigned short State::read(int index) const {
	return bg::lua::readUShort(_luaState, index);
}

template <>
unsigned int State::read(int index) const {
	return bg::lua::readUInt(_luaState, index);
}

template <>
unsigned long State::read(int index) const {
	return bg::lua::readULong(_luaState, index);
}

template <>
unsigned long long State::read(int index) const {
	return bg::lua::readULongLong(_luaState, index);
}

template <>
float State::read(int index) const {
	return bg::lua::readFloat(_luaState, index);
}

template <>
double State::read(int index) const {
	return bg::lua::readDouble(_luaState, index);
}

template <>
char State::read(int index) const {
	return bg::lua::readChar(_luaState, index);
}

template <>
std::string State::read(int index) const {
	return bg::lua::readString(_luaState, index);
}


}
}
#endif