#!/bin/bash

CC="${CC:-gcc}"

xxd -i mines.png > src/mines_png.h
xxd -i icon.png > src/icon_png.h
$CC $CFLAGS src/*.c -lSDL2 -lSDL2_image -DEMBEDDEDPNG -lm -o minesweeper
rm -f src/mines_png.h
rm -f src/icon_png.h
