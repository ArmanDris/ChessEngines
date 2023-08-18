#include "MinMaxEngine.h"

std::pair<sf::Vector2i, sf::Vector2i> MinMaxEngine::returnMove(const Board& b)
{
    static_cast<Board&>(*this) = b;

    Color ally_color = whiteTurn ? Color::White : Color::Black;

    std::vector<move> moves = get_moves();
    std::vector<evaluated_move> move_eval_pairs;

    for (auto& move : moves) {
        evaluated_move move_eval_pair = { move, evaluateBoard({move}) };
		move_eval_pairs.push_back(move_eval_pair);
	}

    // Get the best move
    evaluated_move best_move = move_eval_pairs[0];
    for (auto& move_eval_pair : move_eval_pairs) {
        if (ally_color == Color::White && move_eval_pair.second > best_move.second)
			best_move = move_eval_pair;
        if (ally_color == Color::Black && move_eval_pair.second < best_move.second)
            best_move = move_eval_pair;
    }

    return best_move.first;
}

// Board evaluation after preforming moves
double MinMaxEngine::evaluateBoard(std::vector<move> moves)
{
    MinMaxEngine clone = *this;
    for (auto& move : moves) {
		if (!clone.makeMove(move.first, move.second)) std::cout << "Error, illegal move" << std::endl;
	}
    double material_eval = clone.evaluateBoardMaterial();
    return material_eval;
}

// Positive number better for white, negative better for black
double MinMaxEngine::evaluateBoardMaterial() const
{
    double white_material = 0.0;
    double black_material = 0.0;

    for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
        if (board[x][y].getColor() == Color::White) 
            white_material += board[x][y].getValue();
        else if (board[x][y].getColor() == Color::Black) 
            black_material += board[x][y].getValue();
	}

    return white_material - black_material;
}
