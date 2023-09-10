 #pragma once
 #include "Engine.h"
 #include "Board.h"
 #include <vector>

 class MinMaxEngine : public Engine
 {
 public:
 	virtual std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b);
 private:
 	using move = std::pair<sf::Vector2i, sf::Vector2i>;
    int search(Board& b, int depth);
 	int boardEval(const Board& b);
 	int materialEval(const Board& b);

 	// Helper functions
 	void perftBenchmark();
 	int prefSearch(Board& b, int depth);
 };

