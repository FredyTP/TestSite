
#include <bg/wnd/cocoa_popup_menu.hpp>

#include <Cocoa/Cocoa.h>

@interface MenuHandler : NSObject

@property (readonly) bg::wnd::CocoaPopUpMenu * menuInstance;
@property (readonly) bg::wnd::PopUpMenu::ItemSelectedClosure selectionClosure;

+ (MenuHandler*)singleton;

- (void)setMenuInstance:(bg::wnd::CocoaPopUpMenu *)menuInstance selectionClosure:(std::function<void(int)>)closure;

- (void)showMenu;

- (void)itemSelected:(id)item;

@end

@implementation MenuHandler

+ (MenuHandler*)singleton {
	static dispatch_once_t onceToken;
	static MenuHandler * s_handler = nil;
	dispatch_once(&onceToken, ^{
		s_handler = [[MenuHandler alloc] init];
	});
	
	return s_handler;
}

- (id)init {
	self = [super init];
	if (self) {
		_menuInstance = nullptr;
		_selectionClosure = nullptr;
	}
	return self;
}

- (void)setMenuInstance:(bg::wnd::CocoaPopUpMenu *)menuInstance selectionClosure:(std::function<void(int)>)closure {
	self->_menuInstance = menuInstance;
	self->_selectionClosure = closure;
}

- (void)showMenu {
	NSWindow * wnd = [[NSApplication sharedApplication] keyWindow];
	NSView * view = nil;
	
	if (wnd) {
		view = wnd.contentView;
	}
	
	if (view && self.menuInstance) {
		NSMenu * theMenu = [[NSMenu alloc] initWithTitle:@"Context Menu"];
		
		self.menuInstance->eachMenuItem([&](const bg::wnd::PopUpMenuItem & item, int index) {
			NSMenuItem * menuItem = [theMenu insertItemWithTitle:[NSString stringWithUTF8String:item.title.c_str()] action:@selector(itemSelected:) keyEquivalent:@"" atIndex:index];
			[menuItem setTag:item.identifier];
			[menuItem setTarget:self];
		});
		
		NSPoint clickPoint = NSMakePoint(static_cast<float>(self.menuInstance->getPosition().x()),
										 static_cast<float>(self.menuInstance->getPosition().y()));

		clickPoint = [view convertPointFromBacking:clickPoint];
		
		NSPoint p = NSMakePoint(clickPoint.x,
								[view frame].size.height - clickPoint.y);

		[theMenu popUpMenuPositioningItem:nil atLocation:p inView:view];
	}
}

- (void)itemSelected:(id)item {
	int selectedItem = static_cast<int>([item tag]);
	if (self.selectionClosure) {
		self.selectionClosure(selectedItem);
	}
}

@end

namespace bg {
namespace wnd {

CocoaPopUpMenu::CocoaPopUpMenu()
{
	
}
	
CocoaPopUpMenu::~CocoaPopUpMenu() {
	if ([[MenuHandler singleton] menuInstance]) {
		[[MenuHandler singleton] setMenuInstance:nullptr selectionClosure:nullptr];
	}
}

void CocoaPopUpMenu::show(ItemSelectedClosure closure) {
	MenuHandler * handler = [MenuHandler singleton];
	[handler setMenuInstance:this selectionClosure:closure];
	
	[handler showMenu];
}

}
}
