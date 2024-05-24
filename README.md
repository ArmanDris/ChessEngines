# ChessEngines

An open source C++ chess game built with SFML and CMake. You can play against a friend or against one of five bots. After you have [built the program from source](#building-from-source) you can move the pieces with your mouse and undo moves with the back arrow. Eventually you will want to [play against bots or play bots against each other](#usage-example).

![Chess Demo](src/chess_game_vid.gif)

### Building from source:

You will need to install cmake and git to build this program. 

First you generate the build system (first two commands) then you run the program from the project directory (third command).

#### Linux/Mac

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/bin/ChessEngines
```

(I made a script with these three in `./run.sh`)

#### Windows

For windows you will also need to install mingw to get gcc and g++. Make sure you add your C:/mingw/bin folder to your PATH. [www.mingw.org](www.mingw.org).

```
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build --config Release
build/bin/ChessEngines
```

(I made a script with these three in `./run.bat`)

### Usage example:

By default, the GameBoard object will not call any of the bots to make moves. Call `GameBoard`'s `setPlayer()` function to assign a bot to a particular color. `setPlayer()` expects a pointer to an `Engine` object and a color. Engine classes can be one of:

- `RandomEngine` (SimpleEngines.h)
- `DrunkEngine` (SimpleEngines.h)
- `HippieEngine` (SimpleEngines.h)
- `MinMaxEngine` (MinMaxEngine.h)

Colors can be one of `Color::White` or `Color::Black`. 

The example below would set the black player to the `MinMaxEngine` engine.

```
...
#include "MinMaxEngine.h"

int main() {

    ...
    MinMaxEngine mm;

    GameBoard b;
    b.setPlayer(&mm, Color::Black);

    ...

}
```

Additionally, you can call `GameBoard`'s `preformCPUMoves()` to automatically call upon the bots to make moves. By default you press the space bar to trigger a bot to move.


### Key features:

**GameBoard's importFEN(string FEN):** Updates the board state to exactly match the FEN position.

**GameBoard's start_tournement(int num_games):** Game Board will simulate num_games and print the number of wins/draws for each color. (Meant to be used to compare bot preformance)

**GameBoard's saveLog():** Saves the game log file log.txt. (Accepts a string for custom filename)
Using CMake from the command line is straightforward as well.