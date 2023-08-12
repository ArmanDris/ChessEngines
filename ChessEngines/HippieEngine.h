#pragma once
#include "DrunkEngine.h"

// HippieEngine prioritieses moves in this order:
// 1. Move ends game
// 2. Support undefended piece
// 3. Move undefended piece out of danger
// 4. Castle
// 5. Take undefended enemy piece
// 6. Check enemy king
// 7. Random Move

class HippieEngine : public Board, public Engine
{
public:
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) override;
protected:
	bool endsGame(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool supportsUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool pieceIsDefended(sf::Vector2i oldSquare) const;
	bool moveOutOfDanger(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool takesUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool checkKing(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
};

