
#include <bg/wnd/main_loop.hpp>
#include <bg/wnd/window_controller.hpp>
#include <bg/wnd/cocoa_app_delegate.h>

#include <Cocoa/Cocoa.h>

uint32_t getCocoaModifiers(const bg::base::KeyboardShortcut & shortcut) {
	using namespace bg::base;
	uint32_t result = 0;
	if (shortcut.modifierMask & Keyboard::kCommandKey) {
		result |= NSEventModifierFlagCommand;
	}
	if (shortcut.modifierMask & Keyboard::kCtrlKey) {
		result |= NSEventModifierFlagControl;
	}
	if (shortcut.modifierMask & Keyboard::kAltKey) {
		result |= NSEventModifierFlagOption;
	}
	if (shortcut.modifierMask & Keyboard::kShiftKey) {
		result |= NSEventModifierFlagShift;
	}
	
	return result;
}

NSString * getCocoaKey(const bg::base::KeyboardShortcut & shortcut) {
	using namespace bg::base;
	std::string character;
	character = static_cast<char>(shortcut.keyCode + 32);
	return [NSString stringWithUTF8String:character.c_str()];
}

@interface MainMenuHandler : NSObject
@property (readwrite) bg::base::EventHandler * eventHandler;

- (void)callMenu:(id)sender;

@end

@implementation MainMenuHandler

- (void)callMenu:(id)sender {
	if (self.eventHandler) {
		NSValue * representedValue = [sender representedObject];
		bg::wnd::PopUpMenuItem * item = reinterpret_cast<bg::wnd::PopUpMenuItem*>([representedValue pointerValue]);
		
		self.eventHandler->menuSelected(item->title, item->identifier);
	}
	
}

@end

namespace bg {
namespace wnd {

int MainLoop::run() {
	@autoreleasepool {
		[NSApplication sharedApplication];
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
		
		bg::wnd::MenuDescriptor menu;
		// Add default "quit" menu
		bg::wnd::PopUpMenu * appMenu = bg::wnd::PopUpMenu::New();
			
		NSString * appName = [[NSProcessInfo processInfo] processName];
		NSString * quitTitle = [@"Quit " stringByAppendingString:appName];
		MainMenuHandler * menuHandler = [MainMenuHandler new];
		
		appMenu->setTitle(appName.UTF8String);
		appMenu->addMenuItem({ bg::wnd::kCodeQuit, quitTitle.UTF8String, { bg::base::Keyboard::kCommandKey, bg::base::Keyboard::kKeyQ } });
		menu.push_back(appMenu);
	
		if (window() && window()->windowController() && window()->windowController()->eventHandler()) {
			bg::base::EventHandler * evtHandler = window()->windowController()->eventHandler();
			evtHandler->buildMenu(menu);
			menuHandler.eventHandler = evtHandler;
		}


		id menubar = [NSMenu new];
		[NSApp setMainMenu:menubar];
		for (auto submenu : menu) {
			NSMenuItem * menuItem = [NSMenuItem new];
			menuItem.title = [NSString stringWithUTF8String:submenu->title().c_str()];

			NSMenu * menuItemMenu = [NSMenu new];
			menuItemMenu.title = menuItem.title;
			
			submenu->eachMenuItem([&](const PopUpMenuItem & submenuItem) {
				std::string title = submenuItem.title;
				
				NSMenuItem * cocoa_subMenuItem = [[NSMenuItem alloc] initWithTitle:[NSString stringWithUTF8String:title.c_str()]
														  action:@selector(callMenu:) keyEquivalent:@""];
				[cocoa_subMenuItem setRepresentedObject:[NSValue valueWithPointer:reinterpret_cast<const void*>(&submenuItem)]];
				[cocoa_subMenuItem setTarget:menuHandler];
				
				if (submenuItem.shortcut.valid()) {
					[cocoa_subMenuItem setKeyEquivalentModifierMask:getCocoaModifiers(submenuItem.shortcut)];
					NSString * modifier = getCocoaKey(submenuItem.shortcut);
					[cocoa_subMenuItem setKeyEquivalent:modifier];
				}
				
				[menuItemMenu addItem:cocoa_subMenuItem];
			});
			
			[menuItem setSubmenu:menuItemMenu];
			[menubar addItem:menuItem];
		}

		CocoaAppDelegate * appDelegate = [CocoaAppDelegate new];
		[[NSApplication sharedApplication] setDelegate:appDelegate];
		[NSApp activateIgnoringOtherApps:YES];
		[NSApp run];
	}
	return 0;
}

void MainLoop::quit(int code) {
	[[NSApplication sharedApplication] terminate:nil];
}

}
}
