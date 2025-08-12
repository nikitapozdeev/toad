#!/bin/bash

# Build the project using system libraries
echo "Building toad..."

gcc src/*.c \
    vendor/lua/lapi.c vendor/lua/lauxlib.c vendor/lua/lbaselib.c \
    vendor/lua/lcode.c vendor/lua/lcorolib.c vendor/lua/lctype.c \
    vendor/lua/ldblib.c vendor/lua/ldebug.c vendor/lua/ldo.c \
    vendor/lua/ldump.c vendor/lua/lfunc.c vendor/lua/lgc.c \
    vendor/lua/linit.c vendor/lua/liolib.c vendor/lua/llex.c \
    vendor/lua/lmathlib.c vendor/lua/lmem.c vendor/lua/loadlib.c \
    vendor/lua/lobject.c vendor/lua/lopcodes.c vendor/lua/loslib.c \
    vendor/lua/lparser.c vendor/lua/lstate.c vendor/lua/lstring.c \
    vendor/lua/lstrlib.c vendor/lua/ltable.c vendor/lua/ltablib.c \
    vendor/lua/ltm.c vendor/lua/lundump.c vendor/lua/lutf8lib.c \
    vendor/lua/lvm.c vendor/lua/lzio.c \
    -Ivendor/lua -Ivendor/quickjs \
    vendor/quickjs/libquickjs.a \
    $(pkg-config --cflags --libs sdl2) \
    -lm -lpthread -ldl \
    -o toad