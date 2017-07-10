[![Build Status](https://travis-ci.org/rustushki/keyrunner.svg?branch=master)](https://travis-ci.org/rustushki/keyrunner)

# Summary

Help Moschata escape certain doom. The clock is ticking and she must reach the
door with the key before it's too late. KeyRunner is a free game, available in
source code with the GPLv2 License.

![KeyRunner](https://raw.githubusercontent.com/rustushki/keyrunner/master/screenshots/keyrunner.png)

# Gameplay / Features

* Moschata, the Key Runner
* Doors require keys
* Colored teleporter tiles
* Wrap around edges
* Conveyor belts
* Push-hold and rapid-fire movement
* 35 built in levels and a Level Editor

# Ubuntu Build Instructions

    $ sudo apt install libsdl2-ttf-dev libsdl2-image-dev libsdl2-dev cmake
    $ git clone https://github.com/rustushki/keyrunner.git
    $ cd keyrunner
    $ cmake .
    $ make
    $ sudo make install
