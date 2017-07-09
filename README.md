[![Build Status](https://travis-ci.org/rustushki/keyrunner.svg?branch=master)](https://travis-ci.org/rustushki/keyrunner)

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
* Level Editor

# Downloads

Unfortunately, downloads are not available. I hope to have this setup soon.

# Build Instructions

## Ubuntu Linux

    $ sudo apt install libsdl-ttf2.0-dev libsdl-image1.2-dev libsdl1.2-dev cmake
    $ git clone https://github.com/rustushki/keyrunner.git
    $ cd keyrunner
    $ cmake .
    $ make
    $ sudo make install
