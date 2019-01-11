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

#include <bg/system/energy_saver.hpp>
#import <IOKit/pwr_mgt/IOPMLib.h>

namespace bg {
namespace system {

EnergySaver EnergySaver::s_singleton;

void EnergySaver::disableScreenIdleTimer() {
	CFStringRef reasonForActivity = CFSTR("Starting unatended graphics representation");
	IOPMAssertionID assertionId;
	
	IOReturn success = IOPMAssertionCreateWithName(kIOPMAssertionTypeNoDisplaySleep, kIOPMAssertionLevelOn, reasonForActivity, &assertionId);
	if (success==kIOReturnSuccess) {
		_assertionId = assertionId;
		_screenIdleTimerDisabled = true;
	}
}

void EnergySaver::enableScreenIdleTimer() {
	if (_assertionId!=0) {
		IOPMAssertionRelease(_assertionId);
	}
	_screenIdleTimerDisabled = false;
}
	
}
}
