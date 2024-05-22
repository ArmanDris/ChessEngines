@echo off
rem Generate build files
cmake -S . -B build -G "MinGW Makefiles"
rem Build ChessEngines
cmake --build build --config Release
rem Run the program
build\bin\ChessEngines