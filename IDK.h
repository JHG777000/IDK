/*
 Copyright (c) 2016 Jacob Gordon. All rights reserved.
 
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

#ifdef __APPLE__

#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL

#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


#include "RKMem.h"
#include "RKTasks.h"
#include "codename.h"

typedef codename_scene raster_scene ;

typedef JHGInt* IDKRawData ;

typedef double (*IDKGetTimeHPFuncType)(void) ;

typedef struct IDKWindow_s* IDKWindow ;

typedef void (*IDKFullScreenFuncType)( const void* fullscreen_state) ;

typedef void (*IDKRasterResizeFuncType)(int width, int height) ;

typedef void (*IDKWindowRunLoopFuncType)(void) ;

typedef void (*IDKWindowQuitRunLoopFuncType)(void) ;

typedef void (*IDKInitErrorCallBackFuncType)(void) ;

typedef struct IDK_Page_s* IDKPage ;

typedef struct IDK_Line_s* IDKTextLine ;

typedef struct { float red ; float blue ; float green  ; } IDKColor ;

typedef struct { float x ; float y ; } IDKPoint ;

struct IDKDrawArea_s ;

typedef struct IDKDrawArea_s* IDKDrawArea ;

typedef void (*IDKDrawFunc)(IDKDrawArea area) ;

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

void IDK_theMouse( float x, float y ) ;
void IDK_setMouseActive( void ) ;
void IDK_setMouseInactive( void ) ;
int IDK_GetdoDisplayNeedUpdate(void) ;
void IDK_SetDisplayNeedsToUpdate(void) ;
void IDK_SetDisplayNeedsToUpdateOnTime( double seconds ) ;
void IDK_SetDisplayNeedsToUpdateNextFrame( void ) ;
void IDK_SetDisplayNeedsToUpdateRate( int num_of_fames_to_update, int num_of_fames_to_not_update ) ;
float IDK_GetMouseX( void ) ;
float IDK_GetMouseY( void ) ;
void IDK_SetLeftMouseButton(void) ;
int IDK_GetLeftMouseButton(void) ;
void IDK_SetRightMouseButton(void) ;
int IDK_GetRightMouseButton(void) ;
void IDK_SetMiddleMouseButton(void) ;
int IDK_GetMiddleMouseButton(void) ;
void IDK_SetKey( int key ) ;
int IDK_GetKey( int key ) ;
void IDK_ResetKey( int key ) ;
void IDK_ResetLeftMouseButton(void) ;
void IDK_ResetRightMouseButton(void) ;
void IDK_ResetMiddleMouseButton(void) ;
void IDK_EnableRefresh(void) ;
void IDK_DisableRefresh(void) ;

#define IDK_KeyboardMacro(action,press,release,key) \
if ( action == press ) { \
IDK_SetKey(key) ; \
} else if ( action == release ) {\
IDK_ResetKey(key) ;\
}

#define IDK_MouseMacro(action,press,release,button) \
if ( action == press ) { \
IDK_Set ## button ## MouseButton() ; \
} else if ( action == release ) {\
IDK_Reset ## button ## MouseButton() ;\
}

void IDK_ForceInitForIDK( void ) ;

void IDK_ForceShutdownForIDK( void ) ;

void IDKLog( const char* string, int newline, int error ) ;

void IDKLogInt( int val, int newline, int error ) ;

void IDKLogFloat( float val, int newline, int error ) ;

void IDKLogDouble( double val, int newline, int error ) ;

IDKDrawArea IDK_NewDrawArea( IDKDrawFunc drawfunc, IDKWindow window, int width, int height, float red, float blue, float green ) ;

void IDK_DestroyDrawArea( IDKDrawArea area ) ;

JHGPixels_scene IDK_GetPixelScene( IDKDrawArea area ) ;

void IDK_SetResolution( IDKDrawArea drawarea, int width, int height ) ;

IDKWindow IDK_NewWindow( int win_width, int win_height, const char* win_title, IDKRasterResizeFuncType RasterResizeFunc ) ;

void IDK_DestroyWindow( IDKWindow window ) ;

void IDK_EnableVsync( IDKWindow window ) ;

void IDK_DisableVsync( IDKWindow window ) ;

void IDK_CloseWindow( IDKWindow window ) ;

void IDK_WindowRunLoop( IDKWindow window, IDKWindowRunLoopFuncType IDKWindowRunLoopFunc, IDKWindowQuitRunLoopFuncType IDKWindowQuitRunLoopFunc ) ;

void IDK_WindowRunLoopWithTime( IDKWindow window, double seconds, IDKWindowRunLoopFuncType IDKWindowRunLoopFunc, IDKWindowQuitRunLoopFuncType IDKWindowQuitRunLoopFunc ) ;

void* IDK_GetPtrToGLFWWindow( IDKWindow window ) ;

void IDK_SetWindowSize( IDKWindow window, int width, int height ) ;

void IDK_GetWindowSize( IDKWindow window, int *width, int *height ) ;

void IDK_GetRasterSize( IDKWindow window, int* width, int* height ) ;

void IDK_SetFullScreenFlag( IDKWindow window, int is_fullscreen ) ;

void IDK_EnterFullscreen( IDKWindow window ) ;

void IDK_ExitFullscreen( IDKWindow window ) ;

FILE* IDK_LoadFile( IDKLoadFileType filetype, const char* path, const char* mode ) ;

void IDK_UnloadFile( FILE* file ) ;

void IDK_ChangeBackGroundColor( IDKDrawArea drawarea, float red, float blue, float green ) ;

IDKRawData IDK_GetFrame( IDKDrawArea drawarea, int *x, int * y ) ;

IDKRawData IDK_Draw( IDKDrawArea area, int *x, int * y ) ;

IDKWindow IDK_GetWindowFromDrawArea( IDKDrawArea area ) ;

int IDK_Timer( long* time_count, long seconds ) ;

int IDK_Timer_HP( double* time_count, double seconds ) ;

void IDK_SetGetTimeHPFunc( IDKGetTimeHPFuncType IDKGetTimeHPFunc ) ;

double IDK_GetTimeHP( void ) ;

void IDK_SetInitErrorCallback( IDKInitErrorCallBackFuncType InitErrorCallBack ) ;

IDKTextLine IDK_NewTextLine( IDKPage Page ) ;

void IDK_DestroyTextLine( IDKTextLine TextLine ) ;

const char* IDK_GetTextLine( IDKTextLine TextLine ) ;

IDKPage IDK_NewPage( int max_num_of_text ) ;

void IDK_DestroyPage( IDKPage Page ) ;

void IDK_AddCharToPageList( char c ) ;

void IDK_AddCharToPage( char c, IDKPage Page ) ;

void IDK_PageSetMode( IDKPage Page, int mode ) ;

void IDK_PageMoveCursorLeft( IDKPage Page ) ;

void IDK_PageMoveCursorRight( IDKPage Page ) ;

void IDK_PageSetCursor( IDKPage Page, int cursor ) ;

int IDK_PageGetTextNum( IDKPage Page ) ;

void IDK_PageSetCharAtCursor( IDKPage Page, char c ) ;

char IDK_PageGetCharAtCursor( IDKPage Page ) ;

const char* IDK_PageGetString( IDKPage Page ) ;

void IDK_SpawnThreads( IDKDrawArea area ) ;

void IDK_KillThreads( IDKDrawArea area ) ;

RKTasks_ThreadGroup IDK_GetThreads( IDKDrawArea area ) ;

void IDK_SetPan( IDKDrawArea drawarea, float x, float y ) ;

void IDK_SetZoom( IDKDrawArea drawarea, float zoom ) ;

float IDK_GetZoom( IDKDrawArea drawarea ) ;

void IDK_Progressbar( IDKDrawArea drawarea, float percent, float size_x, float size_y, float x, float y, float red, float blue, float green ) ;

void IDK_FloatLine( IDKDrawArea drawarea, float x1, float y1, float x2, float y2, float red, float blue, float green ) ;

void IDK_FloatLine3D( IDKDrawArea drawarea, float x1, float y1, float x2, float y2, float z1, float z2, float red, float blue, float green ) ;

void IDK_Line( IDKDrawArea drawarea, float x1, float y1, float x2, float y2, float thickness, float red, float blue, float green ) ;

void IDK_Circle( IDKDrawArea drawarea, float a, float b, float r, float red, float blue, float green ) ;

void IDK_Rect( IDKDrawArea drawarea, float size_x, float size_y, float x, float y, float red, float blue, float green ) ;

cn_point IDK_GetPixelFromPoint( IDKDrawArea drawarea, float x, float y ) ;

void IDK_SetPoint( IDKDrawArea drawarea, int x, int y, float red, float blue, float green ) ;

void IDK_SetDot( IDKDrawArea drawarea, float x, float y, float red, float blue, float green ) ;

void IDK_SetColor( IDKDrawArea drawarea, int x, int y, IDKColor color ) ;

void IDK_SetDotColor( IDKDrawArea drawarea, float x, float y, IDKColor color ) ;

IDKColor IDK_Colorit(float red, float blue, float green) ;

IDKColor IDK_Colorthat(float that) ;

IDKColor IDK_Color_add(IDKColor color_a, IDKColor color_b) ;

IDKColor IDK_Color_sub(IDKColor color_a, IDKColor color_b) ;

IDKColor IDK_Color_add(IDKColor color_a, IDKColor color_b) ;

IDKColor IDK_Color_mul(IDKColor color_a, IDKColor color_b) ;

IDKColor IDK_Color_div(IDKColor color_a, IDKColor color_b) ;

IDKColor IDK_Color_map(IDKColor color, double A, double y) ;

float IDK_GetFPS( void ) ;

double IDK_GetFPSHP( void ) ;

void IDK_DisplayFrameRate( IDKDrawArea drawarea, float size, float x, float y, float red, float blue, float green ) ;

void IDK_DisplayFrameRateHP( IDKDrawArea drawarea, float size, float x, float y, float red, float blue, float green ) ;

void IDK_DisplayInt( IDKDrawArea drawarea, int val, float size, float x, float y, float red, float blue, float green ) ;

void IDK_String( IDKDrawArea drawarea, const char* string, float size, float x, float y, float red, float blue, float green ) ;

#endif /* defined(__IDKApp__IDK__) */
