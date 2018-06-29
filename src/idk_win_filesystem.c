/*
 Copyright (c) 2017-2018 Jacob Gordon. All rights reserved.
 
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
 
#include <direct.h>

#include <Shlobj.h>
#include <Objbase.h>
#include <IDK/IDK.h>

static char* IDKWinGetAppDataDir( IDKApp App ) {
    
     static int makedir = 1 ;
    
     PWSTR string = NULL ;
    
     HRESULT result = SHGetKnownFolderPath(&FOLDERID_LocalAppData,0,NULL,&string) ;
   
     if ( (result == E_FAIL) || (result == E_INVALIDARG ) ) return NULL ;
   
     size_t size = wcslen(string) ;
   
     char* retstring = RKMem_CArray((size+1)*4,char) ;
   
     WideCharToMultiByte(CP_UTF8, 0, string, -1, retstring, (size+1)*4, NULL, NULL) ;

     CoTaskMemFree(string) ; 
  
     size_t size2 = RKString_GetSize(IDK_GetAppName(App)) + 1 ;
        
     char* pathstring = RKMem_CArray(((size+1)*4)+size2,char) ;
    
     sprintf(pathstring,"%s\\%s",retstring,RKString_GetString(IDK_GetAppName(App))) ;
    
     if ( makedir ) {
        
         _mkdir(pathstring) ;
        
         makedir = 0 ;
     }
     
     free(retstring) ;
    
     return pathstring ;
}

 static RKString IDKWinGetResourcePath( void ) {
     
     char buffer[1024] ;
     
     char* path = _getcwd(buffer,1024) ;
     
     if ( path != NULL ) return RKString_NewStringFromCString(path) ;
     
     return NULL ;
}

RKString IDKGetFilePathForWin( IDKApp App, IDKLoadFileType filetype ) {
    
    char* path = NULL ;
    
    RKString string = NULL ;
    
    if ( filetype == idk_resource_file ) {
        
        string = IDKWinGetResourcePath()  ;
    }
    
    if ( filetype == idk_data_file ) {
        
        path = IDKWinGetAppDataDir(App) ;
        
        if ( path == NULL ) return NULL ;
        
        string = RKString_NewStringFromCString(path) ;
        
        free(path) ;
    }
    
    if ( string != NULL ) return string ;
    
    return NULL ;
}
 
 #endif