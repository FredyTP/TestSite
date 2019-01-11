
#include <bg/wnd/cocoa_file_dialog.hpp>

#include <Cocoa/Cocoa.h>

#include <bg/system/path.hpp>

namespace bg {
namespace wnd {

CocoaFileDialog::CocoaFileDialog() {
}
	
CocoaFileDialog::~CocoaFileDialog() {
	
}

bool CocoaFileDialog::CocoaFileDialog::show() {
	switch (_type) {
	case kTypeOpenFile:
		return showOpenFileDialog();
	case kTypeSaveFile:
		return showSaveFileDialog();
	case kTypeSelectFolder:
		return showSelectFolderDialog();
	default:
		return false;
	}
}
	
bool CocoaFileDialog::showOpenFileDialog() {
	if (_initPath.empty()) {
		NSString * path = [[NSUserDefaults standardUserDefaults] objectForKey:@"lastOpenDialogPath"];
		if (path) {
			_initPath = path.UTF8String;
		}
		else {
			_initPath = bg::system::Path::AppDir().text();
		}
	}
	NSString * initPath = [NSString stringWithUTF8String:_initPath.c_str()];
	NSOpenPanel * panel = [NSOpenPanel openPanel];
	[panel setAllowsMultipleSelection:NO];
	[panel setCanChooseDirectories:NO];
	[panel setCanChooseFiles:YES];
	[panel setDirectoryURL:[NSURL fileURLWithPath:initPath]];
	
	if (_filters.size()>0) {
		NSMutableArray * filters = [[NSMutableArray alloc] init];
		for (std::string filter : _filters) {
			[filters addObject:[NSString stringWithUTF8String:filter.c_str()]];
		}
		[panel setAllowedFileTypes:filters];
	}
	
	if ([panel runModal]==NSModalResponseOK) {
		_resultPath = [[[panel URL] path] UTF8String];
		bg::system::Path savePath(_resultPath);
		savePath.removeLastComponent();
		[[NSUserDefaults standardUserDefaults] setValue:[NSString stringWithUTF8String:savePath.text().c_str()] forKey:@"lastOpenDialogPath"];
		[[NSUserDefaults standardUserDefaults] synchronize];
		return true;
	}
	return false;
}

bool CocoaFileDialog::showSaveFileDialog() {
	if (_initPath.empty()) {
		NSString * path = [[NSUserDefaults standardUserDefaults] objectForKey:@"lastSaveDialogPath"];
		if (path) {
			_initPath = path.UTF8String;
		}
		else {
			_initPath = bg::system::Path::AppDir().text();
		}
	}
	NSString * initPath = [NSString stringWithUTF8String:_initPath.c_str()];
	NSSavePanel * panel = [NSSavePanel savePanel];

	[panel setDirectoryURL:[NSURL fileURLWithPath:initPath]];
	
	if (_filters.size()>0) {
		NSMutableArray * filters = [[NSMutableArray alloc] init];
		for (std::string filter : _filters) {
			[filters addObject:[NSString stringWithUTF8String:filter.c_str()]];
		}
		[panel setAllowedFileTypes:filters];
	}
	
	if ([panel runModal]==NSModalResponseOK) {
		_resultPath = [[[panel URL] path] UTF8String];
		bg::system::Path savePath(_resultPath);
		savePath.removeLastComponent();
		[[NSUserDefaults standardUserDefaults] setValue:[NSString stringWithUTF8String:savePath.text().c_str()] forKey:@"lastSaveDialogPath"];
		[[NSUserDefaults standardUserDefaults] synchronize];
		return true;
	}
	return false;
}

bool CocoaFileDialog::showSelectFolderDialog() {
	if (_initPath.empty()) {
		NSString * path = [[NSUserDefaults standardUserDefaults] objectForKey:@"lastFolderDialogPath"];
		if (path) {
			_initPath = path.UTF8String;
		}
		else {
			_initPath = bg::system::Path::AppDir().text();
		}
	}
	NSString * initPath = [NSString stringWithUTF8String:_initPath.c_str()];
	NSOpenPanel * panel = [NSOpenPanel openPanel];
	[panel setAllowsMultipleSelection:NO];
	[panel setCanChooseDirectories:YES];
	[panel setCanChooseFiles:NO];
	[panel setDirectoryURL:[NSURL fileURLWithPath:initPath]];
	
	if ([panel runModal]==NSModalResponseOK) {
		_resultPath = [[[panel URL] path] UTF8String];
		[[NSUserDefaults standardUserDefaults] setValue:[panel URL].path forKey:@"lastFolderDialogPath"];
		[[NSUserDefaults standardUserDefaults] synchronize];
		return true;
	}
	return false;
}

}
}
