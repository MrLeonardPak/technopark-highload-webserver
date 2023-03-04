#!/bin/sh

mkdir -p build
cd build
rm -rf CMakeFiles server
cmake -DCMAKE_BUILD_TYPE=Debug .. && \
# cmake .. && \
make && \
sudo ./webserver-highload
