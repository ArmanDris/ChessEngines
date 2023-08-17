# ChessEngines

This project is built on top of the chess game I made 😀. I want to make some chess engines to play against eachother so I can rank them and maybe put them against human players.

## The three simple chess Engines:
1. RandomEngine
Will just make a random legal move <br> <br>
2. Drunk engine
Prioritieses moves in this order: <br>
 i.   Move wins game <br>
 ii.  Check enemy king <br>
 iii. Take undefended enemy piece <br>
 iv.  Castle <br>
 v.   Random Move <br> <br>
3. Hippie Engine
Prioritieses moves in this order: <br>
 i.   Move ends game <br>
 ii.  Take undefended enemy piece <br>
 iii. Support undefended piece <br>
 iv.  Move undefended piece out of danger <br>
 v.   Castle <br>
 vi.  Check enemy king <br>
 vii. Random Move <br> <br>
4. BotFeaster
Bot feaster has additional logic to not hang pieces.Prioritises moves in this order:
 i.    Move wins game <br>
 ii.   Take hanging enemy piece <br>
 iii.  Make positive trade <br>
 iv.   Check enemy king <br>
 v.    Move pieces being attacked by lower value away <br>
 vi.   Defend pieces being attacked by equal or higher value <br>
 vii.  Castle <br>
 viii. Random Move <br>

# Tournament Results
##DrunkEngine(White) vs. BotFeaster(Black) <br>
White wins: 22, Black wins: 338, Draws: 140
##DrunkEngine(White) vs. HippieEngine(Black) <br>


### 8 - 16
I finished BotFeaster! I also added the 50 move rule because that is the only bot good enough to not draw eventually. Now I am done with these simple engines and I can either move onto some very good engines or I can implement an elo rating system for the four engines I have made already.

### 8 - 14
I finished HippieEngine! Now I will try to make the best engine so far: BotFeaster. It will be the best I can come up with without looking more than one move into the game. Then I can finally work on engines that will be able to compete with the best human players.

### 8 - 13
Fixed bug in supportsUndefended() in HippieEngine. The issue was isAttackingSquare() does not count same color pieces so it was nor working propertly in HippieEngine. I also adjusted HippieEngine's priorites to take undefended pieces otherwise it is too much of a push over.
Still need to debug: support_undefended, move_out_of_danger, take_undefended

### 8 - 12
DrunkEngine should be all set up. Putting it against RandomEngine it will win or draw every time. I see that right away Drunkbot will just take all the pieces and then lost all of its pieces trying to check the enemy king. I think that HippieEngine will be able to smoke them both when it is finished.

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
