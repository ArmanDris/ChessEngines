#include "RandomEngine.h"

// Makes a random move
std::pair<sf::Vector2i, sf::Vector2i> RandomEngine::returnMove(const Board& b) {
	static_cast<Board&>(*this) = b;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves = get_moves();
	if (moves.size() == 0) throw std::invalid_argument("No possible moves");
	// Random number generation setup
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<std::size_t> dis(0, moves.size() - 1);
	std::pair<sf::Vector2i, sf::Vector2i> move = moves[dis(gen)];

	return move;
}