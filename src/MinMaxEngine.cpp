#include "MinMaxEngine.h"

std::pair<sf::Vector2i, sf::Vector2i> MinMaxEngine::returnMove(const Board& board)
{
	b = board;
	std::vector<move> moves = b.getMoves();

	std::random_device rd;
	std::default_random_engine rng(rd());
	std::shuffle(moves.begin(), moves.end(), rng);

	move best_move = moves[0];
	int best_eval = INT_MIN;

	for (move m : moves) {
		b.makeSafeMove(m.first, m.second);
		int eval = -search(4, INT_MIN, INT_MAX, true);
		b.undoMove();

		if (eval > best_eval) {
			best_eval = eval;
			best_move = m;
		}
	}

	return best_move;
}

// alpha: The best eval so far
// beta: The worst eval so far
int MinMaxEngine::search(int depth, int alpha, int beta, bool maximizing_player) {
	if (depth == 0)
		return boardEval();

	std::vector<move> legal_moves = b.getMoves();
	if (legal_moves.size() == 0) {
		if (b.isPlayerInCheck())
			return -1000000;
		else
			return 0;
	}


	if (maximizing_player) {
		int max_eval = INT_MIN;
		for (auto move : legal_moves) {
			b.makeSafeMove(move.first, move.second);
			int eval = search(depth - 1, alpha, beta, false);
			max_eval = std::max(max_eval, eval);
			b.undoMove();
			alpha = std::max(alpha, eval);
			if (beta <= alpha)
				break;
		}
		return max_eval;
	}
	else {
		int min_eval = INT_MAX;
		for (auto move : legal_moves) {
			b.makeSafeMove(move.first, move.second);
			int eval = search(depth - 1, alpha, beta, true);
			min_eval = std::min(min_eval, eval);
			b.undoMove();
			beta = std::min(beta, eval);
			if (beta <= alpha)
				break;
		}
		return min_eval;
	}
}

int MinMaxEngine::boardEval() const
{
	int total_eval = 0;
	total_eval += materialEval();

	return total_eval;
}

int MinMaxEngine::materialEval() const
{
	int white_material = 0;
	int black_material = 0;

	for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++)
	{
		Piece p = b.getPiece(sf::Vector2i(x, y));

		if (p.getType() == Type::None) continue;

		switch (p.getColor()) {
		case Color::White: white_material += p.getValue(); break;
		case Color::Black: black_material += p.getValue(); break;
		}

	}

	return white_material - black_material;
}

void MinMaxEngine::perftBenchmark() {
	b = Board();
	std::cout << "size of board: " << sizeof(b) << "\n";
	std::cout << "Benchmarking first 5 ply's on starting board: \n";

	sf::Clock c;

	for (int i = 1; i <= 5; i++) {
		std::cout << prefSearch(i) << " in ";
		sf::Time t = c.restart();
		std::cout << t.asSeconds() << " seconds \n";
	}
}

int MinMaxEngine::prefSearch(int depth) {
	if (depth == 0)
		return 1;

	int num_positions = 0;

	std::vector<move> legal_moves = b.getMoves();

	for (auto move : legal_moves) {
		b.makeSafeMove(move.first, move.second);
		num_positions += prefSearch(depth - 1);
		b.undoMove();
	}

	return num_positions;
}