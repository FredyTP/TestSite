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

#ifndef _bg2e_system_screen_hpp_
#define _bg2e_system_screen_hpp_

#include <bg/math/vector.hpp>

#if BG2E_WINDOWS==1

#ifndef WIN32
#define WIN32
#endif
#include <WinSock2.h>
#include <Windows.h>
#include <ShellScalingApi.h>
#endif


namespace bg {
namespace system {

class BG2E_EXPORT Screen {
public:
	static Screen MainScreen() { return ScreenList().front(); }
	static const std::vector<Screen> ScreenList();

	Screen() :_id(-1), _scale(1.0f) {}
	Screen(int64_t id, const std::string & name, const bg::math::Size2Di & size, bool primary, float scale) :_id(id), _name(name), _size(size), _primary(primary), _scale(scale) {}

	inline int64_t id() const { return _id; }
	inline const std::string & name() const { return _name; }
	inline const bg::math::Size2Di & size() const { return _size; }
	inline bool primary() const { return _primary; }
	inline float scale() const { return _scale; }

	inline void operator=(const Screen & other) {
		_id = other._id;
		_name = other._name;
		_size = other._size;
		_primary = other._primary;
		_scale = other._scale;
	}
	inline bool operator==(const Screen & other) { return _id == other._id; }

protected:
	int64_t _id;
	std::string _name;
	bg::math::Size2Di _size;
	bool _primary;
	float _scale;

	static std::vector<Screen> s_screens;

#if BG2E_WINDOWS==1
	static BOOL CALLBACK MonitorEnumProc(
		_In_ HMONITOR hMonitor,
		_In_ HDC      hdcMonitor,
		_In_ LPRECT   lprcMonitor,
		_In_ LPARAM   dwData
	);

public:
	static float GetHMonitorScale(HMONITOR monit);
#endif
};

}
}

#endif
