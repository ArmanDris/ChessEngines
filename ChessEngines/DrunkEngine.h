#pragma once
#include "Board.h"
#include "Engine.h"

// DrunkEngine prioritieses moves in this order:
// 1. Check enemy king
// 2. Take undefended enemy piece
// 3. Take defended enemy piece
// 4. Castle
// 5. Random Move

class DrunkEngine : public Board, public Engine
{
public:
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(Piece* b[8][8], char turn) override;
private:
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> king_check;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> take_undefended;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> take;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> support_undefended;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> castle;
};

