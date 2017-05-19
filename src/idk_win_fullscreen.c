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
 
 #ifdef _WIN32
 
 #include <IDK/IDK.h>
 
 typedef struct FullScreen_Win_s { IDKWindow idk_window ; DWORD style ; DWORD exstyle ; int is_maxed ; RECT rect ; } *FullScreen_Win ;
 
 const void* IDKSetupFullScreenForWin( IDKWindow idk_window ) {
     
     FullScreen_Win fullScreen_win = RKMem_NewMemOfType(struct FullScreen_Win_s) ;
     
     fullScreen_win->idk_window = idk_window ;
     
     fullScreen_win->style = 0 ;
     
     fullScreen_win->exstyle = 0 ;
     
     fullScreen_win->is_maxed = 0 ;
     
     fullScreen_win->rect.left = -1 ;
     
     fullScreen_win->rect.top = -1 ;
     
     fullScreen_win->rect.right = -1 ;
     
     fullScreen_win->rect.bottom = -1 ;
     
     return fullScreen_win ;
 }
 
  void IDKKillFullScreenForWin( const void* fullscreen_state ) {
    
    free(fullscreen_state) ;
}
 
 void IDKEnterFullScreenForWin( const void* fullscreen_state ) {
     
     FullScreen_Win fullscreen_win = (FullScreen_Win)fullscreen_state ;
     
     HWND window = glfwGetWin32Window(IDK_GetPtrToGLFWWindow(fullscreen_win->idk_window)) ;
     
     fullscreen_win->style = GetWindowLong(window, GWL_STYLE) ;
     
     fullscreen_win->exstyle = GetWindowLong(window, GWL_EXSTYLE) ;
     
     fullscreen_win->is_maxed = IsZoomed(window) ;
     
     if ( fullscreen_win->is_maxed ) ShowWindow(window, SW_RESTORE) ;
     
     if ( fullscreen_win->rect.left == -1 && fullscreen_win->rect.top == -1 && 
     fullscreen_win->rect.right == -1 && fullscreen_win->rect.bottom == -1 ) GetClientRect (window, &fullscreen_win->rect) ;
     
     SetWindowLong(window, GWL_STYLE, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_VISIBLE) ;
     
     SetWindowLong(window, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST) ;
     
     MONITORINFO monitorinfo ;
     
     monitorinfo.cbSize = sizeof(monitorinfo) ; 
     
     HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST) ;
     
     GetMonitorInfo(monitor, &monitorinfo) ;
     
     SetWindowPos(window, NULL,monitorinfo.rcMonitor.left,monitorinfo.rcMonitor.top, monitorinfo.rcMonitor.right,  monitorinfo.rcMonitor.bottom, 
         SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED) ;
 }
 
void IDKExitFullScreenForWin( const void* fullscreen_state ) {
    
     FullScreen_Win fullscreen_win = (FullScreen_Win)fullscreen_state ;
    
     HWND window = glfwGetWin32Window(IDK_GetPtrToGLFWWindow(fullscreen_win->idk_window)) ;
    
     SetWindowLong(window,  GWL_STYLE, fullscreen_win->style) ;
     
     SetWindowLong(window,  GWL_EXSTYLE, fullscreen_win->exstyle) ;
     
     SetWindowPos(window, NULL, fullscreen_win->rect.left, fullscreen_win->rect.top, fullscreen_win->rect.right, fullscreen_win->rect.bottom, 
         SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED) ;
         
     if ( fullscreen_win->is_maxed ) ShowWindow(window, SW_MAXIMIZE) ;
}
 
 #endif