#!/bin/bash
# Generate build files
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
# Build ChessEngines
cmake --build build
# Run the program
cd build/bin
./ChessEngines
