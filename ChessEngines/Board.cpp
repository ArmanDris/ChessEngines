#include "Board.h"


Board::Board() {
	generatePsudoLegalMoves();
}

// Returns true if a move is excecuted
void Board::makeMove(const sf::Vector2i old_square, const sf::Vector2i new_square) {
	move m = { old_square, new_square };
	auto it = std::find(psudoLegalMoves.begin(), psudoLegalMoves.end(), m);
	if (it == psudoLegalMoves.end()) return;

	movePiece(old_square, new_square);
	changeTurn();
	generatePsudoLegalMoves();
}

void Board::undoMove()
{
	if (log.size() == 0) return;
	auto last_log = log.back();

	Piece old_piece = std::get<0>(last_log);
	sf::Vector2i old_square = std::get<1>(last_log);

	Piece new_piece = std::get<2>(last_log);
	sf::Vector2i new_square = std::get<3>(last_log);

	// Undo castle
	if (old_piece.getType() == PieceType::King && abs(old_square.x - new_square.x) > 1) {
		// If undoing left castle
		if (old_square.x > new_square.x) {
			board[0][old_square.y] = Piece(PieceType::Rook, old_piece.getColor());
			board[3][old_square.y] = Piece();
		}
		// If undoing right castle
		else {
			board[7][old_square.y] = Piece(PieceType::Rook, old_piece.getColor());
			board[5][old_square.y] = Piece();
		}
	}

	// Undo en passent
	if (old_piece.getType() == PieceType::Pawn && new_piece.getType() == PieceType::None && old_square.x != new_square.x) {
		// Should place a pawn behind the new square
		if (old_piece.getColor() == PieceColor::White)
			board[new_square.x][new_square.y + 1] = Piece(PieceType::Pawn, PieceColor::Black);
		else
			board[new_square.x][new_square.y - 1] = Piece(PieceType::Pawn, PieceColor::White);
	}

	board[old_square.x][old_square.y] = old_piece;
	board[new_square.x][new_square.y] = new_piece;

	log.pop_back();
	// Reset game over in case move caused draw or such
	whiteVictory = false; blackVictory = false; draw = false;
	changeTurn();
}

void Board::importFEN(std::string FEN)
{
	if (FEN.back() != ' ') FEN += ' ';

	std::string FEN_parts[6];
	size_t pos = FEN.find(' ');
	int i = 0;
	while (pos != std::string::npos) {
		FEN_parts[i] = FEN.substr(0, pos);

		FEN = FEN.substr(pos + 1);
		i++;
		pos = FEN.find(' ');
	}

	FEN_parts[0] += '/';
	std::string pieces[8];
	pos = FEN_parts[0].find('/');
	i = 0;
	while (pos != std::string::npos) {
		pieces[i] = FEN_parts[0].substr(0, pos);

		FEN_parts[0] = FEN_parts[0].substr(pos + 1);
		i++;
		pos = FEN_parts[0].find('/');
	}

	for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
		// Check if the character is a digit (0-9)
		//if (ch >= '0' && ch <= '9') {
		//	std::cout << "The character is a digit." << std::endl;
		//}
		//else {
		//	std::cout << "The character is not a digit." << std::endl;
		//}
	}

	for (int a = 0; a < 8; a++) {
		board[a][0] = pieces[0][a];
	}


	// Printing loops
	for (std::string s : FEN_parts) {
		std::cout << s << ",";
	}

	std::cout << std::endl;

	for (std::string s : pieces) {
		std::cout << s << ",";
	}

	std::cout << std::endl;
}

// Will not check if move is legal
void Board::movePiece(const sf::Vector2i& old_square, const sf::Vector2i& new_square)
{
	logMove(old_square, new_square);

	if (new_square == old_square) return;

	// En passant
	if (moveIsEnPassent(old_square, new_square))
		board[new_square.x][old_square.y] = Piece();

	// Castling
	if (moveIsCastle(old_square, new_square))
		castle(old_square, new_square);

	// Pawn promotion
	if (board[old_square.x][old_square.y].getType() == PieceType::Pawn && (new_square.y == 0 || new_square.y == 7))
		board[old_square.x][old_square.y] = Piece(PieceType::Queen, board[old_square.x][old_square.y].getColor());

	board[new_square.x][new_square.y] = board[old_square.x][old_square.y];
	board[old_square.x][old_square.y] = Piece();
}

bool Board::moveIsCastle(const sf::Vector2i& old_square, const sf::Vector2i& new_square)
{
	return false;
}

bool Board::moveIsEnPassent(const sf::Vector2i& old_square, const sf::Vector2i& new_square)
{
	return false;
}

void Board::castle(const sf::Vector2i& old_square, const sf::Vector2i& new_square)
{
	// The king move will have already been excecuted so just need to move rook

	// If the king is trying to castle left
	if (old_square.x > new_square.x) {
		board[3][old_square.y] = board[0][old_square.y];
		board[0][old_square.y] = Piece();
	}

	// If the king is trying to castle right
	if (old_square.x < new_square.x) {
		board[5][old_square.y] = board[7][old_square.y];
		board[7][old_square.y] = Piece();
	}
}

void Board::changeTurn() {
	if (whiteTurn) whiteTurn = false;
	else           whiteTurn = true;
}

void Board::logMove(const sf::Vector2i& old_square, const sf::Vector2i& new_square) {
	log.push_back({ board[old_square.x][old_square.y], old_square,
					board[new_square.x][new_square.y], new_square });
}

void Board::saveLog(std::string saveLog) {
	std::ofstream file(saveLog);

	// Get the current system time
	auto now = std::chrono::system_clock::now();
	std::time_t time_now = std::chrono::system_clock::to_time_t(now);
	std::tm tm_now;

	// Use localtime_s() for safer handling
	if (localtime_s(&tm_now, &time_now) == 0) {
		char time_buffer[80];
		std::strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", &tm_now);

		// Print the formatted time at the top of the file
		file << "Log saved on: " << time_buffer << std::endl;
	}
	else {
		// Handle error
		std::cerr << "Error getting local time." << std::endl;
	}


	file << "Piece being moved : Location piece is moving to" << std::endl << std::endl;
	for (auto move : log) {
		file << std::get<0>(move).colorToString() << " "
			<< std::get<0>(move).typeToString() << " "
			<< std::get<1>(move).x << "," << std::get<1>(move).y << " : "

			<< std::get<2>(move).colorToString() << " "
			<< std::get<2>(move).typeToString() << " "
			<< std::get<3>(move).x << "," << std::get<3>(move).y << std::endl;
	}
	file.close();
}


// Expensive function because it loops through the whole board
void Board::generatePsudoLegalMoves()
{
	std::vector<Board::move> moves;
	for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
		if (!board[x][y]) continue;
		if (board[x][y].getColor() == PieceColor::White && !whiteTurn) continue;
		if (board[x][y].getColor() == PieceColor::Black && whiteTurn) continue;
		sf::Vector2i square(x, y);
		PieceType type = board[x][y].getType();
		PieceColor color = board[x][y].getColor();

		switch (type) {
		case PieceType::Pawn:   appendPsudoLegalPawnMoves(square, color, moves); break;
		case PieceType::Rook:   appendPsudoLegalRookMoves(square, color, moves); break;
		case PieceType::Knight: appendPsudoLegalKnightMoves(square, color, moves); break;
		case PieceType::Bishop: appendPsudoLegalBishopMoves(square, color, moves); break;
		case PieceType::King:   appendPsudoLegalKingMoves(square, color, moves); break;
		case PieceType::Queen:  appendPsudoLegalQueenMoves(square, color, moves); break;
		}
	}

	psudoLegalMoves = moves;
}

void Board::appendPsudoLegalPawnMoves(const sf::Vector2i& sq, const PieceColor& c, std::vector<move>& moves)
{
	if (c == PieceColor::White) {
		if (sq.y == 6) {
			if (!board[sq.x][4]) {
				moves.push_back({ sq, sf::Vector2i(sq.x, 4) });
			}
		}
		if (!board[sq.x][sq.y - 1]) {
			moves.push_back({ sq, sf::Vector2i(sq.x, sq.y - 1) });
		}
	}
	else {
		if (sq.y == 1) {
			if (!board[sq.x][3]) {
				moves.push_back({ sq, sf::Vector2i(sq.x, 3) });
			}
		}
		if (!board[sq.x][sq.y + 1]) {
			moves.push_back({ sq, sf::Vector2i(sq.x, sq.y + 1) });
		}
	}
}

void Board::appendPsudoLegalRookMoves(const sf::Vector2i& sq, const PieceColor& c, std::vector<move>& moves)
{
	// Look for moves upward
	for (int y = sq.y - 1; y >= 0; y--) {
		if (!board[sq.x][y]) {
			moves.push_back({ sq, sf::Vector2i(sq.x, y) });
		}
		else {
			if (board[sq.x][y].getColor() == c)
				break;
			if (board[sq.x][y].getColor() != c) {
				moves.push_back({ sq, sf::Vector2i(sq.x, y) });
				break;
			}
		}
	}

	// Look for moves downward
	for (int y = sq.y + 1; y <= 7; y++) {
		if (!board[sq.x][y]) {
			moves.push_back({ sq, sf::Vector2i(sq.x, y) });
		}
		else {
			if (board[sq.x][y].getColor() == c)
				break;
			if (board[sq.x][y].getColor() != c) {
				moves.push_back({ sq, sf::Vector2i(sq.x, y) });
				break;
			}
		}
	}

	// Look for moves left
	for (int x = sq.x - 1; x >= 0; x--) {
		if (!board[x][sq.y])
			moves.push_back({ sq, sf::Vector2i(x, sq.y) });
		else {
			if (board[x][sq.y].getColor() == c)
				break;
			else {
				moves.push_back({ sq, sf::Vector2i(x, sq.y) });
				break;
			}
		}
	}

	// Look for moves right
	for (int x = sq.x + 1; x <= 7; x++) {
		if (!board[x][sq.y])
			moves.push_back({ sq, sf::Vector2i(x, sq.y) });
		else {
			if (board[x][sq.y].getColor() == c)
				break;
			else {
				moves.push_back({ sq, sf::Vector2i(x, sq.y) });
				break;
			}
		}
	}
}

void Board::appendPsudoLegalKnightMoves(const sf::Vector2i& sq, const PieceColor& c, std::vector<move>& moves)
{
	if (sq.x >= 2 && sq.y >= 1) {
		if (!board[sq.x-1][sq.y-2] || board[sq.x-1][sq.y-2].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 1, sq.y - 2)}); // up left
	}

	if (sq.x <= 6 && sq.y >= 2) {
		if (!board[sq.x+1][sq.y-2] || board[sq.x+1][sq.y-2].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 1, sq.y - 2)}); // up right
	}

	if (sq.x <= 5 && sq.y > 0) {
		if (!board[sq.x+2][sq.y-1] || board[sq.x+2][sq.y-1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 2, sq.y - 1)}); // right up
	}

	if (sq.x <= 5 && sq.y <= 6) {
		if (!board[sq.x+2][sq.y+1] || board[sq.x+2][sq.y+1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 2, sq.y + 1)}); // right down
	}

	if (sq.x <= 6 && sq.y <= 5) {
		if (!board[sq.x+1][sq.y+2] || board[sq.x+1][sq.y+2].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 1, sq.y + 2)}); // down right
	}

	if (sq.x >= 1 && sq.y <= 5) {
		if (!board[sq.x-1][sq.y+2] || board[sq.x-1][sq.y+2].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 1, sq.y + 2)}); // down left
	}

	if (sq.x >= 2 && sq.y <= 6) {
		if (!board[sq.x-2][sq.y+1] || board[sq.x-2][sq.y+1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 2, sq.y + 1)}); // left down
	}

	if (sq.x >= 2 && sq.y >= 1) {
		if (!board[sq.x-2][sq.y-1] || board[sq.x-2][sq.y-1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 2, sq.y - 1)}); // left up
	}
}

void Board::appendPsudoLegalBishopMoves(const sf::Vector2i& sq, const PieceColor& c, std::vector<move>& moves)
{
	// Generating up left
	int i = 0;
	sf::Vector2i tgt(sq.x - 1, sq.y - 1);
	while (tgt.x >= 0 && tgt.y >= 0) {
		if (!board[tgt.x][tgt.y])
			moves.push_back({ sq, tgt });
		else {
			if (board[tgt.x][tgt.y].getColor() == c)
				break;
			else {
				moves.push_back({ sq, tgt });
				break;
			}
		}
		tgt.x -= 1;
		tgt.y -= 1;
	}

	// Generate up right
	i = 0;
	tgt = sf::Vector2i(sq.x + 1, sq.y - 1);
	while (tgt.x <= 7 && tgt.y >= 0) {
		if (!board[tgt.x][tgt.y])
			moves.push_back({ sq, tgt });
		else {
			if (board[tgt.x][tgt.y].getColor() == c)
				break;
			else {
				moves.push_back({ sq, tgt });
				break;
			}
		}
		tgt.x += 1;
		tgt.y -= 1;
	}

	// Generate down left
	i = 0;
	tgt = sf::Vector2i(sq.x - 1, sq.y + 1);
	while (tgt.x >= 0 && tgt.y <= 7) {
		if (!board[tgt.x][tgt.y])
			moves.push_back({ sq, tgt });
		else {
			if (board[tgt.x][tgt.y].getColor() == c)
				break;
			else {
				moves.push_back({ sq, tgt });
				break;
			}
		}
		tgt.x -= 1;
		tgt.y += 1;
	}

	// Generate down right
	i = 0;
	tgt = sf::Vector2i(sq.x + 1, sq.y + 1);
	while (tgt.x <= 7 && tgt.y <= 7) {
		if (!board[tgt.x][tgt.y])
			moves.push_back({ sq, tgt });
		else {
			if (board[tgt.x][tgt.y].getColor() == c)
				break;
			else {
				moves.push_back({ sq, tgt });
				break;
			}
		}
		tgt.x += 1;
		tgt.y += 1;
	}
}

void Board::appendPsudoLegalKingMoves(const sf::Vector2i& sq, const PieceColor& c, std::vector<move>& moves)
{
	if (sq.x != 0 && sq.y != 0) {
		if (!board[sq.x-1][sq.y-1] || board[sq.x-1][sq.y-1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 1, sq.y - 1)}); // top left
	}

	if (sq.y != 0) {
		if (!board[sq.x][sq.y-1] || board[sq.x][sq.y-1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x    , sq.y - 1)}); // top
	}

	if (sq.x != 7 && sq.y != 0) {
		if (!board[sq.x+1][sq.y-1] || board[sq.x+1][sq.y-1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 1, sq.y - 1)}); // top right
	}

	if (sq.x != 7) {
		if (!board[sq.x+1][sq.y] || board[sq.x+1][sq.y].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 1, sq.y    )}); // right
	}

	if (sq.x != 7 && sq.y != 7) {
		if (!board[sq.x+1][sq.y+1] || board[sq.x+1][sq.y+1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 1, sq.y + 1)}); // bottom right
	}

	if (sq.y != 7) {
		if (!board[sq.x][sq.y+1] || board[sq.x][sq.y+1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x    , sq.y + 1)}); // bottom
	}

	if (sq.x != 0 && sq.y != 7) {
		if (!board[sq.x-1][sq.y+1] || board[sq.x-1][sq.y+1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 1, sq.y + 1)}); // bottom left
	}

	if (sq.x != 0) {
		if (!board[sq.x-1][sq.y] || board[sq.x-1][sq.y].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 1, sq.y)});     // left
	}
}

void Board::appendPsudoLegalQueenMoves(const sf::Vector2i& sq, const PieceColor& c, std::vector<move>& moves)
{
	appendPsudoLegalRookMoves(sq, c, moves);
	appendPsudoLegalBishopMoves(sq, c, moves);
}
