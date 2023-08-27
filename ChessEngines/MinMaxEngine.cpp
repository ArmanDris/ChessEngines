#include "MinMaxEngine.h"

std::pair<sf::Vector2i, sf::Vector2i> MinMaxEngine::returnMove(const Board& b)
{
    perftBenchmark();

    return { sf::Vector2i(-1, -1), sf::Vector2i(-1, -1) };
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

void MinMaxEngine::perftBenchmark() {
    std::pair<int, int> move = {-3, 7};
    sf::Vector2i sf_move(-3, 7);
    Piece empty = Piece();
    Piece queen = Piece(Type::Queen, Color::White);
    Type ty_queen = Type::Queen;
    Color white = Color::White;
    Board br;
    std::cout << "Size of int pair " << sizeof(move) << std::endl;
    std::cout << "Size of sf vectr " << sizeof(sf_move) << std::endl;
    std::cout << "Size of mty piec " << sizeof(empty) << std::endl;
    std::cout << "Size of qen piec " << sizeof(queen) << std::endl;
    std::cout << "Size of type qun " << sizeof(ty_queen) << std::endl;
    std::cout << "Size of color w  " << sizeof(white) << std::endl;
    std::cout << "Size of bord obj " << sizeof(br) << std::endl << std::endl;

    std::cout << "Benchmarking first 5 ply's on starting board: \n";

    Board b;
    sf::Clock c;

    for (int i = 1; i <= 5; i++) {
        std::cout << prefSearch(b, i) << " in ";
        sf::Time t = c.restart();
        std::cout << t.asSeconds() << " seconds \n";
    }
}

int MinMaxEngine::prefSearch(Board& b, int depth) {
    if (depth == 0)
        return 1;

    int num_positions = 0;

    std::vector<move> legal_moves = b.getMoves();

    for (auto move : legal_moves) {
        b.makeMove(move.first, move.second);
        num_positions += prefSearch(b, depth - 1);
        b.undoMove();
    }

    return num_positions;
}