#pragma once
#include "Engine.h"
#include "Board.h"
#include <vector>

class MinMaxEngine : public Engine
{
	virtual std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b);
private:
	int boardEval(const Board& b);
	int materialEval(const Board& b);
};

