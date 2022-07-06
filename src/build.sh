#!/bin/bash

mkdir build
g++ -O0 -g examples/$1.cpp common/wxgl.c -I. -ldl -lGL -lX11 -o build/$1 && ./build/$1
