#pragma once
#include "Engine.h"
#include "Board.h"
#include <vector>

class MinMaxEngine : public Engine
{
	virtual std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b);
private:
	using move = std::pair<sf::Vector2i, sf::Vector2i>;
	using evaluated_move = std::pair<move, double>;
	struct Node {
		Board board;
	};

	int minimax(Board& b, int depth, int alpha, int beta, bool isMaximisingPlayer);
	int evaluateBoard(const Board& b) const;
	int evaluateBoardMaterial(const Board& b) const;

};

