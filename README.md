# ChessEngines

This project is built on top of the chess game I made 😀. I want to make some chess engines to play against eachother so I can rank them and maybe put them against human players.

### 9 - 12
On the 9th I made some solid changes. Today I decoupled checking for game over from making a move so now the board's caller is responsible to check when appropriate. This has lead to insanly satisfying preformance improvements (esp with the preformance I got from the changes on the 9th). <br /> <br />
Perft after checking for game over is not required after making a move on board <br />
<img width="280" alt="Screenshot 2023-09-12 at 11 47 22 AM" src="https://github.com/ArmanDris/ChessEngines/assets/59405643/825aa1ec-669a-4b03-a333-58d17ae65a7c">



### 9 - 09
I made this way way way faster now with smarter checking logic. I still have to main things to do. I will probably get to them after implementing v1 of minimax.
The two things are:
 1. Faster logic for seein if the player has valid moves. Instead of generating the valid moves for all the pieces then returning when the first one is legal. Should generate legal moves piece by piece and return when the first piece has a legal move.
 2. Keep track of where the kings are so I dont have to look them up.

### 9 - 04
I have just gutted and remade the whole program. Today I got it running 40% faster than before 🥳. I was expecting more but for a while I thought I made it slower so at least that is not the case. Now I can fianlly work on implementing minimax.
My board does not allow for underpromotion so it fails some perft tests but realistically I do not care about underpromotion. I also broke my other engines while remaking the board class ☹, so perhaps I will fix them. The win detection for the new board class is also a bit iffy.

Perft after refactor: <br/>
![image](https://github.com/ArmanDris/ChessEngines/assets/59405643/728a7a63-05df-45a5-a0fe-901673f50e50) <br/>

Perft before refactor:<br/>
![image](https://github.com/ArmanDris/ChessEngines/assets/59405643/23f47fa1-c2d1-42d9-baa9-7087cb478d6c) <br/>

Perft before any optimisations: <br/>
![25 august benchmark](https://github.com/ArmanDris/ChessEngines/assets/59405643/b6b884ed-3ae7-4b1e-957c-742317fd933f)


### 8 - 26
I have found out that my program runs way too slowly for minimax. I have done some simple optimisation like passing by reference in board. Ideas to further optimise are:
1. Remove the piece class and use just an enum
2. Create a critical_log, that only logs critical moves. This way I only need to look through the that log for move logic.
3. Go from sf::Vector2i to std::pair to reduce memory usage.

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
