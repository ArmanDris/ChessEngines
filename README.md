# ChessEngines

A project I built on top of my Chess game. You can play against a friend or against one of five bots. This project is written in C++ and uses the SFML for its GUI.

![Chess Demo](Interesting%20files/chess_game_vid.gif)

### Setup:

To run this project clone the repo and open it in Visual Studio. The SFML files are included locally but you will have to edit the project settings to tell Visual Studio where to find them. You can find the steps to do this here: [https://www.sfml-dev.org/tutorials/2.6/start-vc.php](https://www.sfml-dev.org/tutorials/2.6/start-vc.php).

### Usage example:

By default the GameBoard object will not call any of the bots to make moves. Call GameBoard's setPlayer() function to assign a bot to a particular color. setPlayer expects a pointer to a Engine object and a color. Engine classes are defined in the SimpleEngines.h and the MinMaxEngine.h file. SimpleEngines.h includes RandomEngine,DrunkEngine, and HippieEngine classes. MinMaxEngine.h includes the MinMaxEngine class. Colors can be one of Color::White or Color::Black. The example below would set the black player to theMinMaxEngine engine.

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

### Key surface level features:

**GameBoard's importFEN(string FEN):** Updates the board state to exactly match the FEN position.

**GameBoard's start_tournement(int num_games):** Game Board will simulate num_games and print the number of wins/draws for each color. (Meant to be used to compare bot preformance)

**GameBoard's saveLog():** Saves the game log file log.txt. (Accepts a string for custom filename)

### Documentation:

**Game Board:**


**Board:**


**Piece:**


**Engine:**


**MinMaxEngine:**


**SimpleEngines:**
