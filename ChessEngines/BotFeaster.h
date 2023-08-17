#pragma once
#include "Engine.h"
#include "EngineHelpers.h"

// Bot feaster's priorities are:
// 1. Move wins game
// 2. Take hanging enemy piece
// 3. Make positive trade
// 4. Check enemy king
// 5. Move pieces being attacked by lower value away
// 6. Defend pieces being attacked by equal or higher value
// 7. Castle
// 7. Push pawn
// 8. Make equal trade
// 8. Make random non-hanging move
// 9. Random Move

class BotFeaster : public Engine, public EngineHelpers
{
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) override;
};

