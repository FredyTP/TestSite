
#include <bg/system/system.hpp>


#ifdef __OBJC__
#if TARGET_IPHONE_SIMULATOR

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#import "NSFileManager+Tar.h"

#elif TARGET_OS_IPHONE

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#else

#import <Cocoa/Cocoa.h>

#endif


namespace bg {
namespace system {
	
std::string uuid() {
	std::string s;
	NSString * uuidString = [[NSUUID UUID] UUIDString];
	s = [uuidString UTF8String];
	return s;
}

}
}

#endif // __OBJC__