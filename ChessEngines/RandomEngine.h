#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "Board.h"
#include "Piece.h"
#include "Engine.h"

class RandomEngine : public Board, public Engine
{
public:
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) override;
};

