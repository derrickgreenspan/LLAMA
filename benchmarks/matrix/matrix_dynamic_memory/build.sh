#!/bin/bash

clang -emit-llvm -S matrix.c -o simple.bc
opt -S -loop-rotate < simple.bc > simple2.bc
opt -S -load build/libllamaPass.so -llama < simple2.bc > simple3.bc
llc -filetype=obj simple3.bc
#clang simple3.o library/llama.a -o matrix_0
clang simple3.o library/0.a -o matrix_0
clang simple3.o library/10.a -o matrix_10
clang simple3.o library/100.a -o matrix_100
clang simple3.o library/1000.a -o matrix_1000
clang simple3.o library/20000.a -o matrix_20000
clang simple3.o library/30000.a -o matrix_30000
clang simple3.o library/40000.a -o matrix_40000
clang simple3.o library/10000.a -o matrix_10000
clang simple3.o library/100000.a -o matrix_100000
