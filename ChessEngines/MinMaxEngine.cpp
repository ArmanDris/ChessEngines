#include "MinMaxEngine.h"

std::pair<sf::Vector2i, sf::Vector2i> MinMaxEngine::returnMove(const Board& b)
{
	int depth = 3;
	int best_eval = INT_MIN;
	move best_move;

	Board board = b;

	std::vector<move> moves = board.get_moves();

	for (auto move : moves) {
		Board child = b;
		child.makeMove(move.first, move.second);
		int eval = minimax(child, depth - 1, -INT_MAX, INT_MIN, false);

		if (eval > best_eval) {
			best_eval = eval;
			best_move = move;
		}
	}

	return best_move;
}

// Alpha: the worst score the player can get
// Beta: the best score the enemy can get
int MinMaxEngine::minimax(Board& b, int depth, int alpha, int beta,  bool maximizingPlayer)
{
	if (depth <= 0 || b.isWhiteVictory() || b.isBlackVictory() || b.isDraw()) {
		return evaluateBoard(b);
	}

	if (maximizingPlayer) {
		int maxEval = INT_MIN;
		for (auto move : b.get_moves()) {
			Board child = b;
			child.makeMove(move.first, move.second);
			int eval = minimax(child, depth - 1, alpha, beta, false);
			maxEval = std::max(maxEval, eval);
			alpha = std::max(alpha, eval);

			if (beta <= alpha) break;
		}
		return maxEval;
	}
	else {
		int minEval = INT_MAX;
		for (auto move : b.get_moves()) {
			Board child = b;
			child.makeMove(move.first, move.second);
			int eval = minimax(child, depth - 1, alpha, beta, true);
			minEval = std::min(minEval, eval);
			beta = std::min(beta, eval);

			if (beta <= alpha) break;
		}
		return minEval;
	}
}

// Board evaluation after preforming moves
int MinMaxEngine::evaluateBoard(const Board& b) const
{
	return -evaluateBoardMaterial(b);
}

// Positive number better for white, negative better for black
int MinMaxEngine::evaluateBoardMaterial(const Board& b) const
{
	int white_material = 0;
	int black_material = 0;

	for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
		sf::Vector2i square(x, y);
		if (b.getPiece(square).getColor() == Color::White)
			white_material += b.getPiece(square).getValue();
		else if (b.getPiece(square).getColor() == Color::Black)
			black_material += b.getPiece(square).getValue();
	}

	return white_material - black_material;
}
