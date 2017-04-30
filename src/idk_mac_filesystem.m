/*
 Copyright (c) 2017 Jacob Gordon. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <Cocoa/Cocoa.h>
#include <sys/stat.h>
#include <unistd.h>
#include <IDK.h>

static const char* IDKMacGetAppSupportDir( IDKApp App ) {
    
    static int makedir = 1 ;
    
    NSFileManager* filemanager = [NSFileManager defaultManager] ;
    
    NSURL* the_appsupport_dir = nil ;
    
    NSString* app_name = [[NSString alloc] initWithBytes:RKString_GetString(IDK_GetAppName(App)) length:RKString_GetSize(IDK_GetAppName(App)) encoding: NSASCIIStringEncoding] ;
    
    NSString* idk_bundle_id_string = @"com.idk.bundleidentifier.app." ;
    
    NSURL* app_path = nil ;
    
    NSArray* fileurls = [filemanager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask] ;
    
    if ( [fileurls count] > 0 ) {
        
        the_appsupport_dir = [fileurls objectAtIndex:0] ;
        
        NSString* the_bundle_id = [NSBundle mainBundle].bundleIdentifier ;
        
        if ( the_bundle_id == NULL ) the_bundle_id = [idk_bundle_id_string stringByAppendingString:app_name] ;
        
        app_path = [the_appsupport_dir URLByAppendingPathComponent:the_bundle_id];
    }
    
    if ( makedir ) {
        
        mkdir(app_path.fileSystemRepresentation, S_IRWXU) ;
        
        makedir = 0 ;
    }
    
    return app_path.fileSystemRepresentation ;
}

static const char* IDKMacGetResourcePath( void ) {
    
    NSString* resourcePath = [NSBundle mainBundle].resourcePath ;
    
    return [resourcePath cStringUsingEncoding:NSASCIIStringEncoding] ;
}

const char* IDKGetFilePathForMac( IDKApp App, IDKLoadFileType filetype ) {
    
    if ( filetype == idk_resource_file ) return IDKMacGetResourcePath() ;
    
    if ( filetype == idk_data_file ) return IDKMacGetAppSupportDir(App) ;
    
    return NULL ;
}