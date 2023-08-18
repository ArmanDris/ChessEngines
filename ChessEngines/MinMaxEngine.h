#pragma once
#include "Engine.h"
#include "Board.h"
#include <vector>

class MinMaxEngine : public Engine, private Board
{
	virtual std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b);
private:
	using move = std::pair<sf::Vector2i, sf::Vector2i>;
	using evaluated_move = std::pair<move, double>;
	double evaluateBoard(std::vector<move> moves = {});
	double evaluateBoardMaterial() const;

};

