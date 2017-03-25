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


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "IDK.h"

static FILE* IDKLogFile = NULL ;

static IDKInitErrorCallBackFuncType InitErrorCallBackFunc = NULL ;

struct IDKDrawArea_s { RKTasks_ThreadGroup Threads ; codename_pixelcolor background ; raster_scene r_scene ;
    
IDKDrawFunc drawfunc ; IDKWindow window ; int raster_size ; int rect_x ; int rect_y ; float size_x ; float size_y ; float zoom ; } ;

struct IDKWindow_s { int is_fullscreen ; const void* fullscreen_state ; IDKRasterResizeFuncType RasterResizeFunc ; IDKFullScreenFuncType
    
EnterFullScreen ; IDKFullScreenFuncType ExitFullScreen ; GLFWwindow* glfw_window ; void* ptr ; } ;

static IDKGetTimeHPFuncType IDKGetTimeHPFunc_ = NULL ;

struct IDK_Page_s { char* text ; int num_of_text ; int new ; int max_num_of_text ; int cursor ; float pos ; int init ; int mode ; RKList_node node ; } ;

struct IDK_Line_s { IDKPage Page ; int mode ; char flash ; int lock ; } ;

static RKList PageList = NULL ;

static const char* idk_string_error = " " ;

static int num_of_keys = 100 ;

static int* key_array = NULL ;

static int left = 0 ;

static int middle = 0 ;

static int right = 0 ;

static int forceupdate = 0 ;

static int idk_refresh = 1 ;

static int active = 0 ;

static float MouseX = 0 ;

static float MouseY = 0 ;

static float oldX = 0 ;

static float oldY = 0 ;

static void IDK_PlatformInit( int mode ) ;


int IDK_Timer( long* time_count, long seconds ) {
    
    long the_time = ((unsigned)time(NULL) - (*time_count)) ;
    
    if ( the_time >= seconds ) {
        
        (*time_count) = (unsigned)time(NULL) ;
        
        return 1 ;
    }
    
    return 0 ;
}

int IDK_Timer_HP( double* time_count, double seconds ) {
    
    double the_time = IDK_GetTimeHP() - (*time_count) ;
    
    if ( the_time >= seconds ) {
        
        (*time_count) = IDK_GetTimeHP() ;
        
        return 1 ;
    }
    
    return 0 ;
}

int IDK_Flip( int* flip ) {
    
    *flip = !(*flip) ;
    
    return *flip ;
}

void IDK_SetInitErrorCallback( IDKInitErrorCallBackFuncType InitErrorCallBack ) {
    
    InitErrorCallBackFunc = InitErrorCallBack ;
}

static void IDKErrorCallbackForGLFW(int error, const char* description) {
    
    IDKLog("GLFW Error: ", 0, 1) ;
    
    IDKLogInt(error, 0, 1) ;
    
    IDKLog(" with description, ", 0, 1) ;
    
    IDKLog(description, 1, 1) ;
}

static void IDKKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    switch (key) {
            
        case GLFW_KEY_A:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_a_key) ;
            
            break;
            
        case GLFW_KEY_B:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_b_key) ;
            
            break;
            
        case GLFW_KEY_C:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_c_key) ;
            
            break;
            
        case GLFW_KEY_D:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_d_key) ;
            
            break;
            
        case GLFW_KEY_E:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_e_key) ;
            
            break;
            
        case GLFW_KEY_F:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_f_key) ;
            
            break;
            
        case GLFW_KEY_G:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_g_key) ;
            
            break;
            
        case GLFW_KEY_H:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_h_key) ;
            
            break;
            
            
        case GLFW_KEY_I:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_i_key) ;
            
            break;
            
        case GLFW_KEY_J:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_j_key) ;
            
            break;
            
        case GLFW_KEY_K:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_k_key) ;
            
            break;
            
        case GLFW_KEY_L:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_l_key) ;
            
            break;
            
        case GLFW_KEY_M:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_m_key) ;
            
            break;
            
        case GLFW_KEY_N:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_n_key) ;
            
            break;
            
        case GLFW_KEY_O:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_o_key) ;
            
            break;
            
        case GLFW_KEY_P:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_p_key) ;
            
            break;
            
        case GLFW_KEY_Q:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_q_key) ;
            
            break;
            
        case GLFW_KEY_R:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_r_key) ;
            
            break;
            
        case GLFW_KEY_S:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_s_key) ;
            
            break;
            
        case GLFW_KEY_T:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_t_key) ;
            
            break;
            
        case GLFW_KEY_U:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_u_key) ;
            
            break;
            
        case GLFW_KEY_V:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_v_key) ;
            
            break;
            
        case GLFW_KEY_W:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_w_key) ;
            
            break;
            
        case GLFW_KEY_X:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_x_key) ;
            
            break;
            
        case GLFW_KEY_Y:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_y_key) ;
            
            break;
            
        case GLFW_KEY_Z:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_z_key) ;
            
            break;
            
        case GLFW_KEY_RIGHT:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_right_key) ;
            
            break;
            
        case GLFW_KEY_LEFT:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_left_key) ;
            
            break;
            
        case GLFW_KEY_UP:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_up_key) ;
            
            break;
            
        case GLFW_KEY_DOWN:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_down_key) ;
            
            break;
            
        case GLFW_KEY_KP_1:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_1) ;
            
            break;
            
        case GLFW_KEY_KP_2:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_2) ;
            
            break;
            
        case GLFW_KEY_KP_3:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_3) ;
            
            break;
            
        case GLFW_KEY_KP_4:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_4) ;
            
            break;
            
        case GLFW_KEY_KP_5:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_5) ;
            
            break;
            
        case GLFW_KEY_KP_6:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_6) ;
            
            break;
            
        case GLFW_KEY_KP_7:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_7) ;
            
            break;
            
        case GLFW_KEY_KP_8:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_8) ;
            
            break;
            
        case GLFW_KEY_KP_9:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_9) ;
            
            break;
            
        case GLFW_KEY_BACKSPACE:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_bs_key) ;
            
            break;
            
        case GLFW_KEY_SPACE:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_space_key) ;
            
            break;
            
        case GLFW_KEY_1:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_key_1) ;
            
            break;
            
        case GLFW_KEY_2:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_key_2) ;
            
            break;
            
        case GLFW_KEY_3:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_key_3) ;
            
            break;
            
        case GLFW_KEY_4:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_key_4) ;
            
            break;
            
        case GLFW_KEY_5:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_key_5) ;
            
            break;
            
        case GLFW_KEY_6:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_key_6) ;
            
            break;
            
        case GLFW_KEY_7:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_key_7) ;
            
            break;
            
        case GLFW_KEY_8:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_key_8) ;
            
            break;

        case GLFW_KEY_9:
            
            IDK_KeyboardMacro(action, GLFW_PRESS, GLFW_RELEASE, idk_key_9) ;
            
            break;
            
        default:
            break;
    }
    
}

static void IDKCharacterCallback(GLFWwindow* window, unsigned int codepoint) {
    
    IDK_AddCharToPageList((char)codepoint) ;
}

static void IDKMouseFunc(IDKWindow window, double x, double y) {
    
    int WinWidth ;
    
    int WinHeight ;
    
    IDK_GetWindowSize(window, &WinWidth, &WinHeight) ;
    
    y = (y < 1) ? WinHeight : WinHeight - y ;
    
    x = x/WinWidth ;
    
    y = y/WinHeight ;
    
    IDK_theMouse(x,y) ;
}

static void IDKCursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    
    IDKWindow idk_window = (IDKWindow)glfwGetWindowUserPointer(window) ;
    
    IDKMouseFunc(idk_window, xpos, ypos) ;
}

static void IDKMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    
    switch (button) {
            
        case GLFW_MOUSE_BUTTON_RIGHT:
            IDK_MouseMacro(action,GLFW_PRESS,GLFW_RELEASE,Right) ;
            break;
            
        case GLFW_MOUSE_BUTTON_LEFT:
            IDK_MouseMacro(action,GLFW_PRESS,GLFW_RELEASE,Left) ;
            break;
            
        case GLFW_MOUSE_BUTTON_MIDDLE:
            IDK_MouseMacro(action,GLFW_PRESS,GLFW_RELEASE,Middle) ;
            break;
            
        default:
            break;
    }
}

static void IDKCursorEnterCallback(GLFWwindow* window, int entered) {
    
    if (entered) {
        
        IDK_setMouseActive() ;
        
    } else {
        
        IDK_setMouseInactive() ;
    }
}

static void IDKFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
   
    IDKWindow idk_window = (IDKWindow)glfwGetWindowUserPointer(window) ;
    
    if (idk_window->RasterResizeFunc != NULL) idk_window->RasterResizeFunc(idk_window,width,height) ;
}

static GLFWwindow* IDK_MakeGLFWWindow( int win_width, int win_height, const char* win_title ) {
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,1);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(win_width, win_height, win_title, NULL, NULL);
    
    if (!window) {
        
        if ( InitErrorCallBackFunc != NULL ) InitErrorCallBackFunc() ;
        
        glfwTerminate() ;
        
        exit(EXIT_FAILURE) ;
    }
 
    glfwMakeContextCurrent(window) ;
    
    glfwSwapInterval(1) ;
    
    glfwSetKeyCallback(window, IDKKeyCallback);
    
    glfwSetCharCallback(window, IDKCharacterCallback);
    
    glfwSetCursorPosCallback(window, IDKCursorPositionCallback);
    
    glfwSetMouseButtonCallback(window, IDKMouseButtonCallback);
    
    glfwSetCursorEnterCallback(window, IDKCursorEnterCallback);
    
    glfwSetFramebufferSizeCallback(window, IDKFramebufferSizeCallback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    return window ;
}

void IDK_EnableVsync( IDKWindow window ) {
    
    glfwMakeContextCurrent(window->glfw_window) ;
    
    glfwSwapInterval(1) ;
}

void IDK_DisableVsync( IDKWindow window ) {
    
    glfwMakeContextCurrent(window->glfw_window) ;
    
    glfwSwapInterval(0) ;
}

void IDK_SetWindowContextCurrent( IDKWindow window ) {
    
    glfwMakeContextCurrent(window->glfw_window) ;
}

void IDK_CloseWindow( IDKWindow window ) {
    
    glfwSetWindowShouldClose(window->glfw_window, 1) ;
}

void IDK_WindowRunLoopWithTime( IDKWindow window, double seconds, IDKWindowRunLoopFuncType IDKWindowRunLoopFunc, IDKWindowQuitRunLoopFuncType IDKWindowQuitRunLoopFunc ) {
    
    double time_count = IDK_GetTimeHP() ;
    
    while (!glfwWindowShouldClose(window->glfw_window)) {
    
        if ( IDK_Timer_HP(&time_count, seconds) ) {
        
           glfwPollEvents() ;
        
           IDKWindowRunLoopFunc() ;
        
           glfwSwapBuffers(window->glfw_window) ;
            
        }
    }
    
    IDKWindowQuitRunLoopFunc() ;
}

void IDK_WindowRunLoop( IDKWindow window, IDKWindowRunLoopFuncType IDKWindowRunLoopFunc, IDKWindowQuitRunLoopFuncType IDKWindowQuitRunLoopFunc ) {
    
    while (!glfwWindowShouldClose(window->glfw_window)) {
        
        glfwPollEvents() ;
        
        IDKWindowRunLoopFunc() ;
        
        glfwSwapBuffers(window->glfw_window) ;
    }
    
    IDKWindowQuitRunLoopFunc() ;
}

void* IDK_GetPtrToGLFWWindow( IDKWindow window ) {
    
    return (void*)window->glfw_window ;
}

void IDK_SetPtrFromWindow( IDKWindow window, void* ptr ) {
    
    window->ptr = ptr ;
}

void* IDK_GetPtrFromWindow( IDKWindow window ) {
    
    return window->ptr ;
}

void IDK_SetRasterResizeFunc( IDKWindow window, IDKRasterResizeFuncType RasterResizeFunc ) {
    
    window->RasterResizeFunc = RasterResizeFunc ;
}

IDKWindow IDK_NewWindow( int win_width, int win_height, const char* win_title, IDKRasterResizeFuncType RasterResizeFunc ) {
   
    IDK_PlatformInit(1) ;
    
    IDKWindow NewWindow = RKMem_NewMemOfType(struct IDKWindow_s) ;
    
    NewWindow->is_fullscreen = 0 ;
    
    NewWindow->RasterResizeFunc = RasterResizeFunc ;
    
    NewWindow->glfw_window = IDK_MakeGLFWWindow( win_width, win_height, win_title ) ;
    
    glfwSetWindowUserPointer( NewWindow->glfw_window, (void*)NewWindow ) ;
    
    #ifdef __APPLE__
    
    const void* IDKSetupFullScreenForMac( IDKWindow idk_window ) ;
    
    NewWindow->fullscreen_state = IDKSetupFullScreenForMac( NewWindow ) ;
    
    void IDKEnterOrExitFullScreenForMac( const void* fullscreen_state ) ;
    
    NewWindow->EnterFullScreen = IDKEnterOrExitFullScreenForMac ;
    
    NewWindow->ExitFullScreen = IDKEnterOrExitFullScreenForMac ;
    
    #endif
    
    return NewWindow ;
}

void IDK_DestroyWindow( IDKWindow window ) {
    
    glfwDestroyWindow(window->glfw_window) ;
    
    #ifdef __APPLE__
    
    void IDKKillFullScreenForMac( const void* fullscreen_state ) ;
    
    IDKKillFullScreenForMac(window->fullscreen_state) ;
    
    #endif
    
    IDK_PlatformInit(0) ;
    
    free(window) ;
}

void IDK_SetWindowSize( IDKWindow window, int width, int height ) {
    
   glfwSetWindowSize(window->glfw_window, width, height) ;
}

void IDK_GetWindowSize( IDKWindow window, int *width, int *height ) {
    
     glfwGetWindowSize(window->glfw_window, width, height) ;
}

void IDK_GetRasterSize( IDKWindow window, int* width, int* height ) {
    
    glfwGetFramebufferSize(window->glfw_window, width, height) ;
}

void IDK_SetFullScreenFlag( IDKWindow window, int is_fullscreen ) {
    
    if ( is_fullscreen ) {
        
        window->is_fullscreen = 1 ;
        
    } else {
       
        window->is_fullscreen = 0 ;
    }
}

void IDK_EnterFullscreen( IDKWindow window ) {
        
  if (!(window->is_fullscreen)) {
           
     IDK_SetFullScreenFlag(window,1) ;
            
     window->EnterFullScreen(window->fullscreen_state) ;
  }
}

void IDK_ExitFullscreen( IDKWindow window ) {
    
  if ((window->is_fullscreen)) {
            
     IDK_SetFullScreenFlag(window,0) ;
            
     window->ExitFullScreen(window->fullscreen_state) ;
  }
}

IDKTextLine IDK_NewTextLine( IDKPage Page ) {
    
    IDKTextLine TextLine = RKMem_NewMemOfType(struct IDK_Line_s) ;
    
    TextLine->flash = '&' ;
    
    TextLine->mode = 0 ;
    
    TextLine->lock = 0 ;
    
    TextLine->Page = Page ;
    
    return TextLine ;
}

void IDK_DestroyTextLine( IDKTextLine TextLine ) {
    
    free(TextLine) ;
}

const char* IDK_GetTextLine( IDKTextLine TextLine ) {
    
    if ( !TextLine->mode ) {
        
        if (IDK_PageGetCharAtCursor(TextLine->Page) == '&') IDK_PageSetCharAtCursor(TextLine->Page, TextLine->flash) ;
        
        TextLine->Page->pos = 1 ;
        
        IDK_PageSetCursor( TextLine->Page, (TextLine->Page->pos*TextLine->Page->num_of_text) ) ;
    }
    
    IDK_PageSetMode(TextLine->Page, TextLine->mode) ;
    
        if ( IDK_GetKey(idk_left_key) ) {
            
            if (IDK_PageGetCharAtCursor(TextLine->Page) == '&') IDK_PageSetCharAtCursor(TextLine->Page, TextLine->flash) ;
            
            IDK_PageMoveCursorLeft(TextLine->Page) ;
            
            TextLine->lock = 0 ;
        }
        
        if ( IDK_GetKey(idk_right_key) ) {
            
            if (IDK_PageGetCharAtCursor(TextLine->Page) == '&') IDK_PageSetCharAtCursor(TextLine->Page, TextLine->flash) ;
            
            IDK_PageMoveCursorRight(TextLine->Page) ;
            
            TextLine->lock = 0 ;
        }
        
        if ( !TextLine->lock ) {
            
            TextLine->flash = IDK_PageGetCharAtCursor(TextLine->Page) ;
            
            IDK_PageSetCharAtCursor(TextLine->Page, '&') ;
            
            TextLine->lock++ ;
        }
    
    if ( TextLine->Page->pos < 1 ) {
        
        TextLine->mode = 1 ;
        
    } else {
        
        TextLine->mode = 0 ;
    }
    
    return IDK_PageGetString(TextLine->Page) ;
}

IDKPage IDK_NewPage( int max_num_of_text ) {
    
    IDKPage Page = RKMem_NewMemOfType(struct IDK_Page_s) ;
    
    Page->max_num_of_text = max_num_of_text+1 ;
    
    Page->num_of_text = 0 ;
    
    Page->cursor = 0 ;
    
    Page->init = 0 ;
    
    Page->mode = 0 ;
    
    Page->pos = 0 ;
    
    Page->new = 0 ;
    
    Page->text = NULL ;
    
    Page->node = RKList_AddToList(PageList, (void*)Page) ;
    
    return Page ;
    
}

void IDK_DestroyPage( IDKPage Page ) {
    
    RKList_DeleteNode(PageList, Page->node) ;
    
    free( Page->text ) ;
    
    free( Page ) ;
}

void IDK_AddCharToPageList( char c ) {
    
    RKList_node node = RKList_GetFirstNode(PageList) ;
    
    IDKPage Page = NULL ;
    
    while ( node != NULL ) {
        
        Page = (IDKPage)RKList_GetData(node) ;
        
        IDK_AddCharToPage(c, Page) ;
        
        node = RKList_GetNextNode(node) ;
    }
}

void IDK_AddCharToPage( char c, IDKPage Page ) {
    
    Page->new = 1 ;
    
    if ( Page->mode ) {
        
        IDK_PageSetCharAtCursor(Page, c) ;
        
        return ;
    }
    
    Page->num_of_text++ ;
    
    if ( !Page->init ) Page->num_of_text++ ;
    
    if ( Page->num_of_text > Page->max_num_of_text ) {
        
        Page->num_of_text-- ;
        
        if ( !Page->init ) Page->num_of_text-- ;
        
        return ;
    }
    
    if ( Page->text == NULL ) {
        
        Page->text = RKMem_CArray(Page->num_of_text, char) ;
        
    } else {
        
        Page->text = RKMem_Realloc(Page->text, Page->num_of_text, Page->num_of_text-1, char, 1) ;
        
    }
    
    Page->text[Page->num_of_text-2] = c ;
    
    Page->text[Page->num_of_text-1] = '\0' ;
    
    if ( !Page->init ) Page->init++ ;
    
}

void IDK_PageSetMode( IDKPage Page, int mode ) {
    
    Page->mode = mode ;
}

void IDK_PageMoveCursorLeft( IDKPage Page ) {
    
    Page->pos -= 0.01f ;
    
    IDK_PageSetCursor( Page, (Page->pos*Page->num_of_text) ) ;
}

void IDK_PageMoveCursorRight( IDKPage Page ) {
    
    Page->pos += 0.01f ;
    
    IDK_PageSetCursor( Page, (Page->pos*Page->num_of_text) ) ;
}

void IDK_PageSetCursor( IDKPage Page, int cursor ) {
    
    cursor-- ;
    
    if ( (cursor < 0) ) cursor = 0 ;
    
    if ( (cursor > Page->num_of_text-2) ) cursor = Page->num_of_text-2 ;
    
    Page->cursor = cursor ;
}

int IDK_PageGetTextNum( IDKPage Page ) {
    
    return Page->num_of_text ;
}

void IDK_PageSetCharAtCursor( IDKPage Page, char c ) {
    
    if ( Page->text == NULL ) return ;
    
    Page->text[Page->cursor] = c ;
}

char IDK_PageGetCharAtCursor( IDKPage Page ) {
    
    if ( Page->text == NULL ) return ' ' ;
    
    return Page->text[Page->cursor] ;
}

const char* IDK_PageGetString( IDKPage Page ) {
    
    if ( Page->text == NULL ) return idk_string_error ;
    
    return Page->text ;
}

static void IDKAllocKeyboard( void ) {
    
    int i = 0 ;
    
    key_array = RKMem_CArray(num_of_keys, int) ;
    
    while ( i < num_of_keys ) {
        
        key_array[i] = 0 ;
        
        i++ ;
    }
}

static void IDKDeallocKeyboard( void ) {
    
    free(key_array) ;
}

void IDK_SetKey( int key ) {
    
    key_array[key] = 1 ;
}

int IDK_GetKey( int key ) {
    
    return key_array[key] ;
}

void IDK_ResetKey( int key ) {

     key_array[key] = 0 ;
}

void IDK_SetLeftMouseButton(void) {
    
    left = 1 ;
}

void IDK_ResetLeftMouseButton(void) {
    
    left = 0 ;
}

int IDK_GetLeftMouseButton(void) {
    
    return left ;
}

void IDK_SetRightMouseButton(void) {
    
    right = 1 ;
}

void IDK_ResetRightMouseButton(void) {
    
    right = 0 ;
}

int IDK_GetRightMouseButton(void) {
    
    return right ;
}

void IDK_ResetMiddleMouseButton(void) {
    
    middle = 0 ;
}

void IDK_SetMiddleMouseButton(void) {
    
    middle = 1 ;
}

int IDK_GetMiddleMouseButton(void) {
    
    return middle ;
}

float IDK_GetMouseX( void ) {
    
    return MouseX ;
}

float IDK_GetMouseY( void ) {
    
    return MouseY ;
}

void IDK_theMouse( float x, float y ) {
    
    if (active) {
        
        MouseX = x ;
        
        MouseY = y ;
    }
}

void IDK_setMouseActive( void ) {
    
    active = 1 ;
}

void IDK_setMouseInactive( void ) {
    
    active = 0 ;
}

void IDK_SetDisplayNeedsToUpdate( void ) {
    
    forceupdate++ ;
}

void IDK_SetDisplayNeedsToUpdateOnTime( double seconds ) {
    
    static double time_count = 0 ;
    
    if ( time_count == 0 ) time_count = IDK_GetTimeHP() ;
    
    if ( IDK_Timer_HP(&time_count, seconds) ) IDK_SetDisplayNeedsToUpdate() ;
}

void IDK_SetDisplayNeedsToUpdateNextFrame( void ) {
    
    static int flip = 0 ;
    
    IDK_Flip(&flip) ;
    
    if (flip) IDK_SetDisplayNeedsToUpdate() ;
}

void IDK_SetDisplayNeedsToUpdateRate( int num_of_fames_to_update, int num_of_fames_to_not_update ) {
    
    static int count = 0 ;
    
    static int count2 = 0 ;
    
    static int mode = 0 ;
    
    if ( count == 0 ) count = num_of_fames_to_update ;
    
    if ( count2 == 0 ) count2 = num_of_fames_to_not_update ;
    
    if ( !mode ) {
        
        count-- ;
        
        if ( count <= 0 ) {
            
            count = 0 ;
            
            mode = 1 ;
        }
        
        IDK_SetDisplayNeedsToUpdate() ;
    }
    
    if ( mode ) {
        
        count2-- ;
        
        if ( count2 <= 0 ) {
            
            count2 = 0 ;
            
            mode = 0 ;
        }

    }
}

int IDK_GetdoDisplayNeedUpdate(void) {
    
    if ( idk_refresh ) {
    
    if ( forceupdate ) {
        
        forceupdate = 0 ;
        
        return 1 ;
    }
    
    if ((MouseX != oldX) || (MouseY != oldY)) {
        
        oldX = MouseX ;
        
        oldY = MouseY ;
        
        return 1 ;
        
    }
    
    }
    
    return 0 ;
}

void IDK_EnableRefresh(void) {
    
    idk_refresh = 1 ;
}

void IDK_DisableRefresh(void) {
    
    idk_refresh = 0 ;
}

void IDK_SetGetTimeHPFunc( IDKGetTimeHPFuncType IDKGetTimeHPFunc ) {
    
   IDKGetTimeHPFunc_ = IDKGetTimeHPFunc ;
    
}

double IDK_GetTimeHP( void ) {
    
    if (IDKGetTimeHPFunc_ != NULL ) return IDKGetTimeHPFunc_() ;
    
    return 0 ;
}

static double GetFPSFunc_High( void ) {
    
    static int init = 0 ;
    
    static int numofframes = 0 ;
    
    static double time_count = 0 ;
    
    double seconds = 0 ;
    
    static double fps = 0 ;
    
    if (!init) {
        
        time_count = IDK_GetTimeHP() ;
        
        init++ ;
    }
    
    numofframes++ ;
    
    seconds = (IDK_GetTimeHP() - time_count) ;
    
    if ( seconds >= 0.25 ) {
        
        fps = (numofframes / seconds) ;
        
        numofframes = 0 ;
        
        time_count = IDK_GetTimeHP() ;
        
    }
    
    return fps ;
}

static float GetFPSFunc_Low( void ) {
    
    static int init = 0 ;
    
    static int numofframes = 0 ;
    
    static double time_count = 0 ;
    
    double seconds = 0 ;
    
    static float fps = 0 ;
    
    if (!init) {
        
        time_count = (unsigned)time(NULL) ;
        
        init++ ;
    }
    
    numofframes++ ;
    
    seconds = ((unsigned)time(NULL) - time_count) ;
    
    if ( seconds >= 1 ) {
        
        fps = (numofframes / seconds) ;
        
        numofframes = 0 ;
        
        time_count = (unsigned)time(NULL) ;
        
    }
    
    return fps ;
}

static double GetFPSFunc( int Low0High1 ) {
    
    double fps = 0 ;
    
    if ( Low0High1 ) {
       
        fps = GetFPSFunc_High() ;
        
    } else {
        
        fps = (double)GetFPSFunc_Low() ;
    }
    
    return fps ;
}

static const char* myitoa( int val, char* string ) {
    
    snprintf(string, sizeof(string), "%d", val) ;
    
    return string ;
    
}

static const char* myftoa( float val, char* string ) {
    
    snprintf(string, sizeof(string), "%f", val) ;
    
    return string ;
    
}

static const char* mydtoa( double val, char* string ) {
    
    snprintf(string, sizeof(string), "%f", val) ;
    
    return string ;
    
}

static int from_point_to_pixel( float f, int max ) {
    
    return (int)(f * max) ;
}

cn_point IDK_GetPixelFromPoint( IDKDrawArea drawarea, float x, float y ) {
    
    cn_point point ;
    
    point.x = from_point_to_pixel(x, drawarea->rect_x) ;
    
    point.y = from_point_to_pixel(y, drawarea->rect_y) ;
    
    return point ;
}

static raster_scene new_r_scene( int x, int y, codename_pixelcolor background ) {
    
    JHGPixels_scene pixelscene = JHGPixels_newscene(x, y, *background, JHGINT8888REVBGRA) ;
    
    codename_scene codenamescene = codename_NewSceneObject(pixelscene, 0, 0) ;
    
    return (raster_scene)codenamescene ;
}

void IDK_SetResolution( IDKDrawArea drawarea, int width, int height ) {
    
    JHGPixels_scenefree(drawarea->r_scene->pixelscene) ;
    
    free(drawarea->r_scene) ;
    
    drawarea->rect_x = width ;
    
    drawarea->rect_y = height ;
    
    drawarea->raster_size = MAX_JHG(drawarea->rect_x,  drawarea->rect_y) ;
    
    drawarea->r_scene = new_r_scene(drawarea->rect_x, drawarea->rect_y, drawarea->background) ;
}

void IDK_GetResolution( IDKDrawArea drawarea, int *width, int *height ) {
    
    *width = drawarea->rect_x ;
    
    *height = drawarea->rect_y ;
}

IDKRawData IDK_GetFrame( IDKDrawArea drawarea, int *x, int * y ) {
    
    return (IDKRawData)JHG_DrawPixels(drawarea->r_scene->pixelscene, x, y) ;
}

void IDK_ChangeBackGroundColor( IDKDrawArea drawarea, float red, float blue, float green ) {
    
    free(drawarea->background) ;
    
    drawarea->background = codename_NewColorObject(from_point_to_pixel(red, 255), from_point_to_pixel(blue, 255), from_point_to_pixel(green, 255)) ;
    
    JHGPixels_SetBackGroundColor(drawarea->r_scene->pixelscene, drawarea->background->r, drawarea->background->b, drawarea->background->g) ;
}

static const char* IDKGetFilePathForThePlatform( IDKLoadFileType filetype ) {
    
    #ifdef __APPLE__
    
    const char* IDKGetFilePathForMac( IDKLoadFileType filetype ) ;
    
    return IDKGetFilePathForMac(filetype) ;
    
    #endif
    
    return NULL ;    
}

 char* IDK_GetFilePathForPlatform( IDKLoadFileType filetype, const char* path ) {
    
    int size1 = 0 ;
    
    int size2 = 0 ;
    
    char* the_file_path = NULL ;
    
    const char* path_base = IDKGetFilePathForThePlatform(filetype) ;
    
    if ( path_base == NULL ) return NULL ;
    
    size1 = (int)strlen(path_base) ;
    
    size2 = (int)strlen(path) ;
    
    the_file_path = RKMem_CArray(size1+size2+2, char) ;
    
    #ifdef _WIN32
    
     sprintf(the_file_path,"%s\%s",path_base,path) ;
     
    #else
     
     sprintf(the_file_path,"%s/%s",path_base,path) ;
     
    #endif
    
    return the_file_path ;

}

FILE* IDK_LoadFile( IDKLoadFileType filetype, const char* path, const char* mode ) {
    
    FILE* file = NULL ;
    
    char* the_file_path = IDK_GetFilePathForPlatform(filetype, path) ;
    
    if ( the_file_path == NULL ) return NULL ;
    
    file = fopen(the_file_path, mode) ;
    
    free(the_file_path) ;
    
    return file ;
}

void IDK_UnloadFile( FILE* file ) {
    
    fclose(file) ;
}

static void IDKInitLogFile( void ) {
    
    IDKLogFile = IDK_LoadFile(idk_data_file, "Logfile", "w") ;
    
    if ( IDKLogFile == NULL ) {
        
        puts("PANIC: Could not create Logfile!\n") ;
        
        fputs("PANIC: Could not create Logfile!\n", stderr);
        
        exit(EXIT_FAILURE) ;
    }
    
    if ( setvbuf(IDKLogFile, NULL, _IONBF, 0) ) {
        
        puts("PANIC: setvbuf failed!\n") ;
        
        fputs("PANIC: setvbuf failed!\n", stderr);
        
        exit(EXIT_FAILURE) ;

    }
}

static void IDKKillLogFile( void ) {
    
     IDK_UnloadFile(IDKLogFile) ;
}

void IDKLog( const char* string, int newline, int error ) {
    
    if ( newline ) {
    
     if ( !error ) {
        
      printf("%s\n", string) ;
         
     } else {
      
      fprintf(stderr, "%s\n", string) ;
         
     }
    
     fprintf(IDKLogFile, "%s\n", string) ;
        
    } else  {
       
     if ( !error ) {
            
      printf("%s", string) ;
            
      } else {
            
      fprintf(stderr, "%s", string) ;
            
     }

     fprintf(IDKLogFile, "%s", string) ;
        
    }
}

void IDKLogInt( int val, int newline, int error ) {
    
    char string[100] ;
    
    myitoa(val, string) ;
    
    IDKLog(string, newline, error) ;
}

void IDKLogFloat( float val, int newline, int error ) {
    
    char string[100] ;
    
    myftoa(val, string) ;
    
    IDKLog(string, newline, error) ;
}

void IDKLogDouble( double val, int newline, int error ) {
    
    char string[100] ;
    
    mydtoa(val, string) ;
    
    IDKLog(string, newline, error) ;
}

static void IDKGlfwInit(void) {
    
    glfwSetErrorCallback(IDKErrorCallbackForGLFW) ;
    
    if (!glfwInit()) {
        
        if ( InitErrorCallBackFunc != NULL ) InitErrorCallBackFunc() ;
    }
}

static void IDKGlfwKill(void) {
    
    glfwTerminate() ;
}

static void IDK_PlatformInit( int mode ) {
    
    static int count = 0 ;
    
    if ( mode ) {
        
        count++ ;
        
        if ( count == 1 ) {
        
        IDKInitLogFile() ;
        
        IDKGlfwInit() ;
            
        IDKAllocKeyboard() ;
        
        PageList = RKList_NewList() ;
            
        IDK_SetGetTimeHPFunc(glfwGetTime) ;
            
        }
    }
    
    if ( !mode ) {
        
        count-- ;
        
        if ( count <= 0 ) {
            
            count = 0 ;
            
            IDKGlfwKill() ;
            
            IDKKillLogFile() ;
            
            IDKDeallocKeyboard() ;
            
            RKList_DeleteList(PageList) ;
        }
    }
}

void IDK_ForceInitForIDK( void ) {
    
    IDK_PlatformInit(1) ;
}

void IDK_ForceShutdownForIDK( void ) {
    
    IDK_PlatformInit(0) ;
}

IDKDrawArea IDK_NewDrawArea( IDKDrawFunc drawfunc, IDKWindow window, int width, int height, float red, float blue, float green ) {
    
    IDKDrawArea NewDrawArea = RKMem_NewMemOfType(struct IDKDrawArea_s) ;
    
    NewDrawArea->raster_size = MAX_JHG(width, height) ;
    
    NewDrawArea->background = codename_NewColorObject(red*255,blue*255,green*255) ;
    
    NewDrawArea->r_scene = new_r_scene(width, height, NewDrawArea->background) ;
    
    NewDrawArea->drawfunc = drawfunc ;
    
    NewDrawArea->window = window ;
    
    NewDrawArea->Threads = NULL ;
    
    NewDrawArea->rect_x = width ;
    
    NewDrawArea->rect_y = height  ;
    
    NewDrawArea->zoom = 1 ;
    
    IDK_PlatformInit(1) ;
    
    return NewDrawArea ;
}

void IDK_DestroyDrawArea( IDKDrawArea area ) {
    
    JHGPixels_scenefree(area->r_scene->pixelscene) ;
    
    IDK_PlatformInit(0) ;
    
    free(area->background) ;
    
    free(area->r_scene) ;
    
    free(area) ;
}

JHGPixels_scene IDK_GetPixelScene( IDKDrawArea area ) {
    
    return area->r_scene->pixelscene ;
}

raster_scene IDK_GetRasterScene( IDKDrawArea area ) {
    
    return area->r_scene ;
}

IDKRawData IDK_Draw( IDKDrawArea area, int *x, int * y ) {
    
    if (IDK_GetdoDisplayNeedUpdate())JHGPixels_Reset(area->r_scene->pixelscene) ;
    
    area->drawfunc(area) ;
    
    return IDK_GetFrame(area, x, y) ;
}

IDKWindow IDK_GetWindowFromDrawArea( IDKDrawArea area ) {
    
    return area->window ;
}

void IDK_SpawnThreads( IDKDrawArea area ) {
    
    if ( area->Threads == NULL ) area->Threads = RKTasks_NewThreadGroup(10) ;
}

void IDK_KillThreads( IDKDrawArea area ) {
    
    if ( area->Threads != NULL ) {
        
        RKTasks_DestroyThreadGroup(area->Threads)  ;
        
        area->Threads = NULL ;
    }
}

RKTasks_ThreadGroup IDK_GetThreads( IDKDrawArea area ) {
    
    return area->Threads ;
}

void IDK_SetPan( IDKDrawArea drawarea, float x, float y ) {
    
 cn_point point = IDK_GetPixelFromPoint(drawarea, x, y) ;
    
 codename_movescene(drawarea->r_scene, point) ;
    
}

void IDK_SetZoom( IDKDrawArea drawarea, float zoom ) {
    
    drawarea->zoom = zoom ;
}

float IDK_GetZoom( IDKDrawArea drawarea ) {
    
    return drawarea->zoom ;
}

void IDK_Progressbar( IDKDrawArea drawarea, float percent, float size_x, float size_y, float x, float y, float red, float blue, float green ) {
    
    IDK_Rect(drawarea,size_x*percent,size_y,x,y,red,blue,green) ;
}

//Bresenham's line algorithm

void IDK_FloatLine( IDKDrawArea drawarea, float x1, float y1, float x2, float y2, float red, float blue, float green ) {
    
    float X1 = from_point_to_pixel(x1*drawarea->zoom,drawarea->rect_x) ;
    
    float X2 = from_point_to_pixel(x2*drawarea->zoom,drawarea->rect_x) ;
    
    float Y1 = from_point_to_pixel(y1*drawarea->zoom,drawarea->rect_y) ;
    
    float Y2 = from_point_to_pixel(y2*drawarea->zoom,drawarea->rect_y) ;
    
    if ( X1 == X2 ) X2 += 1 ;
    
    if ( Y1 == Y2 ) Y2 += 1 ;
    
    float dx = X2 - X1 ;
    
    float dy = Y2 - Y1 ;
    
    float error = 0 ;
    
    float delta = ABS_JHG(dy/dx) ;
    
    float x = X1 ;
    
    float y = Y1 ;
    
    while ( x != X2 ) {
        
        IDK_SetPoint(drawarea, x, y, red, blue, green) ;
        
        x = x + ZSGN_JHG(X2 - X1) ;
        
        error = error + delta ;
        
        while ( error >= 0.000001 ) {
            
            IDK_SetPoint(drawarea, x, y, red, blue, green) ;
            
            y = y + ZSGN_JHG(Y2 - Y1) ;
            
            error = error - 1.0 ;
        }
    }
}

void IDK_FloatLine3D( IDKDrawArea drawarea, float x1, float y1, float x2, float y2, float z1, float z2, float red, float blue, float green ) {
    
    float X1 = from_point_to_pixel(x1*drawarea->zoom,drawarea->rect_x) ;
    
    float X2 = from_point_to_pixel(x2*drawarea->zoom,drawarea->rect_x) ;
    
    float Y1 = from_point_to_pixel(y1*drawarea->zoom,drawarea->rect_y) ;
    
    float Y2 = from_point_to_pixel(y2*drawarea->zoom,drawarea->rect_y) ;
    
    float Z1 = from_point_to_pixel(z1,1024) ;
    
    float Z2 = from_point_to_pixel(z2,1024) ;
    
    if ( X1 == X2 ) X2 += 1 ;
    
    if ( Y1 == Y2 ) Y2 += 1 ;
    
    if ( Z1 == Z2 ) Z2 += 1 ;
    
    float dx = X2 - X1 ;
    
    float dy = Y2 - Y1 ;
    
    float dz = Z2 - Z1 ;
    
    float error = 0 ;
    
    float error2 = 0 ;
    
    float delta = ABS_JHG(dy/dx) ;
    
    float delta2 = ABS_JHG(dz/dy) ;
    
    float x = X1 ;
    
    float y = Y1 ;
    
    float z = Z1 ;
    
    while ( x != X2 ) {
        
        IDK_SetPoint(drawarea, x, y, z/1024, z/1024, z/1024) ;
        
        x = x + ZSGN_JHG(X2 - X1) ;
        
        error = error + delta ;
        
        while ( error >= 0.000001 ) {
            
            IDK_SetPoint(drawarea, x, y, z/1024, z/1024, z/1024) ;
            
            y = y + ZSGN_JHG(Y2 - Y1) ;
            
            error = error - 1.0 ;
            
            error2 = error2 + delta2 ;
            
            while ( error2 >= 0.000001 ) {
                
                IDK_SetPoint(drawarea, x, y, z/1024, z/1024, z/1024) ;
                
                z = z + ZSGN_JHG(Z2 - Z1) ;
                
                error2 = error2 - 1.0 ;
            }

        }
    }
}

void IDK_Line( IDKDrawArea drawarea, float x1, float y1, float x2, float y2, float thickness, float red, float blue, float green ) {
    
    thickness = ((thickness*0.1*drawarea->zoom) <= 0.8) ? 0.04 : thickness*0.1*drawarea->zoom ;
    
    cnpoint_linesegment_with_thickness(drawarea->r_scene, from_point_to_pixel(x1*drawarea->zoom,drawarea->rect_x), from_point_to_pixel(y1*drawarea->zoom,drawarea->rect_y), from_point_to_pixel(x2*drawarea->zoom,drawarea->rect_x), from_point_to_pixel(y2*drawarea->zoom,drawarea->rect_y), from_point_to_pixel(thickness, (drawarea->raster_size*thickness)), from_point_to_pixel(red,255), from_point_to_pixel(blue,255), from_point_to_pixel(green,255)) ;
}

void IDK_Circle( IDKDrawArea drawarea, float a, float b, float r, float red, float blue, float green ) {
    
    cnpoint_wikiCircle(drawarea->r_scene, from_point_to_pixel(a*drawarea->zoom,drawarea->rect_x), from_point_to_pixel(b*drawarea->zoom,drawarea->rect_y), from_point_to_pixel(r*drawarea->zoom,drawarea->raster_size),  from_point_to_pixel(red,255), from_point_to_pixel(blue,255), from_point_to_pixel(green,255)) ;
}

void IDK_Rect( IDKDrawArea drawarea, float size_x, float size_y, float x, float y, float red, float blue, float green ) {
    
    cnpoint_Rect(drawarea->r_scene, from_point_to_pixel(size_x*drawarea->zoom,drawarea->rect_x), from_point_to_pixel(size_y*drawarea->zoom,drawarea->rect_y), from_point_to_pixel(x*drawarea->zoom,drawarea->rect_x), from_point_to_pixel(y*drawarea->zoom,drawarea->rect_y), from_point_to_pixel(red,255), from_point_to_pixel(blue,255), from_point_to_pixel(green,255)) ;
}

void IDK_Square( IDKDrawArea drawarea, float size, float x, float y, float red, float blue, float green ) {
    
    cnpoint_Rect(drawarea->r_scene, from_point_to_pixel(size*drawarea->zoom,drawarea->raster_size), from_point_to_pixel(size*drawarea->zoom,drawarea->raster_size), from_point_to_pixel(x*drawarea->zoom,drawarea->rect_x), from_point_to_pixel(y*drawarea->zoom,drawarea->rect_y), from_point_to_pixel(red,255), from_point_to_pixel(blue,255), from_point_to_pixel(green,255)) ;
}

void IDK_SetPoint( IDKDrawArea drawarea, int x, int y, float red, float blue, float green ) {
    
    cn_SetPoint(drawarea->r_scene, x, y, from_point_to_pixel(red,255), from_point_to_pixel(blue,255), from_point_to_pixel(green,255)) ;
}

void IDK_SetDot( IDKDrawArea drawarea, float x, float y, float red, float blue, float green ) {
    
    cn_SetPoint(drawarea->r_scene, from_point_to_pixel(x,drawarea->rect_x), from_point_to_pixel(y,drawarea->rect_y), from_point_to_pixel(red,255), from_point_to_pixel(blue,255), from_point_to_pixel(green,255)) ;
}

void IDK_SetColor( IDKDrawArea drawarea, int x, int y, IDKColor color ) {
    
    IDK_SetPoint(drawarea, x, y, color.red, color.blue, color.green) ;
}

void IDK_SetDotColor( IDKDrawArea drawarea, float x, float y, IDKColor color ) {
    
    IDK_SetDot(drawarea, x, y, color.red, color.blue, color.green) ;
}

IDKColor IDK_Colorit(float red, float blue, float green) {
    
    IDKColor newcolor ;
    
    newcolor.red = red ;
    
    newcolor.blue = blue ;
    
    newcolor.green = green ;
    
    return newcolor ;
}

IDKColor IDK_Colorthat(float that) {
    
    return IDK_Colorit(that,that,that) ;
}

IDKColor IDK_Color_add(IDKColor color_a, IDKColor color_b) {
    
    return IDK_Colorit(color_a.red + color_b.red, color_a.blue + color_b.blue, color_a.green + color_b.green) ;
    
}

IDKColor IDK_Color_sub(IDKColor color_a, IDKColor color_b) {
    
    return IDK_Colorit(color_a.red - color_b.red, color_a.blue - color_b.blue, color_a.green - color_b.green) ;
    
}

IDKColor IDK_Color_mul(IDKColor color_a, IDKColor color_b) {
    
    return IDK_Colorit(color_a.red * color_b.red, color_a.blue * color_b.blue, color_a.green * color_b.green) ;
    
}

IDKColor IDK_Color_div(IDKColor color_a, IDKColor color_b) {
    
    return IDK_Colorit(color_a.red / color_b.red, color_a.blue / color_b.blue, color_a.green / color_b.green) ;
    
}

IDKColor IDK_Color_map(IDKColor color, double A, double y) {
    
    color.red = A * (pow(color.red, y)) ;
    
    color.blue = A * (pow(color.blue, y)) ;
    
    color.green = A * (pow(color.green, y)) ;
    
    return color ;
}

float IDK_GetFPS( void ) {
    
    return GetFPSFunc(0) ;
}

double IDK_GetFPSHP( void ) {
    
    return GetFPSFunc(1) ;
}

void IDK_DisplayFrameRate( IDKDrawArea drawarea, float size, float x, float y, float red, float blue, float green ) {
    
    char framestring[100] ;
    
    float fps = IDK_GetFPS() ;
    
    myftoa(fps,framestring) ;
    
    IDK_String(drawarea, framestring, size, x, y, red, blue, green) ;
    
    IDK_SetDisplayNeedsToUpdate() ;
}

void IDK_DisplayFrameRateHP( IDKDrawArea drawarea, float size, float x, float y, float red, float blue, float green ) {
    
    char framestring[100] ;
    
    double fps = IDK_GetFPSHP() ;
    
    mydtoa(fps,framestring) ;
    
    IDK_String(drawarea, framestring, size, x, y, red, blue, green) ;
    
    IDK_SetDisplayNeedsToUpdate() ;
}

void IDK_DisplayInt( IDKDrawArea drawarea, int val, float size, float x, float y, float red, float blue, float green ) {
    
    char string[100] ;
    
    myitoa(val, string) ;
    
    IDK_String(drawarea, string, size, x, y, red, blue, green) ;
}

void IDK_String( IDKDrawArea drawarea, const char* string, float size, float x, float y, float red, float blue, float green ) {
    
    cnpoint_String(drawarea->r_scene, string, from_point_to_pixel(size*00.1*drawarea->zoom, (drawarea->raster_size*size)), from_point_to_pixel(x*drawarea->zoom,drawarea->rect_x),  from_point_to_pixel(y*drawarea->zoom,drawarea->rect_y),  from_point_to_pixel(red,255), from_point_to_pixel(blue,255), from_point_to_pixel(green,255)) ;
}