#include "HippieEngine.h"
#include <random>

std::pair<sf::Vector2i, sf::Vector2i> HippieEngine::returnMove(const Board& b)
{
	static_cast<Board&>(*this) = b;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;

	//std::vector<std::pair<sf::Vector2i, sf::Vector2i>> ends_game;
	//std::vector<std::pair<sf::Vector2i, sf::Vector2i>> take_undefended;
	//std::vector<std::pair<sf::Vector2i, sf::Vector2i>> support_undefended;
	//std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_out_of_danger;
	//std::vector<std::pair<sf::Vector2i, sf::Vector2i>> castle;
	//std::vector<std::pair<sf::Vector2i, sf::Vector2i>> king_check;

	moves = get_moves();
	if (moves.size() == 0) throw std::exception("No moves available");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(moves.begin(), moves.end(), gen);

	// Cycle though moves
	for (std::pair<sf::Vector2i, sf::Vector2i> move : moves) {
		sf::Vector2i oldSquare = move.first;
		sf::Vector2i newSquare = move.second;

		if (wins_game(oldSquare, newSquare))
			return move;

		// Check if move takes undefended enemy piece
		if (takes_hanging(oldSquare, newSquare))
			return move;

		// Check if move supports undefended piece
		if (supportsUndefended(oldSquare, newSquare))
			return move;

		if (moveOutOfDanger(oldSquare, newSquare))
			return move;

		// Check if move castles
		if (moveIsCastle(oldSquare, newSquare))
			return move;

		// Check if move checks enemy king
		if (checks_enemy(oldSquare, newSquare))
			return move;
	}

	// Still need to debug: support_undefended, move_out_of_danger, take_undefended

	//if (ends_game.size() > 0)          return ends_game[0];
	//if (take_undefended.size() > 0)    return take_undefended[0];
	//if (support_undefended.size() > 0) return support_undefended[0];
	//if (move_out_of_danger.size() > 0) return move_out_of_danger[0];
	//if (castle.size() > 0)             return castle[0];
	//if (king_check.size() > 0)         return king_check[0];
	return moves[0];
}