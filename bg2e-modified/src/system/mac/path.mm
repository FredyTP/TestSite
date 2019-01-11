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
 
#include <bg/system/path.hpp>


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
	
static std::string s_execPath;
static std::string s_homePath;
static std::string s_resPath;
static std::string s_tempPath;

Path Path::HomeDir() {
	if (s_homePath=="") {
		NSString * home = NSHomeDirectory();
		s_homePath = [home UTF8String];
		if (s_homePath.back()!='/') s_homePath += '/';
	}
	return Path(s_homePath);
}

Path Path::ExecDir() {
	if (s_execPath=="") {
		NSString * executablePath = [[NSBundle mainBundle] executablePath];
		BOOL isDir;
		if ([[NSFileManager defaultManager] fileExistsAtPath:executablePath isDirectory:&isDir]) {
			if (!isDir) {
				executablePath = [executablePath stringByDeletingLastPathComponent];
			}
		}
		else {
			executablePath = @"";
		}
		s_execPath = [executablePath UTF8String];
		if (s_execPath.back()!='/') s_execPath += '/';
	}
	return Path(s_execPath);
}

Path Path::ResourcesDir() {
	if (s_resPath=="") {
		NSString * resourcePath = [[NSBundle mainBundle] resourcePath];
		s_resPath = [resourcePath UTF8String];
		if (s_resPath.back()!='/') s_resPath += '/';
	}
	return Path(s_resPath);
}



Path Path::TempDir() {
	if (s_tempPath=="") {
		NSString * tmp = NSTemporaryDirectory();
		s_tempPath = [tmp UTF8String];
		if (s_tempPath.back()!='/') s_tempPath += '/';
		s_tempPath += "bg2e-tmp/";
	}
	Path p(s_tempPath);
	p.create(true);
	return p;
}

bool Path::exists() const {
	return [[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:_pathString.c_str()]];
}

bool Path::create(bool recursive) const {
	if (isDirectory()) return true;
	
	NSError * err;
	[[NSFileManager defaultManager] createDirectoryAtPath:[NSString stringWithUTF8String:_pathString.c_str()] withIntermediateDirectories:recursive attributes:nil error:&err];
	if (err) {
		return false;
	}
	return true;
}

bool Path::remove() const {
	if (exists()) {
		NSError * err;
		[[NSFileManager defaultManager] removeItemAtPath:[NSString stringWithUTF8String:_pathString.c_str()] error:&err];
		return err!=nil;
	}
	return false;
}
	
bool Path::copy(const Path & dst) const {
	NSError * err;
	[[NSFileManager defaultManager] copyItemAtPath:[NSString stringWithUTF8String:_pathString.c_str()] toPath:[NSString stringWithUTF8String:dst.text().c_str()] error:&err];
	if (err) {
		return false;
	}
	return true;
}

bool Path::isFile() const {
	BOOL isDir = NO;
	bool exists = [[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:_pathString.c_str()] isDirectory:&isDir];
	return exists && !isDir;
}

bool Path::isDirectory() const {
	BOOL isDir = NO;
	bool exists = [[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:_pathString.c_str()] isDirectory:&isDir];
	return exists && isDir;
}

void Path::listDirectory(const std::string & path, std::vector<std::string> & files, std::vector<std::string> & directories, bool clearVectors) const {
	Path p(path);
	if (!p.isDirectory()) return;
	
	if (clearVectors) {
		files.clear();
		directories.clear();
	}
	NSString * nspath = [NSString stringWithUTF8String:path.c_str()];
	NSArray * contents = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:nspath error:nil];
	BOOL isDir;
	for (NSString * item in contents) {
		NSString * fileFullPath = [nspath stringByAppendingPathComponent:item];
		[[NSFileManager defaultManager] fileExistsAtPath:fileFullPath isDirectory:&isDir];
		if (isDir) {
			directories.push_back(item.UTF8String);
		}
		else {
			files.push_back(item.UTF8String);
		}
	}
}

}
}
#endif // __OBJC__
