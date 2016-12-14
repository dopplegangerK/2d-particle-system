# 2d-particle-system

Final project for Game Technology class at UT (CS 354R).

A basic framework for a 2D particle system using SDL for graphics and Box2D for physics. This library can easily be adapted for a wide variety of uses in 2D games. See the README in the Particles subdirectory for more information about how to use the library.

This project also includes the game "Space!" which was created to demonstrate use of the Particles library. See the README in the SpaceGame subdirectory for more information.

##Build instructions

###Dependencies:
- SDL2
- SDL2_image
- SDL2_gfx
- SDL2_ttf
- SDL2_mixer
- Box2D (can be found [here](https://github.com/erincatto/Box2D))

If cmake has trouble finding any of these libraries (it definitely will if you're using windows, and most likely will for Box2D), create an environment variable for each library (for example, SDL2, SDL2_IMAGE, BOX2D, etc.) pointing to its root directory. For Box2D, make sure the root directory contains the .lib file as well as the include directory.

###CMake
This project uses cmake as its build system. If you don't already have cmake downloaded and installed, do so [here](https://cmake.org/download/). The game's excecutable will be built in the `install/bin` directory (in this project's root directory), and the Particles library will be written to `install/lib`.

First, create a `build` directory inside the root directory and cd into it. Run `cmake ../ -DCMAKE_INSTALL_PREFIX=../install`.

####Windows:
Use the .sln file that is generated to build the solution, then make sure you build the INSTALL project (you may need to do this explicitly). This will create an `install` directory in the project's root directory and copy the built files there.

Note: Be sure to copy the .dll files needed for SDL into the `install/bin` directory (Apart from the .dll's with the same name as the libraries, you'll need libfreetype-6.dll, zlib1.dll, and libpng16-16.dll).

####Linux:
After running cmake (command above), run `make`, `make install`.

###Game credits:
- Sprites: [Kenney Vleugels] (http://opengameart.org/content/space-shooter-redux)
- Music: [Jan125] (http://opengameart.org/content/stereotypical-90s-space-shooter-music#)
- Sound effects: [timgormly] (https://www.freesound.org/people/timgormly/)

I do not own any of the assets used in the game. I just wrote code.
