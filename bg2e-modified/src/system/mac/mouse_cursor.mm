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
 
#include <bg/system/mouse_cursor.hpp>

#include <iostream>

#import <Cocoa/Cocoa.h>
#include <ApplicationServices/ApplicationServices.h>


namespace bg {
namespace system {

NSScreen * currentScreen() {
	NSPoint mouseLoc = [NSEvent mouseLocation];
	NSEnumerator *screenEnum = [[NSScreen screens] objectEnumerator];
	NSScreen * screen;
	while ((screen = [screenEnum nextObject]) && !NSMouseInRect(mouseLoc,[screen frame], NO));
	return screen;
}

void MouseCursor::SetPosition(const bg::math::Position2Di & pos) {
	CGDisplayErr err;
	if ((err = CGWarpMouseCursorPosition(CGPointMake(static_cast<float>(pos.x()), static_cast<float>(pos.y())))) != CGEventNoErr) {
		std::cerr << "Error setting cursor position" << std::endl;
	}
}

bg::math::Position2Di MouseCursor::Position() {
	NSPoint mouseLoc = [NSEvent mouseLocation];
	return bg::math::Position2Di(mouseLoc.x, mouseLoc.y);
}

void MouseCursor::Show() {
	[NSCursor unhide];
}

void MouseCursor::Hide() {
	[NSCursor hide];
}

}
}
