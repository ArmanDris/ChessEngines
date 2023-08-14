#include "EngineHelpers.h"

bool EngineHelpers::wins_game(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	EngineHelpers clone = *this;
	clone.makeMove(oldSquare, newSquare);
	if (board[oldSquare.x][oldSquare.y].getColor() == Color::White && clone.whiteVictory) return true;
	if (board[oldSquare.x][oldSquare.y].getColor() == Color::Black && clone.blackVictory) return true;
	return false;
}

// True if move takes undefended enemy piece
bool EngineHelpers::takesUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	Color ally_color = board[oldSquare.x][oldSquare.y].getColor();
	Color enemy_color = ally_color == Color::White ? Color::Black : Color::White;
	// True if there is an enemy piece at newSquare AND it is not attacked by the enemy
	return board[newSquare.x][newSquare.y] && !square_is_attacked_by(newSquare, enemy_color);
}

// True if move supports undefended piece
bool EngineHelpers::supportsUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	Color ally_color = board[oldSquare.x][oldSquare.y].getColor();
	Color enemy_color = ally_color == Color::White ? Color::Black : Color::White;

	// Test to see if move will support piece at x,y
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {

			// If there is an ally piece at x,y
			if (board[x][y].getColor() != ally_color) continue;

			// If ally piece is already defended continue
			if (square_is_attacked_by(sf::Vector2i(x, y), ally_color)) continue;

			// If after move ally piece is defended, return true
			EngineHelpers clone = *this;
			clone.movePiece(oldSquare, newSquare);
			if (clone.square_is_attacked_by(sf::Vector2i(x, y), ally_color)) return true;
		}
	}
	return false;
}

bool EngineHelpers::moveOutOfDanger(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	Color ally_color = board[oldSquare.x][oldSquare.y].getColor();
	Color enemy_color = ally_color == Color::White ? Color::Black : Color::White;
	int x = oldSquare.x, y = oldSquare.y;

	// If piece is not attacked by enemy, return false
	if (!square_is_attacked_by(oldSquare, enemy_color)) return false;
	EngineHelpers clone = *this;
	if (!clone.makeMove(oldSquare, newSquare)) return false;					// Skip if move is invalid
	if (!clone.square_is_attacked_by(newSquare, enemy_color)) return true;      // If after move square is not attacked by enemy, return true
	return false;
}

bool EngineHelpers::checkKing(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	Color ally_color = board[oldSquare.x][oldSquare.y].getColor();
	Color enemy_color = ally_color == Color::White ? Color::Black : Color::White;

	EngineHelpers clone = *this;
	clone.movePiece(oldSquare, newSquare);
	return clone.isKingInCheck(enemy_color);
}

// True if move takes enemy piece
bool EngineHelpers::takes(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	return board[newSquare.x][newSquare.y] || moveIsEnPassent(oldSquare, newSquare);
}
