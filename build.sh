#!/bin/sh

set -x

SDL2_CFLAGS=$(sdl2-config --cflags)
SDL2_LIBS=$(sdl2-config --libs)

mkdir -p bin
pushd bin
c++ ../src/gnu_linux_sdl_bomber.cpp -o bomber ${SDL2_CFLAGS} ${SDL2_LIBS} -g
popd
