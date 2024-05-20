#!/bin/bash

# Run CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Change to the build directory
cd build

# Run make
make

# Run the program
./bin/ChessEngines