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
#include <IDK.h>

@interface FullScreen_Mac : NSObject

@property IDKWindow idk_window ;

@end

@implementation FullScreen_Mac

const void* IDKSetupFullScreenForMac( IDKWindow idk_window ) {
    
    FullScreen_Mac *fullScreen_mac = [FullScreen_Mac alloc] ;
    
    [fullScreen_mac StartFullScreen] ;
    
    fullScreen_mac.idk_window = idk_window ;
    
    return CFBridgingRetain(fullScreen_mac) ;
}

void IDKKillFullScreenForMac( const void* fullscreen_state ) {
    
    CFRelease(fullscreen_state) ;
}

-(void) StartFullScreen {
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(FullScreener:)
                                                 name:NSWindowWillEnterFullScreenNotification object:nil] ;
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(Windower:)
                                                 name:NSWindowWillExitFullScreenNotification object:nil] ;
}

  void IDKSetFullScreenFlag( IDKWindow window, int is_fullscreen ) ;

- (void)FullScreener:(NSNotification *)notification {
    
    IDKSetFullScreenFlag(self.idk_window,1) ;
    
    [NSCursor hide] ;
}

- (void)Windower:(NSNotification *)notification {
    
    IDKSetFullScreenFlag(self.idk_window,0) ;
    
    [NSCursor unhide] ;
}

void IDKEnterOrExitFullScreenForMac( const void* fullscreen_state ) {
    
    FullScreen_Mac *fullScreen_mac = (__bridge FullScreen_Mac *)(fullscreen_state) ;
    
    [fullScreen_mac EnterOrExitFullScreen] ;
}

-(void)EnterOrExitFullScreen {
    
    [ glfwGetCocoaWindow((GLFWwindow*)IDK_GetPtrToGLFWWindow(self.idk_window)) toggleFullScreen:self ] ;
    
}

@end