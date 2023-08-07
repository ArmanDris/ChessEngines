# ChessEngines
Chess engines

This project is built on top of the chess game I made 😀. I want to make some chess engines to play against eachother so I can rank them and maybe put them against human players.

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
