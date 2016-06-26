# IDK

IDK, or Interactive Development Kit provides the base for an interactive application.

--Features:

-Basic resolution independent 2D drawing:
 
 Line

 Rectangle

 Circle

 Text String

 Support for scene zooming and panning

-Basic input handling for the mouse and keyboard

-Basic text display and editing

-Window creation and management

-Basic file system management

-Basic Logfile system

Current Limitations:

Even though IDK is based on the cross-platform GLFW, and is intended to be cross-platform, in its current state it will
only work on Mac OS X.

While IDK supports basic resolution independent 2D drawing, and via GLFW the creation of an OpenGl context, the output 
texture from IDKDrawArea must be bound to an OpenGL surface to be displayed. IDK as of now, will not bind or create an OpenGL surface.

Dependencies: 

 -RKMem and RKTasks, from RKLib

 -JHGPixels
 
 -codename

 -GLFW http://www.glfw.org