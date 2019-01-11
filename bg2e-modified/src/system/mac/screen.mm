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

#import <Cocoa/Cocoa.h>


namespace bg {
namespace system {
	
std::vector<Screen> Screen::s_screens;

const std::vector<Screen> Screen::ScreenList() {
    s_screens.clear();
    
    NSArray * screenList = [NSScreen screens];
    NSScreen * mainScreen = [NSScreen mainScreen];
    int index = 1;
    for (NSScreen * scr in screenList) {
        NSSize size = scr.frame.size;
        float scale = scr.backingScaleFactor;
        Screen s(static_cast<uint64_t>(scr.hash),"screen_" + std::to_string(index++),
                 bg::math::Size2Di(static_cast<int>(size.width),
                                   static_cast<int>(size.height)),
                 scr==mainScreen, scale);
        if (scr==mainScreen) {
            s_screens.insert(s_screens.begin(), s);
        }
        else {
            s_screens.push_back(s);
        }
    }
    return s_screens;
}

}
}
