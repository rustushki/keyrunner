# Summary

Help Moschata escape certain doom. The clock is ticking and she must reach the door with the key before it's too late.

KeyRunner is a free game, available in source code with the GPLv2 License. I wrote this game as the first in a series of smallish games.

![KeyRunner](https://raw.githubusercontent.com/rustushki/keyrunner/master/screenshots/keyrunner.png)

# Gameplay / Features

* Moschata, the Key Runner
* Doors require keys
* Colored teleporter tiles
* Wrap around edges
* Conveyor Belts
* Push-Hold and Rapid-Fire movement
* Easy to edit and add levels

# Downloads

Unfortunately, downloads are not available.  I hope to have this setup soon.

# Build Instructions

## Ubuntu Linux

    $ sudo apt install libsdl-ttf2.0-0 libsdl-ttf2.0-dev libsdl-image1.2 \
    libsdl-image1.2-dev libsdl1.2-dev libsdl1.2debian
    $ git clone https://github.com/rustushki/keyrunner.git
    $ cd keyrunner
    $ cmake .
    $ make
    $ sudo make install

## Windows

These are some build instructions which allow for the compilation of KeyRunner in Microsoft Windows XP.

1. Download and Install CodeBlocks  (http://codeblocks.org; get 'codeblocks-10.05-setup.exe').  Note that there is also a CodeBlocks package which contains MinGW; I've not tried it yet, but I'll do so soon and update
these instructions.
1. Download and Install MinGW. I used the mingw-get-inst-20120426.exe package.
1. Download the SDL-devel / MinGW package from libsdl.org.  I used SDL-devel-1.2.15-mingw32.tar.gz.
1. Unpackage SDL-devel.
1. Copy SDL-devel\lib\* to C:\Program Files\CodeBlocks\lib\ (create a directory if
you need to).
1. Copy SDL-devel\include\SDL to C:\Program Files\CodeBlocks\include\SDL.
1. Copy SDL-devel\bin\SDL.dll to C:\Windows\.
1. Download SDL_ttf-2.0.11-win32.zip from http://www.libsdl.org/projects/SDL_ttf/.
1. Extract all DLLs from the SDL_ttf package to C:\Windows\.
1. Download SDL_ttf-2.0.11.tar.gz from http://www.libsdl.org/projects/SDL_ttf/.
1. Extract SDL_ttf.h to C:\Program Files\CodeBlocks\include\SDL.
1. Download SDL_image-1.2.12-win32.zip from http://www.libsdl.org/projects/SDL_image/.
1. Extract all DLLs to C:\Windows\.
1. Download SDL_image-1.2.12.tar.gz from http://www.libsdl.org/projects/SDL_image/.
1. Extract SDL_image.h to C:\Program Files\CodeBlocks\include\SDL. You've now unpacked DLLs and header files for SDL, SDL_ttf, SDL_image, and libpng (via SDL_image).  You've added the MinGW compiler to your system.  The CodeBlocks IDE is also installed and will automatically detect that MinGW is compiled.
1. Extract the KeyRunner Package somewhere.
1. Run CodeBlocks create a new Console Project.  Name it KeyRunner.
1. Add all of KeyRunner's *.h and *.cpp files.
1. Add the img\ and level\ folders.  You will also need to add these to the
Release\ and Debug\ folders.
1. CodeBlocks can now successfully compile the project using the Build
functionality.
