#!/bin/bash

# Run CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run the program
./build/bin/ChessEngines