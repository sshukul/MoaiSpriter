//
//  GameController.cpp
//  NyanTest4
//
//  Created by Cartwright Samuel on 9/26/12.
//  Copyright (c) 2012 Cartwright Samuel. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "GameController.h"
#include "glUtil.h"
#include "fileUtil.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <aku/AKU.h>
#include <lua-headers/moai_lua.h>

int gameControllerScreenWidth = 240;
int gameControllerScreenHeight = 480;

//================================================================//
// AKU callbacks (TODO)
//================================================================//

void _AKUEnterFullscreenModeFunc();
void _AKUExitFullscreenModeFunc();
void _AKUOpenWindowFunc(const char* title, int width, int height);

void _AKUEnterFullscreenModeFunc() {
}

void _AKUExitFullscreenModeFunc() {
}

void _AKUOpenWindowFunc(const char* title, int width, int height) {
    // pass the width and height to the NSWindow
    
        // this is why we can have a plain old cpp
        // Of course, this won't work if the window hasn't booted up yet, so thats
        // why we need the global variables (which are referenced in AppDelegate)
    NSWindow* window = [[NSApplication sharedApplication] mainWindow];
    NSRect frame = [window frame];
    frame.size.width = width;
    frame.size.height = height;
    [window setFrame:frame display:YES animate:NO];
    
    gameControllerScreenWidth = width;
    gameControllerScreenHeight = height;
}



GameController::GameController(unsigned int screenHidth, unsigned int screenHeight) :
m_screenWidth(screenHidth), m_screenHeight(screenHeight), m_bIsInitialized(false) {
}

GameController::~GameController() {
}

void GameController::initialise() {
    AKUContextID context = AKUGetContext ();
	if ( context ) {
		AKUDeleteContext ( context );
	}
	AKUCreateContext ();

    AKUDetectGfxContext();
    
	AKUSetInputConfigurationName("AKUOSX");
    
	AKUReserveInputDevices(OSXInputDeviceID::TOTAL);
	AKUSetInputDevice(OSXInputDeviceID::DEVICE, "device");
	
	AKUReserveInputDeviceSensors(OSXInputDeviceID::DEVICE, OSXInputDeviceID::TOTAL);
	AKUSetInputDeviceKeyboard(OSXInputDeviceID::DEVICE, OSXInputDeviceSensorID::KEYBOARD, "keyboard");
	AKUSetInputDevicePointer(OSXInputDeviceID::DEVICE, OSXInputDeviceSensorID::POINTER, "pointer");
	AKUSetInputDeviceButton(OSXInputDeviceID::DEVICE, OSXInputDeviceSensorID::MOUSE_LEFT, "mouseLeft");
	AKUSetInputDeviceButton(OSXInputDeviceID::DEVICE, OSXInputDeviceSensorID::MOUSE_MIDDLE, "mouseMiddle");
	AKUSetInputDeviceButton(OSXInputDeviceID::DEVICE, OSXInputDeviceSensorID::MOUSE_RIGHT, "mouseRight");
    
	AKUSetFunc_EnterFullscreenMode ( _AKUEnterFullscreenModeFunc );
	AKUSetFunc_ExitFullscreenMode ( _AKUExitFullscreenModeFunc );
	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );
        
	AKURunBytecode ( moai_lua, moai_lua_SIZE );
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"main" ofType:@"lua" inDirectory:@"modules"];
    NSString* folder = [path stringByDeletingLastPathComponent];
    
    AKUSetWorkingDirectory([folder UTF8String]);
    
    NSError *error;
    NSString *stringFromFile = [NSString stringWithContentsOfFile:path
                                                     usedEncoding:nil
                                                            error:&error];
    
    if (stringFromFile == nil) {
        // an error occurred
        NSLog(@"Error reading file at %@\n%@", path, [error localizedFailureReason]);    
        return;
    }
    
    AKURunString( [stringFromFile UTF8String] );
        
    m_bIsInitialized = true;
}

void GameController::destroy() {
    m_bIsInitialized = false;
    AKUFinalize ();
}

void GameController::update(float dt) {
    AKUUpdate ();
}

void GameController::draw() {
    AKURender();
}


#pragma mark ---- Event Handlers ----

// TODO: handle shift, control, etc
//void GameController::updateModifiers() {
//	int newModifiers = glutGetModifiers ();
//	int changedModifiers = newModifiers ^ sModifiers;
//	if ( changedModifiers & GLUT_ACTIVE_SHIFT ) {
//		AKUEnqueueKeyboardShiftEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::KEYBOARD, (newModifiers & GLUT_ACTIVE_SHIFT) != 0 );
//	}
//	if ( changedModifiers & GLUT_ACTIVE_CTRL ) {
//		AKUEnqueueKeyboardControlEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::KEYBOARD, (newModifiers & GLUT_ACTIVE_CTRL) != 0 );
//	}
//	if ( changedModifiers & GLUT_ACTIVE_ALT ) {
//		AKUEnqueueKeyboardAltEvent ( GlutInputDeviceID::DEVICE, GlutInputDeviceSensorID::KEYBOARD, (newModifiers & GLUT_ACTIVE_ALT) != 0 );
//	}
//	sModifiers = newModifiers;
//}

void GameController::keyDown(int key) {
	AKUEnqueueKeyboardEvent(OSXInputDeviceID::DEVICE, OSXInputDeviceSensorID::KEYBOARD, key, true);
}

void GameController::keyUp(int key) {
	AKUEnqueueKeyboardEvent(OSXInputDeviceID::DEVICE, OSXInputDeviceSensorID::KEYBOARD, key, false);
}

void GameController::mouseDown(int button, int x, int y) {
    AKUEnqueueButtonEvent(OSXInputDeviceID::DEVICE, button, true);
}

void GameController::mouseUp(int button, int x, int y) {
    AKUEnqueueButtonEvent(OSXInputDeviceID::DEVICE, button, false);
}

void GameController::mouseDrag(int x, int y) {
	AKUEnqueuePointerEvent(OSXInputDeviceID::DEVICE, OSXInputDeviceSensorID::POINTER, x, y );
}

void GameController::mouseMove(int x, int y) {
	AKUEnqueuePointerEvent(OSXInputDeviceID::DEVICE, OSXInputDeviceSensorID::POINTER, x, y);
}

void GameController::scrollWheel(float wheelDelta) {
    
}
