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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <IDK/IDK.h>

typedef struct IDKFPSState_s { int init ; int numofframes ; double time_count ; double fps ; }* IDKFPSState ;

struct IDKApp_s { RKString AppName ; IDKFPSState lowp_fps ; IDKFPSState highp_fps ; IDKGetTimeHPFuncType IDKGetTimeHPFunc ;
    
IDKErrorCallBackFuncType ErrorCallBackFunc ; FILE* LogFile ; float Version ; RKTasks_ThreadGroup Threads ;  } ;

struct IDKWindow_s { IDKApp App ; int is_fullscreen ; const void* fullscreen_state ; IDKRasterResizeFuncType RasterResizeFunc ; IDKFullScreenFuncType
    
EnterFullScreen ; IDKFullScreenFuncType ExitFullScreen ; GLFWwindow* glfw_window ; void* ptr ;  RKList PageList ; int* KeyArray ; int MouseLeft ; int MouseMiddle ;

int MouseRight ; int  MouseActive ; float MouseX  ; float MouseY ; } ;

struct IDK_Page_s { IDKWindow window ; char* text ; int num_of_text ; int new ; int max_num_of_text ; int cursor ; float pos ; int init ; int mode ; RKList_node node ; } ;

struct IDK_Line_s { IDKPage Page ; int mode ; char flash ; int lock ; } ;

static const char* idk_error_string = " " ;

static RKList AppList = NULL ;

static void IDKPlatformInit( IDKApp App, int mode ) ;

static void IDKInitLogFile( IDKApp App ) ;

static void IDKKillLogFile( IDKApp App ) ;

IDKApp IDK_NewApp( RKString AppName, float Version, IDKErrorCallBackFuncType ErrorCallBackFunc ) {
    
    IDKApp App = RKMem_NewMemOfType(struct IDKApp_s) ;
    
    App->ErrorCallBackFunc = ErrorCallBackFunc ;
    
    App->IDKGetTimeHPFunc = glfwGetTime ;
    
    App->AppName = AppName ;
    
    App->Version = Version ;
    
    App->Threads = NULL ;
    
    App->LogFile = NULL ;
    
    App->lowp_fps = NULL ;
    
    App->highp_fps = NULL ;
    
    IDKInitLogFile(App) ;
    
    IDKPlatformInit(App, 1) ;
    
    return App ;
}

void IDK_DestroyApp( IDKApp App ) {
    
    free(App->lowp_fps) ;
    
    free(App->highp_fps) ;
    
    IDKPlatformInit(App, 0) ;
    
    IDKKillLogFile(App) ;
    
    IDK_KillThreads(App) ;
    
    RKString_DestroyString(App->AppName) ;
    
    free(App) ;
}

void IDK_LogVersion( IDKApp App ) {
        
     IDKLog(App, "IDK(Interactive Development Kit), version: ", 0, 0) ;
    
     IDKLogFloat(App, 0, 0, 0) ;
    
     IDKLog(App, ", and build: ", 0, 0) ;
     
     #ifdef __APPLE__
    
     IDKLog(App, "Macintosh-", 0, 0) ;
    
     #endif
     
    #ifdef _WIN32
    
     IDKLog(App, "Windows-", 0, 0) ;
    
     #endif
    
     #ifdef DEBUG
    
     IDKLog(App, "Debug", 1, 0) ;
    
     #else
    
     IDKLog(App, "Release", 1, 0) ;
    
     #endif

     int major, minor, revision ;
    
     glfwGetVersion(&major, &minor, &revision) ;
    
     IDKLog(App, "IDK, linked against: GLFW ", 0, 0) ;
    
     IDKLogInt(App, major, 0, 0) ;
    
     IDKLog(App, ".", 0, 0) ;
    
     IDKLogInt(App, minor, 0, 0) ;
    
     IDKLog(App, ".", 0, 0) ;
    
     IDKLogInt(App, revision, 0, 0) ;
    
     IDKLog(App, " with, ", 0, 0) ;
    
     IDKLog(App, glfwGetVersionString(), 1, 0) ;
}

RKString IDK_GetAppName( IDKApp App ) {
    
    return App->AppName ;
}

float IDK_GetAppVersion( IDKApp App ) {
    
    return App->Version ;
}

int IDK_Timer( long* time_count, long seconds ) {
    
    long the_time = ((unsigned)time(NULL) - (*time_count)) ;
    
    if ( the_time >= seconds ) {
        
        (*time_count) = (unsigned)time(NULL) ;
        
        return 1 ;
    }
    
    return 0 ;
}

int IDK_Timer_HP( IDKApp App, double* time_count, double seconds ) {
    
    double the_time = IDK_GetTimeHP(App) - (*time_count) ;
    
    if ( the_time >= seconds ) {
        
        (*time_count) = IDK_GetTimeHP(App) ;
        
        return 1 ;
    }
    
    return 0 ;
}

void IDK_SetInitErrorCallback( IDKApp App, IDKErrorCallBackFuncType ErrorCallBackFunc ) {
    
    App->ErrorCallBackFunc = ErrorCallBackFunc ;
}

static void IDKErrorCallbackForGLFW(int error, const char* description) {
    
    IDKApp App = NULL ;
    
    RKList_node node = RKList_GetFirstNode(AppList) ;
    
    while (node != NULL) {
        
        App = RKList_GetData(node) ;
        
        IDKLog(App, "GLFW Error: ", 0, 1) ;
        
        IDKLogInt(App, error, 0, 1) ;
        
        IDKLog(App, " with description, ", 0, 1) ;
        
        IDKLog(App, description, 1, 1) ;
        
        if (App->ErrorCallBackFunc != NULL) App->ErrorCallBackFunc(App,idk_glfw_error) ;
        
        node = RKList_GetNextNode(node) ;
    }
    
}

static void IDKAllocKeyboard( IDKWindow window ) {
    
    int i = 0 ;
    
    int num_of_keys = 100 ;
    
    window->KeyArray = RKMem_CArray(num_of_keys, int) ;
    
    while ( i < num_of_keys ) {
        
        window->KeyArray[i] = 0 ;
        
        i++ ;
    }
}

static void IDKDeallocKeyboard( IDKWindow window ) {
    
    free(window->KeyArray) ;
}

static void IDK_SetKey( IDKWindow window, int key ) {
    
    window->KeyArray[key] = 1 ;
}

int IDK_GetKey( IDKWindow window, int key ) {
    
    return window->KeyArray[key] ;
}

static void IDK_ResetKey( IDKWindow window, int key ) {
    
    window->KeyArray[key] = 0 ;
}

static void IDK_SetLeftMouseButton( IDKWindow window ) {
    
    window->MouseLeft = 1 ;
}

static void IDK_ResetLeftMouseButton( IDKWindow window ) {
    
    window->MouseLeft = 0 ;
}

int IDK_GetLeftMouseButton( IDKWindow window ) {
    
    return window->MouseLeft ;
}

static void IDK_SetRightMouseButton( IDKWindow window ) {
    
    window->MouseRight = 1 ;
}

static void IDK_ResetRightMouseButton( IDKWindow window ) {
    
    window->MouseRight = 0 ;
}

int IDK_GetRightMouseButton( IDKWindow window ) {
    
    return window->MouseRight ;
}

static void IDK_ResetMiddleMouseButton( IDKWindow window ) {
    
    window->MouseMiddle = 0 ;
}

static void IDK_SetMiddleMouseButton( IDKWindow window ) {
    
    window->MouseMiddle = 1 ;
}

int IDK_GetMiddleMouseButton( IDKWindow window ) {
    
    return window->MouseMiddle ;
}

float IDK_GetMouseX( IDKWindow window ) {
    
    return window->MouseX ;
}

float IDK_GetMouseY( IDKWindow window ) {
    
    return window->MouseY ;
}

static void IDKtheMouse( IDKWindow window, float x, float y ) {
    
    if (window->MouseActive) {
        
        window->MouseX = x ;
        
        window->MouseY = y ;
    }
}

void IDK_setMouseActive( IDKWindow window ) {
    
    window->MouseActive = 1 ;
}

void IDK_setMouseInactive( IDKWindow window ) {
    
    window->MouseActive = 0 ;
}

static void IDKKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    IDKWindow idk_window = glfwGetWindowUserPointer(window) ;
    
    switch (key) {
            
        case GLFW_KEY_A:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_a_key) ;
            
            break;
            
        case GLFW_KEY_B:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_b_key) ;
            
            break;
            
        case GLFW_KEY_C:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_c_key) ;
            
            break;
            
        case GLFW_KEY_D:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_d_key) ;
            
            break;
            
        case GLFW_KEY_E:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_e_key) ;
            
            break;
            
        case GLFW_KEY_F:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_f_key) ;
            
            break;
            
        case GLFW_KEY_G:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_g_key) ;
            
            break;
            
        case GLFW_KEY_H:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_h_key) ;
            
            break;
            
            
        case GLFW_KEY_I:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_i_key) ;
            
            break;
            
        case GLFW_KEY_J:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_j_key) ;
            
            break;
            
        case GLFW_KEY_K:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_k_key) ;
            
            break;
            
        case GLFW_KEY_L:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_l_key) ;
            
            break;
            
        case GLFW_KEY_M:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_m_key) ;
            
            break;
            
        case GLFW_KEY_N:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_n_key) ;
            
            break;
            
        case GLFW_KEY_O:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_o_key) ;
            
            break;
            
        case GLFW_KEY_P:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_p_key) ;
            
            break;
            
        case GLFW_KEY_Q:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_q_key) ;
            
            break;
            
        case GLFW_KEY_R:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_r_key) ;
            
            break;
            
        case GLFW_KEY_S:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_s_key) ;
            
            break;
            
        case GLFW_KEY_T:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_t_key) ;
            
            break;
            
        case GLFW_KEY_U:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_u_key) ;
            
            break;
            
        case GLFW_KEY_V:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_v_key) ;
            
            break;
            
        case GLFW_KEY_W:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_w_key) ;
            
            break;
            
        case GLFW_KEY_X:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_x_key) ;
            
            break;
            
        case GLFW_KEY_Y:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_y_key) ;
            
            break;
            
        case GLFW_KEY_Z:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_z_key) ;
            
            break;
            
        case GLFW_KEY_RIGHT:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_right_key) ;
            
            break;
            
        case GLFW_KEY_LEFT:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_left_key) ;
            
            break;
            
        case GLFW_KEY_UP:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_up_key) ;
            
            break;
            
        case GLFW_KEY_DOWN:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_down_key) ;
            
            break;
            
        case GLFW_KEY_KP_1:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_1) ;
            
            break;
            
        case GLFW_KEY_KP_2:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_2) ;
            
            break;
            
        case GLFW_KEY_KP_3:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_3) ;
            
            break;
            
        case GLFW_KEY_KP_4:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_4) ;
            
            break;
            
        case GLFW_KEY_KP_5:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_5) ;
            
            break;
            
        case GLFW_KEY_KP_6:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_6) ;
            
            break;
            
        case GLFW_KEY_KP_7:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_7) ;
            
            break;
            
        case GLFW_KEY_KP_8:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_8) ;
            
            break;
            
        case GLFW_KEY_KP_9:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_numpad_9) ;
            
            break;
            
        case GLFW_KEY_BACKSPACE:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_bs_key) ;
            
            break;
            
        case GLFW_KEY_SPACE:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_space_key) ;
            
            break;
            
        case GLFW_KEY_1:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_key_1) ;
            
            break;
            
        case GLFW_KEY_2:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_key_2) ;
            
            break;
            
        case GLFW_KEY_3:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_key_3) ;
            
            break;
            
        case GLFW_KEY_4:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_key_4) ;
            
            break;
            
        case GLFW_KEY_5:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_key_5) ;
            
            break;
            
        case GLFW_KEY_6:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_key_6) ;
            
            break;
            
        case GLFW_KEY_7:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_key_7) ;
            
            break;
            
        case GLFW_KEY_8:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_key_8) ;
            
            break;

        case GLFW_KEY_9:
            
            IDK_KeyboardMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, idk_key_9) ;
            
            break;
            
        default:
            break;
    }
    
}

static void IDKCharacterCallback(GLFWwindow* window, unsigned int codepoint) {
    
    IDKWindow idk_window = glfwGetWindowUserPointer(window) ;
    
    IDK_AddCharToPageList(idk_window, (char)codepoint) ;
}

static void IDKMouseFunc(IDKWindow window, double x, double y) {
    
    int WinWidth ;
    
    int WinHeight ;
    
    IDK_GetWindowSize(window, &WinWidth, &WinHeight) ;
    
    y = (y < 1) ? WinHeight : WinHeight - y ;
    
    x = x/WinWidth ;
    
    y = y/WinHeight ;
    
    IDKtheMouse(window,x,y) ;
}

static void IDKCursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    
    IDKWindow idk_window = glfwGetWindowUserPointer(window) ;
    
    IDKMouseFunc(idk_window, xpos, ypos) ;
}

static void IDKMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    
    IDKWindow idk_window = glfwGetWindowUserPointer(window) ;
    
    switch (button) {
            
        case GLFW_MOUSE_BUTTON_RIGHT:
            
            IDK_MouseMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, Right) ;
            
            break;
            
        case GLFW_MOUSE_BUTTON_LEFT:
            
            IDK_MouseMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, Left) ;
            
            break;
            
        case GLFW_MOUSE_BUTTON_MIDDLE:
            
            IDK_MouseMacro(idk_window, action, GLFW_PRESS, GLFW_RELEASE, Middle) ;
            
            break;
            
        default:
            break;
    }
}

static void IDKCursorEnterCallback(GLFWwindow* window, int entered) {
    
    IDKWindow idk_window = glfwGetWindowUserPointer(window) ;
    
    if (entered) {
        
        IDK_setMouseActive(idk_window) ;
        
    } else {
        
        IDK_setMouseInactive(idk_window) ;
    }
}

static void IDKFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
   
    IDKWindow idk_window = (IDKWindow)glfwGetWindowUserPointer(window) ;
    
    if (idk_window->RasterResizeFunc != NULL) idk_window->RasterResizeFunc(idk_window,width,height) ;
}

static GLFWwindow* IDKMakeGLFWWindow( IDKApp App, int win_width, int win_height, const char* win_title ) {
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,1);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(win_width, win_height, win_title, NULL, NULL);
    
    if (!window) {
        
        if ( App->ErrorCallBackFunc != NULL ) App->ErrorCallBackFunc(App, idk_window_error) ;
        
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
    
     if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
         
         if ( App->ErrorCallBackFunc != NULL ) App->ErrorCallBackFunc(App, idk_glad_error) ;
        
        glfwTerminate() ;
        
        exit(EXIT_FAILURE) ;
    }
    
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

void IDK_WindowRunLoopWithTime( IDKWindow window, double seconds, IDKWindowRunLoopFuncType IDKWindowRunLoopFunc, RKArgs RunArgs, IDKWindowQuitRunLoopFuncType IDKWindowQuitRunLoopFunc, RKArgs QuitArgs ) {
    
    double time_count = IDK_GetTimeHP(window->App) ;
    
    while (!glfwWindowShouldClose(window->glfw_window)) {
    
        if ( IDK_Timer_HP(window->App, &time_count, seconds) ) {
        
           glfwPollEvents() ;
        
           IDKWindowRunLoopFunc(RunArgs) ;
        
           glfwSwapBuffers(window->glfw_window) ;
            
        }
    }
    
    IDKWindowQuitRunLoopFunc(QuitArgs) ;
}

void IDK_WindowRunLoop( IDKWindow window, IDKWindowRunLoopFuncType IDKWindowRunLoopFunc, RKArgs RunArgs, IDKWindowQuitRunLoopFuncType IDKWindowQuitRunLoopFunc, RKArgs QuitArgs ) {
    
    while (!glfwWindowShouldClose(window->glfw_window)) {
        
        glfwPollEvents() ;
        
        IDKWindowRunLoopFunc(RunArgs) ;
        
        glfwSwapBuffers(window->glfw_window) ;
    }
    
    IDKWindowQuitRunLoopFunc(QuitArgs) ;
}

void* IDK_GetPtrToGLFWWindow( IDKWindow window ) {
    
    return window->glfw_window ;
}

void IDK_SetPtrFromWindow( IDKWindow window, void* ptr ) {
    
    window->ptr = ptr ;
}

void* IDK_GetPtrFromWindow( IDKWindow window ) {
    
    return window->ptr ;
}

IDKApp IDK_GetAppFromWindow( IDKWindow window ) {
    
    return window->App ;
}

void IDK_SetRasterResizeFunc( IDKWindow window, IDKRasterResizeFuncType RasterResizeFunc ) {
    
    window->RasterResizeFunc = RasterResizeFunc ;
}

IDKWindow IDK_NewWindow( IDKApp App, int win_width, int win_height, const char* win_title, IDKRasterResizeFuncType RasterResizeFunc ) {
   
    IDKPlatformInit(App, 1) ;
    
    IDKWindow NewWindow = RKMem_NewMemOfType(struct IDKWindow_s) ;
    
    NewWindow->App = App ;
    
    NewWindow->PageList = RKList_NewList() ;
    
    NewWindow->KeyArray = NULL ;
    
    NewWindow->MouseActive = 0 ;
    
    NewWindow->MouseLeft = 0 ;
    
    NewWindow->MouseMiddle = 0 ;
    
    NewWindow->MouseRight = 0 ;
    
    NewWindow->MouseX = 0 ;
    
    NewWindow->MouseY = 0 ;
    
    IDKAllocKeyboard(NewWindow) ;
    
    NewWindow->is_fullscreen = 0 ;
    
    NewWindow->RasterResizeFunc = RasterResizeFunc ;
    
    NewWindow->glfw_window = IDKMakeGLFWWindow( App, win_width, win_height, win_title ) ;
    
    glfwSetWindowUserPointer( NewWindow->glfw_window, (void*)NewWindow ) ;
    
    #ifdef __APPLE__
    
    const void* IDKSetupFullScreenForMac( IDKWindow idk_window ) ;
    
    NewWindow->fullscreen_state = IDKSetupFullScreenForMac( NewWindow ) ;
    
    void IDKEnterOrExitFullScreenForMac( const void* fullscreen_state ) ;
    
    NewWindow->EnterFullScreen = IDKEnterOrExitFullScreenForMac ;
    
    NewWindow->ExitFullScreen = IDKEnterOrExitFullScreenForMac ;
    
    #endif
    
    #ifdef _WIN32
    
    const void* IDKSetupFullScreenForWin( IDKWindow idk_window ) ;
    
    NewWindow->fullscreen_state = IDKSetupFullScreenForWin( NewWindow ) ;
     
    void IDKEnterFullScreenForWin( const void* fullscreen_state ) ;

    void IDKExitFullScreenForWin( const void* fullscreen_state ) ;

    NewWindow->EnterFullScreen = IDKEnterFullScreenForWin ;

    NewWindow->ExitFullScreen = IDKExitFullScreenForWin ;
     
    #endif
    
    return NewWindow ;
}

void IDK_DestroyWindow( IDKWindow window ) {
    
    glfwDestroyWindow(window->glfw_window) ;
    
    #ifdef __APPLE__
    
    void IDKKillFullScreenForMac( const void* fullscreen_state ) ;
    
    IDKKillFullScreenForMac(window->fullscreen_state) ;
    
    #endif
    
    #ifdef _WIN32
    
    void IDKKillFullScreenForWin( const void* fullscreen_state ) ;
    
    IDKKillFullScreenForWin(window->fullscreen_state) ;
    
    #endif
    
    IDKDeallocKeyboard(window) ;
    
    RKList_DeleteList(window->PageList) ;
    
    IDKPlatformInit(window->App, 0) ;
    
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

void IDKSetFullScreenFlag( IDKWindow window, int is_fullscreen ) {
    
    if ( is_fullscreen ) {
        
        window->is_fullscreen = 1 ;
        
    } else {
       
        window->is_fullscreen = 0 ;
    }
}

int IDK_IsFullscreen( IDKWindow window ) {
    
    return window->is_fullscreen ;
}

void IDK_EnterFullscreen( IDKWindow window ) {
        
  if (!(window->is_fullscreen)) {
           
     IDKSetFullScreenFlag(window,1) ;
            
     window->EnterFullScreen(window->fullscreen_state) ;
  }
}

void IDK_ExitFullscreen( IDKWindow window ) {
    
  if ((window->is_fullscreen)) {
            
     IDKSetFullScreenFlag(window,0) ;
            
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
    
        if ( IDK_GetKey(TextLine->Page->window, idk_left_key) ) {
            
            if (IDK_PageGetCharAtCursor(TextLine->Page) == '&') IDK_PageSetCharAtCursor(TextLine->Page, TextLine->flash) ;
            
            IDK_PageMoveCursorLeft(TextLine->Page) ;
            
            TextLine->lock = 0 ;
        }
        
        if ( IDK_GetKey(TextLine->Page->window, idk_right_key) ) {
            
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

IDKPage IDK_NewPage( IDKWindow window, int max_num_of_text ) {
    
    IDKPage Page = RKMem_NewMemOfType(struct IDK_Page_s) ;
    
    Page->max_num_of_text = max_num_of_text+1 ;
    
    Page->window = window ;
    
    Page->num_of_text = 0 ;
    
    Page->cursor = 0 ;
    
    Page->init = 0 ;
    
    Page->mode = 0 ;
    
    Page->pos = 0 ;
    
    Page->new = 0 ;
    
    Page->text = NULL ;
    
    Page->node = RKList_AddToList(window->PageList, (void*)Page) ;
    
    return Page ;
    
}

void IDK_DestroyPage( IDKPage Page ) {
    
    RKList_DeleteNode(Page->window->PageList, Page->node) ;
    
    free( Page->text ) ;
    
    free( Page ) ;
}

void IDK_AddCharToPageList( IDKWindow window, char c ) {
    
    RKList_node node = RKList_GetFirstNode(window->PageList) ;
    
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
    
    if ( Page->text == NULL ) return idk_error_string ;
    
    return Page->text ;
}

double IDK_GetTimeHP( IDKApp App ) {
    
    return App->IDKGetTimeHPFunc() ;
}

static double GetFPSFunc_High( IDKApp App ) {
    
    double seconds = 0 ;
    
    IDKFPSState state ;
    
    if ( App->highp_fps == NULL ) {
        
        App->highp_fps = RKMem_NewMemOfType(struct IDKFPSState_s) ;
        
        App->highp_fps->init = 0 ;
        
        App->highp_fps->numofframes = 0 ;
        
        App->highp_fps->time_count = 0 ;
        
        App->highp_fps->fps = 0 ;
    }
    
    state = App->highp_fps ;
    
    if (!state->init) {
        
        state->time_count = IDK_GetTimeHP(App) ;
        
        state->init++ ;
    }
    
    state->numofframes++ ;
    
    seconds = (IDK_GetTimeHP(App) - state->time_count) ;
    
    if ( seconds >= 0.25 ) {
        
        state->fps = ( state->numofframes / seconds) ;
        
        state->numofframes = 0 ;
        
        state->time_count = IDK_GetTimeHP(App) ;
        
    }
    
    return state->fps ;
}

static double GetFPSFunc_Low( IDKApp App ) {
    
     double seconds = 0 ;
    
    IDKFPSState state ;
    
    if ( App->lowp_fps == NULL ) {
        
        App->lowp_fps = RKMem_NewMemOfType(struct IDKFPSState_s) ;
        
        App->lowp_fps->init = 0 ;
        
        App->lowp_fps->numofframes = 0 ;
        
        App->lowp_fps->time_count = 0 ;
        
        App->lowp_fps->fps = 0 ;
    }
    
    state = App->lowp_fps ;
    
    if (!state->init) {
        
        state->time_count = (unsigned)time(NULL) ;
        
        state->init++ ;
    }
    
    state->numofframes++ ;
    
    seconds = ((unsigned)time(NULL) - state->time_count) ;
    
    if ( seconds >= 1 ) {
        
        state->fps = (state->numofframes / seconds) ;
        
        state->numofframes = 0 ;
        
        state->time_count = (unsigned)time(NULL) ;
        
    }
    
    return state->fps ;
}

static double GetFPSFunc( IDKApp App, int Low0High1 ) {
    
    double fps = 0 ;
    
    if ( Low0High1 ) {
       
        fps = GetFPSFunc_High(App) ;
        
    } else {
        
        fps = GetFPSFunc_Low(App) ;
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

static const char* IDKGetFilePathForThePlatform( IDKApp App, IDKLoadFileType filetype ) {
    
    #ifdef __APPLE__
    
    RKString IDKGetFilePathForMac( IDKApp App, IDKLoadFileType filetype ) ;
    
    return  RKString_ConvertToCString(IDKGetFilePathForMac(App, filetype)) ;
    
    #endif
    
    #ifdef _WIN32
    
    RKString IDKGetFilePathForWin( IDKApp App, IDKLoadFileType filetype ) ;
    
    return RKString_ConvertToCString(IDKGetFilePathForWin(App, filetype)) ;
    
    #endif
    
    return NULL ;    
}

 char* IDK_GetFilePathForPlatform( IDKApp App, IDKLoadFileType filetype, const char* path ) {
    
    int size1 = 0 ;
    
    int size2 = 0 ;
    
    char* the_file_path = NULL ;
    
    const char* path_base = IDKGetFilePathForThePlatform(App,filetype) ;
    
    if ( path_base == NULL ) return NULL ;
    
    size1 = (int)strlen(path_base) ;
    
    size2 = (int)strlen(path) ;
    
    the_file_path = RKMem_CArray(size1+size2+2, char) ;
    
    #ifdef _WIN32
    
     sprintf(the_file_path,"%s\\%s",path_base,path) ;
     
    #else
     
     sprintf(the_file_path,"%s/%s",path_base,path) ;
     
    #endif
    
    free((char*)path_base) ;
    
    return the_file_path ;

}

FILE* IDK_LoadFile( IDKApp App, IDKLoadFileType filetype, const char* path, const char* mode ) {
    
    FILE* file = NULL ;
    
    char* the_file_path = IDK_GetFilePathForPlatform(App, filetype, path) ;
    
    if ( the_file_path == NULL ) return NULL ;
    
    file = fopen(the_file_path, mode) ;
     
    free(the_file_path) ;
    
    return file ;
}

void IDK_UnloadFile( FILE* file ) {
    
    fclose(file) ;
}

static void IDKInitLogFile( IDKApp App ) {
    
    App->LogFile = IDK_LoadFile(App, idk_data_file, "Logfile", "w") ;
    
    if ( App->LogFile == NULL ) {
        
        puts("PANIC: Could not create Logfile!\n") ;
        
        fputs("PANIC: Could not create Logfile!\n", stderr);
        
        exit(EXIT_FAILURE) ;
    }
    
    if ( setvbuf(App->LogFile, NULL, _IONBF, 0) ) {
        
        puts("PANIC: setvbuf failed!\n") ;
        
        fputs("PANIC: setvbuf failed!\n", stderr);
        
        exit(EXIT_FAILURE) ;

    }
}

static void IDKKillLogFile( IDKApp App ) {
    
     IDK_UnloadFile(App->LogFile) ;
}

void IDKLog( IDKApp App, const char* string, int newline, int error ) {
    
    if ( newline ) {
    
     if ( !error ) {
        
      printf("%s\n", string) ;
         
     } else {
      
      fprintf(stderr, "%s\n", string) ;
         
     }
    
     fprintf(App->LogFile, "%s\n", string) ;
        
    } else  {
       
     if ( !error ) {
            
      printf("%s", string) ;
            
      } else {
            
      fprintf(stderr, "%s", string) ;
            
     }

     fprintf(App->LogFile, "%s", string) ;
        
    }
}

void IDKLogInt( IDKApp App, int val, int newline, int error ) {
    
    char string[100] ;
    
    myitoa(val, string) ;
    
    IDKLog(App, string, newline, error) ;
}

void IDKLogFloat( IDKApp App, float val, int newline, int error ) {
    
    char string[100] ;
    
    myftoa(val, string) ;
    
    IDKLog(App, string, newline, error) ;
}

void IDKLogDouble( IDKApp App, double val, int newline, int error ) {
    
    char string[100] ;
    
    mydtoa(val, string) ;
    
    IDKLog(App, string, newline, error) ;
}

static void IDKGlfwInit( IDKApp App ) {
    
    glfwSetErrorCallback(IDKErrorCallbackForGLFW) ;
    
    if (!glfwInit()) glfwTerminate()  ;
}

static void IDKGlfwKill(void) {
    
    glfwTerminate() ;
}

static void IDKPlatformInit( IDKApp App, int mode ) {
    
    static int count = 0 ;
    
    if ( mode ) {
        
        count++ ;
        
        if ( count == 1 ) {
        
        AppList = RKList_NewList() ;
        
        RKList_AddToList(AppList, App) ;
            
        IDKGlfwInit(App) ;
            
        } else {
            
        RKList_AddToList(AppList, App) ;
            
        }
    }
    
    if ( !mode ) {
        
        count-- ;
        
        if ( count <= 0 ) {
            
            count = 0 ;
            
            IDKGlfwKill() ;
            
            RKList_DeleteList(AppList) ;
        }
    }
}

void IDK_SpawnThreads( IDKApp App ) {
    
    if ( App->Threads == NULL ) App->Threads = RKTasks_NewThreadGroup(10) ;
}

void IDK_KillThreads( IDKApp App ) {
    
    if ( App->Threads != NULL ) {
        
        RKTasks_DestroyThreadGroup(App->Threads)  ;
        
        App->Threads = NULL ;
    }
}

RKTasks_ThreadGroup IDK_GetThreads( IDKApp App ) {
    
    return App->Threads ;
}

float IDK_GetFPS( IDKApp App ) {
    
    return GetFPSFunc(App,0) ;
}

double IDK_GetFPSHP( IDKApp App ) {
    
    return GetFPSFunc(App,1) ;
}

void IDK_GetFPSString( IDKApp App, char* string ) {
    
    float fps = IDK_GetFPS(App) ;
    
    myftoa(fps,string) ;
}

void IDK_GetFPSHPString( IDKApp App, char* string ) {
    
    double fps = IDK_GetFPSHP(App) ;
    
    mydtoa(fps,string) ;
}