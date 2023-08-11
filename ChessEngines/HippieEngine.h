#pragma once
#include "DrunkEngine.h"

// HippieEngine prioritieses moves in this order:
// 1. Support undefended piece
// 2. Move undefended piece out of danger
// 3. Castle
// 4. Take undefended enemy piece
// 5. Move ends game
// 6. Check enemy king
// 7. Random Move

class HippieEngine : public Board, public Engine
{
public:
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) override;
protected:
	bool supportsUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool moveOutOfDanger(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool takesUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool endsGame(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool checkKing(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
};

