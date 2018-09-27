#!/bin/bash
g++ -std=c++17 -O2 -Wall ../FileIO.h structured_arrays_read_write.cpp -o structured.o
# Jagged arrays test fails when optimised with O2 for some reason
g++ -std=c++17 -O2 -Wall ../FileIO.h jagged_arrays_test.cpp -o jagged.o
./structured.o
./jagged.o