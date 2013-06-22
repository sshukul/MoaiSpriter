//
//  AppDelegate.m
//  NekoPunch
//
//  Created by Cartwright Samuel on 6/5/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#import "AppDelegate.h"

extern int gameControllerScreenWidth;
extern int gameControllerScreenHeight;

@implementation AppDelegate

@synthesize window;

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{    
    NSRect frame = [window frame];
    frame.size.width = gameControllerScreenWidth;
    frame.size.height = gameControllerScreenHeight;
    [window setFrame:frame display:YES animate:NO];
}

@end
