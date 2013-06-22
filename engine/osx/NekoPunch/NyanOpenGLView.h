//
//  NyanOpenGLView.h
//  NyanTest4
//
//  Created by Cartwright Samuel on 2/14/13.
//  Copyright (c) 2013 Cartwright Samuel. All rights reserved.
//

// The preferred way now to set up a game loop is to use a Core Video display link on OSX > 10.3
// On older hardware, use a NSTimer
// See Tech Q&A 1385: http://developer.apple.com/library/mac/#qa/qa1385/_index.html
//#define USE_COREVIDEO_TIMER           // couldn't get this working in a stable way

#import <Cocoa/Cocoa.h>

#ifdef USE_COREVIDEO_TIMER
#import <CoreVideo/CoreVideo.h>
#endif

class GameController;

@interface NyanOpenGLView : NSOpenGLView <NSWindowDelegate>
{  
	// Flags to help track whether ctrl-clicking or option-clicking is being used
	BOOL isUsingCtrlClick;
	BOOL isUsingOptionClick;
	        
	// Flag to track whether the OpenGL multithreading engine is enabled or not
	BOOL isUsingMultithreadedOpenGLEngine;
    
    GameController* gameController;
    
#ifdef USE_COREVIDEO_TIMER
    // display link for managing rendering thread
    CVDisplayLinkRef displayLink;
#else
    // This timer is used to trigger animation callbacks since everything is event driven.
	NSTimer* animationTimer;
#endif
}

    // My custom static method to create a basic pixel format
+ (NSOpenGLPixelFormat*) basicPixelFormat;


    // Official init methods
- (id) initWithFrame:(NSRect)frame_rect pixelFormat:(NSOpenGLPixelFormat*)pixel_format;
- (id) initWithCoder:(NSCoder*)the_coder;
- (id) initWithFrame:(NSRect)frame_rect;

- (BOOL) acceptsFirstResponder;
- (BOOL) becomeFirstResponder;
- (BOOL) resignFirstResponder;

    // Official function, overridden by this class to prevent flashing/tearing when in splitviews, scrollviews, etc.
- (void) renewGState;

    // Official/Special NSOpenGLView method that gets called for you to prepare your OpenGL state.
- (void) prepareOpenGL;

    // Class dealloc method
- (void) dealloc;
- (void) finalize;

    // Official methods for view stuff and drawing
- (BOOL) isOpaque;
- (void) resizeViewport;
- (void) reshape;
- (void) drawRect:(NSRect)the_rect;

// Method you should subclass to do your OpenGL drawing in.
// drawRect: calls this method. By doing so,
// [[self openGLContext] makeCurrentContext]; has been called before this method,
// and drawRect will call [[self openGLContext] flushBuffer];
// Don't bother calling [super renderScene];
- (void) renderScene;

- (void) keyDown:(NSEvent *)theEvent;
- (void) keyUp:(NSEvent *)theEvent;

- (void) mouseDown:(NSEvent *)theEvent;
- (void) mouseUp:(NSEvent *)theEvent;
- (void) mouseDragged:(NSEvent *)theEvent;
- (void) mouseMoved:(NSEvent *)theEvent;
- (void) scrollWheel:(NSEvent *)theEvent;

@end