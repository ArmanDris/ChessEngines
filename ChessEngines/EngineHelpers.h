#pragma once
#include "Engine.h"

class EngineHelpers : public Board
{
protected:
	bool wins_game(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool takesUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool supportsUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool moveOutOfDanger(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool checkKing(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool takes(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
};

