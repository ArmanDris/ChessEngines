#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.h"

class Engine
{
public:
	virtual std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) = 0;
};

