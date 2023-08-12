#include "HippieEngine.h"
#include <random>

std::pair<sf::Vector2i, sf::Vector2i> HippieEngine::returnMove(const Board& b)
{
	static_cast<Board&>(*this) = b;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> support_undefended;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_out_of_danger;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> castle;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> take_undefended;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> ends_game;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> king_check;

	moves = get_moves();
	if (moves.size() == 0) throw std::exception("No moves available");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(moves.begin(), moves.end(), gen);

	// Cycle though moves
	for (std::pair<sf::Vector2i, sf::Vector2i> move : moves) {
		sf::Vector2i oldSquare = move.first;
		sf::Vector2i newSquare = move.second;

		if (endsGame(oldSquare, newSquare))
			ends_game.push_back(move);

		// Check if move supports undefended piece
		if (supportsUndefended(oldSquare, newSquare))
			support_undefended.push_back(move);

		if (moveOutOfDanger(oldSquare, newSquare))
			move_out_of_danger.push_back(move);

		// Check if move castles
		if (moveIsCastle(oldSquare, newSquare))
			castle.push_back(move);

		// Check if move takes undefended enemy piece
		if (takesUndefended(oldSquare, newSquare))
			take_undefended.push_back(move);

		// Check if move checks enemy king
		if (checkKing(oldSquare, newSquare))
			king_check.push_back(move);
	}

	//if (ends_game.size() > 0)          return ends_game[0];
	if (support_undefended.size() > 0) return support_undefended[0];
	//if (move_out_of_danger.size() > 0) return move_out_of_danger[0];
	//if (castle.size() > 0)             return castle[0];
	//if (take_undefended.size() > 0)    return take_undefended[0];
	std::cout << "Making random move" << std::endl;
	if (king_check.size() > 0)         return king_check[0];

	return moves[0];
}

// True if move supports undefended piece
bool HippieEngine::supportsUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	// Loop through all ally pieces
	for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
		Color ally_color = board[oldSquare.x][oldSquare.y].getColor();
		Color enemy_color = ally_color == Color::White ? Color::Black : Color::White;

		// If there is an ally piece at x,y
		if (board[x][y].getColor() != ally_color) continue;

		// If x,y is this piece continue
		if (x == oldSquare.x && y == oldSquare.y) continue;

		// If ally piece is already defended continue
		if (!pieceIsDefended(sf::Vector2i(x, y))) continue;

		// If after move ally piece is defended, return true
		HippieEngine clone = *this;
		clone.movePiece(oldSquare, newSquare);
		if (clone.pieceIsDefended(sf::Vector2i(x, y))) {
			std::cout << typeToString(board[x][y].getType()) << " " << x << "," << y << " will be defended" << std::endl;
			return true; 
		}
	}
	return false;
}

// True if another piece can attack this piece
bool HippieEngine::pieceIsDefended(sf::Vector2i oldSquare) const
{
	Color ally_color = board[oldSquare.x][oldSquare.y].getColor();
	for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
		// If piece is not ally piece continue
		if (board[x][y].getColor() != ally_color) continue;
		// If piece is oldSquare continue
		if (x == oldSquare.x && y == oldSquare.y) continue;
		// If piece can move to oldSquare return true
		if (piece_is_attacking_square(sf::Vector2i(x,y), oldSquare)) return true;
	}

	return false;
}

bool HippieEngine::moveOutOfDanger(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	return false;
}

// True if move takes undefended enemy piece
bool HippieEngine::takesUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	// True if there is an enemy piece at newSquare AND it is not in check by any enemy
	return board[newSquare.x][newSquare.y] && !isSquareInCheck(newSquare, board[newSquare.x][newSquare.y].getColor());
}

bool HippieEngine::endsGame(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	HippieEngine clone = *this;
	clone.makeMove(oldSquare, newSquare);
	if (board[oldSquare.x][oldSquare.y].getColor() == Color::White && clone.whiteVictory) return true;
	if (board[oldSquare.x][oldSquare.y].getColor() == Color::Black && clone.blackVictory) return true;
	return false;
}

bool HippieEngine::checkKing(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	return false;
}
