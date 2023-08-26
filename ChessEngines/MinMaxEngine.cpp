#include "MinMaxEngine.h"

std::pair<sf::Vector2i, sf::Vector2i> MinMaxEngine::returnMove(const Board& b)
{
    
    return { sf::Vector2i(-1, -1), sf::Vector2i(-1, -1) };
}

void search(const Board& b) {
    vector<move> legal_moves = b.get_moves();

    for ()
}

int MinMaxEngine::boardEval(const Board& b) {
    int total_eval = 0;
    total_eval += materialEval(b);

    return total_eval;
}

int MinMaxEngine::materialEval(const Board& b) {
    int white_material = 0;
    int black_material = 0;

    for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++)
    {
        Piece p = b.getPiece(sf::Vector2i(x, y));

        switch (p.getColor()) {
            case Color::None: continue; break;
            case Color::White: white_material += p.getValue(); break;
            case Color::Black: black_material += p.getValue(); break;
        }
        
    }

    return white_material - black_material;
}