#!/bin/bash
PREFIX=/usr/local
gcc sdl_demo.c -o sdl_demo -I $PREFIX/include -L $PREFIX/lib -lSDL2
