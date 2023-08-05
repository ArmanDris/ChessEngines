#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "Board.h"
#include "Piece.h"

class RandomEngine : Board
{
public:
	std::pair<sf::Vector2i, sf::Vector2i> makeMove(Piece b[8][8]);
};

