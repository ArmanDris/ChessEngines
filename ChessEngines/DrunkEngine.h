#pragma once
#include "Board.h"
#include "Engine.h"

// DrunkEngine prioritieses moves in this order:
// 1. Move wins game
// 1. Check enemy king
// 2. Take undefended enemy piece
// 3. Take defended enemy piece
// 4. Castle
// 5. Random Move

class DrunkEngine : public Board, public Engine
{
public:
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) override;
protected:
	bool moveWinsGame(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool checkKing(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool takesUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool takes(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
};

