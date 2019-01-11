
#include <bg/wnd/cocoa_glview.h>
#include <bg/wnd/cocoa_gl_context.hpp>
#include <bg/wnd/window_controller.hpp>
#include <bg/wnd/main_loop.hpp>
#include <bg/math/vector.hpp>
#include <bg/engine.hpp>
#include <bg/engine/opengl_core.hpp>
#include <bg/wnd/cocoa_utils.h>

#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

#include <iostream>


#define SUPPORT_RETINA_RESOLUTION 1

@interface CocoaView ()

@property (readwrite) NSTimeInterval lastInterval;

- (void)drawView;
- (void)releaseOpenGL:(NSNotification*)notification;
- (void)closeWindow:(NSNotification*)notification;

@end

@implementation CocoaView



- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format {
	self = [super initWithFrame:frameRect pixelFormat:format];
	if (self) {
		self.lastInterval = [[NSDate date] timeIntervalSince1970];
        [self setWantsBestResolutionOpenGLSurface:YES];
	//	NSOpenGLPixelFormatAttribute attrsLegacy[] =
	//	{
	//		NSOpenGLPFADoubleBuffer,
	//		NSOpenGLPFADepthSize, 24,
	//		0
	//	};
		NSOpenGLPixelFormatAttribute attrs[] =
		{
			NSOpenGLPFADoubleBuffer,
			NSOpenGLPFADepthSize, 24,
			NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
			0
		};
		
		NSOpenGLPixelFormat * pf = nil;
		if (bg::Engine::Get() &&
			bg::Engine::Get()->identifier()==bg::Engine::Identifier<bg::engine::OpenGLCore>())
		{
			pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
		}
	//	else if (bg::Engine::Get()->identifier()==bg::Engine::Identifier<bg::engine::OpenGLCompatibility>()) {
	//		pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrsLegacy];
	//	}
		
		if (!pf) {
			NSLog(@"No OpenGL pixel format");
		}
		
		if (pf) {
			NSOpenGLContext * context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
		
		
			[self setPixelFormat:pf];
			[self setOpenGLContext:context];
			[self setWantsBestResolutionOpenGLSurface:YES];
		}
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onClose) name:NSWindowWillCloseNotification object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(windowMoved) name:NSWindowDidMoveNotification  object:nil];

	}
	return self;
}

- (void)notifyQuit {
    
}

- (void)onClose {
}

- (void)windowMoved {
	NSRect rect = self.window.frame;
	NSRect screenFrame = self.window.screen.frame;
	bg::math::Rect engineRect = bg::math::Rect(rect.origin.x, screenFrame.size.height - rect.size.height - rect.origin.y, rect.size.width, rect.size.height);
	self.cocoaWindow->setRect(engineRect);
}

- (void)prepareOpenGL {
	[super prepareOpenGL];
	
	[[self openGLContext] makeCurrentContext];
	
	GLint swapInt = 1;
	[self.openGLContext setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
	
	[NSTimer scheduledTimerWithTimeInterval:1.0/30.0 target:self selector:@selector(drawView) userInfo:Nil repeats:YES];
	
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(closeWindow:) name:NSWindowWillCloseNotification object:[self window]];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(releaseOpenGL:) name:NSApplicationWillTerminateNotification object:[self window]];
	
	bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
	
	if (self.cocoaWindow->isFullScreen() && self.cocoaWindow->rect().width()!=0 && self.cocoaWindow->rect().height()!=0) {
		GLint dim[2] = { self.cocoaWindow->rect().width(), self.cocoaWindow->rect().height() };
		CGLSetParameter([self.openGLContext CGLContextObj], kCGLCPSurfaceBackingSize, dim);
		CGLEnable([self.openGLContext CGLContextObj], kCGLCESurfaceBackingSize);
	}
	
	
	controller->initGL();
}

- (void)reshape
{
	[super reshape];
	[self.openGLContext makeCurrentContext];
	
	if (self.cocoaWindow) {
		bg::wnd::WindowController * controller = self.cocoaWindow->windowController();
		
		if (!self.cocoaWindow->isFullScreen()) {
			NSRect viewRectPoints = [self bounds];
#if SUPPORT_RETINA_RESOLUTION
			NSRect viewRectPixels = [self convertRectToBacking:viewRectPoints];
#else
			NSRect viewRectPixels = viewRectPoints;
#endif
			self.cocoaWindow->setSize(viewRectPoints.size.width, viewRectPoints.size.height);
			controller->reshape(viewRectPixels.size.width, viewRectPixels.size.height);
		}
		else {
			controller->reshape(self.cocoaWindow->rect().width(), self.cocoaWindow->rect().height());
		}
	}
}

/*
- (void)renewGState
{
	[[self window] disableScreenUpdatesUntilFlush];
	
	[super renewGState];
}
*/

- (void)drawRect:(NSRect)dirtyRect {
	[self.openGLContext makeCurrentContext];
	[self drawView];
}

- (void)drawView {
	NSTimeInterval timeInMiliseconds = [[NSDate date] timeIntervalSince1970] - self.lastInterval;
	self.lastInterval = [[NSDate date] timeIntervalSince1970];
	
	[self.openGLContext makeCurrentContext];
	bg::wnd::WindowController * controller = self.cocoaWindow->windowController();

	
	controller->frame(static_cast<float>(timeInMiliseconds));
	controller->draw();
}

- (void)closeWindow:(NSNotification*)notification {
	[[NSApplication sharedApplication] terminate:self];
}

- (void)releaseOpenGL:(NSNotification*)notification {
	[self.openGLContext makeCurrentContext];
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
