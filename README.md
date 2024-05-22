# ChessEngines

A project I built on top of my Chess game. You can play against a friend or against one of five bots. This project is written in C++ and uses SFML for its GUI.

![Chess Demo](src/chess_game_vid.gif)

### Building from source:

This project uses CMake. So first you generate the build system (first two commands) then you run the program from the project directory (last command).
#### Linux/Mac
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/bin/ChessEngines
```

#### Windows

Make sure you have gcc and g++ installed with mingw installed [www.mingw.org](www.mingw.org)

```
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build --config Release
build/bin/ChessEngines
```

### Usage example:

By default, the GameBoard object will not call any of the bots to make moves. Call GameBoard's `setPlayer()` function to assign a bot to a particular color. `setPlayer` expects a pointer to an `Engine` object and a color. Engine classes can be one of:

- `RandomEngine` (SimpleEngines.h)
- `DrunkEngine` (SimpleEngines.h)
- `HippieEngine` (SimpleEngines.h)
- `MinMaxEngine` (MinMaxEngine.h)

Colors can be one of `Color::White` or `Color::Black`. The example below would set the black player to the `MinMaxEngine` engine.


```
...
#include "MinMaxEngine.h"

int main() {

    ...

    GameBoard b;

    MinMaxEngine mm;
    b.setPlayer(&mm,   Color::Black);

    ...

}
```

### Key features:

**GameBoard's importFEN(string FEN):** Updates the board state to exactly match the FEN position.

**GameBoard's start_tournement(int num_games):** Game Board will simulate num_games and print the number of wins/draws for each color. (Meant to be used to compare bot preformance)

**GameBoard's saveLog():** Saves the game log file log.txt. (Accepts a string for custom filename)
Using CMake from the command line is straightforward as well.