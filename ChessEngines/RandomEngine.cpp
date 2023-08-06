#include "RandomEngine.h"

// Makes a random move
std::pair<sf::Vector2i, sf::Vector2i> RandomEngine::makeMove(Piece* b[8][8], char turn) {
	if (turn == 'w') whiteTurn = true;
	else             whiteTurn = false;

	importBoard(b);
	return { sf::Vector2i(-1, -1), sf::Vector2i(-1, -1) };

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves = getPossibleMoves();
	if (moves.size() == 0) return {sf::Vector2i(-1, -1), sf::Vector2i(-1, -1)};
	// Random number generation setup
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<std::size_t> dis(0, moves.size() - 1);
	std::pair<sf::Vector2i, sf::Vector2i> move = moves[dis(gen)];

	return move;
}