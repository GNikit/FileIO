#!/bin/bash
g++ -std=c++17 -O2 -Wall ../FileIO.h structured_arrays_read_write.cpp -o structured.o
# Jagged arrays test fails when optimised with O2 in g++ for some reason, but is fine in icpc
g++ -std=c++17 -Wall -Ofast ../FileIO.h jagged_arrays_test.cpp -o jagged.o
./structured.o
./jagged.o