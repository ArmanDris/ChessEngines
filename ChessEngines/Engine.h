#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.h"

class Engine
{
public:
	virtual std::pair<sf::Vector2i, sf::Vector2i> returnMove(Piece b[8][8], char turn) = 0;
};

