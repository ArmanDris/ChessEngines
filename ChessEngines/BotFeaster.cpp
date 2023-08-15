#include "BotFeaster.h"
#include <random>

std::pair<sf::Vector2i, sf::Vector2i> BotFeaster::returnMove(const Board& b)
{
    static_cast<Board&>(*this) = b;

																					// 1. Move wins game
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_takes_hanging;			// 2. Take hanging enemy piece
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_trades_positive;		// 3. Make positive trade
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_checks_enemy;			// 4. Check enemy king
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_protects_high_value;	// 5. Move pieces being attacked by lower value away
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_supports_undefended;	// 6. Defend pieces being attacked by equal or higher value
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_castles;				// 7. Castle
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> move_trades_equal;			// 8. Make equal trade
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;						// 9. Random Move


	moves = get_moves();
	if (moves.size() == 0) throw std::exception("No moves available");

	//std::random_device rd;
	//std::mt19937 gen(rd());
	//std::shuffle(moves.begin(), moves.end(), gen);

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

		if (checks_enemy(oldSquare, newSquare) && !does_move_lose_value(oldSquare, newSquare))
			move_checks_enemy.push_back(move);

		if (protects_high_value(oldSquare, newSquare) && !does_move_lose_value(oldSquare, newSquare))
			move_protects_high_value.push_back(move);

		if (supportsUndefended(oldSquare, newSquare) && !does_move_lose_value(oldSquare, newSquare))
			move_supports_undefended.push_back(move);

		if (moveIsCastle(oldSquare, newSquare))
			move_castles.push_back(move);

		if (trades_equal_value(oldSquare, newSquare))
			move_trades_equal.push_back(move);
	}

	// Eventually I want to cycle through these vectors and return the one that nets the most value
	
	//if (!move_takes_hanging.empty())		return move_takes_hanging[0];	// Tested. Works
	//if (!move_trades_positive.empty())		return move_trades_positive[0]; // Tested. Works
	if (!move_checks_enemy.empty())			return move_checks_enemy[0];	// Tested. Works
	//if (!move_protects_high_value.empty())	return move_protects_high_value[0];	// Tested. Works
	//if (!move_supports_undefended.empty())	return move_supports_undefended[0]; // Tested, may undefend a piece to defend a new piece
	//if (!move_castles.empty())				return move_castles[0];			// Tested. Works
	//if (!move_trades_equal.empty())			return move_trades_equal[0];		// Tested. Works
	std::cout << "making random move" << std::endl;
	return moves[0];
}
