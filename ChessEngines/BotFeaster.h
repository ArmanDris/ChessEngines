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
// 8. Random Move

// Bot feaster will try to:
// 1. Never hang a piece
// 2. Never risk losing a piece to a lower value piece
// 3. Always take a higher value piece
// 4. Adjust priorities if it sees a very good move

class BotFeaster : public Engine, public EngineHelpers
{
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) override;
};

