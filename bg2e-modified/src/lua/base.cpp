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


#include <bg/lua/base.hpp>

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

}

namespace bg {
namespace lua {

LuaState newState() {
	return luaL_newstate();
}

void closeState(LuaState state) {
	lua_close(reinterpret_cast<lua_State*>(state));
}

bool openFile(LuaState state, const std::string & file) {
	return luaL_dofile(reinterpret_cast<lua_State*>(state), file.c_str()) == 0;
}

bool loadString(LuaState state, const std::string & script) {
	return (luaL_loadstring(reinterpret_cast<lua_State*>(state), script.c_str()) == 0) &&
			(lua_pcall(reinterpret_cast<lua_State*>(state), 0, 0, 0) == 0);
}

void pushNil(LuaState & state) {
	lua_pushnil(reinterpret_cast<lua_State*>(state));
}

void push(LuaState state, bool v) {
	lua_pushboolean(reinterpret_cast<lua_State*>(state), v);
}

void push(LuaState state, short v) {
	lua_pushinteger(reinterpret_cast<lua_State*>(state), static_cast<lua_Integer>(v));
}

void push(LuaState state, int v) {
	lua_pushinteger(reinterpret_cast<lua_State*>(state), static_cast<lua_Integer>(v));
}

void push(LuaState state, long v) {
	lua_pushinteger(reinterpret_cast<lua_State*>(state), static_cast<lua_Integer>(v));
}

void push(LuaState state, long long v) {
	lua_pushinteger(reinterpret_cast<lua_State*>(state), static_cast<lua_Integer>(v));
}

void push(LuaState state, unsigned short v) {
	lua_pushinteger(reinterpret_cast<lua_State*>(state), static_cast<lua_Integer>(v));
}

void push(LuaState state, unsigned int v) {
	lua_pushinteger(reinterpret_cast<lua_State*>(state), static_cast<lua_Integer>(v));
}

void push(LuaState state, unsigned long v) {
	lua_pushinteger(reinterpret_cast<lua_State*>(state), static_cast<lua_Integer>(v));
}

void push(LuaState state, unsigned long long v) {
	lua_pushinteger(reinterpret_cast<lua_State*>(state), static_cast<lua_Integer>(v));
}

void push(LuaState state, float v) {
	lua_pushnumber(reinterpret_cast<lua_State*>(state), static_cast<lua_Number>(v));
}

void push(LuaState state, double v) {
	lua_pushnumber(reinterpret_cast<lua_State*>(state), static_cast<lua_Number>(v));
}

void push(LuaState state, char v) {
	std::string str = std::to_string(v);
	lua_pushstring(reinterpret_cast<lua_State*>(state), str.c_str());
}

void push(LuaState state, std::string & v) {
	lua_pushstring(reinterpret_cast<lua_State*>(state), v.c_str());
}

void push(LuaState state, const std::string & v) {
	lua_pushstring(reinterpret_cast<lua_State*>(state), v.c_str());
}


bool readBool(LuaState state, int index) {
	return lua_toboolean(reinterpret_cast<lua_State*>(state), index) != 0;
}

short readShort(LuaState state, int index) {
	return static_cast<short>(lua_tointeger(reinterpret_cast<lua_State*>(state), index));
}

int readInt(LuaState state, int index) {
	return static_cast<int>(lua_tointeger(reinterpret_cast<lua_State*>(state), index));
}

long readLong(LuaState state, int index) {
	return static_cast<long>(lua_tointeger(reinterpret_cast<lua_State*>(state), index));
}

long long readLongLong(LuaState state, int index) {
	return static_cast<long long>(lua_tointeger(reinterpret_cast<lua_State*>(state), index));
}

unsigned short readUShort(LuaState state, int index) {
	return static_cast<unsigned short>(lua_tointeger(reinterpret_cast<lua_State*>(state), index));
}

unsigned int readUInt(LuaState state, int index) {
	return static_cast<unsigned int>(lua_tointeger(reinterpret_cast<lua_State*>(state), index));
}

unsigned long readULong(LuaState state, int index) {
	return static_cast<unsigned long>(lua_tointeger(reinterpret_cast<lua_State*>(state), index));
}

unsigned long long readULongLong(LuaState state, int index) {
	return static_cast<unsigned long long>(lua_tointeger(reinterpret_cast<lua_State*>(state), index));
}

float readFloat(LuaState state, int index) {
	return static_cast<float>(lua_tonumber(reinterpret_cast<lua_State*>(state), index));
}

double readDouble(LuaState state, int index) {
	return static_cast<double>(lua_tonumber(reinterpret_cast<lua_State*>(state), index));
}

char readChar(LuaState state, int index) {
	std::string str = lua_tostring(reinterpret_cast<lua_State*>(state), index);
	if (str.size()==0) {
		return '\0';
	}
	else {
		return str.front();
	}
}

const std::string & readString(LuaState state, int index) {
	static std::string s_readString = lua_tostring(reinterpret_cast<lua_State*>(state), index);
	return s_readString;
}


bool readBool(LuaState state, int index, bool & isOk) {
	isOk = true;
	return lua_toboolean(reinterpret_cast<lua_State*>(state), index) != 0;
}

short readShort(LuaState state, int index, bool & isOk) {
	int isNum = 0;
	auto result = static_cast<short>(lua_tointegerx(reinterpret_cast<lua_State*>(state), index, &isNum));
	isOk = isNum!=0;
	return result;
}

int readInt(LuaState state, int index, bool & isOk) {
	int isNum = 0;
	auto result = static_cast<int>(lua_tointegerx(reinterpret_cast<lua_State*>(state), index, &isNum));
	isOk = isNum != 0;
	return result;
}

long readLong(LuaState state, int index, bool & isOk) {
	int isNum = 0;
	auto result = static_cast<long>(lua_tointegerx(reinterpret_cast<lua_State*>(state), index, &isNum));
	isOk = isNum != 0;
	return result;
}

long long readLongLong(LuaState state, int index, bool & isOk) {
	int isNum = 0;
	auto result = static_cast<long long>(lua_tointegerx(reinterpret_cast<lua_State*>(state), index, &isNum));
	isOk = isNum != 0;
	return result;
}

unsigned short readUShort(LuaState state, int index, bool & isOk) {
	int isNum = 0;
	auto result = static_cast<unsigned short>(lua_tointegerx(reinterpret_cast<lua_State*>(state), index, &isNum));
	isOk = isNum != 0;
	return result;
}

unsigned int readUInt(LuaState state, int index, bool & isOk) {
	int isNum = 0;
	auto result = static_cast<unsigned int>(lua_tointegerx(reinterpret_cast<lua_State*>(state), index, &isNum));
	isOk = isNum != 0;
	return result;
}

unsigned long readULong(LuaState state, int index, bool & isOk) {
	int isNum = 0;
	auto result = static_cast<unsigned long>(lua_tointegerx(reinterpret_cast<lua_State*>(state), index, &isNum));
	isOk = isNum != 0;
	return result;
}

unsigned long long readULongLong(LuaState state, int index, bool & isOk) {
	int isNum = 0;
	auto result = static_cast<unsigned long long>(lua_tointegerx(reinterpret_cast<lua_State*>(state), index, &isNum));
	isOk = isNum != 0;
	return result;
}

float readFloat(LuaState state, int index, bool & isOk) {
	int isNum = 0;
	auto result = static_cast<float>(lua_tonumberx(reinterpret_cast<lua_State*>(state), index, &isNum));
	isOk = isNum != 0;
	return result;
}

double readDouble(LuaState state, int index, bool & isOk) {
	int isNum = 0;
	auto result = static_cast<double>(lua_tonumberx(reinterpret_cast<lua_State*>(state), index, &isNum));
	isOk = isNum != 0;
	return result;
}

char readChar(LuaState state, int index, bool & isOk) {
	std::string result = readString(state,index);
	isOk = result.size()>0;
	return result.front();
}

const std::string & readString(LuaState state, int index, bool & isOk) {
	size_t size = 0;
	char const * buff = lua_tolstring(reinterpret_cast<lua_State*>(state), index, &size);
	isOk = buff!=nullptr;
	static std::string s_readStringResult = std::string{ buff,size };
	return s_readStringResult;
}

void pop(LuaState state, int index) {
	lua_pop(reinterpret_cast<lua_State*>(state), index);
}

void getGlobal(LuaState state, const std::string & name) {
	lua_getglobal(reinterpret_cast<lua_State*>(state), name.c_str());
}

void setGlobal(LuaState state, const std::string & name) {
	lua_setglobal(reinterpret_cast<lua_State*>(state), name.c_str());
}

void call(LuaState state, int numArgs, int numRet) {
	lua_call(reinterpret_cast<lua_State*>(state), numArgs, numRet);
}

void pushLightUserData(LuaState state, void * data) {
	lua_pushlightuserdata(reinterpret_cast<lua_State*>(state), data);
}

void pushClosure(LuaState state, LuaFunction fn, int index) {
	lua_pushcclosure(reinterpret_cast<lua_State*>(state), reinterpret_cast<lua_CFunction>(fn), index);
}

int functionDispatcher(LuaState l) {
	lua_State * state = reinterpret_cast<lua_State*>(l);
	BaseFunction * function = static_cast<BaseFunction*>(lua_touserdata(state, lua_upvalueindex(1)));
	return function->apply(l);
}

}
}