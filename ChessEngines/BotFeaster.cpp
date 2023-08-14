#include "BotFeaster.h"
#include <random>

std::pair<sf::Vector2i, sf::Vector2i> BotFeaster::returnMove(const Board& b)
{
    static_cast<Board&>(*this) = b;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;


	moves = get_moves();
	if (moves.size() == 0) throw std::exception("No moves available");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(moves.begin(), moves.end(), gen);

	// Cycle though moves
	for (std::pair<sf::Vector2i, sf::Vector2i> move : moves) {
		sf::Vector2i oldSquare = move.first;
		sf::Vector2i newSquare = move.second;

		// If move wins game dont bother with other moves
		if (wins_game(oldSquare, newSquare)) 
			return move;

	}

	return moves[0];
}
