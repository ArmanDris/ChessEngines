 #include "MinMaxEngine.h"

 std::pair<sf::Vector2i, sf::Vector2i> MinMaxEngine::returnMove(const Board& board)
 {
     Board b = board;
     std::vector<move> moves = b.getMoves();

     move best_move = moves[0];
     int best_eval = INT_MIN;
     for (move m : moves) {
         b.makeSafeMove(m.first, m.second);
         int eval = -search(b, 3);
         b.undoMove();

         if (eval > best_eval) {
			 best_eval = eval;
			 best_move = m;
		 }
     }

     return best_move;
 }

 int MinMaxEngine::search(Board & b, int depth) {
     if (depth == 0)
         return boardEval(b);

     std::vector<move> legal_moves = b.getMoves();
     if (legal_moves.size() == 0) {
         if (b.isPlayerInCheck())
             return -1000000;
         else if (b.isDraw())
             return 0;
         else
             return 1000000;
     }

     int best_eval = INT_MIN;
     for (auto move : legal_moves) {
         b.makeSafeMove(move.first, move.second);
         int evaluation = -search(b, depth - 1);
         best_eval = std::max(best_eval, evaluation);
         b.undoMove();
     }

     return best_eval;
 }

 int MinMaxEngine::boardEval(const Board& b) {
     int total_eval = 0;
     total_eval += materialEval(b);

     int perspective = b.isWhiteTurn() ? 1 : -1;
     return total_eval * perspective;
 }

 int MinMaxEngine::materialEval(const Board& b) {
     int white_material = 0;
     int black_material = 0;

     for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++)
     {
         Piece p = b.getPiece(sf::Vector2i(x, y));

         if (p.getType() == Type::None) continue;

         switch (p.getColor()) {
             case Color::White: white_material += p.getValue(); break;
             case Color::Black: black_material += p.getValue(); break;
         }
        
     }

     return white_material - black_material;
 }

 void MinMaxEngine::perftBenchmark() {
     Board br;
     std::cout << "Size of bord obj " << sizeof(br) << std::endl << std::endl;
     std::cout << "Benchmarking first 5 ply's on starting board: \n";

     Board b;
     sf::Clock c;

     for (int i = 1; i <= 5; i++) {
         std::cout << prefSearch(b, i) << " in ";
         sf::Time t = c.restart();
         std::cout << t.asSeconds() << " seconds \n";
     }
 }

 int MinMaxEngine::prefSearch(Board& b, int depth) {
     if (depth == 0)
         return 1;

     int num_positions = 0;

     std::vector<move> legal_moves = b.getMoves();

     for (auto move : legal_moves) {
         b.makeSafeMove(move.first, move.second);
         num_positions += prefSearch(b, depth - 1);
         b.undoMove();
     }

     return num_positions;
 }