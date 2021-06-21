# IDK

IDK, or Interactive Development Kit provides the basis for an interactive application.

--Features:

-Basic input handling for the mouse and keyboard

-Basic text input and editing

-Window creation and management

-Basic file system management

-Basic Logfile system

-Built-in support for the [Nuklear][2] GUI toolkit(Nuklear is included)

[2]:https://github.com/vurtun/nuklear

See main.c for an example.

Current Limitations:

Even though IDK is based on the cross-platform GLFW, and is intended to be cross-platform, in its current state it will
only work on Mac OS X, and Windows. Linux support is planned.


Uses: 

 -RKLib https://github.com/JHG777000/RKLib
 
 -Nuklear https://github.com/vurtun/nuklear

 -GLAD https://github.com/Dav1dde/glad

 -GLFW http://www.glfw.org
 
## Building

IDK uses [builder][1] for its build system.

[1]:https://github.com/JHG777000/builder

To download and build use this command:


	builder -u https://raw.githubusercontent.com/JHG777000/IDK/main/buildfile
	
To run test, add -i __t:

	builder -i __t -u https://raw.githubusercontent.com/JHG777000/IDK/main/buildfile