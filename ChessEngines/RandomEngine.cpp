#include "RandomEngine.h"

bool RandomEngine::makeRandomMove(Piece b[8][8]) {
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves = getPossibleMoves();
	if (moves.size() == 0) return false;
	// Random number generation setup
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<std::size_t> dis(0, moves.size() - 1);
	std::pair<sf::Vector2i, sf::Vector2i> move = moves[dis(gen)];

	return true;
}