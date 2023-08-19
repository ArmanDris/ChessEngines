#include "SimpleEngines.h"

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

std::pair<sf::Vector2i, sf::Vector2i> DrunkEngine::returnMove(const Board& b)
{
	static_cast<Board&>(*this) = b;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> game_winner;
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

		if (wins_game(oldSquare, newSquare))
			game_winner.push_back(move);

		// Check if move checks enemy king
		if (checks_enemy(oldSquare, newSquare))
			king_check.push_back(move);

		// Check if move takes undefended enemy piece
		if (takes_hanging(oldSquare, newSquare))
			take_undefended.push_back(move);

		// Check if move takes enemy piece
		if (takes(oldSquare, newSquare))
			take.push_back(move);

		// Check if move castles
		if (moveIsCastle(oldSquare, newSquare))
			castle.push_back(move);
	}

	if (game_winner.size() > 0)     return game_winner[0];
	if (king_check.size() > 0)      return king_check[0];
	if (take_undefended.size() > 0) return take_undefended[0];
	if (take.size() > 0)            return take[0];
	if (castle.size() > 0)          return castle[0];
	return moves[0];

}

std::pair<sf::Vector2i, sf::Vector2i> HippieEngine::returnMove(const Board& b)
{
	static_cast<Board&>(*this) = b;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> ends_game;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> take_undefended;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> support_undefended;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_out_of_danger;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> castle;
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

		if (wins_game(oldSquare, newSquare))
			ends_game.push_back(move);

		// Check if move takes undefended enemy piece
		if (takes_hanging(oldSquare, newSquare))
			take_undefended.push_back(move);

		// Check if move supports undefended piece
		if (supportsUndefended(oldSquare, newSquare))
			support_undefended.push_back(move);

		if (moveOutOfDanger(oldSquare, newSquare))
			move_out_of_danger.push_back(move);

		// Check if move castles
		if (moveIsCastle(oldSquare, newSquare))
			castle.push_back(move);

		// Check if move checks enemy king
		if (checks_enemy(oldSquare, newSquare))
			king_check.push_back(move);
	}

	// Still need to debug: support_undefended, move_out_of_danger, take_undefended

	if (ends_game.size() > 0)          return ends_game[0];
	if (take_undefended.size() > 0)    return take_undefended[0];
	if (support_undefended.size() > 0) return support_undefended[0];
	if (move_out_of_danger.size() > 0) return move_out_of_danger[0];
	if (castle.size() > 0)             return castle[0];
	if (king_check.size() > 0)         return king_check[0];
	return moves[0];
}

std::pair<sf::Vector2i, sf::Vector2i> BotFeasterEngine::returnMove(const Board& b)
{
	static_cast<Board&>(*this) = b;
	// 1. Move wins game
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_takes_hanging;			// 2. Take hanging enemy piece
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_trades_positive;		// 3. Make positive trade
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_checks_enemy;			// 4. Check enemy king
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_protects_high_value;	// 5. Move pieces being attacked by lower value away
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_supports_undefended;	// 6. Defend pieces being attacked by equal or higher value
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_castles;				// 7. Castle
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_pushes_pawn;			// 7. Push pawn
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_trades_equal;			// 8. Make equal trade
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_does_not_hang;			// 8. Make random non-hanging move
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;						// 9. Random Move


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

		if (takes_hanging(oldSquare, newSquare))
			move_takes_hanging.push_back(move);

		if (trades_positive(oldSquare, newSquare))
			move_trades_positive.push_back(move);

		if (checks_enemy(oldSquare, newSquare) && !does_move_hang_pice(oldSquare, newSquare))
			move_checks_enemy.push_back(move);

		if (protects_high_value(oldSquare, newSquare) && !does_move_hang_pice(oldSquare, newSquare))
			move_protects_high_value.push_back(move);

		if (supportsUndefended(oldSquare, newSquare) && !does_move_hang_pice(oldSquare, newSquare))
			move_supports_undefended.push_back(move);

		if (moveIsCastle(oldSquare, newSquare))
			move_castles.push_back(move);

		if (moveIsPushPawn(oldSquare, newSquare) && !does_move_hang_pice(oldSquare, newSquare))
			move_pushes_pawn.push_back(move);

		if (trades_equal_value(oldSquare, newSquare))
			move_trades_equal.push_back(move);

		if (!does_move_hang_pice(oldSquare, newSquare))
			move_does_not_hang.push_back(move);
	}

	if (!move_takes_hanging.empty())		return move_takes_hanging[0];		// Tested. Works
	if (!move_trades_positive.empty())		return move_trades_positive[0];		// Tested. Works
	if (!move_checks_enemy.empty())			return move_checks_enemy[0];		// Tested. Works
	if (!move_protects_high_value.empty())	return move_protects_high_value[0];	// Tested. Works
	if (!move_supports_undefended.empty())	return move_supports_undefended[0];	// I feel like ive seen it work
	if (!move_castles.empty())				return move_castles[0];				// Tested. Works
	if (!move_pushes_pawn.empty())			return move_pushes_pawn[0];			// 
	if (!move_trades_equal.empty())			return move_trades_equal[0];		// Tested. Works
	if (!move_does_not_hang.empty())		return move_does_not_hang[0];		// I doubt this does not work
	std::cout << "making random move\n";
	return moves[0];
}
