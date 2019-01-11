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

#ifndef _bg2e_lua_function_hpp_
#define _bg2e_lua_function_hpp_

#include <bg/lua/base.hpp>

namespace bg {
namespace lua {

template <int N, class Ret, class...Args>
class Function : public BaseFunction {
private:
	using FunctionType = std::function<Ret(Args...)>;

public:
	Function(LuaState state, const std::string & name, Ret(*fun)(Args...)) :Function(state,name,FunctionType{fun}) {}
	Function(const Function & other) = delete;
	Function(Function && other) :_function(other._function), _name(other._name), _state(other._state) { *other._state = nullptr; }
	Function(LuaState state, const std::string & name, FunctionType func)
		:_function(func), _name(name), _state(state)
	{
		pushLightUserData(_state, (void*)static_cast<BaseFunction*>(this));
		pushClosure(_state, &functionDispatcher, 1);
		setGlobal(_state, name);
	}
	~Function() {
		if (_state!=nullptr) {
			pushNil(_state);
			setGlobal(_state, _name);
		}
	}

	int apply(LuaState state) {
		std::tuple<Args...> args = getArgs<Args...>(state);
		push(state, lift(_function, args));
		return N;
	}

protected:
	FunctionType _function;
	std::string _name;
	LuaState _state;
};

}
}

#endif
