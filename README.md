# ChessEngines
Chess engines

This project is built on top of the chess game I made 😀. I want to make some chess engines to play against eachother so I can rank them and maybe put them against human players.

### 8 - 11
I have finally made the game run smoothly again 😁. DrunkEngine still needs some logic but so far it is good. Engines now get passed a const reference to a board object so that they can set all their member variables equal to the GameBoard. This is important because en-passent and castling need the game log as well as the board state. This also cleans up the jank with needing to set the turn each time I call getMove(). Drunk move is also very very fun to watch.

### 8 - 09
I refactored for like 10 hours today. This is just straight up a new program despite looking the same. I removed all dynamic memory, all pointers to pieces and make a whole bunch of functions constants as well as removing some excess functions. I also massively simplified Piece objects to the point where they can just become a struct in Board. For now I need to work on castling logic and then figure out why the program runs so slowly then I can work on my DrunkEngine :).

### 8 -08
 - Need to remember to remove the taken piece after en passent

### 8 - 07
Now the GameBoard supports engines playing as black and white 🥳. My idea for the first real engine I will make is:
 - (DrunkEngine) Look through all the moves and rank them in this order
     1. Check enemy king
     2. Take undefended enemy piece
     3. Take defended enemy piece
     4. Castle
     5. Random Move
  
 - (HipsterEngine) I could also make one that plays similarly but prioritises like this:
     1. Support undefended piece
     2. Move undefended piece out of danger
     3. Take undefended enemy piece
     4. Check enemy king
     5. Random Move
   
After I make these two I can see if I can make them better by taking into account piece values.

### 08 - 05
Now I need to make srue Board can pass its board to other classes then I can make the game loop for the ChessEngine
