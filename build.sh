#!/bin/bash

CC="${CC:-gcc}"
CFLAGS="${CFLAGS:--lSDL2 -lSDL2_image -DEMBEDDEDPNG -lm}"
xxd -i mines.png > src/mines_png.h
xxd -i icon.png > src/icon_png.h
$CC src/*.c $CFLAGS -o minesweeper
rm -f src/mines_png.h
rm -f src/icon_png.h
