
#include <bg/wnd/cocoa_mtlview.h>
#include <bg/wnd/window_controller.hpp>
#include <bg/wnd/main_loop.hpp>
#include <bg/math/vector.hpp>
#include <bg/engine.hpp>
#include <bg/engine/opengl_core.hpp>
#include <bg/wnd/cocoa_utils.h>

#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

#include <iostream>
#include <mutex>
#include <thread>


#define SUPPORT_RETINA_RESOLUTION 1

@interface MetalView ()

- (void)releaseMtl:(NSNotification*)notification;
- (void)closeWindow:(NSNotification*)notification;
- (void)windowResized;

@end

@implementation MetalView {
    CVDisplayLinkRef _displayLink;
    std::mutex _exitMutex;
    bool _running;
}

-(BOOL) wantsUpdateLayer { return YES; }
+(Class) layerClass { return [CAMetalLayer class]; }
-(CALayer*) makeBackingLayer { return [self.class.layerClass layer]; }
- (BOOL)acceptsFirstResponder { return YES; }

- (id)initWithFrame:(NSRect)frameRect window:(bg::wnd::CocoaWindow*)window {
	self = [super initWithFrame:frameRect];
	if (self) {
        _running = true;
        self.wantsLayer = YES;
        self.cocoaWindow = window;
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onClose) name:NSWindowWillCloseNotification object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowMoved) name:NSWindowDidMoveNotification  object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowResized) name:NSWindowDidResizeNotification object:nil];
	}
	return self;
}

- (void)initGL {
    self.cocoaWindow->windowController()->initGL();
    [self windowResized];
    CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
    CVDisplayLinkSetOutputCallback(_displayLink, &DisplayLinkCallback, self.cocoaWindow->windowController());
    CVDisplayLinkStart(_displayLink);
}

static CVReturn DisplayLinkCallback(CVDisplayLinkRef displayLink,
                                    const CVTimeStamp* now,
                                    const CVTimeStamp* outputTime,
                                    CVOptionFlags flagsIn,
                                    CVOptionFlags* flagsOut,
                                    void* target)
{
    static NSTimeInterval lastInterval = [[NSDate date] timeIntervalSince1970];
    NSTimeInterval timeInMiliseconds = [[NSDate date] timeIntervalSince1970] - lastInterval;
    lastInterval = [[NSDate date] timeIntervalSince1970];
    ((bg::wnd::WindowController*) target)->frame(static_cast<float>(timeInMiliseconds));
    ((bg::wnd::WindowController*) target)->draw();
    return kCVReturnSuccess;
}

- (void)notifyQuit {
    CVDisplayLinkStop(_displayLink);
}

- (void)onClose {
    CVDisplayLinkStop(_displayLink);
    bg::wnd::MainLoop::Get()->quit(0);
}

- (void)windowMoved {
    NSRect rect = self.window.frame;
    NSRect screenFrame = self.window.screen.frame;
    bg::math::Rect engineRect = bg::math::Rect(rect.origin.x, screenFrame.size.height - rect.size.height - rect.origin.y, rect.size.width, rect.size.height);
    self.cocoaWindow->setRect(engineRect);
}

- (void)windowResized {
    NSRect rect = self.window.frame;
    NSRect screenFrame = self.window.screen.frame;
    bg::math::Rect engineRect = bg::math::Rect(rect.origin.x, screenFrame.size.height - rect.size.height - rect.origin.y, rect.size.width, rect.size.height);
    self.cocoaWindow->setRect(engineRect);
    self.cocoaWindow->windowController()->reshape(static_cast<int>(engineRect.width()), static_cast<int>(engineRect.height()));
}

- (void)closeWindow:(NSNotification*)notification {
	[[NSApplication sharedApplication] terminate:self];
}

- (void)releaseMtl:(NSNotification*)notification {
	bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
	if (controller) {
		controller->destroy();
	}
	self.cocoaWindow->setWindowController(nullptr);
}

- (NSPoint)convertToEnginePoint:(NSEvent*)theEvent
{
	NSPoint clickPoint = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	NSRect viewRect = self.frame;
	
#if SUPPORT_RETINA_RESOLUTION
		clickPoint = [self convertPointToBacking:clickPoint];
		viewRect = [self convertRectToBacking:viewRect];
#endif
	
	clickPoint.y = viewRect.size.height - clickPoint.y;
	
	if (self.cocoaWindow->isFullScreen()) {
		bg::math::Vector4i scaledSize = self.cocoaWindow->rect();
		clickPoint.x = clickPoint.x * scaledSize.width() / viewRect.size.width;
		clickPoint.y = clickPoint.y * scaledSize.height() / viewRect.size.height;
	}
	
	return clickPoint;
}

- (void)mouseDown:(NSEvent*)theEvent
{
	NSPoint clickPoint = [self convertToEnginePoint:theEvent];
	
	bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
	mainLoop->mouse().setMouseDown(bg::base::Mouse::kLeftButton);

	bg::base::MouseEvent mouseEvent;
	fillMouseEvent(mouseEvent, mainLoop, clickPoint);
	bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
	controller->mouseDown(mouseEvent);
}

- (void)mouseUp:(NSEvent*)theEvent
{
	NSPoint clickPoint = [self convertToEnginePoint:theEvent];

	bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
	mainLoop->mouse().setMouseUp(bg::base::Mouse::kLeftButton);
	
	bg::base::MouseEvent mouseEvent;
	fillMouseEvent(mouseEvent, mainLoop, clickPoint);
	bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
	controller->mouseUp(mouseEvent);
}

- (BOOL)isPointInWindow:(NSPoint)point {
	NSRect viewRectPoints = [self bounds];
	
#if SUPPORT_RETINA_RESOLUTION
	viewRectPoints = [self convertRectToBacking:viewRectPoints];
#endif

	return point.x>=0 && point.y>=0 && point.x<=viewRectPoints.size.width && point.y<=viewRectPoints.size.height;
}

- (void)mouseMoved:(NSEvent*)theEvent
{
	NSPoint movePoint = [self convertToEnginePoint:theEvent];
	if ([self isPointInWindow:movePoint]) {
		bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
		bg::base::MouseEvent mouseEvent;
		fillMouseEvent(mouseEvent, mainLoop, movePoint);
		bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
		controller->mouseMove(mouseEvent);
	}
}

- (void)mouseDragged:(NSEvent*)theEvent
{
	NSPoint movePoint = [self convertToEnginePoint:theEvent];
	if ([self isPointInWindow:movePoint]) {
		bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
		bg::base::MouseEvent mouseEvent;
		fillMouseEvent(mouseEvent, mainLoop, movePoint);
		bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
		controller->mouseMove(mouseEvent);
		controller->mouseDrag(mouseEvent);
	}
}

- (void)rightMouseDown:(NSEvent*)theEvent
{
	NSPoint clickPoint = [self convertToEnginePoint:theEvent];
	bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
	mainLoop->mouse().setMouseDown(bg::base::Mouse::kRightButton);
	
	bg::base::MouseEvent mouseEvent;
	fillMouseEvent(mouseEvent, mainLoop, clickPoint);
	bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
	controller->mouseDown(mouseEvent);
}

- (void)rightMouseUp:(NSEvent*)theEvent
{
	NSPoint clickPoint = [self convertToEnginePoint:theEvent];
	bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
	mainLoop->mouse().setMouseUp(bg::base::Mouse::kRightButton);
	
	bg::base::MouseEvent mouseEvent;
	fillMouseEvent(mouseEvent, mainLoop, clickPoint);
	bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
	controller->mouseUp(mouseEvent);
}

- (void)rightMouseDragged:(NSEvent*)theEvent
{
	NSPoint movePoint = [self convertToEnginePoint:theEvent];
	if ([self isPointInWindow:movePoint]) {
		bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
		bg::base::MouseEvent mouseEvent;
		fillMouseEvent(mouseEvent, mainLoop, movePoint);
		bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
		controller->mouseMove(mouseEvent);
		controller->mouseDrag(mouseEvent);
	}
}

- (void)otherMouseDown:(NSEvent*)theEvent
{
	NSPoint clickPoint = [self convertToEnginePoint:theEvent];
	bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
	mainLoop->mouse().setMouseDown(bg::base::Mouse::kMiddleButton);
	
	bg::base::MouseEvent mouseEvent;
	fillMouseEvent(mouseEvent, mainLoop, clickPoint);
	bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
	controller->mouseDown(mouseEvent);
}

- (void)otherMouseUp:(NSEvent*)theEvent
{
	NSPoint clickPoint = [self convertToEnginePoint:theEvent];
	bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
	mainLoop->mouse().setMouseUp(bg::base::Mouse::kMiddleButton);
	
	bg::base::MouseEvent mouseEvent;
	fillMouseEvent(mouseEvent, mainLoop, clickPoint);
	bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
	controller->mouseUp(mouseEvent);
}

- (void)otherMouseDragged:(NSEvent*)theEvent
{
	NSPoint movePoint = [self convertToEnginePoint:theEvent];
	if ([self isPointInWindow:movePoint]) {
		bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
		bg::base::MouseEvent mouseEvent;
		fillMouseEvent(mouseEvent, mainLoop, movePoint);
		bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
		controller->mouseMove(mouseEvent);
		controller->mouseDrag(mouseEvent);
	}
}

- (void)scrollWheel:(NSEvent*)theEvent
{
	NSPoint movePoint = [self convertToEnginePoint:theEvent];
	bg::wnd::MainLoop * mainLoop = bg::wnd::MainLoop::Get();
	bg::base::MouseEvent mouseEvent;
	fillMouseEvent(mouseEvent, mainLoop, movePoint);
	bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
	float deltaX = [theEvent scrollingDeltaX];
	float deltaY = [theEvent scrollingDeltaY];
	deltaX = deltaX==0.0f ? 0.0f:deltaX>0 ? 1.0f:-1.0f;
	deltaY = deltaY==0.0f ? 0.0f:deltaY>0 ? 1.0f:-1.0f;
	mouseEvent.setDelta(bg::math::Vector2(deltaX,deltaY));
	controller->mouseWheel(mouseEvent);
}

- (void)keyDown:(NSEvent *)theEvent {
	bg::base::KeyboardEvent kbEvent;
	fillKeyboard(kbEvent.keyboard(), theEvent.characters, theEvent.keyCode, theEvent.modifierFlags);
	bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
	controller->keyDown(kbEvent);
	switch (kbEvent.keyboard().key()) {
	case bg::base::Keyboard::kKeyRePag:
	case bg::base::Keyboard::kKeyDown:
	case bg::base::Keyboard::kKeyStart:
	case bg::base::Keyboard::kKeyAvPag:
	case bg::base::Keyboard::kKeyUp:
	case bg::base::Keyboard::kKeyEnd:
	case bg::base::Keyboard::kKeyLeft:
	case bg::base::Keyboard::kKeyRight:
	case bg::base::Keyboard::kKeyEsc:
		break;
	default:
		controller->charPress(kbEvent);
	};
	
}

- (void)keyUp:(NSEvent *)theEvent {
	bg::base::KeyboardEvent kbEvent;
	fillKeyboard(kbEvent.keyboard(), theEvent.characters, theEvent.keyCode, theEvent.modifierFlags);
	bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
	controller->keyUp(kbEvent);
}

@end
