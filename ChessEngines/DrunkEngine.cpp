#include "DrunkEngine.h"
#include <random>

std::pair<sf::Vector2i, sf::Vector2i> DrunkEngine::returnMove(Piece* b[8][8], char turn)
{
	setTurn(turn);
	importBoard(b);
	
	moves = getPossibleMoves();
	std::cout << "Possible Moves: " << moves.size() << std::endl;
	char enemy_color = (turn == 'w') ? 'b' : 'w';

	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(moves.begin(), moves.end(), gen);

	// Cycle though moves
	for (std::pair<sf::Vector2i, sf::Vector2i> move : moves) {

		// Test castling first because you dont need to make the move to check if it is a castle
		bool is_castle = moveIsCastling(board[move.first.x][move.first.y], move.second);
		if (is_castle) castle.push_back(move);

		Piece* board_copy[8][8];
		cloneBoard(board_copy);

		// Make move to check if it fulfills any of the priorities
		makeMove(move.first, move.second);

		bool is_enemy_king_in_check = isKingInCheck(enemy_color);
		if (is_enemy_king_in_check) king_check.push_back(move);

		bool takes_undefended = isSquareInCheck(move.second, turn);
		if (takes_undefended) take_undefended.push_back(move);

		bool takes_piece = board[move.second.x][move.second.y];
		if (takes_piece) take.push_back(move);

		// Revert board to previous state
		importBoard(board_copy);
		setTurn(turn);
	}

	if (king_check.size() > 0)      return king_check[0];
	if (take_undefended.size() > 0) return take_undefended[0];
	if (take.size() > 0)            return take[0];
	if (castle.size() > 0)          return castle[0];
	return moves[0];

}
