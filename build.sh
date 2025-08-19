#!/usr/bin/env bash
rm -rf build
mkdir build
cmake -B build -S .
cd build
make
./main