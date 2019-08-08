# Connected Developer Documentation

This document should serve as a guide to anyone who wants to make additions to the Connected brain viewer software suite. It will cover where functionality is defined within the codebase, as well as how to compile and setup the project on a new machine.

## Setting up the project
In order to compile Connected, you will need to install Qt onto your computer. It can be compiled just using QMake and make or msvc on the command line, but I have found it is much easier to initially setup the project using Qt Creator. The download for Qt can be found [here](https://www.qt.io/download) (select the open source version). Once you have Qt installed, clone the git project and open it using Qt Creator. 

From here open **Connected.pro**, which is the QMake rules for building the project. In order to compile the project you need the following dependencies:
* OpenGL
* OpenGL Math Library (GLM, header only)
* Boost (Header only)
* LibTIFF
* HoloPlay API (FOR LOOKING GLASS BUILDS ONLY)

All of these libraries can be found online (except openGL which should come with your graphics drivers). For Windows users, you must place the libraries somewhere on your system and modify two key lines in **Connected.pro**
>win32:INCLUDEPATH
>win32:  LIBS

by default the libraries will be in program files, but you can change the paths on those two lines for each library to customize it for your system. **PLEASE DO NOT COMMIT CHANGES TO CONNECTED.PRO TO GITHUB**

From here you can go to **Build->Run QMake** (This step has to be done everytime you make a change to **Connected.pro**, such as when adding a new class to the codebase) in the top left corner of Qt Creator, and then **Build->Rebuild all**. Hit the run button in the bottom left, and you should have a working build of connected.

## Code Guide

Connected is comprised of many classes, some of which are related to Qt, and some are tied to OpenGL rendering (or both). Below is a list of the important classes and an overview of what they do.

* **main.cpp** is the program entry point, very basic setup code, contains an instance of the main window
* **connected.cpp** houses all the code relating to the main window and everything accessed from the main window (essentially the entire project). It is responsible for handling the interactions between the UI portion of the application and the rendering portion. Values from settings files, and commands from GUI presses go through here to be passed onto **GLWidget**.
*  **GLWidget.cpp** is where everything that relates to rendering is handled. It is a Qt Widget type class, and is the black screen in the center of the window. It instances the **Brain** class, controls the **Camera** along with most other major rendering processes.
* **brain.cpp** handles the rendering of Connectome data. Mainly it handles the loading of data files and then turning that into a rendered image. It uses **Model** instances to do the actual rendering, and mostly doesn't call OpenGL directly.
* **model.cpp** handles loading Obj or NV files and using OpenGL to render them
* **camera.cpp** keeps important camera variables (position, orientation, projection matrix)
* **nifti.cpp** is a very poorly named class, since it only loads TIFF images into OpenGL Textures
* **mri.cpp** renders a texture to a moving plane, for use in the MRI display feature.
* **networksettings.cpp**, **mrisettings.cpp**, **colorsettings.cpp** all are windows that configure and store user settings
