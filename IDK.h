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

#ifndef __IDKApp__IDK__
#define __IDKApp__IDK__

#include <stdio.h>

#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT


#ifdef __APPLE__

#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL

#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


#include "RKMem.h"
#include "RKArgs.h"
#include "RKMath.h"
#include "RKTasks.h"

typedef double (*IDKGetTimeHPFuncType)(void) ;

typedef struct IDKWindow_s* IDKWindow ;

typedef struct IDK_Page_s* IDKPage ;

typedef struct IDK_Line_s* IDKTextLine ;

typedef struct IDKApp_s* IDKApp ;

typedef enum { idk_glfw_error, idk_app_error, idk_window_error } IDKErrorType ;

typedef void (*IDKFullScreenFuncType)( const void* fullscreen_state) ;

typedef void (*IDKRasterResizeFuncType)(IDKWindow window, int width, int height) ;

typedef void (*IDKWindowRunLoopFuncType)(RKArgs args) ;

typedef void (*IDKWindowQuitRunLoopFuncType)(RKArgs args) ;

typedef void (*IDKErrorCallBackFuncType)(IDKApp App, IDKErrorType ErrorType) ;

#define IDK_DefineKey(key,lastkey) key = (lastkey + 1)

typedef enum { idk_a_key = 0,
    
IDK_DefineKey(idk_b_key,idk_a_key),
    
IDK_DefineKey(idk_c_key,idk_b_key),
    
IDK_DefineKey(idk_d_key,idk_c_key),
    
IDK_DefineKey(idk_e_key,idk_d_key),
    
IDK_DefineKey(idk_f_key,idk_e_key),
    
IDK_DefineKey(idk_g_key,idk_f_key),
    
IDK_DefineKey(idk_h_key,idk_g_key),
    
IDK_DefineKey(idk_i_key,idk_h_key),
    
IDK_DefineKey(idk_j_key,idk_i_key),
    
IDK_DefineKey(idk_k_key,idk_j_key),
    
IDK_DefineKey(idk_l_key,idk_k_key),
    
IDK_DefineKey(idk_m_key,idk_l_key),
    
IDK_DefineKey(idk_n_key,idk_m_key),
    
IDK_DefineKey(idk_o_key,idk_n_key),
    
IDK_DefineKey(idk_p_key,idk_o_key),
    
IDK_DefineKey(idk_q_key,idk_p_key),
    
IDK_DefineKey(idk_r_key,idk_q_key),
    
IDK_DefineKey(idk_s_key,idk_r_key),
    
IDK_DefineKey(idk_t_key,idk_s_key),
    
IDK_DefineKey(idk_u_key,idk_t_key),
    
IDK_DefineKey(idk_v_key,idk_u_key),
    
IDK_DefineKey(idk_w_key,idk_v_key),
    
IDK_DefineKey(idk_x_key,idk_w_key),
    
IDK_DefineKey(idk_y_key,idk_x_key),

IDK_DefineKey(idk_z_key,idk_y_key),
    
IDK_DefineKey(idk_right_key,idk_z_key),
    
IDK_DefineKey(idk_left_key,idk_right_key),
    
IDK_DefineKey(idk_up_key,idk_left_key),
    
IDK_DefineKey(idk_down_key,idk_up_key),
    
IDK_DefineKey(idk_numpad_1,idk_down_key),
    
IDK_DefineKey(idk_numpad_2,idk_numpad_1),
    
IDK_DefineKey(idk_numpad_3,idk_numpad_2),
    
IDK_DefineKey(idk_numpad_4,idk_numpad_3),
    
IDK_DefineKey(idk_numpad_5,idk_numpad_4),
    
IDK_DefineKey(idk_numpad_6,idk_numpad_5),
    
IDK_DefineKey(idk_numpad_7,idk_numpad_6),
    
IDK_DefineKey(idk_numpad_8,idk_numpad_7),
    
IDK_DefineKey(idk_numpad_9,idk_numpad_8),
    
IDK_DefineKey(idk_bs_key,idk_numpad_9),

IDK_DefineKey(idk_space_key,idk_bs_key),
    
IDK_DefineKey(idk_key_1,idk_space_key),

IDK_DefineKey(idk_key_2,idk_key_1),

IDK_DefineKey(idk_key_3,idk_key_2),

IDK_DefineKey(idk_key_4,idk_key_3),
    
IDK_DefineKey(idk_key_5,idk_key_4),
    
IDK_DefineKey(idk_key_6,idk_key_5),

IDK_DefineKey(idk_key_7,idk_key_6),
    
IDK_DefineKey(idk_key_8,idk_key_7),
    
IDK_DefineKey(idk_key_9,idk_key_8),
    
} IDK_Key ;

typedef enum { idk_resource_file, idk_data_file } IDKLoadFileType ;

void IDK_setMouseActive( IDKWindow window ) ;
void IDK_setMouseInactive( IDKWindow window )  ;
float IDK_GetMouseX( IDKWindow window ) ;
float IDK_GetMouseY( IDKWindow window ) ;
int IDK_GetLeftMouseButton( IDKWindow window ) ;
int IDK_GetRightMouseButton( IDKWindow window ) ;
int IDK_GetMiddleMouseButton( IDKWindow window ) ;
int IDK_GetKey( IDKWindow window, int key ) ;

#define IDK_KeyboardMacro(window,action,press,release,key) \
if ( action == press ) { \
IDK_SetKey(window,key) ; \
} else if ( action == release ) {\
IDK_ResetKey(window,key) ;\
}

#define IDK_MouseMacro(window,action,press,release,button) \
if ( action == press ) { \
IDK_Set ## button ## MouseButton(window) ; \
} else if ( action == release ) {\
IDK_Reset ## button ## MouseButton(window) ;\
}

IDKApp IDK_NewApp( RKString AppName, float Version, IDKErrorCallBackFuncType ErrorCallBackFunc ) ;

void IDK_DestroyApp( IDKApp App ) ;

RKString IDK_GetAppName( IDKApp App ) ;

float IDK_GetAppVersion( IDKApp App ) ;

void IDKLog( IDKApp App, const char* string, int newline, int error ) ;

void IDKLogInt( IDKApp App, int val, int newline, int error ) ;

void IDKLogFloat( IDKApp App, float val, int newline, int error ) ;

void IDKLogDouble( IDKApp App, double val, int newline, int error ) ;

void IDK_SetRasterResizeFunc( IDKWindow window, IDKRasterResizeFuncType RasterResizeFunc ) ;

IDKWindow IDK_NewWindow( IDKApp App, int win_width, int win_height, const char* win_title, IDKRasterResizeFuncType RasterResizeFunc ) ;

void IDK_DestroyWindow( IDKWindow window ) ;

void IDK_EnableVsync( IDKWindow window ) ;

void IDK_DisableVsync( IDKWindow window ) ;

void IDK_SetWindowContextCurrent( IDKWindow window ) ;

void IDK_CloseWindow( IDKWindow window ) ;

void IDK_WindowRunLoop( IDKWindow window, IDKWindowRunLoopFuncType IDKWindowRunLoopFunc, RKArgs RunArgs, IDKWindowQuitRunLoopFuncType IDKWindowQuitRunLoopFunc, RKArgs QuitArgs )  ;

void IDK_WindowRunLoopWithTime( IDKWindow window, double seconds, IDKWindowRunLoopFuncType IDKWindowRunLoopFunc, RKArgs RunArgs, IDKWindowQuitRunLoopFuncType IDKWindowQuitRunLoopFunc, RKArgs QuitArgs ) ;

void* IDK_GetPtrToGLFWWindow( IDKWindow window ) ;

void IDK_SetPtrFromWindow( IDKWindow window, void* ptr ) ;

void* IDK_GetPtrFromWindow( IDKWindow window ) ;

IDKApp IDK_GetAppFromWindow( IDKWindow window ) ;

void IDK_SetWindowSize( IDKWindow window, int width, int height ) ;

void IDK_GetWindowSize( IDKWindow window, int *width, int *height ) ;

void IDK_GetRasterSize( IDKWindow window, int* width, int* height ) ;

void IDK_EnterFullscreen( IDKWindow window ) ;

void IDK_ExitFullscreen( IDKWindow window ) ;

char* IDK_GetFilePathForPlatform( IDKApp App, IDKLoadFileType filetype, const char* path ) ;

FILE* IDK_LoadFile( IDKApp App, IDKLoadFileType filetype, const char* path, const char* mode ) ;

void IDK_UnloadFile( FILE* file ) ;

int IDK_Timer( long* time_count, long seconds ) ;

int IDK_Timer_HP( IDKApp App, double* time_count, double seconds ) ;

double IDK_GetTimeHP( IDKApp App ) ;

void IDK_SetErrorCallback( IDKApp App, IDKErrorCallBackFuncType ErrorCallBack ) ;

IDKTextLine IDK_NewTextLine( IDKPage Page ) ;

void IDK_DestroyTextLine( IDKTextLine TextLine ) ;

const char* IDK_GetTextLine( IDKTextLine TextLine ) ;

IDKPage IDK_NewPage( IDKWindow window, int max_num_of_text ) ;

void IDK_DestroyPage( IDKPage Page ) ;

void IDK_AddCharToPageList( IDKWindow window, char c ) ;

void IDK_AddCharToPage( char c, IDKPage Page ) ;

void IDK_PageSetMode( IDKPage Page, int mode ) ;

void IDK_PageMoveCursorLeft( IDKPage Page ) ;

void IDK_PageMoveCursorRight( IDKPage Page ) ;

void IDK_PageSetCursor( IDKPage Page, int cursor ) ;

int IDK_PageGetTextNum( IDKPage Page ) ;

void IDK_PageSetCharAtCursor( IDKPage Page, char c ) ;

char IDK_PageGetCharAtCursor( IDKPage Page ) ;

const char* IDK_PageGetString( IDKPage Page ) ;

void IDK_SpawnThreads( IDKApp App ) ;

void IDK_KillThreads( IDKApp App ) ;

RKTasks_ThreadGroup IDK_GetThreads( IDKApp App ) ;

float IDK_GetFPS( IDKApp App ) ;

double IDK_GetFPSHP( IDKApp App ) ;

void IDK_GetFPSString( IDKApp App, char* string ) ;

void IDK_GetFPSHPString( IDKApp App, char* string );

#endif /* defined(__IDKApp__IDK__) */
