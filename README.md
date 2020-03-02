![screenshot](doc/np3.png)

What is PixFu
=============

PixFu is a minimalistic modular game / realtime system engine. It is 100% written in C++ and OpenGL and runs on Desktop computers and Mobile devices (Android at the moment, IOS coming next).

PixFu core is based on famous engine OneLoneCoder Pixel Game Engine (http://onelonecoder.com) and has a redesigned full modular approach to allow optimizations for mobile devices and embedded platforms.

 Core: OpenGL initialization, platform abstraction, android/ios launchers

 Input Devices: Keyboard, Mouse, Virtual Mouse, Virtual Keyboard, Axis Controller, Gyroscope

 Extensions:

    - Primary Surface: provides a 2D canvas with functions to draw circles, lines, rectangles and multi-font strings. This extension is enabled by default. Source code for this extension comes from OLC PGE core. 
    
    - Sprites: OpenGL/powered sprites with scaling, rotation, fx and multi sprite atlas support

    - World: Complete minimalistic 3D world with terrain model, height maps, lighting, camera, and 3D-model object clusters with multi-texture support, optimized for mobile devices. 

Other Features:

    - OLC Pixel Game Engine compatibility layer, provides a base class that mimics the PGE Engine class. You can easily port your PGE modules to PixFu! (Note- Extensions are not supported, and not all functionality is emulated)

    - Includes GLM library, used internally, but also exported so it is ready to use in your project just including it. The library is compiled with FORCE_LEFT_HANDED, so angles are positive clockwise and physics behave as expected.
    
    - 100% modular structure allows to enable just the features you need. Even the primary UI surface is an extension that maybe you don't need to enable to get some fps!

![screenshot](doc/page2.png)

![screenshot](doc/page1.png)

PixFu Workflow
=============

PixFu provides a base Engine class that you will extend to your convenience. The base class abstracts all platfom details, and automatically manages extensions and input devices. 
Your code will be very tight without any extraneous background libraries or services !

![screenshot](doc/page3.png)


How To Start
=============

This is a template Android Studio project that you can use to create your PixFu based applications.

It includes the PixFu/Android core as an AAR, and all build scripts and CMAKE configurations ready so you just can deploy one class, hit build and play the application in your phone.

Please refer to the root project PixFu https://github.com/nebular/PixFu_Android for technical details.

Pre-requisites
--------------

- Android Studio 3.5.2+ (lower may work)
- (not sure if you also need to install the NDK, easy anyways:  [NDK] (https://developer.android.com/ndk/) bundle.

Getting Started
---------------
1. Get the binary release of this project
 
`git clone https://github.com/nebular/PixFu_Android_demos.git
`
1. Open Xcode and press New Project, scroll to the bottom and you should see PixFu.

![screenshot](doc/np1.png)

1. You will get a new PixFu blank project with 3 demos. The PixFu engine is included as a Framework that contains all binary and headers.

![screenshot](doc/np2.png)

1. Just press PLAY and the first demo should run. You can launch the other two demos by editing the file run.cpp


Examples
--------------------------------------

- demo_hello.h: Just 4 lines of a hello world.

![screenshot](doc/np3.png)
 
- demo_sprites.h: A demo using the Sprites extension.

  ![screenshot](doc/np4.png)

    
- demo_3d.h: A demo using the 3D world extension.

  ![screenshot](doc/np5.png)


Support
-------
If you've found an error please [file an issue] (https://github.com/nebular/PixFu_Android/issues/new).

Patches are encouraged, and may be submitted by [forking this project](https://github.com/nebular/PixFu_Android/fork) and submitting a pull request through GitHub.

License
-------

https://creativecommons.org/licenses/by/4.0/
