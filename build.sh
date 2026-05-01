#!/bin/bash

xxd -i mines.png > src/mines_png.h
xxd -i icon.png > src/icon_png.h
gcc -lSDL2 -lSDL2_image -DEMBEDDEDPNG -lm src/*.c -o main
rm -f src/mines_png.h
rm -f src/icon_png.h
