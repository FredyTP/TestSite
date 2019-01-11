
#import <Cocoa/Cocoa.h>

#include <bg/wnd/message_box.hpp>

#include <iostream>

namespace bg {
namespace wnd {

MessageBox::Response MessageBox::Show(Window * parentModal, const std::string & title, const std::string & message, StandardButton standardButtons) {
	@autoreleasepool {
		NSAlert * alert = [NSAlert new];
		[alert setMessageText:[NSString stringWithUTF8String:title.c_str()]];
		[alert setInformativeText:[NSString stringWithUTF8String:message.c_str()]];
		switch (standardButtons) {
			case kButtonOk:
				[alert addButtonWithTitle:@"Ok"];
				[alert runModal];
				return kResponseOk;
			case kButtonOkCancel:
				[alert addButtonWithTitle:@"Ok"];
				[alert addButtonWithTitle:@"Cancel"];
				return ([alert runModal]==NSAlertFirstButtonReturn) ? kResponseOk:kResponseCancel;
			case kButtonYesNo:
				[alert addButtonWithTitle:@"Yes"];
				[alert addButtonWithTitle:@"No"];
				return ([alert runModal]==NSAlertFirstButtonReturn) ? kResponseYes:kResponseNo;
				break;
			case kButtonYesNoCancel:
				[alert addButtonWithTitle:@"Yes"];
				[alert addButtonWithTitle:@"No"];
				[alert addButtonWithTitle:@"Cancel"];
				NSModalResponse response = [alert runModal];
				if (response==NSAlertFirstButtonReturn) {
					return kResponseYes;
				}
				else if (response==NSAlertSecondButtonReturn) {
					return kResponseNo;
				}
				else {
					return kResponseCancel;
				}
		}
	}
	
	return kResponseCancel;
}

}
}