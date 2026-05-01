#!/bin/bash
x86_64-w64-mingw32-windres mines.rc -O coff -o mines.res
CC=x86_64-w64-mingw32-gcc CFLAGS="mines.res -lmingw32 -lSDL2_image -lSDL2main -lSDL2 -DEMBEDDEDPNG -lm -mwindows" ./build.sh 
rm -f mines.res
