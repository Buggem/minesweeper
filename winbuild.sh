#!/bin/bash
CC=x86_64-w64-mingw32-gcc CFLAGS="-lmingw32 -lSDL2_image -lSDL2main -lSDL2 -DEMBEDDEDPNG -lm -mwindows" ./build.sh 

