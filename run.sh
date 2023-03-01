#!/bin/sh

mkdir -p build
cd build
rm -rf CMakeFiles server
cmake .. && \
make && \
sudo ./webserver-highloads
