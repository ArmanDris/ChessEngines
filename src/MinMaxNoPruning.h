#pragma once
#include "Engine.h"
#include "Board.h"
#include <vector>
#include <random>

class MinMaxNoPruning : public Engine
{
public:
	virtual std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& board);
	void perftBenchmark();
private:
	Board b;
	using move = std::pair<sf::Vector2i, sf::Vector2i>;
	int search(int depth);
	int boardEval() const;
	int materialEval() const;

	// Helper functions
	int perftSearch(int depth);
};

