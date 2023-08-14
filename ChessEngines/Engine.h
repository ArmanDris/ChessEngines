#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.h"
#include "Board.h"

class Engine
{
public:
	virtual std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) = 0;
};