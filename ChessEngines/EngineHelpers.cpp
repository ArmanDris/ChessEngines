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
bool EngineHelpers::takes_hanging(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	Color ally_color = board[oldSquare.x][oldSquare.y].getColor();
	Color enemy_color = ally_color == Color::White ? Color::Black : Color::White;
	// True if there is an enemy piece at newSquare AND it is not attacked by the enemy
	return board[newSquare.x][newSquare.y] && !square_is_attacked_by(newSquare, enemy_color);
}

// True if move supports undefended piece under attack
bool EngineHelpers::supportsUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	Color ally_color = board[oldSquare.x][oldSquare.y].getColor();
	Color enemy_color = ally_color == Color::White ? Color::Black : Color::White;

	// Test to see if move will support piece at x,y
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {

			// If there is an ally piece at x,y
			if (board[x][y].getColor() != ally_color) continue;

			// If not under attack, continue
			if (!square_is_attacked_by(sf::Vector2i(x, y), enemy_color)) continue;

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

bool EngineHelpers::checks_enemy(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
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

// True if taking a higher value piece
bool EngineHelpers::trades_positive(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	if (!board[newSquare.x][newSquare.y]) return false;

	int old_square_value = board[oldSquare.x][oldSquare.y].getValue();
	int new_square_value = board[newSquare.x][newSquare.y].getValue();
	return board[newSquare.x][newSquare.y] && old_square_value < new_square_value;
}

// True if retreats a high value piece being attacked by a lower value piece
bool EngineHelpers::protects_high_value(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	Color ally_color = board[oldSquare.x][oldSquare.y].getColor();
	Color enemy_color = ally_color == Color::White ? Color::Black : Color::White;
	if (!square_is_attacked_by(oldSquare, enemy_color)) return false;
	EngineHelpers clone = *this;
	clone.makeMove(oldSquare, newSquare);
	if (!clone.square_is_attacked_by(newSquare, enemy_color)) return true;
	return false;
}

// True if moves takes a piece of equal value
bool EngineHelpers::trades_equal_value(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	if (!board[newSquare.x][newSquare.y]) return false;
	int new_square_value = board[newSquare.x][newSquare.y].getValue();
	int old_square_value = board[oldSquare.x][oldSquare.y].getValue();

	if (new_square_value == old_square_value) return true;
	return false;
}

bool EngineHelpers::moveIsPushPawn(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	return board[oldSquare.x][oldSquare.y].getType() == Type::Pawn;
}

// Get lowest value piece of color c that is attacking sq
int EngineHelpers::get_lowest_attacker(sf::Vector2i sq, Color c) const
{
	sf::Vector2i highest_value_attacker(-1, -1);
	int value = 0;
	// If any of the enemy pieces on the board can make a valid move into p then p is in check
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; ++j) {
			if (sf::Vector2i(i, j) != sq) continue;
			if (!board[i][j]) continue;
			if (board[i][j].getColor() != c) continue;

			if (piece_is_attacking_square(sf::Vector2i(i, j), sq) && board[i][j].getValue() > value) {
				highest_value_attacker = sf::Vector2i(i, j);
				value = board[i][j].getValue();
			}
		}
	}
	return value;
}

// True if newSquare is attacked by enemy and not defended by ally
bool EngineHelpers::does_move_hang_pice(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	Color ally_color = board[oldSquare.x][oldSquare.y].getColor();
	Color enemy_color = ally_color == Color::White ? Color::Black : Color::White;

	EngineHelpers clone = *this;
	clone.makeMove(oldSquare, newSquare);
	return clone.square_is_attacked_by(newSquare, enemy_color) && !clone.square_is_attacked_by(newSquare, ally_color);
}