#pragma once
#include "Board.h"
#include "Engine.h"
#include "EngineHelpers.h"

// DrunkEngine prioritieses moves in this order:
// 1. Move wins game
// 1. Check enemy king
// 2. Take undefended enemy piece
// 3. Take defended enemy piece
// 4. Castle
// 5. Random Move

class DrunkEngine : public Engine, public EngineHelpers
{
public:
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) override;
};

