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

#include <bg/system/screen.hpp>
#include <iostream>



namespace bg {
namespace system {

std::vector<Screen> Screen::s_screens;

BOOL CALLBACK Screen::MonitorEnumProc(
	_In_ HMONITOR hMonitor,
	_In_ HDC      hdcMonitor,
	_In_ LPRECT   lprcMonitor,
	_In_ LPARAM   dwData
) {
	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor,&mi);

	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(hMonitor, &miex);

	bool primary = mi.rcMonitor.left==0 && mi.rcMonitor.top==0;

	float scale = GetHMonitorScale(hMonitor);
	
	Screen s(reinterpret_cast<int64_t>(hMonitor), miex.szDevice,
		bg::math::Size2Di(mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top),
		primary,scale);
	if (primary) {
		s_screens.insert(s_screens.begin(),s);
	}
	else {
		s_screens.push_back(s);
	}
	return true;
}

float Screen::GetHMonitorScale(HMONITOR monit) {
	uint32_t dpix = 0, dpiy = 0;
	GetDpiForMonitor(monit, MDT_EFFECTIVE_DPI, &dpix, &dpiy);

	float scale = 1.0f;
	if(dpix <= 96) {
		scale = 1.0f;
	}
	else if(dpix <= 120) {
		scale = 1.2f;
	}
	else if(dpix <= 144) {
		scale = 1.5f;
	}
	else if(dpix <= 168) {
		scale = 1.75f;
	}
	else if(dpix <= 192) {
		scale = 2.0f;
	}
	else {
		scale = 2.25f;
	}

	return scale;
}

const std::vector<Screen> Screen::ScreenList() {
	// Create the list each time it's called, to update
	// the screen scale.
	s_screens.clear();
	EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, 0);
	return s_screens;
}

}
}