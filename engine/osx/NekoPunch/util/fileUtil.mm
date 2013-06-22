//
//  fileUtil.c
//  NyanTest4
//
//  Created by Cartwright Samuel on 1/20/13.
//  Copyright (c) 2013 Cartwright Samuel. All rights reserved.
//

#include <CoreFoundation/CFURL.h>
#include "fileUtil.h"

bool getFilePath(const char* resourceName, const char* resourceType, const char* subDirName,
                        char* buffer, unsigned int maxBufLen) {
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    
    NSString* name = NULL;
    if(resourceName)
        name = [[[NSString alloc] initWithUTF8String: resourceName] autorelease];
    NSString* type = NULL;
    if(resourceType)
        type = [[[NSString alloc] initWithUTF8String: resourceType] autorelease];
    NSString* dir = NULL;
    if(subDirName)
        dir = [[[NSString alloc] initWithUTF8String: subDirName] autorelease];
    
    CFURLRef url = CFBundleCopyResourceURL(mainBundle, (CFStringRef)name, (CFStringRef)type, (CFStringRef)dir);
    bool success = CFURLGetFileSystemRepresentation(url, true, (UInt8*)buffer, maxBufLen);
    if(url)
        CFRelease(url);
    return success;
}
