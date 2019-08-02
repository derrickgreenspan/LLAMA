#!/bin/bash
ml clang
ml cmake/cmake-3.7.2-gcc-7.1.0

mkdir -p ../build/
cd ../build/
cmake ../llvm
make
