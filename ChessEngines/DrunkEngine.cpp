#include "DrunkEngine.h"
#include <random>

std::pair<sf::Vector2i, sf::Vector2i> DrunkEngine::returnMove(const Board& b)
{
	static_cast<Board&>(*this) = b;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> king_check;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> take_undefended;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> take;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> support_undefended;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> castle;

	moves = get_moves();
	if (moves.size() == 0) throw std::exception("No moves available");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(moves.begin(), moves.end(), gen);

	// Cycle though moves
	for (std::pair<sf::Vector2i, sf::Vector2i> move : moves) {
		sf::Vector2i oldSquare = move.first;
		sf::Vector2i newSquare = move.second;

		// Check if move checks enemy king
		if (checkKing(oldSquare, newSquare)) {
			king_check.push_back(move);
			continue;
		}

		// Check if move takes undefended enemy piece
		if (takesUndefended(oldSquare, newSquare)) {
			take_undefended.push_back(move);
			continue;
		}

		// Check if move takes enemy piece
		if (takes(oldSquare, newSquare)) {
			take.push_back(move);
			continue;
		}

		// Check if move supports undefended piece
		if (supportsUndefended(oldSquare, newSquare)) {
			support_undefended.push_back(move);
			continue;
		}

		// Check if move castles
		if (moveIsCastle(oldSquare, newSquare)) {
			moveIsCastle(oldSquare, newSquare);
			castle.push_back(move);
			continue;
		}
	}

	if (king_check.size() > 0)      return king_check[0];
	if (take_undefended.size() > 0) return take_undefended[0];
	if (take.size() > 0)            return take[0];
	if (castle.size() > 0)          return castle[0];
	return moves[0];

}

// True if move checks enemy king
bool DrunkEngine::checkKing(sf::Vector2i oldSquare, sf::Vector2i newSquare)
{
	Color ally_color = board[oldSquare.x][oldSquare.y].getColor();
	Color enemy_color = ally_color == Color::White ? Color::Black : Color::White;

	DrunkEngine clone = *this;
	clone.movePiece(oldSquare, newSquare);
	return clone.isKingInCheck(enemy_color);
}

// True if move takes undefended enemy piece
bool DrunkEngine::takesUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare)
{
	return false;
}

// True if move takes enemy piece
bool DrunkEngine::takes(sf::Vector2i oldSquare, sf::Vector2i newSquare)
{
	return board[newSquare.x][newSquare.y] || moveIsEnPassent(oldSquare, newSquare);
}

// True if move supports undefended piece
bool DrunkEngine::supportsUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare)
{
	return false;
}

