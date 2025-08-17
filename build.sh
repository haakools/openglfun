#/usr/env/bin bash

rm build -r
mkdir build && cd build
cmake ..
make
./main