#include "DrunkEngine.h"
#include <random>

std::pair<sf::Vector2i, sf::Vector2i> DrunkEngine::returnMove(const Board& b)
{
	static_cast<Board&>(*this) = b;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> wins_game;
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

		if (moveWinsGame(oldSquare, newSquare))
			wins_game.push_back(move);


		// Check if move checks enemy king
		if (checkKing(oldSquare, newSquare))
			king_check.push_back(move);

		// Check if move takes undefended enemy piece
		if (takesUndefended(oldSquare, newSquare))
			take_undefended.push_back(move);

		// Check if move takes enemy piece
		if (takes(oldSquare, newSquare))
			take.push_back(move);

		// Check if move castles
		if (moveIsCastle(oldSquare, newSquare))
			castle.push_back(move);
	}

	if (wins_game.size() > 0)       return wins_game[0];
	if (king_check.size() > 0)      return king_check[0];
	if (take_undefended.size() > 0) return take_undefended[0];
	if (take.size() > 0)            return take[0];
	if (castle.size() > 0)          return castle[0];
	return moves[0];

}

bool DrunkEngine::moveWinsGame(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	DrunkEngine clone = *this;
	clone.makeMove(oldSquare, newSquare);
	if (board[oldSquare.x][oldSquare.y].getColor() == Color::White && clone.whiteVictory) return true;
	if (board[oldSquare.x][oldSquare.y].getColor() == Color::Black && clone.blackVictory) return true;
	return false;
}

// True if move checks enemy king
bool DrunkEngine::checkKing(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	Color ally_color = board[oldSquare.x][oldSquare.y].getColor();
	Color enemy_color = ally_color == Color::White ? Color::Black : Color::White;

	DrunkEngine clone = *this;
	clone.movePiece(oldSquare, newSquare);
	return clone.isKingInCheck(enemy_color);
}

// True if move takes undefended enemy piece
bool DrunkEngine::takesUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	// True if there is an enemy piece at newSquare AND it is not in check by any enemy
	return board[newSquare.x][newSquare.y] && !isSquareInCheck(newSquare, board[newSquare.x][newSquare.y].getColor());
}

// True if move takes enemy piece
bool DrunkEngine::takes(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	return board[newSquare.x][newSquare.y] || moveIsEnPassent(oldSquare, newSquare);
}

