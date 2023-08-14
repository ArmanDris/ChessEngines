#pragma once
#include "Engine.h"
#include "EngineHelpers.h"

// HippieEngine prioritieses moves in this order:
// 1. Move ends game
// 2. Take undefended enemy piece
// 3. Support undefended piece
// 4. Move undefended piece out of danger
// 5. Castle
// 6. Check enemy king
// 7. Random Move

class HippieEngine : public Engine, public EngineHelpers
{
public:
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) override;
};

