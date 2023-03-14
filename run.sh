#!/bin/sh

mkdir -p build
cd build
rm -rf CMakeFiles server
cmake -DCMAKE_BUILD_TYPE=Release .. && \
# cmake .. && \
make && \
sudo ./webserver-highload
