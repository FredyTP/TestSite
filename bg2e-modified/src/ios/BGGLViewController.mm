//
//  GameViewController.m
//  event-handler
//
//  Created by Fernando Serrano Carpena on 3/5/16.
//  Copyright © 2016 Fernando Serrano Carpena. All rights reserved.
//

#import <bg/ios/BGGLViewController.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <CoreMotion/CoreMotion.h>

#include <bg/base/event_handler.hpp>

#include <bg/engine.hpp>
#include <bg/engine/opengl_es_3.hpp>
#include <bg/ios/gles_context.hpp>

#include <bg/tools/texture_cache.hpp>

@interface BGGLViewController () {
	bool _firstResize;
}
@property (strong, nonatomic) EAGLContext *context;
@property (readwrite) bg::base::EventHandler * eventHandler;
@property (strong) CMMotionManager * motionManager;

- (bg::base::EventHandler*) createEventHandler;

- (void)setupGL;
- (void)tearDownGL;
- (void)initTouchEvent:(bg::base::TouchEvent&)touchEvent withTouches:(NSSet*) touches;

@end

@implementation BGGLViewController

- (void)viewDidLoad
{
	[super viewDidLoad];
	_firstResize = false;
	
	self.eventHandler = [self createEventHandler];
	self.eventHandler->inc_ref();
	
	if (!bg::Engine::Get()) {
		bg::Engine::Init(new bg::engine::OpenGLES3());
	}
	
	self.eventHandler->willCreateContext();
	
	self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
	
	self.eventHandler->setContext(new bg::ios::GLESContext((__bridge void*)self.context, (__bridge void*)self.view));
	
	self.eventHandler->context()->inc_ref();
	
	if (!self.context) {
		NSLog(@"Failed to create ES context");
	}
	
	GLKView *view = (GLKView *)self.view;
	view.context = self.context;
	view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
	
	self.preferredFramesPerSecond = 60;
	
	[self setupGL];
	
	NSOperationQueue * queue = [[NSOperationQueue alloc] init];
	self.motionManager = [[CMMotionManager alloc] init];
	
	[self.motionManager startAccelerometerUpdatesToQueue:queue withHandler:^(CMAccelerometerData * data, NSError * err) {
		float x = data.acceleration.x;
		float y = data.acceleration.y;
		float z = data.acceleration.z;
		
		dispatch_sync(dispatch_get_main_queue(), ^() {
			self.eventHandler->sensorEvent(bg::base::SensorEvent(bg::math::Vector3(x, y, z)));
		});
		
	}];
}

- (void)dealloc
{
	[self tearDownGL];
	
	self.eventHandler->willDestroyContext();
	self.eventHandler->context()->dec_ref();
	self.eventHandler->dec_ref();
	if ([EAGLContext currentContext] == self.context) {
		[EAGLContext setCurrentContext:nil];
	}
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];
	
	self.eventHandler->onMemoryWarning();
	
	if ([self isViewLoaded] && ([[self view] window] == nil)) {
		self.view = nil;
		
		[self tearDownGL];
		
		if ([EAGLContext currentContext] == self.context) {
			[EAGLContext setCurrentContext:nil];
		}
		self.context = nil;
	}
}

- (void)setupGL
{
	[EAGLContext setCurrentContext:self.context];
	self.eventHandler->initGL();
}

- (void)tearDownGL
{
	[EAGLContext setCurrentContext:self.context];
	self.eventHandler->destroy();
}

- (bg::base::EventHandler*) createEventHandler {
	return new bg::base::EventHandler();
}


- (void)update
{
	[EAGLContext setCurrentContext:self.context];
	GLKView *view = (GLKView *)self.view;
	[view bindDrawable];
	
	if (_firstResize) {
		self.eventHandler->frame(self.timeSinceLastUpdate);
		self.eventHandler->draw();
	}
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	self.eventHandler->reshape(rect.size.width * self.view.contentScaleFactor, rect.size.height * self.view.contentScaleFactor);
	if (!_firstResize) {
		self.eventHandler->frame(self.timeSinceLastUpdate);
		self.eventHandler->draw();
		_firstResize = true;
	}
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	bg::base::TouchEvent touchEvent;
	[self initTouchEvent:touchEvent withTouches:event.allTouches];
	self.eventHandler->touchStart(touchEvent);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	bg::base::TouchEvent touchEvent;
	[self initTouchEvent:touchEvent withTouches:event.allTouches];
	self.eventHandler->touchMove(touchEvent);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	bg::base::TouchEvent touchEvent;
	[self initTouchEvent:touchEvent withTouches:event.allTouches];
	self.eventHandler->touchEnd(touchEvent);
}

- (void)initTouchEvent:(bg::base::TouchEvent&)touchEvent withTouches:(NSSet*) touches {
	for (id touch in touches) {
		CGPoint point = [touch locationInView:self.view];
		touchEvent.addTouch(bg::math::Position2Di(static_cast<int>(point.x * self.view.contentScaleFactor), static_cast<int>(point.y * self.view.contentScaleFactor)));
	}
}

@end
